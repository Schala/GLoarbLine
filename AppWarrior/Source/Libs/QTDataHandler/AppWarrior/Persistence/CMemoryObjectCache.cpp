/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// The generic file reference helper functions

#include "AW.h"
#include "CMemoryObjectCache.h"
#include "CPersistenceException.h"
#include "USyncroHelpers.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  Instance		                                	[public][static]
// ---------------------------------------------------------------------
// singleton management

std::auto_ptr<CMemoryObjectCache> CMemoryObjectCache::sInstance(nil);

CMemoryObjectCache& CMemoryObjectCache::Instance()
{
	try
	{
		if (nil == sInstance.get())
			sInstance = std::auto_ptr<CMemoryObjectCache>(new CMemoryObjectCache);
	}
	catch (...) { RETHROW_PERSISTENCE_(eCreateFactory); }
	return *sInstance.get();
}

void CMemoryObjectCache::ClearInstance()
{
	try
	{
		if (nil != sInstance.get())
			delete sInstance.release();
	}		
	catch (...) { RETHROW_PERSISTENCE_(eDeleteFactory); }
}

// ---------------------------------------------------------------------
//  CMemoryObjectCache::Items::~Items						
// ---------------------------------------------------------------------
// items destructor

CMemoryObjectCache::Items::~Items()
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
				catch (...) { UDebug::Message("delete CMemoryObjectCache::Item failed\n"); }
		}	
	}
	catch (...) 
	{
		UDebug::Message("CMemoryObjectCache::~Items failed\n");
	}	
}

// ---------------------------------------------------------------------
//  Item						
// ---------------------------------------------------------------------
// item destructor
CMemoryObjectCache::Item::Item(CMemoryObjectCache& inParent, const CFSFileRef& inRef)
: mState(eStart)
, mFileRef(static_cast<CFSFileRef*> (inRef.Clone().release()))
, mFlattenable( nil )
, mParent(inParent)
{}

// ---------------------------------------------------------------------
//  ~Item						
// ---------------------------------------------------------------------
// item destructor
CMemoryObjectCache::Item::~Item()
{}


// ---------------------------------------------------------------------
//  Message														[public]
// ---------------------------------------------------------------------
// Message constructor
CMemoryObjectCache::Message::Message(  UInt32 inID
									, CMemoryObjectCache& inSource
									, CFSFileRef& inDestination
									, bool inSuccess )
: CMessage(inID)
, mSource(inSource)
, mDestination((CFSFileRef*)inDestination.Clone().release())
, mSuccess(inSuccess)
{}	

// ---------------------------------------------------------------------
//  Message														[public]
// ---------------------------------------------------------------------
// Message copy constructor
CMemoryObjectCache::Message::Message( const Message& inMsg )
: CMessage(inMsg)
, mSource(inMsg.mSource)
, mDestination((CFSFileRef*)inMsg.mDestination->Clone().release())
, mSuccess(inMsg.mSuccess)
{}	

// ---------------------------------------------------------------------
//  ~Message										   [public][virtual]
// ---------------------------------------------------------------------
// Message destructor
CMemoryObjectCache::Message::~Message()
{ 
	try
	{
		delete mDestination; 
		mDestination = nil; 
	}
	catch (...)	
	{
		UDebug::Message("CMemoryObjectCache::~Message failed\n");
	}	
}

// ---------------------------------------------------------------------
//  Clone											   [public][virtual]
// ---------------------------------------------------------------------
// Message clone method

CMessage*
CMemoryObjectCache::Message::Clone() const
{ 
	return new Message( *this );
}


// ---------------------------------------------------------------------
//  CMemoryObjectCache                                		 [protected]
// ---------------------------------------------------------------------
// constructor

CMemoryObjectCache::CMemoryObjectCache()
{}

// ---------------------------------------------------------------------
//  ~CMemoryObjectCache                                			[public]
// ---------------------------------------------------------------------
// destructor

CMemoryObjectCache::~CMemoryObjectCache()
{}

// ---------------------------------------------------------------------
//  ListenToMessage	                                   		 [protected]
// ---------------------------------------------------------------------
// listens to messages from the FSFileRef

void
CMemoryObjectCache::Item::ListenToMessage( const CMessage &inMessage )
{
	try
	{
		bool success = false;
		const CFSRef::Message* msg = nil;
		switch (inMessage.GetID())
		{
		case CFSFileRef::eType:
				msg	= static_cast <const CFSRef::Message*> (&inMessage);
				if(mFileRef.get() == &msg->GetSource())
					success = msg->GetMsg();
				break;
		case CFSFileRef::eInput:
				msg	= static_cast <const CFSRef::Message*> (&inMessage);
				if(mFileRef.get() == &msg->GetSource())
					success = msg->GetMsg();
				break;	
		default:
				return;		
		}		
		if (success)
			NextState();
		else
			mParent.OnCompleted(*this,false);
	}
	catch (...) { RETHROW_MESSAGE_(eListenToMessage); }
}

// ---------------------------------------------------------------------
//  NextState		                                   		 [protected]
// ---------------------------------------------------------------------
// listens to messages from the FSFileRef

void
CMemoryObjectCache::Item::NextState()
{
	try
	{
		switch (mState)
		{
		case eStart:
			mState = eLoadType;
			mFileRef->AddListener(*this);
			mFileRef->LoadType();
			break;
		case eLoadType:
			mState = eLoadInStream;
			mFileRef->LoadInputStream();
			break;
		case eLoadInStream:
			mState = eCompleted;
			mFlattenable = CFlattenable::GetObjectFromType(mFileRef->GetFileType());
			// notify the parent and allow the object's user to link to it
			mParent.OnCompleted(*this,true);
			// make object listen to data comming from the stream
			if (mFlattenable.get() != nil)
				mFlattenable->AttachInStream(mFileRef->GetInputStream());
			break;
		case eCompleted:	
		default:
			ASSERT(false);
		} // switch
	}
	catch (...) { RETHROW_PERSISTENCE_(eCreateObject); }	
}

// ---------------------------------------------------------------------
//  OnCompleted		                                   		   [private]
// ---------------------------------------------------------------------
// notification received from the item when it completed the download
// or when failed to complete one of it's internal steps

void 
CMemoryObjectCache::OnCompleted(  CMemoryObjectCache::Item& inItem
								, bool inSuccess )
{
	try
	{
		Message msg(eObject, *this, inItem.GetFileRef(), inSuccess);
		BroadcastMessage(msg);
	}	
	catch (...) { RETHROW_PERSISTENCE_(eCreateObject); }	
}

// ---------------------------------------------------------------------
//  LoadObject	                                       			[public]
// ---------------------------------------------------------------------
// load the object, notify when done

void
CMemoryObjectCache::LoadObject(CFSFileRef& inFileRef)
{
	try
	{
		Item* item = nil;
		{ // thread safe
			StMutex stLock(mLock);
		
			Items::iterator i = mItems.find(&inFileRef);
			if (i != mItems.end())
				item = i->second;
		
			if (item == nil)
			{
				std::auto_ptr<Item> safeItem(new Item(*this, inFileRef));
				// mItems is now the owner of the item
				mItems.insert(std::pair<CFSFileRef* const,Item*>(&safeItem->GetFileRef(),safeItem.get()));
				item = safeItem.release(); // ownership was transfered to mItems
				item->NextState(); // start the download
			}
			else 
			{	// although found the object may not be completely created
				if (! item->IsComplete())
					item = nil;
			}
		} // thread safe	
		if (item != nil)
		{
			Message msg(eObject, *this, item->GetFileRef(), true);
			BroadcastMessage(msg);
		}	
	}	
	catch (...) { RETHROW_PERSISTENCE_(eCreateObject); }	
}

// ---------------------------------------------------------------------
//  GetObject	                                       			[public]
// ---------------------------------------------------------------------
// ask the memory cache to return the object this FSRef identifies
CRefObj<CFlattenable>&
CMemoryObjectCache::GetLoadedObject(CFSFileRef& inFileRef)
{
	Item* item = nil;
	try
	{
		{ // thread safe	
			StMutex stLock(mLock);
			Items::iterator i = mItems.find(&inFileRef);
			ASSERT (i != mItems.end());
			item = i->second;
		} // thread safe	
		ASSERT ((item != nil) && item->IsComplete());
	}	
	catch (...) { RETHROW_PERSISTENCE_(eCreateObject); }	
	return *item;
}


HL_End_Namespace_BigRedH
