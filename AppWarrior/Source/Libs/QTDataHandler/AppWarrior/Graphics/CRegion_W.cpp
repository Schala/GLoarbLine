/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// Win32 specific implementation of Regions

#include "AW.h"
#include "CModuleWin.h"
#include "CRegion.h"
#include "CRegionPS_W.h"


HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  CRegion                                                   	[public]
// ---------------------------------------------------------------------
// Constructor

CRegion::CRegion()
	: mPlatform(nil)
{
	mPlatform = new CRegion::CRegionPS(CRect());
}


// ---------------------------------------------------------------------
//  CRegion                                                   	[public]
// ---------------------------------------------------------------------
// Constructor

CRegion::CRegion( const CRect &inRect )
	: mPlatform(nil)
{
	mPlatform = new CRegion::CRegionPS(inRect);
}

// ---------------------------------------------------------------------
//  CRegion                                                   	[public]
// ---------------------------------------------------------------------
// Copy Constructor

CRegion::CRegion( const CRegion& inRegion )
	: mPlatform(nil)
{
	mPlatform = inRegion.mPlatform->Clone();
}


// ---------------------------------------------------------------------
//  ~CRegion                                                    [public]
// ---------------------------------------------------------------------
// Destructor

CRegion::~CRegion()
{
	if (mPlatform != nil)
	{
		delete mPlatform;
		mPlatform = nil;
	}
}


// ---------------------------------------------------------------------
//  GetBounds                                                   [public]
// ---------------------------------------------------------------------
// Get the surrounding rectangle for this region

CRect
CRegion::GetBounds() const
{
	ASSERT (mPlatform != nil);
	RECT r;
	int style = ::GetRgnBox(*mPlatform,&r);
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION
	mPlatform->SetStyle(style);	
	return CRect(r.top,r.left,r.bottom,r.right);
}


// ---------------------------------------------------------------------
//  OffsetBy                                                    [public]
// ---------------------------------------------------------------------
// Moves the region by the specified amount

void
CRegion::OffsetBy( const CPoint &inPoint )
{
	ASSERT (mPlatform != nil);
	int style = ::OffsetRgn( *mPlatform, inPoint.GetH(), inPoint.GetV() );
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION, ERROR
	if (style == ERROR)
		throw 0;
	mPlatform->SetStyle(style);	
}


// ---------------------------------------------------------------------
//  ResetOrigin                                                 [public]
// ---------------------------------------------------------------------
// Moves the region to the origin 0,0

void
CRegion::ResetOrigin()
{
	CRect theBounds = GetBounds();
	CPoint theOrigin( -theBounds.GetTop(), -theBounds.GetLeft() );
	OffsetBy( theOrigin );
}


// ---------------------------------------------------------------------
//  ContainsPoint                                               [public]
// ---------------------------------------------------------------------
// Check if the region contains that point.

bool
CRegion::ContainsPoint( const CPoint &inPoint ) const
{
	ASSERT (mPlatform != nil);
	return 0!=::PtInRegion(*mPlatform, inPoint.GetH(), inPoint.GetV() );
}

// ---------------------------------------------------------------------
//  Intersects                                                  [public]
// ---------------------------------------------------------------------
// Check if the region intersects the rectangle
bool
CRegion::Intersects( const CRect &inRect ) const
{
	ASSERT (mPlatform != nil);
	RECT r = {inRect.GetLeft(),inRect.GetTop()
			 ,inRect.GetRight(),inRect.GetBottom()};
	return 0!=::RectInRegion(*mPlatform, &r);
}


// ---------------------------------------------------------------------
//  Intersects                                                  [public]
// ---------------------------------------------------------------------
// Check if the region intersects the other region

bool
CRegion::Intersects( const CRegion& inRegion ) const
{
	ASSERT (mPlatform != nil);
	CRegion::CRegionPS result(CRect());
	int style = ::CombineRgn(result,*mPlatform,*inRegion.mPlatform,RGN_DIFF);
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION, ERROR
	if (style == ERROR)
		throw 0;
	return style!=NULLREGION;	
}


// ---------------------------------------------------------------------
//  Union                                                       [public]
// ---------------------------------------------------------------------
// Creates the union of two combined regions.

void
CRegion::Union( const CRegion& inRegion )
{
	ASSERT (mPlatform != nil);
	int style = ::CombineRgn(*mPlatform,*mPlatform,*inRegion.mPlatform,RGN_OR);
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION, ERROR
	if (style == ERROR)
		throw 0;
	mPlatform->SetStyle(style);	
}


// ---------------------------------------------------------------------
//  Difference                                                  [public]
// ---------------------------------------------------------------------
// Combines the parts of this that are not part of inRegion.

void
CRegion::Difference( const CRegion& inRegion )
{
	ASSERT (mPlatform != nil);
	int style = ::CombineRgn(*mPlatform,*mPlatform,*inRegion.mPlatform,RGN_DIFF);
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION, ERROR
	if (style == ERROR)
		throw 0;
	mPlatform->SetStyle(style);	
}

// ---------------------------------------------------------------------
//  Intersection                                                [public]
// ---------------------------------------------------------------------
// Creates the intersection of the two combined regions.

void
CRegion::Intersection( const CRegion& inRegion )
{
	ASSERT (mPlatform != nil);
	int style = ::CombineRgn(*mPlatform,*mPlatform,*inRegion.mPlatform,RGN_AND);
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION, ERROR
	if (style == ERROR)
		throw 0;
	mPlatform->SetStyle(style);	
}

// ---------------------------------------------------------------------
//  ExclusiveOR                                                 [public]
// ---------------------------------------------------------------------
// Creates the union of two combined regions except for any 
// overlapping areas.

void
CRegion::ExclusiveOR( const CRegion& inRegion )
{
	ASSERT (mPlatform != nil);
	int style = ::CombineRgn(*mPlatform,*mPlatform,*inRegion.mPlatform,RGN_XOR);
	// style == NULLREGION, SIMPLEREGION, COMPLEXREGION, ERROR
	if (style == ERROR)
		throw 0;
	mPlatform->SetStyle(style);	
}

// ---------------------------------------------------------------------
//  IsEmpty                                                     [public]
// ---------------------------------------------------------------------
// Checks if the region is empty

bool
CRegion::IsEmpty( ) const
{
	ASSERT (mPlatform != nil);
	int style = mPlatform->GetStyle();
	return style == NULLREGION || style == ERROR;
}

HL_End_Namespace_BigRedH
