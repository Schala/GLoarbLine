// =====================================================================
//	CStorage.h                          (C) Hotline Communications 1999
// =====================================================================
//
//	A lightweight storage object for a fixed size buffer

#ifndef _H_CStorage_
#define _H_CStorage_

#include "AW.h"

#if PRAGMA_ONCE
	#pragma once
#endif

HL_Begin_Namespace_BigRedH

class CStorage 
{
	public:
							CStorage( UInt32 inSize )
									: mStorage( nil ), mSize(inSize), mLength(0)
								{ mStorage = new UInt8[inSize]; }
							~CStorage()
								{ delete mStorage; }
		
		inline 				operator UInt8*()
								{ return mStorage; }
		inline 				operator char*()
								{ return (char*)mStorage; }
		inline UInt8&		operator[]( UInt32 inIndex )
								{ return mStorage[ inIndex ]; }
		inline UInt8*		operator()( UInt32 inIndex )
								{ return mStorage + inIndex; }

		inline UInt8*		CurrentPtr()
								{ return mStorage + mLength; }

		inline UInt32		Size()
								{ return mSize; }
		inline UInt32		Length()
								{ return mLength; }
		inline void			SetLength( UInt32 inLength )
								{ mLength = inLength; }
		inline void			AdjustLength( UInt32 inLength )
								{ mLength += inLength; }

	private:
		UInt8				*mStorage;
		UInt32				mSize, mLength;
};

HL_End_Namespace_BigRedH

#endif	// _H_CStorage_
