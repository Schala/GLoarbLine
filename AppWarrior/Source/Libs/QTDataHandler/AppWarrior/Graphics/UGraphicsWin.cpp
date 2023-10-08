/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// Definitions for the graphics utility class on Win32.

#include "AW.h"
#include "UGraphicsWin.h"
#include "PixelWin.h"
#include "Blend.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  BlendBitmap                                         [static][public]
// ---------------------------------------------------------------------
// 
// Converts from integer BitsPerPixel to the correct pixelformat classes.
void 
UGraphicsWin::BlendBitmap	(unsigned char* inSrcRows
							,long inSrcRowBytes
							,unsigned char* inDestRows
							,long inDestRowBytes
							,unsigned char* inMaskRows
							,long inMaskRowBytes
							,unsigned char* outRows
							,long inOutRowBytes
							,unsigned long inRowPixels
							,unsigned long inRowCount
							,int inBPPA
							,int inBPPB
							,int inBPPOut)
{
	if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat16Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH BlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,inMaskRows, inMaskRowBytes, outRows, inOutRowBytes
				,inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat16Win, PixelFormat16Win);
	} else
		ASSERT(0);	
}

// ---------------------------------------------------------------------
//  InlineBlendBitmap                                   [static][public]
// ---------------------------------------------------------------------
// 
// Converts from integer bits per pixel to the correct pixelformat classes.
void
UGraphicsWin::InlineBlendBitmap	 (unsigned char* inSrcRows
								,long inSrcRowBytes
								,unsigned char* inDestRows
								,long  inDestRowBytes
								,unsigned char* outRows
								,long  inOutRowBytes 
								,unsigned long  inRowPixels
								,unsigned long  inRowCount
								,int inBPPA
								,int inBPPB
								,int inBPPOut)
{
	if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat32Win, PixelFormat16Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH InlineBlendBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount
				,PixelFormat16Win, PixelFormat16Win, PixelFormat16Win);
	} else
		ASSERT(0);
}

// ---------------------------------------------------------------------
//  BlendConstantAlphaBitmap                            [static][public]
// ---------------------------------------------------------------------
// 
// Converts from integer bits per pixel to the correct pixelformat classes.
void 
UGraphicsWin::BlendConstantAlphaBitmap (unsigned char* inSrcRows
										,long inSrcRowBytes
										,unsigned char* inDestRows
										,long  inDestRowBytes
										,unsigned char* outRows
										,long  inOutRowBytes 
										,unsigned long  inRowPixels
										,unsigned long  inRowCount
										,unsigned char inAlpha
										,int inBPPA
										,int inBPPB
										,int inBPPOut)
{
	if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat32Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat32Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat32Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat32Win, PixelFormat16Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat16Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat16Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat16Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH BlendConstantAlphaBitmap(inSrcRows, inSrcRowBytes, inDestRows, inDestRowBytes
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inAlpha
				,PixelFormat16Win, PixelFormat16Win, PixelFormat16Win);
	} else
		ASSERT(0);
}


// ---------------------------------------------------------------------
//  ColorKeyBitmap                                      [static][public]
// ---------------------------------------------------------------------
// 
// Converts from integer bits per pixel to the correct pixelformat classes.
void
UGraphicsWin::ColorKeyBitmap	(	unsigned char* inSrcRowsA
									,long inSrcRowBytesA
									,unsigned char* inSrcRowsB
									,long  inSrcRowBytesB
									,unsigned char* outRows
									,long  inOutRowBytes 
									,unsigned long  inRowPixels
									,unsigned long  inRowCount
									,const CColor& inColorKey
									,int inBPPA
									,int inBPPB
									,int inBPPOut)
{
	if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat32Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat32Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat32Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 32 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat32Win, PixelFormat16Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 32) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat16Win, PixelFormat32Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 32 && inBPPOut == 16) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat16Win, PixelFormat32Win, PixelFormat16Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 32) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat16Win, PixelFormat16Win, PixelFormat32Win);
	} else if (inBPPA == 16 && inBPPB == 16 && inBPPOut == 16) {
		HL_BigRedH ColorKeyBitmap(inSrcRowsA, inSrcRowBytesA, inSrcRowsB, inSrcRowBytesB
				,outRows, inOutRowBytes, inRowPixels, inRowCount, inColorKey
				,PixelFormat16Win, PixelFormat16Win, PixelFormat16Win);
	} else
		ASSERT(0);	
}

HL_End_Namespace_BigRedH