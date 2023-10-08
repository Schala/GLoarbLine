/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CPictureView.h"

#pragma options align=packed

typedef struct {
	Int16 pictureID;
} SPictureView;

#pragma options align=reset

/* -------------------------------------------------------------------------- */

CPictureView::CPictureView(CViewHandler *inHandler, const SRect& inBounds)
	: CView(inHandler, inBounds)
{
#if MACINTOSH
	mPictureID = 0;
#endif
}

CPictureView::CPictureView(CViewHandler *inHandler, const SRect& inBounds, Int32 inPictID)
	: CView(inHandler, inBounds)
{
#if MACINTOSH
	mPictureID = inPictID;
#else
	mRez.Reload('PIXM', inPictID);
#endif
}

#if MACINTOSH
void CPictureView::SetPicture(Int16 inID)
{
	mPictureID = inID;
	Refresh();
}
Int16 CPictureView::GetPicture() const
{
	return mPictureID;
}
#endif

void CPictureView::Draw(TImage inImage, const SRect& /* inUpdateRect */, Uint32 /* inDepth */)
{
#if MACINTOSH
	UGraphics::DrawPicture(inImage, mPictureID, mBounds);
#else
	UPixmap::Draw(mRez.GetHdl(), 0, inImage, mBounds, align_Center, transform_None);
	// ****** have an option for a different alignment than center ?  Useful for different pictures with same rectangle, multilingual pics etc
#endif
}

char **_GetMacRes(Uint32 inType, Int16 inID);

CWindow *NewPictureWindow(Int16 inID, Int16 inLayer, bool inInset)
{
	CWindow *win;
	CPictureView *pv;
	CContainerView *vc;
	SRect pr, r;
	
	// get size of picture
#if MACINTOSH
	TPicture pic = (TPicture)_GetMacRes('PICT', inID);
	{
		pr.top = pr.left = inInset ? -1 : 0;
		pr.bottom = pr.top + UGraphics::GetPictureHeight(pic);
		pr.right = pr.left + UGraphics::GetPictureWidth(pic);
	}
#else
	pr.top = pr.left = 0;
	pr.bottom = pr.right = 200;
#endif
	
	// create window
	r.top = r.left = 0;
	r.bottom = pr.GetHeight();
	r.right = pr.GetWidth();
	if (inInset)
	{
		r.bottom -= 2;
		r.right -= 2;
	}
	win = new CWindow(r, inLayer, 0, 1);
	
	try
	{
		// create view container
		vc = new CContainerView(win, r);
		vc->Show();
		
		// create picture view
		pv = new CPictureView(vc, pr);
		pv->SetPicture(inID);
		pv->Show();
		
		// center window
		win->SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	}
	catch(...)
	{
		// clean up
		delete win;
		throw;
	}
	
	// return a window centered on screen with a picture in it
	return win;
}


