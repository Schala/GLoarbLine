/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CResourceFile.h"
#include "CStringObject.h"
#include "CResourceList.h"
#include "USyncroHelpers.h"
// temporary : for resource / file implementation
#if TARGET_OS_WIN32
	#include "CFSLocalFileRefWin.h"
#elif TARGET_OS_MAC	
	#include "CFSLocalFileRefMac.h"
#elif TARGET_OS_UNIX	
	#include "CFSLocalFileRefUnix.h"
#endif

HL_Begin_Namespace_BigRedH

const CResourceFile::Type CResourceFile::kString = 'STR_';
const CResourceFile::Type CResourceFile::kPNG    = 'PNG_';
const CResourceFile::Type CResourceFile::kResourceList = 'RLST';


// ---------------------------------------------------------------------
//  CResourceFile                                    			[public]
// ---------------------------------------------------------------------
// Constructor
//
// This is a stupid implementation for resources , 
// it actually uses folders for resource tables and one file
// for each resource.

CResourceFile::CResourceFile( const CFSLocalFileRef &inRef )
: mFolderName(inRef.GetName())
{}

// ---------------------------------------------------------------------
//  ~CResourceFile                                     			[public]
// ---------------------------------------------------------------------
// Destructor

CResourceFile::~CResourceFile()
{}

// ---------------------------------------------------------------------
//  _sTypeToMimeTable                                  			[static]
// ---------------------------------------------------------------------

struct STypeToMime
{
	CResourceFile::Type 		resType;
	CString::value_type*					mimeType;
};

static STypeToMime _sTypeToMimeTable [] = 
{
  	{CResourceFile::kResourceList,	L"resource/list" }
  , {CResourceFile::kString,		L"text/plain" }
  , {CResourceFile::kPNG, 			L"image/png" }
	// WHEN UPDATING this table don't forget to update 
	// Persistence/CFlattenable.cpp _sMimeToObjectTable too
};

// ---------------------------------------------------------------------
//  CreateResourceObject                               		   [private]
// ---------------------------------------------------------------------
// create the resource object out of the Type and ID

std::auto_ptr<CFlattenable>
CResourceFile::CreateResourceObject( CResourceFile::Type inType
									, UInt32 inID )
{
	std::auto_ptr<CFlattenable> theObject;
	try
	{
		CString objType;
		{
			UInt32 foundAt;
			UInt32 foundLimit = sizeof(_sTypeToMimeTable)/sizeof(*_sTypeToMimeTable);
			for( foundAt = 0; foundAt < foundLimit; ++foundAt )
				if ( _sTypeToMimeTable[foundAt].resType == inType )
					break;
			if (foundAt < foundLimit)		
				objType = _sTypeToMimeTable[foundAt].mimeType;
			else
			{
				// this type will not be found in the table
				objType = L"data/unknown";
				// I believe this should never happen...
				ASSERT(false);
			}	
		}
			
		// ==========================================================
		// 			this is the file/resource implementation
		// ==========================================================
		#if TARGET_OS_WIN32
			CString::value_type *separator = L"\\";
		#elif TARGET_OS_MAC	
			CString::value_type *separator = L":";
		#elif TARGET_OS_UNIX	
			CString::value_type *separator = L"/";
		#endif
			
		CString resType;
		{ // convert the resource type id into a folder name (unicode)
			CString::value_type buf[sizeof(inType)+1];
			for(UInt32 k=0; k<sizeof(inType); k++)
			{
				#if TARGET_RT_LITTLE_ENDIAN
					char c = ((char*)&inType)[sizeof(inType)-k-1];
				#else
					char c = ((char*)&inType)[k];
				#endif
				c = (c == 0) ? '_' : (c == ' ') ? '_' : c;
				buf[k] = c;
			}	
			buf[sizeof(inType)] = 0;
			resType = buf;
		}	
	
		// convert the ID to a string so that it can be used as a filename
		CString resID;
		for(;;)
		{
			UInt32 digit = inID % 10;
			resID = CString() + digit[L"0123456789"] + resID;
			inID /= 10;
			if (inID == 0)
				break;
		}

		// 	now get the filename for that resource
		CString filename = mFolderName + separator + resType + separator + resID;
	
		// get the local fsref for the filename
		#if TARGET_OS_WIN32
			CFSLocalFileRefWin fileRef(filename);
		#elif TARGET_OS_MAC	
			CFSLocalFileRefMac fileRef(filename);
		#elif TARGET_OS_UNIX	
			CFSLocalFileRefUnix fileRef(filename);
		#endif
		
		// now that we have the fsref we can finally get the stream
		// we make the assumption that the stream is local since we are 
		// getting it from a local file ref we just created
		fileRef.LoadInputStream();
		// create the object
		theObject = CFlattenable::GetObjectFromType(objType);
		// it's a local file so we can get it right ahead
		// no state machine is required
		theObject->AttachInStream(fileRef.GetInputStream());
	}
	catch (...)
	{
		RETHROW_RESOURCE_(eGet);
	}	

	return theObject;
}								


// ---------------------------------------------------------------------
//  ~ObjPool			                              			[public]
// ---------------------------------------------------------------------
// delete all the objects constructed from this resource file

CResourceFile::ObjPool::~ObjPool()
{
	try
	{
		Item* item = nil;
		for (iterator i(begin()), e(end()); i != e; ++i)
		{
			Item* item = i->second;
			i->second = nil;
			if (item != nil)
				try			{ delete item; }	
				catch (...) { UDebug::Message("delete CResourceFile::Item failed\n"); }
		}	
	}
	catch (...) 
	{	UDebug::Message("CResourceFile::~ObjPool failed\n");	}	
}

// ---------------------------------------------------------------------
//  Item				                              			[public]
// ---------------------------------------------------------------------
// the constructor for an item

CResourceFile::Item::Item( CResourceFile& inParent
						 , Type inType
						 , UInt32 inID )
: mParent(inParent)
, mID(inType, inID)
{
	mFlattenable = inParent.CreateResourceObject(inType, inID);
}

// ---------------------------------------------------------------------
//  GetResourceObject                               		  	[public]
// ---------------------------------------------------------------------
// Find in the object pool the object identified by this type and id 
// and return it.  If the object was not found create one and ad it to 
// the object pool.

CRefObj<CFlattenable>&
CResourceFile::GetResourceObject( CResourceFile::Type inType
								, UInt32 inID )
{
	Item* item = nil;
	try
	{
		{ // thread safe
			StMutex stLock(mLock);
		
			ItemID itemID(inType, inID);
			ObjPool::iterator i = mObjectPool.find(&itemID);
			if (i != mObjectPool.end())
				item = i->second;
			else
			{	// we create a new item (that will call back to CreateResourceObject)
				std::auto_ptr<Item> safeItem(new Item(*this, inType, inID));
				mObjectPool.insert(std::pair<const ItemID* const,Item*>
							(&safeItem->GetID(),safeItem.get()));
				item = safeItem.release(); // we gave ownership to mObjectPool
			}
		} // thread safe
		ASSERT( item != nil );
	}	
	catch (...) 
	{  RETHROW_RESOURCE_(eGet); }	
	return *item;
}								


// ---------------------------------------------------------------------
//  GetStringObject                                  			[public]
// ---------------------------------------------------------------------
// Conveniance function for strings based on GetResourceObject
// Works with the assumption that resource files are local files
// and no notifications need to be received

CString 
CResourceFile::GetResourceString( UInt32 inID )
{
	try
	{
		CRefObj<CFlattenable>& resStr = GetResourceObject(kString, inID);
		CStringObject* strObj = static_cast<CStringObject*>(resStr.GetSibling());
		ASSERT(strObj != nil);
		return *strObj;
	}	
	catch (...)
	{
		RETHROW_RESOURCE_(eGet);
		return CString(); // stupid compiler
	}	
}

// ---------------------------------------------------------------------
//  GetResourceList                                  			[public]
// ---------------------------------------------------------------------
// Conveniance function based on GetResourceObject
// Works with the assumption that resource files are local files
// and no notifications need to be received

CRefObj<CFlattenable>&
CResourceFile::GetResourceList( UInt32 inID )
{
	try
	{
		CRefObj<CFlattenable>& resLst = GetResourceObject(kResourceList, inID);
		(static_cast<CResourceList*>(resLst.GetSibling()))->AttachResourceFile(*this);
		return resLst;
	}	
	catch (...)
	{
		RETHROW_RESOURCE_(eGet);
		return *(CRefObj<CFlattenable>*) nil; // stupid compiler
	}	
}

HL_End_Namespace_BigRedH
