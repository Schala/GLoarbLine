// =====================================================================
//  CAsyncReader.cpp               (C) Hotline Communications 2000
// =====================================================================
//
//  Implementation of CAsyncReader class
//  (singleton, serves asynchronous requests from all connections )

//-----------------------------------------------------------------------
// includes

#include "aw.h"
#undef PRAGMA_ONCE // to avoid macro redefinition warning
#include <Components.h>
#include <QuickTimeComponents.h>
#include "CAsyncReader.h"
#include "USyncroHelpers.h"

HL_Begin_Namespace_BigRedH

std::auto_ptr<CAsyncReader> CAsyncReader::sInstance( nil ); // singleton

// ---------------------------------------------------------------------
//  CDataRequest::CDataRequest								    [public]
// ---------------------------------------------------------------------
// initialization constructor for CDataRequest helping class
CAsyncReader::CDataRequest::CDataRequest(
			CDataHandlerConnection* inConnection,
			CDataProvider* inDataProvider,
	        Ptr inPlaceToPutData,
		    UInt32 inFileOffset,
			UInt32 inDataSize,
			UInt32 inRefCon,
			DataHSchedulePtr inScheduleRec,
			DataHCompletionUPP inCompletionRtn):
	mSuccess(false),
	mConnection(inConnection),
    mDataProvider (inDataProvider),
	mPlaceToPutData(inPlaceToPutData),
	mFileOffset(inFileOffset),
	mDataSize(inDataSize),
	mRefCon(inRefCon),
	mScheduleRec(*inScheduleRec),
	mCompletionRtn(inCompletionRtn)
{
	mDataProvider->AddRef();
}


CAsyncReader::CDataRequest::~CDataRequest()
{
	static CMutex mCallback;
	{
		StMutex lock(mCallback);
		UDebug::Message("Request (%x) finished\n",mPlaceToPutData);
		CallDataHCompletionProc(
			mCompletionRtn, 
			mPlaceToPutData, 
			mRefCon, 
			mSuccess?noErr:readErr);
		UDebug::Message("Request (%x) really finished\n",mPlaceToPutData);
	}
	mDataProvider->Release();
}


// ---------------------------------------------------------------------
//  Instance                                            [public][static]
// ---------------------------------------------------------------------
// returns the unique instance of Async Reader
CAsyncReader&
CAsyncReader::Instance()
{
	if( sInstance.get() == 0 ){
		sInstance = std::auto_ptr<CAsyncReader> ( new CAsyncReader );
	}
	return *sInstance;
}


// ---------------------------------------------------------------------
//  CAsyncReader											    [public]
// ---------------------------------------------------------------------
// constructor
CAsyncReader::CAsyncReader(): 
	CThread()
{
	mCurrentRequest = nil;
}


// ---------------------------------------------------------------------
//  ~CAsyncReader										    [public]
// ---------------------------------------------------------------------
// destructor
CAsyncReader::~CAsyncReader()
{
	Quit();
	Resume();

	// Spin until thread terminates
	while( !IsDone() ){
		CThread::Sleep();
	}

	std::list<CDataRequest*>::iterator it = mRequests.begin();
	while (it!=mRequests.end())
		delete *it++;
	mRequests.clear();
}



// ---------------------------------------------------------------------
//  Read													    [public]
// ---------------------------------------------------------------------
// Clients of that class use Read function to initiate asynchronous reading.
bool 
CAsyncReader::Read(
				CDataHandlerConnection* inConnection,
				CDataProvider*		inProvider, 
				Ptr					inPtr, 
				UInt32				inFileOffset, 
				UInt32				inDataSize,
		        UInt32				inRefCon,
				DataHSchedulePtr	inScheduleRec,
				DataHCompletionUPP	inCompletionRtn)
{
	CDataRequest* new_request = new CDataRequest(
		inConnection, inProvider, inPtr, inFileOffset, 
		inDataSize, inRefCon, inScheduleRec, inCompletionRtn);

	{
		StMutex lock(mListLock);
		mRequests.push_back(new_request);
	}
	Resume();
	return true;
}




// ---------------------------------------------------------------------
//  Run														 [protected]
// ---------------------------------------------------------------------
// Threaded gettting of data from an synchronous data source
UInt32
CAsyncReader::Run()
{
	mCurrentRequest = nil;

	// 1. Stop if there is nothing to do
	/*
	if (mRequests.size()==0)
		Suspend();
	*/

	if( IsRunning() )
	{
		CThread::Sleep(); // Yield

		// 2. Get a pointer to the current request
		{
			StMutex lock(mListLock);

			std::list<CDataRequest*>::iterator it = mRequests.begin();
			
			while (mCurrentRequest==nil && it!=mRequests.end() && IsRunning())
			{
				if ((*it)->mScheduleRec.timeNeededBy.scale!=-1 &&
					(*it)->mDataProvider->GetAvailableSize() >= (*it)->mFileOffset+(*it)->mDataSize)
				{
					mCurrentRequest = *it;
					mRequests.remove(mCurrentRequest);
				}
				else
					it++;
			}
		}


		// 3. Reading
		if (mCurrentRequest!=nil && IsRunning() )
		{
			mCurrentRequest->mSuccess = mCurrentRequest->mDataProvider->Read(
				mCurrentRequest->mPlaceToPutData, 
				mCurrentRequest->mFileOffset, 
				mCurrentRequest->mDataSize);
		}

		delete mCurrentRequest, mCurrentRequest=nil;
	}



	return 0;
}	


// ---------------------------------------------------------------------
//  SpeedUpData												    [public]
// ---------------------------------------------------------------------
// Clients use this function to increase priority of the previous request
bool 
CAsyncReader::SpeedUpData (Ptr inPlaceToPutData)
{

	CDataRequest* req = nil;
	UDebug::Message("Speed Up (%x)\n", inPlaceToPutData);
	{
		StMutex lock(mListLock);

		while (mCurrentRequest != nil && 
				mCurrentRequest->mPlaceToPutData==inPlaceToPutData)
		{
			CThread::Sleep();
		}

		std::list<CDataRequest*>::iterator it = mRequests.begin();
		while (req==nil && it!=mRequests.end())
		{
			if ((*it)->mPlaceToPutData == inPlaceToPutData)
			{
				req = *it;
				mRequests.remove(req);
			}
			else
				it++;
		}
	}

	if (req)
	{
		req->mSuccess = req->mDataProvider->Read(
				req->mPlaceToPutData, 
				req->mFileOffset, 
				req->mDataSize);
		delete req;
	}

	UDebug::Message("Speed Up (%x) finished\n", inPlaceToPutData);
	return true;
}


// ---------------------------------------------------------------------
//  FinishData												    [public]
// ---------------------------------------------------------------------
// Clients use this function to cancel the previous request
bool
CAsyncReader::FinishData (Ptr inPlaceToPutData)
{
	UDebug::Message("Finish Data (%x)\n", inPlaceToPutData);

	StMutex lock(mListLock);

	while (mCurrentRequest != nil && 
			mCurrentRequest->mPlaceToPutData==inPlaceToPutData)
	{
		CThread::Sleep();
	}

	std::list<CDataRequest*>::iterator it = mRequests.begin();
	while (it!=mRequests.end())
	{
		CDataRequest* req = *it++;
		if (inPlaceToPutData==req->mPlaceToPutData)
		{
			mRequests.remove(req);
			delete req;
		}
	}

	UDebug::Message("Finish Data (%x) finished\n", inPlaceToPutData);
	return true;
}


// ---------------------------------------------------------------------
//  FinishAll												    [public]
// ---------------------------------------------------------------------
// Clients use this function to cancel all previous requests
bool 
CAsyncReader::FinishAll(CDataHandlerConnection* inConnection)
{
	UDebug::Message("Finish All\n");

	StMutex lock(mListLock);

	while (mCurrentRequest)
	{
		CThread::Sleep();
	}

	std::list<CDataRequest*>::iterator it = mRequests.begin();
	while (it!=mRequests.end())
	{
		CDataRequest* req = *it++;
		if (inConnection==req->mConnection)
		{
			mRequests.remove(req);
			delete req;
		}
	}

	UDebug::Message("Finish All finished\n");
	return true;
}





HL_End_Namespace_BigRedH
