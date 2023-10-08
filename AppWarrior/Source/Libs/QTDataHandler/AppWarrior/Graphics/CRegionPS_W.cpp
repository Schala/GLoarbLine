/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CModuleWin.h"
#include "CRegionPS_W.h"


HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  CRegionPS                                                   [public]
// ---------------------------------------------------------------------
// Constructor

CRegion::CRegionPS::CRegionPS(const CRect &inRect)
	: mHandle(NULL),mStyle(ERROR)
{
	CRect r(inRect);
	mHandle = ::CreateRectRgn(inRect.GetLeft(),inRect.GetTop()
							 ,inRect.GetRight(),inRect.GetBottom());
	// cannot use nil since I'm not supposed to know nil==NULL==0
	if (mHandle == NULL) 
	{
		throw 0;
	}
	mStyle = inRect.IsEmpty() ? NULLREGION : SIMPLEREGION;
}


// ---------------------------------------------------------------------
//  ~CRegionPS                                                  [public]
// ---------------------------------------------------------------------
// Destructor

CRegion::CRegionPS::~CRegionPS()
{
	// cannot use nil since I'm not supposed to know nil==NULL==0
	if (mHandle != NULL) 
		::DeleteObject( mHandle );
	mStyle = ERROR;
}


// ---------------------------------------------------------------------
//  Clone                                                       [public]
// ---------------------------------------------------------------------
// Make a copy of the region

CRegion::CRegionPS * 
CRegion::CRegionPS::Clone()
{
	// cannot use nil since I'm not supposed to know nil==NULL==0
	HRGN newHandle = ::CreateRectRgn(0,0,0,0);
	ASSERT(newHandle != NULL);
	int newStyle = ::CombineRgn(newHandle,mHandle,NULL,RGN_COPY);
	CRegionPS* clone = new CRegionPS( newHandle, newStyle );
	return clone;
}

HL_End_Namespace_BigRedH
