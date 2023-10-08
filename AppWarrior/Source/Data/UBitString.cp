/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
/*
A bitstring stores a sequence consisting of a varying number of bits.
Each bit represents either the value zero (clear) or one (set).

Individual bits are accessed by their 0-based index.   UBitString does not
allocate the bitstring - all UBitString functions expect a pointer to the
start of the bitstring.

-- Portability --

UBitString makes no platform specific calls, nor relies on any other units.
Thus, it's completely portable and should compile without changes.
*/

#include "UBitString.h"

/*
 * Macros
 */

#define GET_BIT(p, i)		(( ( ((Int8 *)p)[((Uint32)i)>>3] >> (((Uint32)i) & 7) ) & 1))
#define SET_BIT(p, i)		((Int8 *)p)[((Uint32)i)>>3] |= (Int8)1 << (((Uint32)i) & 7)
#define CLR_BIT(p, i)		((Int8 *)p)[((Uint32)i)>>3] &= ~((Int8)1 << (((Uint32)i) & 7))
#define CPY_BIT(p, d, s)	if (GET_BIT(p, s)) SET_BIT(p, d); else CLR_BIT(p, d)

/* -------------------------------------------------------------------------- */

bool UBitString::Get(const void *p, Uint32 i)
{
	return GET_BIT(p, i) != 0;
}

void UBitString::Set(void *p, Uint32 i, bool b)
{
	if (b)
		SET_BIT(p, i);
	else
		CLR_BIT(p, i);
}

void UBitString::Set(void *p, Uint32 i, Uint32 n, bool b)
{
	n += i;
	
	if (b)
	{
		for ( ; i<n; i++)
			SET_BIT(p, i);
	}
	else
	{
		for ( ; i<n; i++)
			CLR_BIT(p, i);
	}
}

void UBitString::Set(void *p, Uint32 i)
{
	SET_BIT(p, i);
}

void UBitString::Clear(void *p, Uint32 i)
{
	CLR_BIT(p, i);
}

// handles overlapping source and destination blocks
void UBitString::Copy(void *p, Uint32 to, Uint32 from, Uint32 n)
{
	if (n)
	{
		if (to > from)
		{
			to += n;
			from += n;
			while (n--)
			{
				to--;
				from--;
				CPY_BIT(p, to, from);
			}
		}
		else
		{
			while (n--)
			{
				CPY_BIT(p, to, from);
				to++;
				from++;
			}
		}
	}
}

bool UBitString::GetFirstSet(const void *inData, Uint32 inMaxBits, Uint32& outIndex)
{
	for (Uint32 i=0; i<inMaxBits; i++)
	{
		if (GET_BIT(inData, i))
		{
			outIndex = i;
			return true;
		}
	}
	
	return false;
}

bool UBitString::GetLastSet(const void *inData, Uint32 inMaxBits, Uint32& outIndex)
{
	if (inMaxBits)
	{
		while (inMaxBits--)
		{
			if (GET_BIT(inData, inMaxBits))
			{
				outIndex = inMaxBits;
				return true;
			}
		}
	}
	
	return false;
}

bool UBitString::GetNextSet(const void *inData, Uint32 inMaxBits, Uint32& ioIndex)
{
	for (Uint32 i=ioIndex+1; i<inMaxBits; i++)
	{
		if (GET_BIT(inData, i))
		{
			ioIndex = i;
			return true;
		}
	}
	
	return false;
}


