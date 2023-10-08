/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CThreadManager.h"
#include "CThread.h"
#include "USyncroHelpers.h"


using namespace std;

HL_Begin_Namespace_BigRedH

	// Singleton holder
std::auto_ptr<CThreadManager> CThreadManager::sInstance(0);


// ---------------------------------------------------------------------
//  Instance                                            [public][static]
// ---------------------------------------------------------------------
// Singleton instance access

CThreadManager&
CThreadManager::Instance()
{
	if( sInstance.get() == 0 ){
		sInstance.reset( new CThreadManager );
	}
	return *sInstance;
}

// ---------------------------------------------------------------------
//  ClearInstance                                       [public][static]
// ---------------------------------------------------------------------
// Singleton clear

void
CThreadManager::ClearInstance()
{
	if( sInstance.get() == 0 ){
		sInstance = std::auto_ptr<CThreadManager> (nil);
	}
}


// ---------------------------------------------------------------------
//  CThreadManager                                             [private]
// ---------------------------------------------------------------------
// Constructor

CThreadManager::CThreadManager()
{
	CThread::Initialize();
}


// ---------------------------------------------------------------------
//  ~CThreadManager                                             [public]
// ---------------------------------------------------------------------
// Destructor

CThreadManager::~CThreadManager()
{
//	RunCleanup();
}


// ---------------------------------------------------------------------
//  Preallocate                                                 [public]
// ---------------------------------------------------------------------
// Adds a thread to the thread list

void
CThreadManager::Preallocate( UInt32 inNum )
{
	CThread::Preallocate( inNum );
}


// ---------------------------------------------------------------------
//  GetCurrentThread                                            [public]
// ---------------------------------------------------------------------
// Gets a point to the current CThread object

CThread*
CThreadManager::GetCurrentThread()
{
	TThreadID theThreadID = CThread::GetCurrentID();

	StMutex lock(mTLock);
	if( mThreads.find( theThreadID ) != mThreads.end() ){
		return mThreads[theThreadID];
	} else {
		return nil;
	}
}


// ---------------------------------------------------------------------
//  AddThread                                                   [public]
// ---------------------------------------------------------------------
// Adds a thread to the thread list

void
CThreadManager::AddThread( CThread *inThread )
{
	StMutex lock(mTLock);
	mThreads[inThread->mID] = inThread;
}


// ---------------------------------------------------------------------
//  RemoveThread                                                [public]
// ---------------------------------------------------------------------
// Removes a thread from the thread list

void
CThreadManager::RemoveThread( CThread *inThread )
{
	StMutex lock(mTLock);
	mThreads.erase( inThread->mID );
}


// ---------------------------------------------------------------------
//  TerminateAll                                                [public]
// ---------------------------------------------------------------------
// Terminates all the threads

void
CThreadManager::TerminateAll()
{
	TContainer::iterator it;

	StMutex lock(mTLock);
	for( it = mThreads.begin(); it != mThreads.end(); it++ ){
		(it->second)->Terminate();
	}
}


// ---------------------------------------------------------------------
//  KillAll                                                     [public]
// ---------------------------------------------------------------------
// Kills (forceably) all the threads

void
CThreadManager::KillAll()
{
	TContainer::iterator it;

	StMutex lock(mTLock);
	RunCleanup();
	for( it = mThreads.begin(); it != mThreads.end(); it++ ){
		(it->second)->Kill();
	}
}


// ---------------------------------------------------------------------
//  RunCleanup                                                  [public]
// ---------------------------------------------------------------------
// Deletes all finished threads

void
CThreadManager::RunCleanup()
{
	list<CThread*> deadThreads;

	{ // thread safe lock
		StMutex lock(mTLock);
		bool foundSomeDeadThreads = false;
		{
			TContainer::iterator i;
			for( i = mThreads.begin(); i != mThreads.end(); i++ ){
				if( (i->second)->IsDone() ){
					foundSomeDeadThreads = true;
					break;
				} 
			}
		}
		// if we did not find any dead threads we can quickly return 
		if (! foundSomeDeadThreads)
			return;
			
		// A lot of work ahead: remove all 	the dead threads from mThreads
		//TContainer allThreads;
		//TContainer::iterator i;
		//copy( mThreads.begin(), mThreads.end(), inserter(allThreads, allThreads.begin()) );
		
		std::list<TContainer::value_type> allThreads;
		std::list<TContainer::value_type>::iterator i;
		copy( mThreads.begin(), mThreads.end(), inserter( allThreads, allThreads.begin()) );
		
		mThreads.clear();
		ASSERT(mThreads.empty());
		
		for( i = allThreads.begin(); i != allThreads.end(); i++ ){
			if( (i->second)->IsDone() ){
				// put the dead threads a list
				deadThreads.push_back( i->second );
				i->second = nil;
				break;
			} else {
				// put the live threads back into mThreads
				mThreads.insert(*i);
			}
		}
	} // thread safe lock

	// we found some dead threads so now we delete them
	list<CThread*>:: iterator it;
	for( it = deadThreads.begin(); it != deadThreads.end(); it++ ){
		delete *it;
	}
}

HL_End_Namespace_BigRedH
