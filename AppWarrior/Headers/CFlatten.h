/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"

class CFlatten
{
	public:
		CFlatten(void *inBuf)										{	mPtr = mBuf = (Uint8 *)inBuf;									}
		
		void SetBuffer(void *inBuf)									{	mPtr = mBuf = (Uint8 *)inBuf;									}
		Uint8 *GetBuf() const										{	return mBuf;													}
		Uint8 *GetPtr() const										{	return mPtr;													}
		
		void Write(const void *inData, Uint32 inDataSize)			{	UMemory::Copy(mPtr, inData, inDataSize); mPtr += inDataSize;	}
		
		void WriteByte(Uint8 inByte)								{	*((Uint8 *)mPtr)++ = inByte;									}
		void WriteWord(Uint16 inWord)								{	*((Uint16 *)mPtr)++ = TB(inWord);								}
		void WriteLong(Uint32 inLong)								{	*((Uint32 *)mPtr)++ = TB(inLong);								}
		
		void WritePString(const Uint8 *inStr)						{	mPtr += UMemory::Copy(mPtr, inStr, inStr[0]+1);					}
		void WritePString(const void *inData, Uint32 inDataSize);
		void WriteWString(const void *inData, Uint32 inDataSize);
		void WriteLString(const void *inData, Uint32 inDataSize);
		
		void WriteShortColor(const SColor& inCol);
		void WriteShortPoint(const SPoint& inPoint);
		void WriteShortRect(const SRect& inRect);
		void WriteDateTimeStamp(const SDateTimeStamp& inDT);
		
		Uint32 GetSize() const										{	return mPtr - mBuf;												}
		void Advance(Uint32 inSize)									{	mPtr += inSize;													}
		
		void Reserve(Uint32 inSize)									{	while (inSize--) *mPtr++ = 0;									}
		void ReserveByte()											{	*((Uint8 *)mPtr)++ = 0;											}
		void ReserveWord()											{	*((Uint16 *)mPtr)++ = 0;										}
		void ReserveLong()											{	*((Uint32 *)mPtr)++ = 0;										}
		void ReservePString()										{	*((Uint8 *)mPtr)++ = 0;											}
		
		void Align2()												{	if (RoundUp2(mPtr-mBuf) != (mPtr-mBuf)) *mPtr++ = 0;			}
		void Align4()												{	Reserve(RoundUp4(mPtr-mBuf) - (mPtr-mBuf));						}
		void Align8()												{	Reserve(RoundUp8(mPtr-mBuf) - (mPtr-mBuf));						}		// mPtr-mBuf is to make no assumptions about the alignment of mBuf

	protected:
		Uint8 *mPtr, *mBuf;
};

class CUnflatten
{
	public:
		CUnflatten(const void *inData, Uint32 inDataSize)			{	mPtr = mBuf = (Uint8 *)inData; mEndPtr = (Uint8 *)inData + inDataSize;	}
		
		void SetData(const void *inData, Uint32 inDataSize)			{	mPtr = mBuf = (Uint8 *)inData; mEndPtr = (Uint8 *)inData + inDataSize;	}
		const Uint8 *GetPtr() const									{	return mPtr;													}
		void Advance(Uint32 inSize)									{	mPtr += inSize;													}
		void Skip(Uint32 inSize)									{	mPtr += inSize;													}
		void SkipByte()												{	mPtr++;															}
		void SkipWord()												{	mPtr += sizeof(Uint16);											}
		void SkipLong()												{	mPtr += sizeof(Uint32);											}

		Uint8 ReadByte()											{	return *((Uint8 *)mPtr)++;										}
		Uint16 ReadWord()											{	return FB( *((Uint16 *)mPtr)++ );								}
		Uint32 ReadLong()											{	return FB( *((Uint32 *)mPtr)++ );								}

		bool ReadPString(Uint8 *outStr);
		bool ReadPString(Uint8 *outStr, Uint32 inBufSize);
		Uint8 *ReadPString();
		bool SkipPString();
		Uint8 *ReadWString(Uint32& outSize);
		bool SkipWString();

		void ReadShortColor(SColor& outCol);
		void ReadShortPoint(SPoint& outPoint);
		void ReadShortRect(SRect& outRect);
		void ReadDateTimeStamp(SDateTimeStamp& outDT);
		
		bool EnoughData(Uint32 inSize) const						{	return (mPtr + inSize) <= mEndPtr;								}
		bool EnufData(Uint32 inSize) const							{	return (mPtr + inSize) <= mEndPtr;								}
		bool NotEnoughData(Uint32 inSize) const						{	return (mPtr + inSize) > mEndPtr;								}
		bool NotEnufData(Uint32 inSize) const						{	return (mPtr + inSize) > mEndPtr;								}

		void Align2()												{	mPtr += RoundUp2(mPtr-mBuf) - (mPtr-mBuf);						}
		void Align4()												{	mPtr += RoundUp4(mPtr-mBuf) - (mPtr-mBuf);						}
		void Align8()												{	mPtr += RoundUp8(mPtr-mBuf) - (mPtr-mBuf);						}

	protected:
		const Uint8 *mPtr, *mBuf, *mEndPtr;
};

// inline operator write functions
inline CFlatten& operator<< (CFlatten& s, Int8 v)
{	s.WriteByte(v); return s;				}
inline CFlatten& operator<< (CFlatten& s, Uint8 v)
{	s.WriteByte(v); return s;				}
inline CFlatten& operator<< (CFlatten& s, Int16 v)
{	s.WriteWord(v); return s;				}
inline CFlatten& operator<< (CFlatten& s, Uint16 v)
{	s.WriteWord(v); return s;				}
inline CFlatten& operator<< (CFlatten& s, Int32 v)
{	s.WriteLong(v); return s;				}
inline CFlatten& operator<< (CFlatten& s, Uint32 v)
{	s.WriteLong(v); return s;				}

// inline operator read functions
inline const CUnflatten& operator>> (CUnflatten& s, Int8& v)
{	v = s.ReadByte(); return s;				}
inline const CUnflatten& operator>> (CUnflatten& s, Uint8& v)
{	v = s.ReadByte(); return s;				}
inline const CUnflatten& operator>> (CUnflatten& s, Int16& v)
{	v = s.ReadWord(); return s;				}
inline const CUnflatten& operator>> (CUnflatten& s, Uint16& v)
{	v = s.ReadWord(); return s;				}
inline const CUnflatten& operator>> (CUnflatten& s, Int32& v)
{	v = s.ReadLong(); return s;				}
inline const CUnflatten& operator>> (CUnflatten& s, Uint32& v)
{	v = s.ReadLong(); return s;				}


inline void CFlatten::WritePString(const void *inData, Uint32 inDataSize)
{
	if (inDataSize > max_Uint8) inDataSize = max_Uint8;
	*((Uint8 *)mPtr)++ = inDataSize;
	UMemory::Copy(mPtr, inData, inDataSize);
	mPtr += inDataSize;
}

inline void CFlatten::WriteWString(const void *inData, Uint32 inDataSize)
{
	if (inDataSize > max_Uint16) inDataSize = max_Uint16;
	*((Uint16 *)mPtr)++ = TB((Uint16)inDataSize);
	UMemory::Copy(mPtr, inData, inDataSize);
	mPtr += inDataSize;
}

inline void CFlatten::WriteLString(const void *inData, Uint32 inDataSize)
{
	*((Uint32 *)mPtr)++ = TB(inDataSize);
	UMemory::Copy(mPtr, inData, inDataSize);
	mPtr += inDataSize;
}

inline void CFlatten::WriteShortColor(const SColor& inCol)
{
	mPtr[0] = inCol.red >> 8;
	mPtr[1] = inCol.green >> 8;
	mPtr[2] = inCol.blue >> 8;
	mPtr[3] = 0;
	mPtr += 4;
}

inline void CFlatten::WriteShortPoint(const SPoint& inPoint)
{
	((Int16 *)mPtr)[0] = TB((Int16)inPoint.x);
	((Int16 *)mPtr)[1] = TB((Int16)inPoint.y);
	mPtr += 4;
}

inline void CFlatten::WriteShortRect(const SRect& inRect)
{
	((Int16 *)mPtr)[0] = TB((Int16)inRect.left);
	((Int16 *)mPtr)[1] = TB((Int16)inRect.top);
	((Int16 *)mPtr)[2] = TB((Int16)inRect.right);
	((Int16 *)mPtr)[3] = TB((Int16)inRect.bottom);
	mPtr += 8;
}

inline void CFlatten::WriteDateTimeStamp(const SDateTimeStamp& inDT)
{
	((Uint16 *)mPtr)[0] = TB(inDT.year);
	((Uint16 *)mPtr)[1] = TB(inDT.msecs);
	((Uint32 *)mPtr)[1] = TB(inDT.seconds);
	mPtr += 8;
}

// returns whether or not valid
inline bool CUnflatten::ReadPString(Uint8 *outStr)
{
	return ((mPtr < mEndPtr) && (mPtr + *((Uint8 *)mPtr) + 1) <= mEndPtr)
		? mPtr += UMemory::Copy(outStr, mPtr, *((Uint8 *)mPtr) + 1), true : false;
}

// returns whether or not valid
inline bool CUnflatten::ReadPString(Uint8 *outStr, Uint32 inBufSize)
{
	const Uint8 *p = mPtr;
	
	if ((p < mEndPtr) && (p + *p + 1) <= mEndPtr)
	{
		mPtr += *p + 1;
		pstrncpy(outStr, p, inBufSize-1);
		return true;
	}
	
	return false;
}

// returns nil if not valid (not enough data)
inline Uint8 *CUnflatten::ReadPString()
{
	Uint8 *p = (Uint8 *)mPtr;
	return ((p < mEndPtr) && (p + *p + 1) <= mEndPtr)
		? mPtr += *p + 1, p : nil;
}

// returns whether or not valid
inline bool CUnflatten::SkipPString()
{
	const Uint8 *p = mPtr;
	return ((p < mEndPtr) && (p + *p + 1) <= mEndPtr)
		? mPtr += *p + 1, true : false;
}

// returns nil if not valid (not enough data)
inline Uint8 *CUnflatten::ReadWString(Uint32& outSize)
{
	Uint8 *p = (Uint8 *)mPtr;
	
	if (p+1 < mEndPtr)					// if got the Uint16 length
	{
		Uint32 s = FB(*(Uint16 *)p);		// get the length
		Uint32 ts = s + sizeof(Uint16);	// total size of WString is length + size of the Uint16 to store the length
		
		if ((p + ts) <= mEndPtr)		// if got the Uint16 and as much data as it says
		{
			outSize = s;				// output length
			mPtr += ts;					// advance ptr total size
			return p + sizeof(Uint16);	// return ptr to WString's data
		}
	}
	
	return nil;							// invalid!
}

// returns whether or not valid
inline bool CUnflatten::SkipWString()
{
	const Uint8 *p = mPtr;
	return ((p+1 < mEndPtr) && (p + FB(*((Uint16 *)p)) + sizeof(Uint16)) <= mEndPtr)
		? mPtr += FB(*((Uint16 *)p)) + sizeof(Uint16), true : false;
}

inline void CUnflatten::ReadShortColor(SColor& outCol)
{
	outCol.Set(mPtr[0] * 257, mPtr[1] * 257, mPtr[2] * 257);
	mPtr += 4;
}

inline void CUnflatten::ReadShortPoint(SPoint& outPoint)
{
	outPoint.Set(FB(((Int16 *)mPtr)[0]), FB(((Int16 *)mPtr)[1]));
	mPtr += 4;
}

inline void CUnflatten::ReadShortRect(SRect& outRect)
{
	outRect.Set(FB(((Int16 *)mPtr)[0]), FB(((Int16 *)mPtr)[1]), FB(((Int16 *)mPtr)[2]), FB(((Int16 *)mPtr)[3]));
	mPtr += 8;
}

inline void CUnflatten::ReadDateTimeStamp(SDateTimeStamp& outDT)
{
	outDT.year = FB( ((Uint16 *)mPtr)[0] );
	outDT.msecs = FB( ((Uint16 *)mPtr)[1] );
	outDT.seconds = FB( ((Uint32 *)mPtr)[1] );
	mPtr += 8;
}




