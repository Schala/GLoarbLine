/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "UAWError.h"
#include "CMutex.h"
#include "CThreadException.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
// CMutexPS                                                    [private]
// ---------------------------------------------------------------------
// Platform specific mutex class

class CMutex::CMutexPS
{
	public:
		#define __WIN32_MUTEX__
		#ifdef __WIN32_MUTEX__
		HANDLE				mID;
		#else
		CRITICAL_SECTION	mID;
		#endif
};

// ---------------------------------------------------------------------
//  CMutex                                                      [public]
// ---------------------------------------------------------------------
// Constructor

CMutex::CMutex()
	: mPlatform( new CMutex::CMutexPS )
{
#ifdef __WIN32_MUTEX__
    if((mPlatform->mID = ::CreateMutex(nil, false, nil)) == 0)
	{
		THROW_THREAD_(eCreatingMutex, eUnknownOSError, ::GetLastError());
	}
#else
	::InitializeCriticalSection(&mPlatform->mID);
#endif
}

// ---------------------------------------------------------------------
//  ~CMutex                                                     [public]
// ---------------------------------------------------------------------
// Destructor

CMutex::~CMutex()
{
#ifdef __WIN32_MUTEX__
	ASSERT(mPlatform->mID != 0);
	if(mPlatform->mID != 0)
	{
		::CloseHandle(mPlatform->mID), mPlatform->mID = 0;
	}
#else
	::DeleteCriticalSection(&mPlatform->mID);
#endif
	delete mPlatform;
}

// ---------------------------------------------------------------------
//  Acquire                                                     [public]
// ---------------------------------------------------------------------

void
CMutex::Acquire()
{
#ifdef __WIN32_MUTEX__
	ASSERT(mPlatform->mID != 0);
	if(mPlatform->mID != 0)
	{
		UInt32 rc = ::WaitForSingleObject(mPlatform->mID, INFINITE);
	}
#else
    ::EnterCriticalSection(&mPlatform->mID);
#endif
}

// ---------------------------------------------------------------------
//  TryAcquire                                                  [public]
// ---------------------------------------------------------------------

bool
CMutex::TryAcquire()
{
#ifdef __WIN32_MUTEX__
	ASSERT(mPlatform->mID != 0);
	if(mPlatform->mID != 0)
	{
		UInt32 rc = ::WaitForSingleObject(mPlatform->mID, 0);

		if(rc != WAIT_OBJECT_0 && rc != WAIT_ABANDONED)
		{
			return false;
		}
		
		return true;
	}
#else
	// not relevant
#endif

	return false;
}

// ---------------------------------------------------------------------
//  Release                                                     [public]
// ---------------------------------------------------------------------

void
CMutex::Release()
{
#ifdef __WIN32_MUTEX__
	ASSERT(mPlatform->mID != 0);
	if(mPlatform->mID != 0)
	{
		::ReleaseMutex(mPlatform->mID);
	}
#else
    ::LeaveCriticalSection(&mPlatform->mID);
#endif
}


HL_End_Namespace_BigRedH

