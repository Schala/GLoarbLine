/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CProgressView.h"
#include "UProgressBar.h"

#pragma options align=packed

typedef struct {
	Uint32 val, max;
	Int16 rsvd;
} SProgressView;

#pragma options align=reset

/* -------------------------------------------------------------------------- */

CProgressView::CProgressView(CViewHandler *inHandler, const SRect& inBounds)
	: CView(inHandler, inBounds)
{
	mValue = 0;
	mMaxValue = 1000;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CProgressView::SetValue(Uint32 inValue)
{
	if (mValue != inValue)
	{
		mValue = inValue;
		Refresh();
	}
}

Uint32 CProgressView::GetValue() const
{
	return mValue;
}

void CProgressView::SetMaxValue(Uint32 inValue)
{
	if (mMaxValue != inValue)
	{
		mMaxValue = inValue;
		Refresh();
	}
}

Uint32 CProgressView::GetMaxValue() const
{
	return mMaxValue;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CProgressView::Draw(TImage inImage, const SRect& /* inUpdateRect */, Uint32 inDepth)
{
	SProgressBarInfo info;
	
	info.val = mValue;
	info.max = mMaxValue;
	info.options = (inDepth >= 4) ? 0 : 0x8000;
	
	UProgressBar::Draw(inImage, mBounds, info);
}



