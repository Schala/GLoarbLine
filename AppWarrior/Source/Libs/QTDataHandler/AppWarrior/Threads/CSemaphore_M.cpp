/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CSemaphore.h"
#include <Threads.h>
#include <map>
#include <queue>

HL_Begin_Namespace_BigRedH

class CSemaphore::CSemaphorePS {
	public:
		UInt16						mMaxCount;
			// Keeps Acquire counts with the threads
		std::map<ThreadID,SInt16>	mOwningThreads;
		std::queue<ThreadID>		mWaitingThreads;
};


// ---------------------------------------------------------------------
//  CSemaphore                                                  [public]
// ---------------------------------------------------------------------
// Constructor

CSemaphore::CSemaphore( UInt16 inMaxCount )
	: mPlatform( new CSemaphore::CSemaphorePS )
{
	mPlatform->mMaxCount = inMaxCount;
}


// ---------------------------------------------------------------------
//  ~CSemaphore                                                 [public]
// ---------------------------------------------------------------------
// Destructor

CSemaphore::~CSemaphore()
{
}


// ---------------------------------------------------------------------
//  Acquire                                                     [public]
// ---------------------------------------------------------------------

void
CSemaphore::Acquire()
{
	ThreadID currThread;
	
	::GetCurrentThread( &currThread );
	if( mPlatform->mOwningThreads.find( currThread ) !=
			mPlatform->mOwningThreads.end() ){ // Already owned
		mPlatform->mOwningThreads[ currThread ]++;
	} else if( mPlatform->mOwningThreads.size() <
			mPlatform->mMaxCount ){ // Available
		mPlatform->mOwningThreads[ currThread ] = 1;
	} else {
		::ThreadBeginCritical();
		mPlatform->mWaitingThreads.push( currThread );
		OSErr anErr = ::SetThreadStateEndCritical( currThread,
							kStoppedThreadState, kNoThreadID );
	}
}


// ---------------------------------------------------------------------
//  TryAcquire                                                  [public]
// ---------------------------------------------------------------------

bool
CSemaphore::TryAcquire()
{
	ThreadID currThread;
	
	::GetCurrentThread( &currThread );
	if( mPlatform->mOwningThreads.find( currThread ) !=
			mPlatform->mOwningThreads.end() ){ // Already owned
		mPlatform->mOwningThreads[ currThread ]++;
		return true;
	} else if( mPlatform->mOwningThreads.size() <
			mPlatform->mMaxCount ){ // Available
		mPlatform->mOwningThreads[ currThread ] = 1;
		return true;
	} else {
		return false;
	}
}


// ---------------------------------------------------------------------
//  Release                                                     [public]
// ---------------------------------------------------------------------

void
CSemaphore::Release()
{
	ThreadID currThread, nextOwner;
	
	::GetCurrentThread( &currThread );
	if( mPlatform->mOwningThreads.find( currThread ) ==
			mPlatform->mOwningThreads.end() ){ // Release called from wrong thread
		//?? Throw Exception?
	} else {
		mPlatform->mOwningThreads[ currThread ]--;
		if( mPlatform->mOwningThreads[ currThread ] <= 0 ){
			mPlatform->mOwningThreads.erase( currThread );
			if( !mPlatform->mWaitingThreads.empty() ){
				::ThreadBeginCritical();
				nextOwner = mPlatform->mWaitingThreads.front();
				mPlatform->mWaitingThreads.pop();
				mPlatform->mOwningThreads[ nextOwner ] = 1;
				OSErr anErr = ::SetThreadStateEndCritical( nextOwner,
									kReadyThreadState, kNoThreadID );
			}
		}
	}
}

HL_End_Namespace_BigRedH

