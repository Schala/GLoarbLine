/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// A memory stream that looks like a CInStream at one 
// end and like a COutStream at the other.

#include "AW.h"
#include "CMemoryStreamBuffer.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  CMemoryStreamBuffer                                        	[public]
// ---------------------------------------------------------------------
// constructor

CMemoryStreamBuffer::CMemoryStreamBuffer()
: mWroteEOF(false)
, mNotifyReader(true)
, mNotifyWriter(true)
, mFirst(nil), mLast(nil)
, mAvailable(0), mOffset(0), mExpect(1)
{}

// ---------------------------------------------------------------------
//  CMemoryStreamBuffer                                [public][virtual]
// ---------------------------------------------------------------------
// constructor
CMemoryStreamBuffer::~CMemoryStreamBuffer()
{
	try
	{
		Close();
		UInt8 buf[1024];
		while(0 != Read(buf,sizeof(buf))){};
	}
	catch(...){} // nothing goes out	
}

// ---------------------------------------------------------------------
//  Open				                               [public][virtual]
// ---------------------------------------------------------------------
// open the stream (in our case just send a notification to writer)

void 
CMemoryStreamBuffer::Open()
{ 
	try
	{
		// generate a DataSent notification to initiate
		// the connected writer
		if (mNotifyWriter)
			NotifyDataSent();
		// if the writer wrote some data and the Expect condition is met
		// the reader was also notified so there's nothing left for us to do	
	}
	catch (...) { RETHROW_STREAM_(eOpen); }
}

// ---------------------------------------------------------------------
//  IsOpen				                               [public][virtual]
// ---------------------------------------------------------------------
// check if the stream is opened

bool
CMemoryStreamBuffer::IsOpen()
{ 
	return true; 
}

// ---------------------------------------------------------------------
//  Close				                               [public][virtual]
// ---------------------------------------------------------------------
// close the stream (write the end of file)

void
CMemoryStreamBuffer::Close()
{ 
	mWroteEOF = true; 
}

// ---------------------------------------------------------------------
//  Write				                               [public][virtual]
// ---------------------------------------------------------------------
// write a block of data to stream

UInt32
CMemoryStreamBuffer::Write(const void* inBuf, UInt32 inSize)
{ 
	using namespace std;
	UInt32 written = 0;
	try
	{
		mNotifyWriter = false;
		UInt8* bufPtr = (UInt8*)inBuf;
		if (mLast && eBlockSize > mLast->mSize)
		{
			UInt32 z = eBlockSize-mLast->mSize;
			if (inSize < z) z = inSize;
			if (z > 0)
			{
				std::memcpy( mLast->mData+mLast->mSize, bufPtr, z );
				inSize  -= z;
				written += z;
				bufPtr  += z;
				mLast->mSize += z;
			} 
		}	
		if (inSize > 0)
		{
			UInt32 z = inSize;
			if (z < eBlockSize) z = eBlockSize;
			DataBlock* block = (DataBlock*) new UInt8[sizeof(DataBlock)-1+z];
			std::memcpy( block->mData, bufPtr, inSize );
			block->mNext = nil;
			block->mSize = inSize;
			written += inSize;
			if (mLast)
				mLast->mNext = block;
			else
				mFirst = block;	
			mLast = block;	
		}
		mAvailable += written;
		#if DEBUG
			if (mAvailable > 1024*1024) // 1MB at most
				UDebug::Message("Hey you're writing to big chunks!!\n");
		#endif	
		//always when new data in 
		//mNotifyReader = true; 
		if ((mAvailable >= mExpect)&& mNotifyReader)
			NotifyDataReceived();
	}
	catch (...) { RETHROW_STREAM_(eWrite); }
	return written;
}

// ---------------------------------------------------------------------
//  CanWriteSomeMore	                               [public][virtual]
// ---------------------------------------------------------------------
// check if it's possible to write some more
// if the answer is no a notification will be sent later

bool
CMemoryStreamBuffer::CanWriteSomeMore()
{ 
	return !(mNotifyWriter=!(mAvailable < 0x10000)); 
}

// ---------------------------------------------------------------------
//  Read				                               [public][virtual]
// ---------------------------------------------------------------------
// read a block of data

UInt32
CMemoryStreamBuffer::Read(void* inBuf, UInt32 inSize)
{
	using namespace std;
	UInt32 read = 0;	
	try
	{
		mNotifyReader = false;
		if ((mAvailable == 0)&&(mNotifyWriter))
			NotifyDataSent();
		UInt8* bufPtr = (UInt8*) inBuf;
		while (mAvailable > 0 && inSize > 0)
		{
			ASSERT(mFirst != nil);
	
			UInt32 z = mFirst->mSize-mOffset;
			if (z > inSize)
				z = inSize;
	
			std::memcpy( bufPtr, mFirst->mData+mOffset, z );
			mOffset+=z;
			mAvailable -= z;
			read   +=z;
			bufPtr +=z;
			inSize -=z;
			if (mOffset == mFirst->mSize)
			{
				UInt8* tmp = (UInt8*)mFirst;
				mFirst = mFirst->mNext;
				if (mFirst == nil)
					mLast = nil;
				mOffset = 0;
				delete tmp;
			}	
		}
		mExpect = 1; // reset expectations to 1 byte
		if ((mAvailable == 0)&&(mNotifyWriter))
			NotifyDataSent();
	}
	catch (...) { RETHROW_STREAM_(eRead); }
	return read;	
}

// ---------------------------------------------------------------------
//  GetAvailable   		                               [public][virtual]
// ---------------------------------------------------------------------
// check how much data is available for reading without blocking
// if the answer is less than the expected amount a future notification 
// will be sent

UInt32
CMemoryStreamBuffer::GetAvailable()
{ 
	try
	{
		mNotifyReader = false;
		if ((mAvailable == 0)&&(mNotifyWriter))
			NotifyDataSent();
		mNotifyReader = !(mAvailable >= mExpect);
	}
	catch (...) { RETHROW_STREAM_(eRead); }
	return mAvailable; 
}

// ---------------------------------------------------------------------
//  SetExpect			                               [public][virtual]
// ---------------------------------------------------------------------
// set the expected size of data that must be available for a read 
// notification to be triggered

void
CMemoryStreamBuffer::SetExpect(UInt32 inSize)
{ 
	mExpect = inSize;
}

// ---------------------------------------------------------------------
//  IsEOF				                               [public][virtual]
// ---------------------------------------------------------------------
// did we reach the EOF while reading
bool
CMemoryStreamBuffer::IsEOF()
{ 
	return mWroteEOF && (mAvailable == 0); 
}

HL_End_Namespace_BigRedH
