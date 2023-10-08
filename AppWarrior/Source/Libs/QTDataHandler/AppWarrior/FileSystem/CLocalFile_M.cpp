/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CLocalFile.h"
#include "CFSLocalFileRefMac.h"
#include <Files.h>
#include "USyncroHelpers.h"
#include "MoreFilesExtras.h"


HL_Begin_Namespace_BigRedH

class CLocalFile::CLocalFilePS {
	public:
		short				mFileRefNum;
};


// ---------------------------------------------------------------------
//  CLocalFile                                                  [public]
// ---------------------------------------------------------------------
// constructor

CLocalFile::CLocalFile( const CFSLocalFileRef &inRef )
	: mPlatform(*new CLocalFilePS)
{
	mPlatform.mFileRefNum = 0;
	mFileRef.reset( (CFSLocalFileRef*)inRef.Clone().release() );
}


// ---------------------------------------------------------------------
//  ~CLocalFile	                                       [public][virtual]
// ---------------------------------------------------------------------
// destructor

CLocalFile::~CLocalFile()
{
	try {
		Close();
		delete &mPlatform;
	} catch( ... ){
	}
}


// ---------------------------------------------------------------------
//  Open                                                        [public]
// ---------------------------------------------------------------------
// Open the file

void
CLocalFile::Open( EMode inMode, EShareMode inShareMode ) 
{
	ASSERT(mPlatform.mFileRefNum == 0);
	try {
		StMutex lock(mLock);
		const CFSLocalFileRefMac &fileRef =
					*dynamic_cast<const CFSLocalFileRefMac*>(mFileRef.get());
				
		short openMode;
		switch( inMode ){
			case eMode_ReadOnly:
				openMode = dmRd;
				break;
			case eMode_Append:
			case eMode_WriteOnly:
				openMode = dmWr;
				break;
			case eMode_ReadWrite:
				openMode = dmRdWr;
				break;
		}
		switch( inShareMode ){
			case eShare_None:
				openMode |= dmNoneDenyRdWr;
				break;
			case eShare_Read:
				openMode |= dmNoneDenyWr;
				break;
			case eShare_Write:
				openMode |= dmNoneDenyRd;
				break;
		}
		FSSpec theSpec = fileRef.GetFSpec();
		OSErr anErr = FSpOpenAware( &theSpec, openMode, &mPlatform.mFileRefNum );
//		OSErr anErr = ::FSpOpenDF( &theSpec, openMode, &mPlatform.mFileRefNum );
		if( inMode == eMode_ReadOnly ){
			THROW_OS_( anErr );
		} else if( anErr == fnfErr ){
			THROW_OS_( ::FSpCreate( &theSpec, fileRef.GetMacCreator(),
						fileRef.GetMacType(), smSystemScript ) );
			THROW_OS_( FSpOpenAware( &theSpec, openMode, &mPlatform.mFileRefNum ) );
		} else {
			THROW_OS_( anErr );
		}
		if( inMode == eMode_Append ){	// go to the end of the file
			THROW_OS_( ::SetFPos( mPlatform.mFileRefNum, fsFromLEOF, 0 ) );
		} else if( inMode == eMode_WriteOnly ){
			THROW_OS_( ::SetEOF( mPlatform.mFileRefNum, 0 ) );
		}	
		// send the apropriate notifications to start the listening devices
	} catch( ... ){
		RETHROW_FS_( eOpen, mFileRef->GetName() );
	}
}


// ---------------------------------------------------------------------
//  Close                                                       [public]
// ---------------------------------------------------------------------
// Close the file

void
CLocalFile::Close() 
{
	try {
		StMutex lock(mLock);
		if( mPlatform.mFileRefNum != 0 ){
			THROW_OS_( ::FSClose( mPlatform.mFileRefNum ) );
			mPlatform.mFileRefNum = 0;
		}	
	} catch( ... ){
		RETHROW_FS_( eClose, mFileRef->GetName() );
	}
}


// ---------------------------------------------------------------------
//  Flush                                                       [public]
// ---------------------------------------------------------------------
// Flushes the OS buffer

void
CLocalFile::Flush() 
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		ParamBlockRec pb;
		pb.fileParam.ioCompletion = nil;
		pb.fileParam.ioFRefNum = mPlatform.mFileRefNum;
		THROW_OS_( ::PBFlushFileSync( &pb ) );
	} catch( ... ){
		RETHROW_FS_( eFlush, mFileRef->GetName() );
	}
}


// ---------------------------------------------------------------------
//  GetSize                                                     [public]
// ---------------------------------------------------------------------
// Returns the size of the file, datafork only?

UInt64
CLocalFile::GetSize() const
{
	try {
		StMutex lock(mLock);
		short fRef = mPlatform.mFileRefNum;
		SInt32 size;
		if( fRef == 0 ){ // File is not open
			const CFSLocalFileRefMac &fileRef =
						*dynamic_cast<const CFSLocalFileRefMac*>(mFileRef.get());
					
			FSSpec theSpec = fileRef.GetFSpec();
			long dataSize, rsrcSize;
			THROW_OS_( FSpGetFileSize( &theSpec, &dataSize, &rsrcSize ) );
			size = dataSize;
		} else { // File is already open
			THROW_OS_( ::GetEOF( fRef, &size ) );
		}
		return size;
	} catch( ... ){
		RETHROW_FS_( eGetSize, mFileRef->GetName() );
	}
	return 0; // to suppress compiler warning
}


// ---------------------------------------------------------------------
//  GetPosition                                                 [public]
// ---------------------------------------------------------------------
// Returns the current position of the file pointer within the file

UInt64
CLocalFile::GetPosition() const
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		long pos;
		THROW_OS_( ::GetFPos( mPlatform.mFileRefNum, &pos ) );
		return pos;
	} catch( ... ){
		RETHROW_FS_( eGetPosition, mFileRef->GetName() );
	}
	return 0; // to suppress compiler warning
}


// ---------------------------------------------------------------------
//  SetPosition                                                 [public]
// ---------------------------------------------------------------------
// Set the current position of the file pointer within the file

void
CLocalFile::SetPosition( EFrom inFrom, SInt64 inDelta )
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		long pos = inDelta;
		short posMode;
		switch( inFrom ){
			case eFrom_Start:
				posMode = fsFromStart;
				break;
			case eFrom_Current:
				posMode = fsFromMark;
				break;
			case eFrom_End:
				posMode = fsFromLEOF;
				break;
		}
		THROW_OS_( ::SetFPos( mPlatform.mFileRefNum, posMode, pos ) );
	} catch( ... ){
		RETHROW_FS_( eSetPosition, mFileRef->GetName() );
	}
}


// ---------------------------------------------------------------------
//  Read                                                        [public]
// ---------------------------------------------------------------------
// Read up to inSize bytes of data into the buffer from the current
// position pointer in the file. Return the acutal number of bytes read.

UInt32
CLocalFile::Read( UInt8 *inBuffer, UInt32 inSize )
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		long size = inSize;
		OSErr anErr = ::FSRead( mPlatform.mFileRefNum, &size, inBuffer );
		if( (anErr != eofErr) || (size == 0) ){
			THROW_OS_( anErr );
		}
		return size;
	} catch( ... ){
		RETHROW_FS_( eRead, mFileRef->GetName() );
	}
	return 0; // to suppress compiler warning
}


// ---------------------------------------------------------------------
//  ReadFrom                                                    [public]
// ---------------------------------------------------------------------
// Read up to inSize bytes of data into the buffer from the ioPosition
// bytes from the start of the file. Return the acutal number of bytes read.

UInt32
CLocalFile::ReadFrom( UInt8 *inBuffer, UInt32 inSize,
		UInt64 &ioPosition )
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		SetPosition( eFrom_Start, ioPosition );
		UInt32 size = Read( inBuffer, inSize );
		ioPosition = GetPosition();
		return size;
	} catch( ... ){
		RETHROW_FS_( eRead, mFileRef->GetName() );
	}
	return 0; // to suppress compiler warning
}


// ---------------------------------------------------------------------
//  Write                                                       [public]
// ---------------------------------------------------------------------
// Write up to inSize bytes of data from the buffer to the current
// position pointer into the file. Return the acutal number of bytes
// written.

UInt32
CLocalFile::Write( const UInt8 *inBuffer, UInt32 inSize )
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		long size = inSize;
		THROW_OS_( ::FSWrite( mPlatform.mFileRefNum, &size, inBuffer ) );
		return size;
	} catch( ... ){
		RETHROW_FS_( eWrite, mFileRef->GetName() );
	}
	return 0; // to suppress compiler warning
}


// ---------------------------------------------------------------------
//  ReadFrom                                                    [public]
// ---------------------------------------------------------------------
// Write up to inSize bytes of data from the buffer from the ioPosition
// bytes from the start of the file. Return the acutal number of bytes
// written.

UInt32
CLocalFile::WriteFrom( const UInt8 *inBuffer, UInt32 inSize,
		UInt64 &ioPosition )
{
	ASSERT( mPlatform.mFileRefNum != 0);
	try {
		StMutex lock(mLock);
		SetPosition( eFrom_Start, ioPosition );
		UInt32 size = Write( inBuffer, inSize );
		ioPosition = GetPosition();
		return size;
	} catch( ... ){
		RETHROW_FS_( eWrite, mFileRef->GetName() );
	}
	return 0; // to suppress compiler warning
}

HL_End_Namespace_BigRedH
