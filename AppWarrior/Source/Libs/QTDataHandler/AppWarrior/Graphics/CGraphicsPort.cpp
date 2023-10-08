/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CGraphicsPort.h"

#include <math.h>

HL_Begin_Namespace_BigRedH

// =====================================================================
//	CGraphicsPort
// =====================================================================
//

// ---------------------------------------------------------------------
//  CGraphicsPort                           				   [private]
// ---------------------------------------------------------------------
// Constructor. 
CGraphicsPort::CGraphicsPort ()
{}

// ---------------------------------------------------------------------
//  ~CGraphicsPort									   [virtual][public]
// ---------------------------------------------------------------------
// Destructor
CGraphicsPort::~CGraphicsPort()
{}


// ---------------------------------------------------------------------
//  GetOrigin													[public]
// ---------------------------------------------------------------------
// 
// Returns the current drawing origin

CPoint
CGraphicsPort::GetOrigin () const
{
	return mOrigin;
}

// ---------------------------------------------------------------------
//  SetOrigin													[public]
// ---------------------------------------------------------------------
// 
// Sets the current drawing origin

void
CGraphicsPort::SetOrigin ( const CPoint& inPoint )
{
	mOrigin = inPoint;
}

// ---------------------------------------------------------------------
//  OffsetOriginBy                                              [public]
// ---------------------------------------------------------------------
// Offsets the origin for the graphics port to the passed point

void
CGraphicsPort::OffsetOriginBy( const CPoint &inPoint )
{
	CPoint theOrigin = GetOrigin();
	theOrigin += inPoint;
	SetOrigin( theOrigin );
}


// ---------------------------------------------------------------------
//  Transform													[public]
// ---------------------------------------------------------------------
// 
// Transforms a point from the local (drawing) coordinate system,
// to the surface coordinate system

CPoint
CGraphicsPort::Transform (const CPoint& inPoint) const
{
	return inPoint + GetOrigin();
}


// ---------------------------------------------------------------------
//  Transform													[public]
// ---------------------------------------------------------------------
// 
// Transforms a rect from the local (drawing) coordinate system,
// to the surface coordinate system

CRect
CGraphicsPort::Transform (const CRect& inRect) const
{
	CRect retVal = inRect;
	retVal.OffsetBy(GetOrigin());
	return retVal;
}

// ---------------------------------------------------------------------
//  InvertTransform												[public]
// ---------------------------------------------------------------------
// 
// Transforms a point from the local (drawing) coordinate system,
// to the surface coordinate system

CPoint
CGraphicsPort::InvertTransform (const CPoint& inPoint) const
{
	return inPoint - GetOrigin();
}


// ---------------------------------------------------------------------
//  InvertTransform												[public]
// ---------------------------------------------------------------------
// 
// Transforms a rect from the local (drawing) coordinate system,
// to the surface coordinate system

CRect
CGraphicsPort::InvertTransform (const CRect& inRect) const
{
	CRect retVal = inRect;
	retVal.OffsetBy(-GetOrigin());
	return retVal;
}

// ---------------------------------------------------------------------
//  TileDrawImage                                               [public]
// ---------------------------------------------------------------------
// Tile the drawimage call over a rect
void
CGraphicsPort::TileDrawImage (CImage& inSrcImage, const CRect& inDestRect) 
{
	try {
		if (inSrcImage.GetSize() == CSize(1,1)) {
			CColor fillColor = inSrcImage.GetGraphicsPort()->GetPixel(CPoint(0,0));
			FillRect(inDestRect, fillColor);
		} else {
			CSize sourceSize = inSrcImage.GetSize();
			SInt32 destY = inDestRect.GetTop();
			for (int heightRemaining = inDestRect.GetSize().GetHeight();
					heightRemaining > 0;
					heightRemaining -= sourceSize.GetHeight()
					,destY += sourceSize.GetHeight())
			{
				SInt32 destX = inDestRect.GetLeft();
				for (int widthRemaining = inDestRect.GetSize().GetWidth();
					widthRemaining > 0; 
					widthRemaining -= sourceSize.GetWidth()
					,destX += sourceSize.GetWidth()) {
					
					DrawImage(inSrcImage, CPoint(destX, destY));
				}
			}
		}
	} catch (...) {
		RETHROW_GRAPHICS_(eTileDrawImage);
	}
}

// ---------------------------------------------------------------------
//  TileAlphaMaskImage                                          [public]
// ---------------------------------------------------------------------
// Tile the AlphaMaskImage call over a rect
void
CGraphicsPort::TileAlphaMaskImage (CImage& inSrcImage, const CRect& inDestRect, CImage* inMask) 
{
	try {
		CSize sourceSize = inSrcImage.GetSize();
		SInt32 destY = inDestRect.GetTop();
		for (int heightRemaining = inDestRect.GetSize().GetHeight();
				heightRemaining > 0;
				heightRemaining -= sourceSize.GetHeight()
				,destY += sourceSize.GetHeight())
		{
			SInt32 destX = inDestRect.GetLeft();
			for (int widthRemaining = inDestRect.GetSize().GetWidth();
				widthRemaining > 0; 
				widthRemaining -= sourceSize.GetWidth()
				,destX += sourceSize.GetWidth()) {
				
				AlphaMaskImage(inSrcImage, CPoint(destX, destY), inMask);
			}
		}
	} catch (...) {
		RETHROW_GRAPHICS_(eTileAlphaMaskImage);
	}
}


// ---------------------------------------------------------------------
//  TileAlphaDrawImage                                          [public]
// ---------------------------------------------------------------------
// Tile the AlphaDrawImage call over a rect
void
CGraphicsPort::TileAlphaDrawImage (CImage& inSrcImage
									,const CRect& inDestRect
									,ColorChannel inAlpha) 
{
	try {
		CSize sourceSize = inSrcImage.GetSize();
		SInt32 destY = inDestRect.GetTop();
		for (int heightRemaining = inDestRect.GetSize().GetHeight();
				heightRemaining > 0;
				heightRemaining -= sourceSize.GetHeight()
				,destY += sourceSize.GetHeight())
		{
			SInt32 destX = inDestRect.GetLeft();
			for (int widthRemaining = inDestRect.GetSize().GetWidth();
				widthRemaining > 0; 
				widthRemaining -= sourceSize.GetWidth()
				,destX += sourceSize.GetWidth()) {
				
				AlphaDrawImage(inSrcImage, CPoint(destX, destY), inAlpha);
			}
		}
	} catch (...) {
		RETHROW_GRAPHICS_(eTileAlphaDrawImage);
	}
}

// ---------------------------------------------------------------------
//  TileColorKeyedDrawImage                                     [public]
// ---------------------------------------------------------------------
// Tile the ColorKeyedDrawImage call over a rect
void
CGraphicsPort::TileColorKeyedDrawImage (CImage& inSrcImage
										,const CRect& inDestRect
										,const CColor& inKeyColor) 
{
	try {
		CSize sourceSize = inSrcImage.GetSize();
		SInt32 destY = inDestRect.GetTop();
		for (int heightRemaining = inDestRect.GetSize().GetHeight();
				heightRemaining > 0;
				heightRemaining -= sourceSize.GetHeight()
				,destY += sourceSize.GetHeight())
		{
			SInt32 destX = inDestRect.GetLeft();
			for (int widthRemaining = inDestRect.GetSize().GetWidth();
				widthRemaining > 0; 
				widthRemaining -= sourceSize.GetWidth()
				,destX += sourceSize.GetWidth()) {
				
				ColorKeyedDrawImage(inSrcImage, CPoint(destX, destY), inKeyColor);
			}
		}
	} catch (...) {
		RETHROW_GRAPHICS_(eTileColorKeyedDrawImage);
	}
}

// ---------------------------------------------------------------------
//  ApplyImage			                                        [public]
// ---------------------------------------------------------------------
// Convenience function for some common operations
void
CGraphicsPort::ApplyImage( CImage& inSrcImage, const CRect& inDestRect
									,EApplyImageOp inOp)
{
	try {
		switch (inOp) {
			case eCenter:
				{
					int widthDelta = inDestRect.GetWidth() - inSrcImage.GetSize().GetWidth();
					int heightDelta = inDestRect.GetHeight() - inSrcImage.GetSize().GetHeight();
					if (widthDelta < 0 || heightDelta < 0)
						ApplyImage(inSrcImage, inDestRect, eScale);
					else
						DrawImage(inSrcImage, CPoint(widthDelta/2, heightDelta/2));
				}
				break;
				
			case eTile:
				TileDrawImage(inSrcImage, inDestRect);
				break;
			case eScale:
				DrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize()), inDestRect);
				break;
			default:
				break;
		}
	} catch (...) {
		RETHROW_GRAPHICS_(eApplyImage);
	}
}

// ---------------------------------------------------------------------
//  DrawImage                                                   [public]
// ---------------------------------------------------------------------
// Pass-through to standard DrawImage, that avoids scaling

void
CGraphicsPort::DrawImage ( CImage& inSrcImage, const CRect& inSourceRect, 
						const CPoint& inDestWhere)
{
	DrawImage(inSrcImage, inSourceRect, CRect(inDestWhere, inSourceRect.GetSize()));
}

// ---------------------------------------------------------------------
//  DrawImage                                                   [public]
// ---------------------------------------------------------------------
// Pass-through to standard DrawImage, no cropping or scaling
void
CGraphicsPort::DrawImage ( CImage& inSrcImage, const CPoint& inDestWhere)
{
	DrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize())
			,CRect(inDestWhere, inSrcImage.GetSize()));
}

// ---------------------------------------------------------------------
//  DrawImage                                                   [public]
// ---------------------------------------------------------------------
// Pass-through to standard DrawImage, no cropping
void
CGraphicsPort::DrawImage ( CImage& inSrcImage, const CRect& inDestRect)
{
	DrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize())
			,inDestRect);
}

// ---------------------------------------------------------------------
//  AlphaMaskImage                                              [public]
// ---------------------------------------------------------------------
// Pass-through to AlphaMaskImage, that avoids scaling

void
CGraphicsPort::AlphaMaskImage ( CImage& inSrcImage, const CRect& inSourceRect, 
						const CPoint& inDestWhere, CImage* inAlphaMask)
{
	AlphaMaskImage(inSrcImage, inSourceRect, CRect(inDestWhere, inSourceRect.GetSize())
			,inAlphaMask);
}

// ---------------------------------------------------------------------
//  AlphaMaskImage                                              [public]
// ---------------------------------------------------------------------
// Pass-through to AlphaMaskImage, no cropping or scaling
void
CGraphicsPort::AlphaMaskImage ( CImage& inSrcImage, const CPoint& inDestWhere,
		CImage* inAlphaMask)
{
	AlphaMaskImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize())
			,CRect(inDestWhere, inSrcImage.GetSize()), inAlphaMask);
}

// ---------------------------------------------------------------------
//  AlphaMaskImage                                              [public]
// ---------------------------------------------------------------------
// Pass-through to AlphaMaskImage, no cropping
void
CGraphicsPort::AlphaMaskImage ( CImage& inSrcImage, const CRect& inDestRect,
		CImage* inAlphaMask)
{
	AlphaMaskImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize())
			, inDestRect, inAlphaMask);
}

// ---------------------------------------------------------------------
//  AlphaDrawImage                                              [public]
// ---------------------------------------------------------------------
// Pass-through to AlphaDrawImage, that avoids scaling

void
CGraphicsPort::AlphaDrawImage ( CImage& inSrcImage, const CRect& inSourceRect, 
						const CPoint& inDestWhere, ColorChannel inAlpha)
{
	AlphaDrawImage(inSrcImage, inSourceRect
			, CRect(inDestWhere,inSourceRect.GetSize()) ,inAlpha);
}

// ---------------------------------------------------------------------
//  AlphaDrawImage                                              [public]
// ---------------------------------------------------------------------
// Pass-through to AlphaDrawImage, no cropping or scaling
void
CGraphicsPort::AlphaDrawImage ( CImage& inSrcImage, const CPoint& inDestWhere,
		ColorChannel inAlpha)
{
	AlphaDrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize()),
			CRect(inDestWhere, inSrcImage.GetSize()), inAlpha);
}

// ---------------------------------------------------------------------
//  AlphaDrawImage                                              [public]
// ---------------------------------------------------------------------
// Pass-through to AlphaDrawImage, no cropping
void
CGraphicsPort::AlphaDrawImage ( CImage& inSrcImage, const CRect& inDestRect,
		ColorChannel inAlpha)
{
	AlphaDrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize()),
			inDestRect, inAlpha);
}

// ---------------------------------------------------------------------
//  ColorKeyedDrawImage                                         [public]
// ---------------------------------------------------------------------
// Pass-through to ColorKeyedDrawImage, that avoids scaling

void
CGraphicsPort::ColorKeyedDrawImage ( CImage& inSrcImage, const CRect& inSourceRect, 
						const CPoint& inDestWhere, const CColor& inKeyColor)
{
	ColorKeyedDrawImage(inSrcImage, inSourceRect
			, CRect(inDestWhere,inSourceRect.GetSize()) ,inKeyColor);
}

// ---------------------------------------------------------------------
//  ColorKeyedDrawImage                                         [public]
// ---------------------------------------------------------------------
// Pass-through to ColorKeyedDrawImage, no cropping or scaling
void
CGraphicsPort::ColorKeyedDrawImage ( CImage& inSrcImage, const CPoint& inDestWhere,
		const CColor& inKeyColor)
{
	ColorKeyedDrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize()),
			CRect(inDestWhere, inSrcImage.GetSize()), inKeyColor);
}


// ---------------------------------------------------------------------
//  ClipImageBlast                                   [static][protected]
// ---------------------------------------------------------------------
// Clipping may need to create a temporary source, but it will not change dest
// It may alter the dest rect, however.

CImage*
CGraphicsPort::ClipImageBlast (	CImage& inSrcImage,
								CRect& ioSrcRect,
								CRect& ioDestRect,
								const CRect& inClipRect)
{	
	// Clip the source rect to the source image
	// This will make proportional changes to the dest rect as well
	ioSrcRect.ClipProportional(CRect(CPoint(0,0), inSrcImage.GetSize()), ioDestRect);

	// Clip the modified dest rect to the clip rect
	// Again this may proportionally shrink the source rect
	ioDestRect.ClipProportional(inClipRect, ioSrcRect);

	if (ioSrcRect.GetSize() == ioDestRect.GetSize() || ioSrcRect.IsEmpty() || ioDestRect.IsEmpty())
		return &inSrcImage;
		
	// Ok, now we handle scaling....
	CImage* tempBuffer = new CImage(ioDestRect.GetSize(), inSrcImage.GetColorDepth());
	TProxy<CGraphicsPort> tempPort = tempBuffer->GetGraphicsPort();
	tempPort->AlphaMaskImage(inSrcImage, ioSrcRect, CRect(CPoint(0,0), ioDestRect.GetSize()), 0);
	
	ioSrcRect.OffsetBy(CPoint(-ioSrcRect.GetLeft(), -ioSrcRect.GetTop()));
	ioSrcRect.GrowBy(CPoint(ioDestRect.GetWidth() - ioSrcRect.GetWidth(),
						ioDestRect.GetHeight() - ioSrcRect.GetHeight()));
	
	// Whew... done the scaling and the temp buffer allocation...
	return tempBuffer;
}

// ---------------------------------------------------------------------
//  ColorKeyedDrawImage                                         [public]
// ---------------------------------------------------------------------
// Pass-through to ColorKeyedDrawImage, no cropping
void
CGraphicsPort::ColorKeyedDrawImage ( CImage& inSrcImage, const CRect& inDestRect,
		const CColor& inKeyColor)
{
	ColorKeyedDrawImage(inSrcImage, CRect(CPoint(0,0), inSrcImage.GetSize()),
			inDestRect, inKeyColor);
}

// ---------------------------------------------------------------------
//  DrawRect													[public] 
// ---------------------------------------------------------------------
// 
// Draw the edges of the specified rect

void
CGraphicsPort::DrawRect (const CRect& inRect, const CColor& inColor, int inThickness)
{
	try {
		CPoint topRight(inRect.GetRight()-1, inRect.GetTop());
		CPoint bottomLeft(inRect.GetLeft(), inRect.GetBottom()-1);
		CPoint bottomRight(inRect.GetRight()-1, inRect.GetBottom()-1);
		DrawLine(inRect.GetTopLeft(), topRight, inColor, inThickness);
		DrawLine(inRect.GetTopLeft(), bottomLeft, inColor, inThickness);
		DrawLine(bottomRight, topRight, inColor, inThickness);
		DrawLine(bottomRight, bottomLeft, inColor, inThickness);
	} catch (...) {
		RETHROW_GRAPHICS_( eDrawRect );
	}
}

// ---------------------------------------------------------------------
//  ComputeNormal                                            [protected]
// ---------------------------------------------------------------------
// 
// Compute the normal for a line (from a to b)

CPoint 
CGraphicsPort::ComputeNormal (const CPoint& a, const CPoint& b, double inLength) {
	double tX = b.GetX() - a.GetX();
	double tY = b.GetY() - a.GetY();
	double tLength = tX * tX + tY * tY;
	tLength = sqrt(tLength);
	tX = tX / tLength;
	tY = tY / tLength;

	// Compute the normal from the tangent
	double nX = tY;
	double nY = -tX;

	// Scale up for integerness
	nX *= inLength;
	nY *= inLength;
	
	return CPoint(static_cast<SInt32>(nX), static_cast<SInt32>(nY));
}

// ---------------------------------------------------------------------
//  ComputeNormal                                              [private]
// ---------------------------------------------------------------------
// 
// Compute the normal for a vertex (on line from a->b, b->c)
// Done by intersection the two line normals.

CPoint 
CGraphicsPort::ComputeNormal (const CPoint& a, const CPoint& b, const CPoint& c, double inLength) {
	double tX1 = b.GetX() - a.GetX();
	double tY1 = b.GetY() - a.GetY();
	double tLength = tX1 * tX1 + tY1 * tY1;
	tLength = sqrt(tLength);
	tX1 = tX1 / tLength;
	tY1 = tY1 / tLength;

	// Compute the normal from the tangent
	double nX1 = tY1;
	double nY1 = -tX1;
	
	double tX2 = c.GetX() - b.GetX();
	double tY2 = c.GetY() - b.GetY();
	tLength = tX2 * tX2 + tY2 * tY2;
	tLength = sqrt(tLength);
	tX2 = tX2 / tLength;
	tY2 = tY2 / tLength;

	// Compute the normal from the tangent
	double nX2 = tY2;
	double nY2 = -tX2;

	double baseX1 = a.GetX() + nX1 * inLength;
	double baseY1 = a.GetY() + nY1 * inLength;
	double baseX2 = c.GetX() + nX2 * inLength;
	double baseY2 = c.GetY() + nY2 * inLength;
	
	if (tX2 != 0) {
		// t1 = (m2*x1+b2-y1)/(Ty1-m2*Tx1)
		double slope2 = tY2 / tX2;
		double yIntercept2 = baseY2 - baseX2 * slope2;
		double t1 = (slope2 * baseX1 + yIntercept2 - baseY1) / (tY1 - slope2 * tX1);

		return CPoint(static_cast<SInt32>(baseX1 + t1 * tX1)
						,static_cast<SInt32>(baseY1 + t1 * tY1));
	} else if (tX1 != 0) {
		// t2 = (m1*x2+b1-y2)/(Ty2-m1*Tx2)
		double slope1 = tY1 / tX1;
		double yIntercept1 = baseY1 - baseX1 * slope1;
		double t2 = (slope1 * baseX2 + yIntercept1 - baseY2) / (tY2 - slope1 * tX2);

		return CPoint(static_cast<SInt32>(baseX2 + t2 * tX2)
						,static_cast<SInt32>(baseY2 + t2 * tY2));
	} else {
		double nSumX = nX2 + nX1;
		double nSumY = nY2 + nY1;
		double nSumLength = nSumX * nSumX + nSumY * nSumY;
		nSumX /= nSumLength;
		nSumY /= nSumLength;
		return CPoint(static_cast<SInt32>(b.GetX() + nSumX * inLength)
					,static_cast<SInt32>(b.GetY() + nSumY * inLength));
	}
}


// ---------------------------------------------------------------------
//  DrawPoints													[public]
// ---------------------------------------------------------------------
// 
// Draw a set of points as either a polyline, a polygon or a filled
// polygon.

void
CGraphicsPort::DrawPoints (int inPointCount, const CPoint* inPoints
							,const CColor& inColor , EPointMode inMode
							,int inThickness)
{
	if (inMode == ePolygonFill) {
		FillPolygon(inPointCount, inPoints, inColor);
		return;
	} else if (inMode == eBezierDraft) {
		// Bezier curve drawing...
		// This is offloaded to a seperate function
		DrawBezier(inPointCount, inPoints, inColor, 8, inThickness);
		return;
	} else if (inMode == eBezierDetail) {
		DrawBezier(inPointCount, inPoints, inColor, 1024, inThickness);
		return;
	} else if (inThickness > 1) {
		inThickness = (inThickness + 1) / 2;
		// We need to be more clever on thick polylines,
		// otherwise we get cracks...  ugh...
		CPoint firstPointA;
		CPoint firstPointB;
		
		if (inMode == ePolygon) {
			firstPointA = ComputeNormal(inPoints[inPointCount-1], inPoints[0], inPoints[1], inThickness);
			firstPointB = ComputeNormal(inPoints[1], inPoints[0], inPoints[inPointCount-1], inThickness);
		} else {
			firstPointA = inPoints[0] + ComputeNormal(inPoints[0], inPoints[1], inThickness);
			firstPointB = inPoints[0] + ComputeNormal(inPoints[1], inPoints[0], inThickness);
		}
		
		
		CPoint lastPointA = firstPointA;
		CPoint lastPointB = firstPointB;
		
		// Draw all but n-1 to n, and (possibly) n to 0
		for (int i=1;i<inPointCount-1;++i) {
			CPoint thisPointA = ComputeNormal(inPoints[i-1], inPoints[i], inPoints[i+1], inThickness);
			CPoint thisPointB = 2 * inPoints[i] - thisPointA;

			CPoint pointArray[] = { lastPointA, thisPointA, thisPointB, lastPointB };
			DrawPoints (4, pointArray, inColor, ePolygonFill);
			lastPointA = thisPointA;
			lastPointB = thisPointB;
		}
		
		if (inMode == ePolyLine) {
			CPoint thisPointB = inPoints[inPointCount-1]
					+ ComputeNormal(inPoints[inPointCount-1], inPoints[inPointCount-2], inThickness);
			CPoint thisPointA = inPoints[inPointCount-1]
					+ ComputeNormal(inPoints[inPointCount-2], inPoints[inPointCount-1], inThickness);
			CPoint pointArray[] = { lastPointA, thisPointA
									,thisPointB, lastPointB };
			DrawPoints (4, pointArray, inColor, ePolygonFill);
		} else {
			CPoint thisPointA = ComputeNormal(inPoints[inPointCount-2], inPoints[inPointCount-1]
										, inPoints[0], inThickness);
			CPoint thisPointB = ComputeNormal(inPoints[0], inPoints[inPointCount-1]
										, inPoints[inPointCount-2], inThickness);
	
			CPoint pointArray1[] = { lastPointA, thisPointA, thisPointB, lastPointB };
			DrawPoints(4, pointArray1, inColor, ePolygonFill);
			
			CPoint pointArray2[] = { thisPointA, firstPointA, firstPointB, thisPointB };
			DrawPoints(4, pointArray2, inColor, ePolygonFill);
		}
	} else {
		for (int i=0;i<inPointCount-1;++i)
			DrawLine(inPoints[i], inPoints[i+1], inColor, inThickness);
		if (inMode == ePolygon)
			DrawLine(inPoints[inPointCount-1], inPoints[0], inColor, inThickness);
	}
}

// ---------------------------------------------------------------------
//  EvaluateBezier                                             [private]
// ---------------------------------------------------------------------
// 
// Evaluate the Bezier curve function at 0 <= inT <= 1
// The 4 points are the control points; the line passes through A and D.

CPoint
CGraphicsPort::EvaluateBezier (const CPoint& inA, const CPoint& inB
				,const CPoint& inC, const CPoint& inD
				,double inT)
{
	double t = inT;
	double invT = 1 - t;
	
	double x = inA.GetX() * invT * invT * invT;
	x += 3 * t * invT * invT * inB.GetX();
	x += 3 * t * t * invT * inC.GetX();
	x += t * t * t * inD.GetX();

	double y = inA.GetY() * invT * invT * invT;
	y += 3 * t * invT * invT * inB.GetY();
	y += 3 * t * t * invT * inC.GetY();
	y += t * t * t * inD.GetY();
	
	return CPoint(static_cast<SInt32>(x),static_cast<SInt32>(y));
}


// ---------------------------------------------------------------------
//  DrawBezier                                                 [private]
// ---------------------------------------------------------------------
// 
// Draw a Bezier curve out of lines.

void
CGraphicsPort::DrawBezier (int inPointCount, const CPoint* inPoints, const CColor& inColor, int inSteps, int inThickness)
{
	// The number of points must be a multiple of 4
	ASSERT((inPointCount & 3) == 0);
	
	// Detail is an issue we need to resolve another way
	double inDetail = 1.0 / inSteps;
	
	if (inThickness <= 1) {
		// If it's infinitely thin, simply compute all the points
		// and then draw a polyline.  Doesn't work for
		// thick curves, because the normal equations collapse
		// on high-resolution lines (i.e. 1 pixel long or less).
		for (int baseIndex = 0; baseIndex < inPointCount; baseIndex += 4)
		{
			const CPoint& ptA = inPoints[baseIndex];
			const CPoint& ptB = inPoints[baseIndex+1];
			const CPoint& ptC = inPoints[baseIndex+2];
			const CPoint& ptD = inPoints[baseIndex+3];
		
			CPoint* pointArray = new CPoint[inSteps+1];	
			for (int i = 0;i <= inSteps;++i)
			{
				double t = inDetail * i;
				pointArray[i] = EvaluateBezier(ptA,ptB,ptC,ptD,t);
			}
			DrawPoints(inSteps+1, pointArray, inColor, ePolyLine, 0);	
			delete[] pointArray;
		}
	} else {
		inThickness = (inThickness + 1) / 2;
		// Manually compute the polygons rather than call DrawPoints(ePolyline)
		// Because the polyline thickness computations are not safe for tiny line
		// segments (as in one pixel long or less).  This solution uses the normals
		// for the line between the first and last point on the curve, which is wrong
		// but is fast.  The correct solution is to compute the normal for each
		// point on the line which is rendered; however, that's one sqrt call per 
		// rendered point.  Slow.  Very slow.
		// We could use the bitwise shift+mask sqrt hack, but making it platform-independant
		// would take more time than is appropriate.
		CPoint lastPointA = EvaluateBezier(inPoints[0], inPoints[1], inPoints[2], inPoints[3], 0);
		CPoint firstNormal = ComputeNormal(lastPointA, 
				EvaluateBezier(inPoints[0], inPoints[1], inPoints[2], inPoints[3], 1), inThickness);
		CPoint lastPointB = lastPointA - firstNormal;
		lastPointA = lastPointA + firstNormal;
						
		for (int baseIndex = 0; baseIndex < inPointCount; baseIndex += 4)
		{
			const CPoint& ptA = inPoints[baseIndex];
			const CPoint& ptB = inPoints[baseIndex+1];
			const CPoint& ptC = inPoints[baseIndex+2];
			const CPoint& ptD = inPoints[baseIndex+3];
		
			CPoint thisNormal = ComputeNormal(
				EvaluateBezier(inPoints[baseIndex], inPoints[baseIndex + 1]
					,inPoints[baseIndex + 2], inPoints[baseIndex + 3], 0)
				,EvaluateBezier(inPoints[baseIndex], inPoints[baseIndex + 1]
					,inPoints[baseIndex + 2], inPoints[baseIndex + 3], 1), inThickness);

		
			for (int i=1; i <= inSteps; ++i)
			{
				double t = inDetail * i;
				CPoint thisPointA = EvaluateBezier(ptA, ptB, ptC, ptD, inDetail * i);
				CPoint thisPointB = thisPointA - thisNormal;
				thisPointA = thisPointA + thisNormal;
				
				CPoint pointArray[] = { lastPointA, thisPointA, thisPointB, lastPointB };
				DrawPoints(4, pointArray, inColor, ePolygonFill, 0);
				lastPointA = thisPointA;
				lastPointB = thisPointB;
			}		
		}
	}
}


// =====================================================================
//	StPortPen
// =====================================================================
//

/*
// ---------------------------------------------------------------------
//  StPortRegion                           
// ---------------------------------------------------------------------
// Constructor. Saves port origin and clipping

StPortRegion::StPortRegion( CGraphicsPort& inPort )
		: mSavePort( inPort ),
			mSaveOrigin( inPort.GetOrigin() ),
			mSaveClipping( inPort.GetClipping() )
{
}	


// ---------------------------------------------------------------------
//  StPortRegion                           
// ---------------------------------------------------------------------
// Constructor. Saves port origin and clipping and sets to new rect

StPortRegion::StPortRegion( CGraphicsPort& inPort, const CRect &inRect )
		: mSavePort( inPort ),
			mSaveOrigin( inPort.GetOrigin() ),
			mSaveClipping( inPort.GetClipping() )
{
	inPort.ClipTo( inRect );
	inPort.SetOrigin( inRect.GetTopLeft() );
}	


// ---------------------------------------------------------------------
//  ~StPortRegion                           
// ---------------------------------------------------------------------
// Destructor. Restores port origin and clipping

StPortRegion::~StPortRegion()
{
	mSavePort.SetOrigin( mSaveOrigin );
	mSavePort.SetClipping( mSaveClipping );
}	
*/

/*
#if 0
	#pragma mark -
#endif
// =====================================================================
//	StPortPen
// =====================================================================
//


// ---------------------------------------------------------------------
//  StPortPen                           
// ---------------------------------------------------------------------
// Constructor. Saves current pen state

StPortPen::StPortPen( CGraphicsPort& inPort )
		: mSavePort( inPort ),
			mSavePen( inPort.GetPen() )
{
}	


// ---------------------------------------------------------------------
//  StPortRegion                           
// ---------------------------------------------------------------------
// Constructor. Saves current pen state and sets to a new state

StPortPen::StPortPen( CGraphicsPort& inPort, const CPen &inPen )
		: mSavePort( inPort ),
			mSavePen( inPort.GetPen() )
{
	inPort.SetPen( inPen );
}	


// ---------------------------------------------------------------------
//  ~StPortPen                           
// ---------------------------------------------------------------------
// Destructor. Restores the saved pen state

StPortPen::~StPortPen()
{
	mSavePort.SetPen( mSavePen );
}	

*/
HL_End_Namespace_BigRedH
