/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// The Win32 file naming

#include "AW.h"
#include "CFSLocalFolderRefMac.h"
#include "CFSLocalFileRefMac.h"
#include "FullPath.h"
#include "MoreFiles.h"
#include "MoreFilesExtras.h"
#include "StString.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  CFSLocalFolderRefMac	                                    [public]
// ---------------------------------------------------------------------
// constructor

CFSLocalFolderRefMac::CFSLocalFolderRefMac(const FSSpec& inFSpec)
	: mFSpec(inFSpec)
	, mSpecSetup(true)
	, mRightsLoaded(false)
	, mContentLoaded(false)
{
	// Build filename
	OSErr anErr;
	short len;
	Handle path;
	
	anErr = ::FSpGetFullPath( &mFSpec,  &len,  &path );
	//?? should I care about any error
	if( path != nil ){
		::HLock( path );
		mFolderName = CString( *path, len );
		::HUnlock( path );
		::DisposeHandle( path );
	}
}

CFSLocalFolderRefMac::CFSLocalFolderRefMac(const CString& inFolderName)
	: mFolderName(inFolderName)
	, mSpecSetup(false)
	, mRightsLoaded(false)
	, mContentLoaded(false)
{
	try {
		SetupSpec( false ); // Try to set up the FSpec although it
							// might not exist yet
	} catch( ... ){
		// don't need to worry about problems yet
		// we will care when we try to use the folder
	}
}

CFSLocalFolderRefMac::CFSLocalFolderRefMac(const CFSLocalFolderRefMac& inOther)
	: mFSpec(inOther.mFSpec)
	, mSpecSetup(inOther.mSpecSetup)
	, mFolderName(inOther.mFolderName)
	, mRights(inOther.mRights)
	, mContentLoaded(false) //?? let's not duplicate the content on cloning
	, mRightsLoaded(inOther.mRightsLoaded)
{}

std::auto_ptr<CFSRef> 
CFSLocalFolderRefMac::Clone() const
{
	CFSRef* fsref = nil;
	try {
		fsref = new CFSLocalFolderRefMac(*this);
	} catch( ... ){
		RETHROW_FS_( eClone, mFolderName );
	}

	return std::auto_ptr<CFSRef> (fsref);
}

// ---------------------------------------------------------------------
//  GetName()	                                       [public][virtual]
// ---------------------------------------------------------------------
// get the name of the folder

const CString&
CFSLocalFolderRefMac::GetName() const
{ 
	return mFolderName; 
}

// ---------------------------------------------------------------------
//  IsEqual		                                       [public][virtual]
// ---------------------------------------------------------------------
// check if the two file references point to the same actual file

bool
CFSLocalFolderRefMac::IsEqual(const CFSRef& inOther) const
{ 
	const CFSLocalFolderRefMac* other = dynamic_cast<const
									CFSLocalFolderRefMac*> (&inOther);
	if (other == nil)
		return false;
	if( mSpecSetup && other->mSpecSetup ){ // use the FSpecs
		if( (mFSpec.vRefNum == other->mFSpec.vRefNum) &&
				(mFSpec.parID == other->mFSpec.parID) ){
			// now compare names using current script
			return ::CompareString( mFSpec.name,
						other->mFSpec.name, nil ) == 0;
		} else {
			return false;
		}
	} else {
		return mFolderName == other->mFolderName;
	}
}	

// ---------------------------------------------------------------------
//  IsLessThan	                                       [public][virtual]
// ---------------------------------------------------------------------
// for ordered pools like std::set or std::map
bool
CFSLocalFolderRefMac::IsLessThan(const CFSRef& inOther) const
{
	const CFSLocalFolderRefMac* other = dynamic_cast<const
						CFSLocalFolderRefMac*> (&inOther);
	if (other == nil)
		return false;
	return mFolderName < other->mFolderName;
}

// ---------------------------------------------------------------------
//  LoadRights			                               [public][virtual]
// ---------------------------------------------------------------------
// load the rights

void
CFSLocalFolderRefMac::LoadRights()
{
	try {
		if( !mRightsLoaded ){
			//?? Look into this later
			mRights.SetRead( true );
			mRights.SetWrite( true );
			mRights.SetVisible( true );
//			SetupSpec(); // Throws if it doesn't exist
//			long ownerID, groupID, accessRights;
//			OSErr anErr = ::FSpGetDirAccess( &mFSpec, &ownerID, &groupID,
//									&accessRights);
//			THROW_OS_( anErr );
//			THROW_OS_( FSpGetDirAccess( &mFSpec, &ownerID, &groupID,
//									&accessRights) );
			mRightsLoaded = true;
		}
		Notify(eRights, mRightsLoaded);
	} catch( ... ){
		RETHROW_FS_( eLoadRights, mFolderName );
	}
}


// ---------------------------------------------------------------------
//  SaveRights			                               [public][virtual]
// ---------------------------------------------------------------------
// save the rights (apply them to the folder)
/*
void
CFSLocalFolderRefMac::SaveRights()
{
	bool saved = false;
	try
	{
		StStringOS fileNameOS(mFolderName);
		
		//FILE_ATTRIBUTE_COMPRESSED
		//FILE_ATTRIBUTE_ENCRYPTED
		DWORD attr = FILE_ATTRIBUTE_DIRECTORY
					|(mRights.CanWrite() ? 0U : FILE_ATTRIBUTE_READONLY)
					|(mRights.IsVisible() ? 0U : FILE_ATTRIBUTE_HIDDEN);
		REQUIRE(0 != ::SetFileAttributes(fileNameOS, attr));
	
		saved = true;
	}
	catch (CException& err)
	{}
	Notify(eWRights, saved);
}
*/

// ---------------------------------------------------------------------
//  GetRights			                               [public][virtual]
// ---------------------------------------------------------------------
// get the previously loaded rights

CFileRights&
CFSLocalFolderRefMac::GetRights()
{
	if( !mRightsLoaded ){
		THROW_UNKNOWN_FS_( eGetRights, mFolderName );
	}
	return mRights;
}


// ---------------------------------------------------------------------
//  LoadContent		                                   [public][virtual]
// ---------------------------------------------------------------------
// retrieve the folder's content
	
void
CFSLocalFolderRefMac::LoadContent()
{
	try {
		if( !mContentLoaded ){
			SetupSpec(); // Throws if it doesn't exist
			CInfoPBRec pb;
			Str255 theName;
			OSErr anErr = noErr;
			Boolean isDirectory;
			long theDirectory;
			// Get the directory ID of the directory
			// the directory ID in the FSpec points to the parent
			THROW_OS_( FSpGetDirectoryID( &mFSpec,  &theDirectory,
					&isDirectory ) );
			if( !isDirectory ){ // the FSpec is not pointing to a directory
				THROW_UNKNOWN_();
			}
			pb.hFileInfo.ioVRefNum = mFSpec.vRefNum;
			pb.hFileInfo.ioNamePtr = (StringPtr)&theName;
			pb.hFileInfo.ioFDirIndex = 0;

			// Spin througn until we get an error, that means we are done
			while( anErr == noErr ){
				pb.hFileInfo.ioDirID = theDirectory;
				pb.hFileInfo.ioFDirIndex++;
				theName[0] = 0;
				anErr = ::PBGetCatInfoSync( &pb );
				if( anErr == noErr ){
					FSSpec theSpec;
					THROW_OS_( ::FSMakeFSSpec( mFSpec.vRefNum, theDirectory,
							theName, &theSpec ) );
					if( (pb.hFileInfo.ioFlAttrib & ioDirMask) != 0 ){ // A folder
						mContent.push_back( new CFSLocalFolderRefMac( theSpec ) );
					} else { // A file
						mContent.push_back( new CFSLocalFileRefMac( theSpec ) );
					}
				}
			}
			mContentLoaded = true;
		}	
		Notify( eLoaded, mContentLoaded );
	} catch( ... ){
		RETHROW_FS_( eLoadContent, mFolderName );
	}
}


// ---------------------------------------------------------------------
//  GetContent		                                   [public][virtual]
// ---------------------------------------------------------------------
// get the items in a folder

const CFSLocalFolderRefMac::Content& 
CFSLocalFolderRefMac::GetContent()
{
	if( !mContentLoaded ){
		THROW_UNKNOWN_FS_( eGetContent, mFolderName );
	}
	return mContent;
}


// ---------------------------------------------------------------------
//  Create                                             [public][virtual]
// ---------------------------------------------------------------------
// create an empty folder, only creates if parent already exists

void
CFSLocalFolderRefMac::Create()
{
	try {
		SetupSpec( false ); // Throws if parent doesn't exist
		long dirID;
		THROW_OS_( ::FSpDirCreate( &mFSpec, smSystemScript, &dirID ) );
		Notify( eCreated, true );
	} catch( ... ){
		RETHROW_FS_( eCreate, mFolderName );
	}
}


// ---------------------------------------------------------------------
//  Delete                                             [public][virtual]
// ---------------------------------------------------------------------
// delete the file and notify the listener

void
CFSLocalFolderRefMac::Delete()
{
	try {
		SetupSpec(); // Throws if it doesn't exist
		// Remove the contents (recursive) then the directory
		THROW_OS_( DeleteDirectoryContents( mFSpec.vRefNum,
								mFSpec.parID, mFSpec.name ) );
		THROW_OS_( DeleteDirectory( mFSpec.vRefNum,
								mFSpec.parID, mFSpec.name ) );
		Notify( eDeleted, true );
	} catch( ... ){
		RETHROW_FS_( eDelete, mFolderName );
	}
}


// ---------------------------------------------------------------------
//  SetupSpec                                                  [private]
// ---------------------------------------------------------------------
// Sets up the FSpec from the folder name if required

void
CFSLocalFolderRefMac::SetupSpec( bool inMustExist ) const
{
	if( !mSpecSetup ){ // Already done?
		OSErr anErr;
		CString name = mFolderName;
		if( name.length() < 256 ){ // try to use the MacOS call
									// only works for Pascal strings
			StPStyleString s( name );
			anErr = ::FSMakeFSSpec( 0, 0, s, &mFSpec );
			if( (anErr == noErr) || ((anErr == fnfErr) && !inMustExist) ){
					// Found it or found its parent if it doesn't need to exist
				mSpecSetup = true;
			} else {
				THROW_OS_( anErr );
			}
		} else { // Use MoreFiles
			StCStyleString s( name );
			anErr = FSpLocationFromFullPath( name.length(), s, &mFSpec );
			if( anErr == noErr ){
				mSpecSetup = true;
			} else {
				THROW_OS_( anErr );
			}
		}
	}
}


// ---------------------------------------------------------------------
//  GetFSpec	                                       [public][virtual]
// ---------------------------------------------------------------------
// get the FSpec of the folder

const FSSpec&
CFSLocalFolderRefMac::GetFSpec() const 
{ 
	try {
		SetupSpec();
	} catch( ... ){
		RETHROW_FS_( eGetName, mFolderName );
	}
	return mFSpec; 
}

HL_End_Namespace_BigRedH
