//======================================================================
//	CGraphicsPortWin.h                   (C) Hotline Communications 1999
//======================================================================
//
// Win32 platform specific GraphicsPort implementation

#ifndef _H_CGraphicsPortWin_
#define _H_CGraphicsPortWin_

#if PRAGMA_ONCE
	#pragma once
#endif

#include "CModuleWin.h"
#include "CGraphicsPort.h"

		  
HL_Begin_Namespace_BigRedH

class CGraphicsPortWin : public CGraphicsPort
{
	public:
								// DC, we don't own it
							CGraphicsPortWin(HDC inDC);
									// throws nothing
								
								// DC from an image... we own it
							CGraphicsPortWin(HDC inDC
									, HBITMAP inBitmap
									, BITMAPINFOHEADER* inInfo 
									, UInt8* inBits);
									// throws CGraphicsException
									
								// From a window, we own it
							CGraphicsPortWin(HWND inHwnd);
									// throws CGraphicsException
											
								// Full paintstruct constructor; by definition from paint
								// We own it, and must EndPaint on it.
							CGraphicsPortWin(PAINTSTRUCT* inPS
									, HWND inHwnd);
									// throws nothing
									
		virtual				~CGraphicsPortWin ();
									// throws nothing
									
		
								// Is this an offscreen bitmap?
		virtual bool		IsOffscreen () const;
									// throws nothing			
											     
		// Uses alpha, if available
		virtual void		DrawImage ( CImage& inSrcImage
									, const CRect& inSourceRect
									, const CRect& inDestRect );
									// throws CGraphicsException
								
		// Uses specified alpha mask
		virtual void		AlphaMaskImage (CImage& inSrcImage
								, const CRect& inSourceRect
								, const CRect& inDestRect
								, CImage* inAlphaMask);
								// throws CGraphicsException		
											
		// Uses specified alpha value
		virtual void		AlphaDrawImage (CImage& inSrcImage
								, const CRect& inSourceRect
								, const CRect& inDestRect
								, ColorChannel inAlpha);
								// throws CGraphicsException

		// Ignores alpha; makes pixel with keyColor transparent
		virtual void		ColorKeyedDrawImage (CImage& inSrcImage
								, const CRect& inSourceRect
								, const CRect& inDestRect
								, const CColor& inKeyColor );
								// throws CGraphicsException
								
			// ** Surface **								
							// Getting and Setting Clip Region
		virtual void		SetClipRect(
								const CRect &inRect );
								//??
		virtual CRect		GetClipRect() const;
								//??

			// ** Pen **
		virtual CPen		GetPen() const;
								//??
		virtual void		SetPen( 
								const CPen &inPen );
								//??

			// ** Brush **
		virtual CBrush		GetBrush() const;
								//??
		virtual void		SetBrush( 
								const CBrush &inBrush );
								//??

		/*	// ** Font **
		virtual CFont		GetFont() const;
								//??
		virtual void		SetFont( 
								const CFont &inFont );
								//??
*/
			// ** Pixels **
		virtual CColor		GetPixel(
								const CPoint& inPixel) const;
								//throws CGraphicsException
								
		virtual void		SetPixel(
								const CPoint& inPixel
								,const CColor& inColor);
								// throws CGraphicsException
								
		virtual void		SolidFill(
								const CColor& inColor);
								// throws CGraphicsException

			// ** Geometric Primitives **
		virtual void		DrawLine (const CPoint& inStart
									, const CPoint& inEnd
									, const CColor& inColor
									, int inThickness = 0);
											
		virtual void 		FillRect (const CRect& inRect
									, const CColor& inColor);
									
		virtual void		DrawOval (const CRect& inBounds
									, const CColor& inColor
									, int inThickness = 0);
		
		virtual void 		FillOval (const CRect& inBounds
									, const CColor& inColor);

		virtual CImage*		GetAlphaMask();
				
		virtual void		DrawText ( const CString &inText
								 , const CFont& inFont
							     , const CPoint& inWhere);
							     //??

	protected:
		virtual void		FillPolygon (int inPointCount
									, const CPoint* inPoints
									, const CColor& inColor);

	private:
			// ** Access to the backing store **
		unsigned char*		GetPortBits();
		unsigned char*		GetPortBits(const CPoint& inPixelPos);
		int					GetPortBitsPerPixel() const;
		int					GetPortBytesPerPixel() const;
		long				GetPortRowBytes() const;
		CRect				GetPortBounds() const;
	
		::PAINTSTRUCT		mPS;
		::HWND				mHwnd;
		bool 				mOwned;			// True if we should delete the DC on destruct
		bool				mFromPaint;
		::HBITMAP			mOldHandleBMP;  // The old bitmap selected into 
											// this DC so it can be restored later		
		::BITMAPINFOHEADER	mBitmapInfo;
		UInt8*				mBitmapBits;
		
		// Originally statics of the implementation,
		// These are related to creating palettes for
		// temporary alpha maps.
		static bool 		sIsGrayscaleInitialized;
		static ::RGBQUAD	sGrayScalePalette [256];
		
		// Assures the grayscale palette is built
		static void			InitGrayscale();
		
		// Font needs access to DC for measurement
		friend class CFontWin;
};

HL_End_Namespace_BigRedH
#endif	// _H_CGraphicsPortWin_
