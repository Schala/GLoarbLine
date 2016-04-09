/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */// A File stream#include "AW.h"#include "CFileStreamBuffer.h"#include "CFSLocalFileRefWin.h"#include "StString.h"HL_Begin_Namespace_BigRedHclass CFileStreamBuffer::CFileStreamBufferPS{	public:		HANDLE			mHandle;		bool			mEOF;};// ---------------------------------------------------------------------//  CFileStreamBuffer                                        	[public]// ---------------------------------------------------------------------// constructorCFileStreamBuffer::CFileStreamBuffer(): mOpenMode( eNONE ), mPlatform(*new CFileStreamBufferPS ){	mPlatform.mHandle = INVALID_HANDLE_VALUE;	mPlatform.mEOF = false;}// ---------------------------------------------------------------------//  ~CFileStreamBuffer                                 [public][virtual]// ---------------------------------------------------------------------// destructorCFileStreamBuffer::~CFileStreamBuffer(){	try	{		if (IsOpen())			Close();		delete &mPlatform;	}	catch (...) {} // sorry, exceptions don't get out of here	}// ---------------------------------------------------------------------//  Open			                                 			[public]// ---------------------------------------------------------------------// open the filevoidCFileStreamBuffer::Open(const CFSFileRef& inFileRef, OpenMode inMode){	try	{		ASSERT(mPlatform.mHandle == INVALID_HANDLE_VALUE);		ASSERT(inMode != eNONE);		ASSERT(nil != dynamic_cast<const CFSLocalFileRefWin*>(&inFileRef));						DWORD createMode = 0;		switch (mOpenMode = inMode)		{		case eReadOnly:		createMode = OPEN_EXISTING; break;//		case eModify:		createMode = OPEN_ALWAYS; break;		case eAppend:		createMode = OPEN_ALWAYS; break;		case eWrite:		createMode = CREATE_ALWAYS; break;//		case eCreateAlways:	createMode = CREATE_ALWAYS; break;//		case eCreateOrFail:	createMode = CREATE_NEW; break;		}; 			StCStyleString fileNameOS(inFileRef.GetName());		mPlatform.mHandle = ::CreateFile( fileNameOS						,(DWORD) (GENERIC_READ | ((mOpenMode==eReadOnly) ? 0:GENERIC_WRITE))						,(DWORD) ((mOpenMode==eReadOnly) ? FILE_SHARE_READ : 0) // never FILE_SHARE_WRITE						,NULL						,createMode						,(DWORD) (FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL)						,NULL );		if (mPlatform.mHandle == INVALID_HANDLE_VALUE)			THROW_OS_(::GetLastError());			if (mOpenMode == eAppend)		{	// go to the end of the file			UInt64 pos = 0;			0[(UInt32*)&pos]  = ::SetFilePointer(mPlatform.mHandle, 0, &1[(SInt32*)&pos], FILE_END);			OS_ErrorCode error = ::GetLastError();			if ((0[(UInt32*)&pos] == UInt32(-1)) && (NO_ERROR != error))				THROW_OS_(error);		}			// send the apropriate notifications to start the listening devices		if (GetAvailable() > 0)			NotifyDataReceived();		if (mOpenMode != eReadOnly)			NotifyDataSent();	}	catch (...) 	{ 		RETHROW_STREAM_(eOpen); 	}}// ---------------------------------------------------------------------//  IsOpen			                             	   [public][virtual]// ---------------------------------------------------------------------// check if the file is openboolCFileStreamBuffer::IsOpen(){	return mPlatform.mHandle != INVALID_HANDLE_VALUE;}// ---------------------------------------------------------------------//  Close			                             	   [public][virtual]// ---------------------------------------------------------------------// close the filevoidCFileStreamBuffer::Close(){	try	{		bool success = (mPlatform.mHandle != INVALID_HANDLE_VALUE);		if (success)		{			success = (0 != ::CloseHandle(mPlatform.mHandle));			if (! success)				THROW_OS_(::GetLastError());			mPlatform.mHandle = INVALID_HANDLE_VALUE;		}			NotifyStreamClosed(success);	}	catch (...) 	{ 		RETHROW_STREAM_(eClose); 	}}// ---------------------------------------------------------------------//  Write			                             	   [public][virtual]// ---------------------------------------------------------------------// write a chunk of data to the fileUInt32CFileStreamBuffer::Write(const void* inBuf, UInt32 inSize){	DWORD size = 0;	try	{		ASSERT(mOpenMode != eReadOnly);		if (0 == ::WriteFile(mPlatform.mHandle,inBuf,inSize,&size,NULL))			THROW_OS_(::GetLastError());		ASSERT(inSize == size); // disk full ?	}	catch (...) 	{ 		RETHROW_STREAM_(eWrite); 	}	return size;}// ---------------------------------------------------------------------//  CanWriteSomeMore                            	   [public][virtual]// ---------------------------------------------------------------------// check if a future write is allowed or notboolCFileStreamBuffer::CanWriteSomeMore(){	ASSERT (mPlatform.mHandle != INVALID_HANDLE_VALUE);	ASSERT (mOpenMode != eReadOnly);	return true;}// ---------------------------------------------------------------------//  Read			                            	   [public][virtual]// ---------------------------------------------------------------------// read a block of data from the fileUInt32CFileStreamBuffer::Read(void* inBuf, UInt32 inSize){	DWORD size = 0;	try	{		if (0 == ::ReadFile(mPlatform.mHandle,inBuf,inSize,&size,NULL))			THROW_OS_(::GetLastError());		mPlatform.mEOF = (size < inSize); // we reached EOF	}	catch (...) 	{ 		RETHROW_STREAM_(eRead); 	}	return size;}// ---------------------------------------------------------------------//  Read			                            	   [public][virtual]// ---------------------------------------------------------------------// How much more data is available in the file// If the answer exceeds 4Gb then the return value will be 4Gb-1// because we don't really care about the exact amount of data that is // available but rather if a specific amount is available or not.UInt32CFileStreamBuffer::GetAvailable(){	UInt64 size = 0;	try	{		::SetLastError(NO_ERROR); // NT requires this W98 does not		0[(UInt32*)&size]  = ::GetFileSize(mPlatform.mHandle, &1[(UInt32*)&size]);		OS_ErrorCode error = ::GetLastError();		if ((0[(UInt32*)&size] == UInt32(-1)) && (NO_ERROR != error))			THROW_OS_(error);			UInt64 pos = 0;		0[(UInt32*)&pos]  = ::SetFilePointer(mPlatform.mHandle, 0, &1[(SInt32*)&pos], FILE_CURRENT);		error = ::GetLastError();		if ((0[(UInt32*)&pos] == UInt32(-1)) || (NO_ERROR != error))			THROW_OS_(error);			size -= pos;		mPlatform.mEOF = (size == 0);	}		catch (...) 	{ 		RETHROW_STREAM_(eCanRead); 	}	//?? we limit ourselves to 4G	return (size > UInt32(-1)) ? UInt32(-1) : UInt32(size);}// ---------------------------------------------------------------------//  SetExpect		                            	   [public][virtual]// ---------------------------------------------------------------------// set the amount of data that has to be available before a DataReceived// notification is sent// Local files do not send notifications so we ignore this function callvoidCFileStreamBuffer::SetExpect(UInt32 inSize){	//we don't do anything in here}// ---------------------------------------------------------------------//  IsEOF			                            	   [public][virtual]// ---------------------------------------------------------------------// check if the file pointer has reached the end of file markerboolCFileStreamBuffer::IsEOF(){	ASSERT (mPlatform.mHandle != INVALID_HANDLE_VALUE);	return mPlatform.mEOF;}HL_End_Namespace_BigRedH