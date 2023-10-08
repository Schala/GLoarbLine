/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CUnflattenFile.h"
#include "CFSLocalFolderRef.h"
#include "CLocalFile.h"
#include "CStorage.h"

#if TARGET_OS_MAC
	#include "CFSLocalFileRefMac.h"
	#include "CLocalResourceFileMac.h"
#elif TARGET_OS_WIN32
	#include "CFSLocalFileRefWin.h"
#elif TARGET_OS_UNIX
	#include "CFSLocalFileRefUnix.h"
#endif


HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  CUnflattenFile                                              [public]
// ---------------------------------------------------------------------
// constructor - just for testing

CUnflattenFile::CUnflattenFile( const CString &inContainer )
	: mContainer( inContainer )
	, mState(eState_Header)
	, mFileRef( nil )
	, mFile( nil )
	, mDataForkCreated(false)
	, mDataForkSize(0)
{
}


// ---------------------------------------------------------------------
//  CUnflattenFile                                              [public]
// ---------------------------------------------------------------------
// constructor - just for testing

CUnflattenFile::CUnflattenFile( const CString &inContainer,
			const CString &inAltFileName )
	: mContainer( inContainer )
	, mAltFileName( inAltFileName )
	, mState(eState_Header)
	, mFileRef( nil )
	, mFile( nil )
	, mDataForkCreated(false)
	, mDataForkSize(0)
{
}


// ---------------------------------------------------------------------
//  ~CUnflattenFile	                                   [public][virtual]
// ---------------------------------------------------------------------
// destructor

CUnflattenFile::~CUnflattenFile()
{
	delete mFile;
	delete mFileRef;
}


// ---------------------------------------------------------------------
//  Open                                                        [public]
// ---------------------------------------------------------------------
// Open the file

void
CUnflattenFile::ProcessData( UInt8 *inBuffer, UInt32 inSize ) 
{
	const UInt32 kHeaderSize = sizeof(UInt32) + sizeof(UInt16) +
						sizeof(UInt8) * 16 + sizeof(UInt16);
	const UInt32 kForkHeaderSize = sizeof(UInt32) + sizeof(UInt32) +
						sizeof(UInt32) + sizeof(UInt32);
	const UInt32 kInfoForkSize = sizeof(UInt32) * 5 + sizeof(UInt8) * 32 +
						(sizeof(UInt16) + sizeof(UInt16) + sizeof(UInt32)) * 2 +
						sizeof(UInt16) * 2;
	UInt8 hdrDummy[16], infoDummy[32];
	UInt32 temp32;

	try {
		bool done;
		if( mState != eState_Done ){
			mBuffer.Write( inBuffer, inSize );
			done = false;
		} else {
			done = true;
		}
		while( !done ){ // Continue until the buffer is used up or
						// there isn't enough data to complete the next state
			switch( mState ){
				case eState_Header: { // Stream header
					if( mBuffer.GetAvailable() >= kHeaderSize ){
						mBuffer >> mHeaderFormat;
						mBuffer >> mHeaderVers;
						mBuffer.Read( &hdrDummy, sizeof(hdrDummy) );
						mBuffer >> mHeaderForkCount;
						mCurrentFork = 1;
						mState = eState_ForkHeader;
					} else {
						done = true;
					}
					break;
					}
				case eState_ForkHeader: { // Current fork's header
					if( mBuffer.GetAvailable() >= kForkHeaderSize ){
						mBuffer >> mForkType;
						mBuffer >> mForkCompressionType;
						mBuffer >> temp32;
						mBuffer >> mForkSize;
						mCurrentDataSize = 0;
						mState = eState_Fork;
					} else {
						done = true;
					}
					break;
					}
				case eState_Fork: {
					bool forkDone = false;
					switch( mForkType ){
						case 'INFO': {
							if( mBuffer.GetAvailable() >= mForkSize ){
								mBuffer >> mInfoPlatform;
								mBuffer >> mInfoTypeSig;
								mBuffer >> mInfoCreatorSig;
								mBuffer >> mInfoFlags;
								mBuffer >> mInfoPlatformFlags;
								
								mBuffer.Read( &infoDummy, sizeof(infoDummy) );

								mBuffer >> mInfoCreateDate.mYear;
								mBuffer >> mInfoCreateDate.mMilliSecs;
								mBuffer >> mInfoCreateDate.mSecs;
								mBuffer >> mInfoModifyDate.mYear;
								mBuffer >> mInfoModifyDate.mMilliSecs;
								mBuffer >> mInfoModifyDate.mSecs;
								
								mBuffer >> mInfoNameScript;
								mBuffer >> mInfoNameSize;

								CStorage nameBuf(mInfoNameSize);
								mBuffer.Read( (UInt8*)nameBuf, nameBuf.Size() );
								CString fileName = mContainer;
								if( mAltFileName.length() > 0 ){
									fileName += mAltFileName;
								} else {
									fileName += CString((UInt8*)nameBuf, mInfoNameSize );
								}
								
								//?? Ask container to create child in final version
								//?? What to do with the other attributes of the file
								#if TARGET_OS_MAC	
									mFileRef = new CFSLocalFileRefMac( fileName );
									((CFSLocalFileRefMac*)mFileRef)->SetMacType( mInfoTypeSig );
									((CFSLocalFileRefMac*)mFileRef)->SetMacCreator( mInfoCreatorSig );
								#elif TARGET_OS_WIN32
									mFileRef = new CFSLocalFileRefWin( fileName );
								#elif TARGET_OS_UNIX
									mFileRef = new CFSLocalFileRefUnix( fileName );
								#endif
								BroadcastMessage( eRefLoaded );

								mCurrentDataSize = kInfoForkSize + mInfoNameSize;
								CStorage temp( mForkSize - mCurrentDataSize );
								mBuffer.Read( (UInt8*)temp, temp.Size() );
								mCurrentDataSize = mForkSize;
								forkDone = true;
							} else {
								done = true;
							}
							break;
							}
						case 'DATA':
							if( mFile == nil ){
								mDataForkCreated = true;
								mDataForkSize = mForkSize;
								mFile = new CLocalFile( *mFileRef );
								mFile->Open( CLocalFile::eMode_WriteOnly, CLocalFile::eShare_Read );
								BroadcastMessage( eDataCreated );
							}
						case 'MACR': {
							#if TARGET_OS_MAC
								if( mFile == nil ){
									mFile = new CLocalResourceFileMac( *mFileRef );
									mFile->Open( CLocalFile::eMode_WriteOnly, CLocalFile::eShare_Read );
									BroadcastMessage( eResourceCreated );
								}
							#endif
							if( mBuffer.GetAvailable() > 0 ){
								UInt32 size = mBuffer.GetAvailable();
								if( size > (mForkSize - mCurrentDataSize) ){
									size = mForkSize - mCurrentDataSize;
								}
								CStorage temp( size );
								mBuffer.Read( (UInt8*)temp, temp.Size() );
								if( mFile != nil ){
									size = mFile->Write(  (UInt8*)temp, temp.Size() );
								}
								mCurrentDataSize += temp.Size();
								if( mCurrentDataSize >= mForkSize ){
									if( mFile != nil ){
										mFile->Close();
									}
									delete mFile;
									mFile = nil;
									forkDone = true;
								}
							} else {
								done = true;
							}
							break;
							}
					}
					if( forkDone ){
						mCurrentFork++;
						if( mCurrentFork > mHeaderForkCount ){
							mState = eState_Done;
							done = true;
						} else {
							mState = eState_ForkHeader;
						}
					}
					break;
					}
			}
		}
	} catch( ... ){
//		RETHROW_FS_( eOpen, mFileRef.GetName() );
	}
}


// ---------------------------------------------------------------------
//  GetFileRef	                                                [public]
// ---------------------------------------------------------------------
// Returns the FSRef to the local file being created

CFSLocalFileRef&
CUnflattenFile::GetFileRef()
{
	if( mFileRef == nil ){
		THROW_UNKNOWN_FS_( eGetFSRef, L"" );
	}
	return *mFileRef;
}


// ---------------------------------------------------------------------
//  GetDataForkSize                                             [public]
// ---------------------------------------------------------------------
// Returns the size of the data fork

UInt32
CUnflattenFile::GetDataForkSize()
{
	if( !mDataForkCreated ){
		THROW_UNKNOWN_FS_( eGetSize, L"" );
	}
	return mDataForkSize;
}

HL_End_Namespace_BigRedH
