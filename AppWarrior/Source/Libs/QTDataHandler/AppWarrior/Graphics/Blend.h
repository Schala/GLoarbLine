// =====================================================================
//	Blend.h				                 (C) Hotline Communications 1999
// =====================================================================
// A bunch of static inline templates to blend bitmaps.
// Require specification of pixel formats, which are defined in
// PixelWin.h and PixelMac.h

#ifndef _H_Blend_
#define _H_Blend_

#include "CColor.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
// RowBlend			                                	 		[inline]
// ---------------------------------------------------------------------
// Blends an entire row with an alpha mask.

template <class PixelFormatA, class PixelFormatB, class PixelFormatOut>
inline void
RowBlend				(unsigned char* inSrcRow
							, unsigned char* inDestRow
							, unsigned char* inMaskRow
							, unsigned char* outRow
							, unsigned long  inRowPixels
							, const PixelFormatA& inPFA
							, const PixelFormatB& inPFB
							, const PixelFormatOut& inPFOut)
{
	while (inRowPixels > 0) {
		CColor stackPixelA = PixelFormatA::CrackPixel(inSrcRow);
		CColor stackPixelB = PixelFormatB::CrackPixel(inDestRow);
		PixelFormatOut::MakePixel(outRow,
				stackPixelA.Blend255(stackPixelB, *inMaskRow
				,static_cast<unsigned char>(0xFF - *inMaskRow)));

		inSrcRow += PixelFormatA::GetPixelBytes();
		inDestRow += PixelFormatB::GetPixelBytes();
		++inMaskRow;
		outRow += PixelFormatOut::GetPixelBytes();
		--inRowPixels;
	}
}

// ---------------------------------------------------------------------
// BlendBitmap		                                	 		[inline]
// ---------------------------------------------------------------------
// Blends an entire bitmap with an alpha mask.

template <class PixelFormatA, class PixelFormatB, class PixelFormatOut>
inline void
BlendBitmap 		(	unsigned char* inSrcRows
						,long inSrcRowBytes
						,unsigned char* inDestRows
						,long  inDestRowBytes
						,unsigned char* inMaskRows
						,long inMaskRowBytes
						,unsigned char* outRows
						,long  inOutRowBytes 
						,unsigned long  inRowPixels
						,unsigned long  inRowCount
						,const PixelFormatA& inPFA
						,const PixelFormatB& inPFB
						,const PixelFormatOut& inPFOut)
{
	while (inRowCount > 0) {
		RowBlend<PixelFormatA,PixelFormatB,PixelFormatOut>
			(inSrcRows, inDestRows, inMaskRows
			,outRows, inRowPixels,inPFA, inPFB, inPFOut);
		inSrcRows += inSrcRowBytes;
		inDestRows += inDestRowBytes;
		inMaskRows += inMaskRowBytes;
		outRows += inOutRowBytes;
		--inRowCount;
	}
}			


// ---------------------------------------------------------------------
// InlineBlendRow				                 	          	[inline]
// ---------------------------------------------------------------------
// Blend an entire row of pixels with the inline alpha values

template <typename PixelFormatA, class PixelFormatB, class PixelFormatOut>
inline void
InlineBlendRow				(unsigned char* inSrcRowA
							, unsigned char* inSrcRowB
							, unsigned char* outRow
							, unsigned long inRowPixels
							, const PixelFormatA& inPFA
							, const PixelFormatB& inPFB
							, const PixelFormatOut& inPFOut)
{
	while (inRowPixels > 0) {
		CColor stackPixelA = PixelFormatA::CrackPixelAlpha(inSrcRowA);
		CColor stackPixelB = PixelFormatB::CrackPixel(inSrcRowB);
		PixelFormatOut::MakePixel(outRow, stackPixelA.Blend255(stackPixelB
				,stackPixelA.GetAlpha(), static_cast<unsigned char>
				(0xFF - stackPixelA.GetAlpha())));
		
		inSrcRowA += PixelFormatA::GetPixelBytes();
		inSrcRowB += PixelFormatB::GetPixelBytes();
		outRow += PixelFormatOut::GetPixelBytes();
		--inRowPixels;
	}
}


// ---------------------------------------------------------------------
// InlineBlendBitmap		                           	 
// ---------------------------------------------------------------------
// Blend an entire bitmap using the inline alpha values in that bitmap.

template <class PixelFormatA, class PixelFormatB, class PixelFormatOut>
void
InlineBlendBitmap	 (	unsigned char* inSrcRowsA
						,long inSrcRowBytesA
						,unsigned char* inSrcRowsB
						,long  inSrcRowBytesB
						,unsigned char* outRows
						,long  inOutRowBytes 
						,unsigned long  inRowPixels
						,unsigned long  inRowCount
						,const PixelFormatA& inPFA
						,const PixelFormatB& inPFB
						,const PixelFormatOut& inPFOut)
{
	while (inRowCount > 0) {
		InlineBlendRow	(inSrcRowsA, inSrcRowsB, outRows, inRowPixels
				, inPFA, inPFB, inPFOut);
		inSrcRowsA += inSrcRowBytesA;
		inSrcRowsB += inSrcRowBytesB;
		outRows += inOutRowBytes;
		--inRowCount;
	}
}			


// ---------------------------------------------------------------------
// RowBlendConstantAlpha                              	 [inline]
// ---------------------------------------------------------------------
// Blends an entire row with a fixed alpha for all pixels.

template <class PixelFormatA, class PixelFormatB, class PixelFormatOut>
inline void
RowBlendConstantAlpha		(unsigned char* inSrcRow
							, unsigned char* inDestRow
							, unsigned char* outRow
							, unsigned long  inRowPixels
							, unsigned char  inAlpha
							, const PixelFormatA& inPFA
							, const PixelFormatB& inPFB
							, const PixelFormatOut& inPFOut)
{
	unsigned char InvAlpha = static_cast<unsigned char>(0xFF - inAlpha);
	while (inRowPixels > 0) {
		CColor pixelA = PixelFormatA::CrackPixel(inSrcRow);
		CColor pixelB = PixelFormatB::CrackPixel(inDestRow);
		PixelFormatOut::MakePixel(outRow, pixelA.Blend255(pixelB, inAlpha, InvAlpha));
		inSrcRow += PixelFormatA::GetPixelBytes();
		inDestRow += PixelFormatB::GetPixelBytes();
		outRow += PixelFormatOut::GetPixelBytes();
		--inRowPixels;
	}
}

// ---------------------------------------------------------------------
// BlendConstantAlphaBitmap                            	 [inline]
// ---------------------------------------------------------------------
// Blends an entire bitmap with a fixed alpha for all pixels.

template <class PixelFormatA, class PixelFormatB, class PixelFormatOut>
inline void
BlendConstantAlphaBitmap (	unsigned char* inSrcRows
						,long inSrcRowBytes
						,unsigned char* inDestRows
						,long  inDestRowBytes
						,unsigned char* outRows
						,long  inOutRowBytes 
						,unsigned long  inRowPixels
						,unsigned long  inRowCount
						,unsigned char inAlpha
						,const PixelFormatA& inPFA
						,const PixelFormatB& inPFB
						,const PixelFormatOut& inPFOut)
{
	while (inRowCount > 0) {
		RowBlendConstantAlpha<PixelFormatA,PixelFormatB,PixelFormatOut>
			(inSrcRows, inDestRows, outRows, inRowPixels, inAlpha
			,inPFA,inPFB,inPFOut);
		inSrcRows += inSrcRowBytes;
		inDestRows += inDestRowBytes;
		outRows += inOutRowBytes;
		--inRowCount;
	}
}			


// ---------------------------------------------------------------------
// ColorKeyRow			                            	 [inline]
// ---------------------------------------------------------------------
// Color Key an entire row of pixels

template <typename PixelFormatA, class PixelFormatB, class PixelFormatOut>
inline void
ColorKeyRow					(unsigned char* inSrcRowA
							, unsigned char* inSrcRowB
							, unsigned char* outRow
							, unsigned long inRowPixels
							, const CColor& inKeyColor
							, const PixelFormatA& inPFA
							, const PixelFormatB& inPFB
							, const PixelFormatOut& inPFOut)
{
	while (inRowPixels > 0) {
		CColor stackPixel = PixelFormatA::CrackPixel(inSrcRowA);
		if (stackPixel == inKeyColor) 
			stackPixel = PixelFormatB::CrackPixel(inSrcRowB);
		PixelFormatOut::MakePixel(outRow, stackPixel);
		
		inSrcRowA += PixelFormatA::GetPixelBytes();
		inSrcRowB += PixelFormatB::GetPixelBytes();
		outRow += PixelFormatOut::GetPixelBytes();
		--inRowPixels;
	}
}


// ---------------------------------------------------------------------
// ColorKeyBitmap		                            	 [static inline]
// ---------------------------------------------------------------------
// Color Key an entire bitmap

template <class PixelFormatA, class PixelFormatB, class PixelFormatOut>
void
ColorKeyBitmap		 (	unsigned char* inSrcRowsA
						,long inSrcRowBytesA
						,unsigned char* inSrcRowsB
						,long  inSrcRowBytesB
						,unsigned char* outRows
						,long  inOutRowBytes 
						,unsigned long  inRowPixels
						,unsigned long  inRowCount
						,const CColor& inColorKey
						,const PixelFormatA& inPFA
						,const PixelFormatB& inPFB
						,const PixelFormatOut& inPFOut)
{
	// This little hack is required to convert the key color channels into
	// what source A stores that color as;
	// In binary, 11111001 will never appear as a channel
	// in a 16 bpp image, it should actually map
	// to any pixel that matches whatever 11111 maps to
	// as an 8 bit channel.

	// Make the temp buffer far too large, just to be safe
	unsigned char tempStorage[8];
	PixelFormatA::MakePixel(tempStorage, inColorKey);
	CColor stackKeyColor = PixelFormatA::CrackPixel(tempStorage);

	while (inRowCount > 0) {
		ColorKeyRow	(inSrcRowsA, inSrcRowsB, outRows, inRowPixels
				,stackKeyColor, inPFA, inPFB, inPFOut);
		inSrcRowsA += inSrcRowBytesA;
		inSrcRowsB += inSrcRowBytesB;
		outRows += inOutRowBytes;
		--inRowCount;
	}
}			

HL_End_Namespace_BigRedH

#endif	// _H_Blend_
