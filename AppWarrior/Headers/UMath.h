/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */#pragma once#include "typedefs.h"struct SHuge {#if CONVERT_INTS	Uint32 lo;	Uint32 hi;#else	Uint32 hi;	Uint32 lo;#endif};struct SSignedHuge {#if CONVERT_INTS	Uint32 lo;	Int32 hi;#else	Int32 hi;	Uint32 lo;#endif};extern const fast_float pi;class UMath{	public:		// random		static void SetRandomSeed(Int32 inSeed);		static Int32 CalcRandomSeed();		static Int32 GetRandom(Int32 inMin = min_Int32, Int32 inMax = max_Int32);		static bool FlipCoin();				// trigonometric functions		static fast_float Cosine(fast_float x);		static fast_float Sine(fast_float x);		static fast_float Tangent(fast_float x);		static fast_float ArcCosine(fast_float x);		static fast_float ArcSine(fast_float x);		static fast_float ArcTangent(fast_float x);		static fast_float SquareRoot(fast_float x);		// 64-bit math functions		static void Add64(const SHuge& inA, const SHuge& inB, SHuge& outSum);		static void Sub64(const SHuge& inA, const SHuge& inB, SHuge& outDiff);		static void Mul64U(Uint32 inA, Uint32 inB, SHuge& outProd);		static void Div64U(const SHuge& inNumerator, Uint32 inDenominator, Uint32& outQuotient, Uint32& outRemainder);};inline Int32 RoundToLong(fast_float x){	return x < 0 ? x - 0.5 : x + 0.5;}	inline Uint32 RoundToULong(fast_float x){	return x + 0.5;}//#define cos(x)	UMath::Cosine(x)//#define sin(x)	UMath::Sine(x)//#define tan(x)	UMath::Tangent(x)//#define acos(x)	UMath::ArcCosine(x)//#define asin(x)	UMath::ArcSine(x)//#define atan(x)	UMath::ArcTangent(x)//#define sqrt(x)	UMath::SquareRoot(x)inline bool AddWillOverflow(Uint32 inA, Uint32 inB){	return ((inA + inB) < inA);}inline bool SubWillUnderflow(Uint32 inA, Uint32 inB){	return ((inA - inB) > inA);}inline bool MulWillOverflow(Uint32 inA, Uint32 inB){	return inB && (inA > (max_Uint32 / inB));}