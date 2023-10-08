/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// A memory stream that looks like a CInStream at one 
// end and like a COutStream at the other.

#ifndef _H_CMemoryStreamBuffer_
#define _H_CMemoryStreamBuffer_

#include "CAsyncStreamBuffer.h"

#if PRAGMA_ONCE
	#pragma once
#endif
			   
HL_Begin_Namespace_BigRedH

class CMemoryStreamBuffer : public CAsyncStreamBuffer
{
	public:
							CMemoryStreamBuffer();
								// throws ???
		virtual 			~CMemoryStreamBuffer();
								// throws nothing

		void 				Open();
								// throws CStreamException
		virtual bool		IsOpen();
								// throws CStreamException
		virtual void		Close();
								// throws CStreamException
		virtual UInt32 		Write(const void* inBuf, UInt32 inSize);
								// throws CStreamException
		virtual bool		CanWriteSomeMore();
								// throws CStreamException
		virtual UInt32		Read(void* inBuf, UInt32 inSize);
								// throws CStreamException
		virtual UInt32		GetAvailable();
								// throws CStreamException
		virtual void 		SetExpect(UInt32 inSize);
								// throws CStreamException
		virtual bool		IsEOF();
								// throws CStreamException

	private:
		UInt32				mAvailable;
		UInt32				mExpect;
		UInt32				mOffset;
		bool				mWroteEOF;
		bool				mNotifyReader;
		bool				mNotifyWriter;
		enum			{	eBlockSize = 4096-64 };
		struct DataBlock
		{
			DataBlock*		mNext;
			UInt32			mSize;
			UInt8			mData[1];
		};
		DataBlock*			mFirst;
		DataBlock*			mLast;
};

HL_End_Namespace_BigRedH
#endif // _H_CMemoryStreamBuffer_
