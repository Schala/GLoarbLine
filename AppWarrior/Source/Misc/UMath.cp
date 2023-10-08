/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "UMath.h"

/* -------------------------------------------------------------------------- */

static Int32 gRandomSeed = 0;

void UMath::SetRandomSeed(Int32 inSeed)
{
	gRandomSeed = inSeed;
}

Int32 UMath::GetRandom(Int32 inMin, Int32 inMax)
{
	const Int32 kMaxRand = max_Int32;
	const Int32 kMinRand = min_Int32 + 1;
	
	ASSERT(inMin <= inMax);
	
	// initialise random number generator
	if (gRandomSeed == 0) 
		gRandomSeed = UMath::CalcRandomSeed();

	// find random number using linear congruetional method
	gRandomSeed = gRandomSeed * 1103515245 + 12345;
	
	// scale random to [0.0..1.0] and then to [min, max + 1.0]
	fast_float x = ((fast_float)gRandomSeed - kMinRand) / (kMaxRand + 1.0 - kMinRand);
	return x * (inMax + 1.0 - inMin) + inMin;
}

// heads or tails!!
bool UMath::FlipCoin()
{
	const Uint32 kBit1	= 1;
	const Uint32 kBit2	= 2;
	const Uint32 kBit5	= 16;
	const Uint32 kBit18	= 131072;
	const Uint32 kMask	= kBit1 + kBit2 + kBit5;

	static Int32 flipSeed = 0;
	if (flipSeed == 0) flipSeed = UMath::CalcRandomSeed();

	if (flipSeed & kBit18)
	{
		flipSeed = (((flipSeed ^ kMask) << 1) | kBit1);
		return true;
	}
	else
	{
		flipSeed <<= 1;
		if (flipSeed == 0) flipSeed = 1;
		return false;
	}
}

/* -------------------------------------------------------------------------- */
#pragma mark -

#ifndef SI_TYPE_SIZE
#define SI_TYPE_SIZE 32
#endif

#define __BITS4 (SI_TYPE_SIZE / 4)
#define __ll_B (1L << (SI_TYPE_SIZE / 2))
#define __ll_lowpart(t) ((Uint32) (t) % __ll_B)
#define __ll_highpart(t) ((Uint32) (t) / __ll_B)

void UMath::Add64(const SHuge& inA, const SHuge& inB, SHuge& outSum)
{
	Uint32 x = inA.lo + inB.lo;
	outSum.hi = inA.hi + inB.hi + (x < inA.lo);
	outSum.lo = x;
}

void UMath::Sub64(const SHuge& inA, const SHuge& inB, SHuge& outDiff)
{
	Uint32 x = inA.lo - inB.lo;
	outDiff.hi = inA.hi - inB.hi - (x > inA.lo);
	outDiff.lo = x;
}

void UMath::Mul64U(Uint32 inA, Uint32 inB, SHuge& outProd)
{
	Uint32 x0, x1, x2, x3;
	Uint32 ul, vl, uh, vh;
	
	ul = __ll_lowpart(inA);
	uh = __ll_highpart(inA);
	vl = __ll_lowpart(inB);
	vh = __ll_highpart(inB);
	
	x0 = (Uint32) ul * vl;
	x1 = (Uint32) ul * vh;
	x2 = (Uint32) uh * vl;
	x3 = (Uint32) uh * vh;
	
	x1 += __ll_highpart(x0);	// this can't give carry
	x1 += x2;					// but this indeed can
	if (x1 < x2)				// did we get it?
		x3 += __ll_B;			// yes, add it in the proper pos
	
	outProd.hi = x3 + __ll_highpart(x1);
	outProd.lo = __ll_lowpart(x1) * __ll_B + __ll_lowpart(x0);
}

void UMath::Div64U(const SHuge& inNumer, Uint32 inDenom, Uint32& outQuo, Uint32& outRem)
{
	Uint32 __d1, __d0, __q1, __q0;
	Uint32 __r1, __r0, __m;
	
	__d1 = __ll_highpart (inDenom);
	__d0 = __ll_lowpart (inDenom);
	
	__r1 = inNumer.hi % __d1;
	__q1 = inNumer.hi / __d1;
	__m = (Uint32) __q1 * __d0;
	__r1 = __r1 * __ll_B | __ll_highpart (inNumer.lo);
	
	if (__r1 < __m)
	{
		__q1--, __r1 += inDenom;
		if (__r1 >= inDenom)
			if (__r1 < __m)
				__q1--, __r1 += inDenom;
	}
	
	__r1 -= __m;
	__r0 = __r1 % __d1;
	__q0 = __r1 / __d1;
	__m = (Uint32) __q0 * __d0;
	__r0 = __r0 * __ll_B | __ll_lowpart (inNumer.lo);
	
	if (__r0 < __m)
	{
		__q0--, __r0 += inDenom;
		if (__r0 >= inDenom)
			if (__r0 < __m)
				__q0--, __r0 += inDenom;
	}
	
	__r0 -= __m;
	outQuo = (Uint32) __q1 * __ll_B | __q0;
	outRem = __r0;
}



