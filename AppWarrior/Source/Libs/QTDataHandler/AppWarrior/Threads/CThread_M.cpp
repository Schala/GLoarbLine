/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CThread.h"
#include "CThreadManager.h"
#include <Processes.h>
#include <Threads.h>
#include <Timer.h>


HL_Begin_Namespace_BigRedH

#if 0
	#pragma mark CThreadLow
#endif
class CThread::CThreadLow {
	public:
		static void			Initialize();

		static pascal void*	Start( void *inArg );

		static void			ThreadResume( ThreadID inThreadID );
		static void			ThreadResume( void* inTaskRef, ThreadID inThreadID );

	#if TARGET_API_MAC_CARBON
		static ThreadEntryUPP sThreadEntryUPP;
	#endif

	private:	
		static ProcessSerialNumber	sPSN;				// ptr to application's PSN
		static void*				sThreadTaskRef;		// thread task ref
};
#if TARGET_API_MAC_CARBON
ThreadEntryUPP CThread::CThreadLow::sThreadEntryUPP = NewThreadEntryUPP(CThread::CThreadLow::Start);
#endif
ProcessSerialNumber	CThread::CThreadLow::sPSN			= {0, kNoProcess};
void*				CThread::CThreadLow::sThreadTaskRef	= nil;

#if 0
	#pragma mark CThreadTimer
#endif

class CThread::CThreadTimer {

	typedef struct {
		CThread::CThreadTimer*	mTimer;
		TMTask					mTask;
		QHdr					mSemQ;
		QElem					mSemEl;
	} SThreadTMTask;

	public:
								CThreadTimer( ThreadID inThreadID,
											UInt32 inMilliSecs,
											void* inTaskRef = nil );
								~CThreadTimer();

	private:
		static TimerUPP			sThreadTimerUPP;	// UPP for time mgr
		
		SThreadTMTask			mTask;
		ThreadID				mThreadID;
		void*					mTaskRef;

		static pascal void		Execute( TMTaskPtr inTask );

		static SThreadTMTask*	GetTimeMgrPtr( TMTaskPtr inTaskPtr );
		void					InsertTimeTask();
		void					PrimeTimeTask( UInt32 inMilliSecs );
		void					RemoveTimeTask();

								CThreadTimer();
};
TimerUPP CThread::CThreadTimer::sThreadTimerUPP	= NewTimerUPP(CThread::CThreadTimer::Execute);


#if 0
	#pragma mark -
#endif
// ---------------------------------------------------------------------
//  Initialize                                         [private][static]
// ---------------------------------------------------------------------
// description

void
CThread::Initialize()
{
	CThreadLow::Initialize();
}


// ---------------------------------------------------------------------
//  Preallocate                                        [private][static]
// ---------------------------------------------------------------------
// description

void
CThread::Preallocate( UInt32 inNum )
{
	THROW_OS_THREAD_( ::CreateThreadPool( kCooperativeThread,
						(SInt16)inNum, 0 ), eIniting );
}


// ---------------------------------------------------------------------
//  GetCurrentID                                       [private][static]
// ---------------------------------------------------------------------
// Get the currenttly executing thread

TThreadID
CThread::GetCurrentID()
{
	ThreadID theThread;
	THROW_OS_THREAD_( ::MacGetCurrentThread( &theThread ), eQuerying );
	return theThread;
}


// ---------------------------------------------------------------------
//  Sleep                                               [public][static]
// ---------------------------------------------------------------------
// Suspends the current thread for a specified amount of time

void
CThread::Sleep( UInt32 inSleepMilliSecs )
{
		// interval is zero -- don't sleep, just blink !
	if( inSleepMilliSecs == 0 ){
		::YieldToAnyThread();
	} else {
		// else sleep the current thread
#if 1
		// try spinning method
		long theEnd = ::TickCount() + ((inSleepMilliSecs * 60L) / 1000L );
		while( ::TickCount() < theEnd ){
			::YieldToAnyThread();
		}
#else
		void *theTaskRef;
		::GetThreadCurrentTaskRef( &theTaskRef );
		CThread *theThread = CThreadManager::Instance().GetCurrentThread();
		if( theThread != nil ){
			::ThreadBeginCritical();
			if( theThread->mTimer != nil ){
				delete theThread->mTimer;
				theThread->mTimer = nil;
			}
			theThread->mTimer = new CThreadTimer( theThread->mID, inSleepMilliSecs,
													theTaskRef );
			OSErr anErr = ::SetThreadStateEndCritical( theThread->mID,
								kStoppedThreadState, kNoThreadID );
			delete theThread->mTimer;
			theThread->mTimer = nil;
		} else { // Could be Application
			ThreadID theThreadID = GetCurrentID();
			if( theThreadID != kNoThreadID ){
				::ThreadBeginCritical();
				CThreadTimer *theTimer = new CThreadTimer( theThreadID, inSleepMilliSecs,
															theTaskRef );
				OSErr anErr = ::SetThreadStateEndCritical( theThreadID,
									kStoppedThreadState, kNoThreadID );
				delete theTimer;
			}
		}
#endif
	}
}


#if 0
	#pragma mark -
#endif
// ---------------------------------------------------------------------
//  CThread                                                     [public]
// ---------------------------------------------------------------------
// Constructor

CThread::CThread( EPriority inPriority )
	: mID( kNoThreadID ), mRunning( true ),
		mTerminated( false ), mTimer(nil), mDone(false)
{
	OSErr anErr = ::NewThread( kCooperativeThread, 
							#if TARGET_API_MAC_CARBON
								CThreadLow::sThreadEntryUPP,
							#else	
								&CThreadLow::Start,
							#endif
								this, 0, kNewSuspend, nil, &mID );

	CThreadManager::Instance().AddThread( this );
}


// ---------------------------------------------------------------------
//  ~CThread                                        [protected][virtual]
// ---------------------------------------------------------------------
// Destructor

CThread::~CThread()
{
	delete mTimer;
	CThreadManager::Instance().RemoveThread( this );
}


// ---------------------------------------------------------------------
//  Suspend                                            [public][virtual]
// ---------------------------------------------------------------------
// Suspend the thread, remove it from the schedule

void
CThread::Suspend()
{
	if( mID != kNoThreadID ){
		OSErr anErr = ::SetThreadState( mID, kStoppedThreadState,
								kNoThreadID );
	}
}


// ---------------------------------------------------------------------
//  Resume                                             [public][virtual]
// ---------------------------------------------------------------------
// Resume the thread, make it ready for scheduling

void
CThread::Resume()
{
	if( mID != kNoThreadID ){
		OSErr anErr = ::SetThreadState( mID, kReadyThreadState,
								kNoThreadID );
		Sleep();
	}
}


// ---------------------------------------------------------------------
//  Execute                                         [protected][virtual]
// ---------------------------------------------------------------------
// Execute the thread. Handle the looping here

UInt32
CThread::Execute()
{
	UInt32 retValue = 0;
	while( IsRunning() && !IsTerminated() ){
		retValue = Run();
	}
	return retValue;
}


#if 0
	#pragma mark -
#endif
// ---------------------------------------------------------------------
//  Terminate                                          [public][virtual]
// ---------------------------------------------------------------------
// Mark the thread as terminated

void
CThread::Terminate()
{
	mTerminated = true;
}


// ---------------------------------------------------------------------
//  Kill                                                       [private]
// ---------------------------------------------------------------------
// Forceable kill the thread

void
CThread::Kill()
{
	if( mID != kNoThreadID ){
		//?? Cleanup any semaphores etc.
		delete mTimer;
		mTimer = nil;
		
		// Kill it
		OSErr anErr = ::DisposeThread( mID, nil, false );
		mID = kNoThreadID;
	}
}


// ---------------------------------------------------------------------
//  IsDone                                                     [private]
// ---------------------------------------------------------------------
// Check if thread has finished executing (as far as TM is concerned)

bool
CThread::IsDone()
{
	return mDone;
}


#if 0
	#pragma mark -
#endif
// ---------------------------------------------------------------------
//  Initialize                                          [public][static]
// ---------------------------------------------------------------------
// Gather any info the ThreadManager needs later

void
CThread::CThreadLow::Initialize()
{
	// low-level initialization
	::MacGetCurrentProcess( &sPSN );
	::GetThreadCurrentTaskRef( &sThreadTaskRef );
}


// ---------------------------------------------------------------------
//  Start                                               [public][static]
// ---------------------------------------------------------------------
// Callback for the TM. This is where the work happens

pascal void*
CThread::CThreadLow::Start( void *inArg )
{
	CThread* volatile theThread	= reinterpret_cast<CThread*>(inArg);
	
	// Initialize
	try {
		theThread->ThreadEnter();
	} catch(...){
	}
	
	// Run it
	UInt32 retValue = 0;
	try {
		retValue = theThread->Execute();
	} catch(...){
	}
	
	// Exit
	if( !theThread->IsTerminated() ){
		try {
			theThread->ThreadExit( retValue );
		} catch( ... ){
		}
	}

	theThread->mDone = true;
	
	return nil;
}


// ---------------------------------------------------------------------------
//  ThreadResume                                                      [static]
// ---------------------------------------------------------------------------
// Resume a thread at interrupt time.

void
CThread::CThreadLow::ThreadResume( ThreadID inThreadID )
{
	ThreadState	state;
	SInt16		err;
	
	// get the thread's state
	err = ::GetThreadStateGivenTaskRef( sThreadTaskRef, inThreadID, &state );
	
	// could we get its state?
	if( err == noErr ){
		// is the thread stopped ?
		if( state == kStoppedThreadState ){
			err = ::SetThreadReadyGivenTaskRef( sThreadTaskRef, inThreadID );
			// wake up this process (so thread can get a chance to run)
//			::WakeUpProcess( &sPSN );
		}
	}
}


// ---------------------------------------------------------------------------
//  ThreadResume                                                      [static]
// ---------------------------------------------------------------------------
// Resume a thread at interrupt time.

void
CThread::CThreadLow::ThreadResume( void *inTaskRef, ThreadID inThreadID )
{
	ThreadState	state;
	SInt16		err;
	
	// get the thread's state
	err = ::GetThreadStateGivenTaskRef( inTaskRef, inThreadID, &state );
	
	// could we get its state?
	if( err == noErr ){
		// is the thread stopped ?
		if( state == kStoppedThreadState ){
			err = ::SetThreadReadyGivenTaskRef( inTaskRef, inThreadID );
			// wake up this process (so thread can get a chance to run)
//			::WakeUpProcess( &sPSN );
		}
	}
}


#if 0
	#pragma mark -
#endif
// ---------------------------------------------------------------------
//  CThreadTimer                                                [public]
// ---------------------------------------------------------------------
// Constructor

CThread::CThreadTimer::CThreadTimer( ThreadID inThreadID,
									UInt32 inMilliSecs,
									void* inTaskRef )
	: mThreadID( inThreadID ), mTaskRef(inTaskRef)
{
	mTask.mTimer = this;

	mTask.mTask.qLink = nil;
	mTask.mTask.qType = 0;
	mTask.mTask.tmAddr = sThreadTimerUPP;
	mTask.mTask.tmCount = 0;
	mTask.mTask.tmWakeUp = 0;
	mTask.mTask.tmReserved = 0;

	mTask.mSemQ.qFlags = 0;
	mTask.mSemQ.qHead = nil;
	mTask.mSemQ.qTail = nil;

	mTask.mSemEl.qLink = nil;
	mTask.mSemEl.qType = 0;

	InsertTimeTask();
	PrimeTimeTask( inMilliSecs );
}


// ---------------------------------------------------------------------
//  ~CThreadTimer                                               [public]
// ---------------------------------------------------------------------
// Destructor

CThread::CThreadTimer::~CThreadTimer()
{
	RemoveTimeTask();
}


// ---------------------------------------------------------------------
//  Execute                                            [private][static]
// ---------------------------------------------------------------------
// Time Manager completion routine.  Converts the given Time Manager 
// task into a SThreadTMTask, then resumes the given thread.

pascal void
CThread::CThreadTimer::Execute( TMTaskPtr inTask )
{
	SThreadTMTask*	tpb = GetTimeMgrPtr(inTask);
	CThread::CThreadTimer *theTimer = tpb->mTimer;
	if( theTimer->mTaskRef != nil ){
		CThreadLow::ThreadResume( theTimer->mTaskRef, theTimer->mThreadID );
	} else {
		CThreadLow::ThreadResume( theTimer->mThreadID );
	}
}


// ---------------------------------------------------------------------
//	GetTimeMgrPtr                                      [private][static]
// ---------------------------------------------------------------------

inline CThread::CThreadTimer::SThreadTMTask*
CThread::CThreadTimer::GetTimeMgrPtr( TMTaskPtr inTaskPtr )
{
	using namespace std;

	return (reinterpret_cast<SThreadTMTask *>(
			reinterpret_cast<char *>(inTaskPtr) - 
			OFFSET_OF(SThreadTMTask, mTask)));
}


// ---------------------------------------------------------------------
//  InsertTimeTask                                             [private]
// ---------------------------------------------------------------------
// Insert the task into the Time Manager's queue.

inline void
CThread::CThreadTimer::InsertTimeTask()
{
	// signal timer "semaphore"
	::Enqueue( &mTask.mSemEl, &mTask.mSemQ );
	
	// add element to timer queue
#if TARGET_API_MAC_CARBON
	::InstallTimeTask(reinterpret_cast<QElemPtr>(&mTask.mTask));
#else
	::InsTime(reinterpret_cast<QElemPtr>(&mTask.mTask));
#endif
}


// ---------------------------------------------------------------------
//  PrimeTimeTask                                              [private]
// ---------------------------------------------------------------------
// Activate the task in the Time Manager's queue.

inline void
CThread::CThreadTimer::PrimeTimeTask( UInt32 inMilliSecs )
{
	::PrimeTime( reinterpret_cast<QElemPtr>(&mTask.mTask),
						inMilliSecs );
}


// ---------------------------------------------------------------------
//  RemoveTimeTask                                             [private]
// ---------------------------------------------------------------------
// Remove the task from the Time Manager's queue.

inline void
CThread::CThreadTimer::RemoveTimeTask()
{
	// attempt to grab timer "semaphore"
	if( ::Dequeue( &mTask.mSemEl, &mTask.mSemQ ) == noErr ){
		::RmvTime( reinterpret_cast<QElemPtr>(&mTask.mTask) );
	}
}

HL_End_Namespace_BigRedH
