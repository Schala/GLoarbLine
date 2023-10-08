#if WIN32

/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "UProgressBar.h"

extern HBRUSH _3DFACE_BRUSH, _ACTIVECAPTION_BRUSH;
extern HPEN _3DHILIGHT_PEN, _3DSHADOW_PEN;

HDC _ImageToDC(TImage inImage);

/* -------------------------------------------------------------------------- */

void UProgressBar_Draw(TImage inImage, const SRect& inBounds, const SProgressBarInfo& inInfo)
{
	HDC dc = _ImageToDC(inImage);
	HPEN savedPen;
	RECT r;
	Uint32 wid, maxWid;

	// draw dark (top and left edges)
	r = *(RECT *)&inBounds;
	savedPen = (HPEN)SelectObject(dc, _3DSHADOW_PEN);
	MoveToEx(dc, r.right-2, r.top, NULL);
	LineTo(dc, r.left, r.top);
	LineTo(dc, r.left, r.bottom-1);
	
	// draw light (bottom and right edges)
	SelectObject(dc, _3DHILIGHT_PEN);
	MoveToEx(dc, r.left, r.bottom-1, NULL);
	LineTo(dc, r.right-1, r.bottom-1);
	LineTo(dc, r.right-1, r.top-1);
	
	// restore pen
	SelectObject(dc, savedPen);

	// draw face
	r.top++;
	r.left++;
	r.bottom--;
	r.right--;
	FrameRect(dc, &r, _3DFACE_BRUSH);
	
	// calculate pixel width for progress bar
	maxWid = inBounds.GetWidth() - 4;
	if (inInfo.val < inInfo.max)
		wid = ((fast_float)inInfo.val / (fast_float)inInfo.max) * maxWid;
	else if (inInfo.val == 0)
		wid = 0;
	else
		wid = maxWid;
		
	// dark "done" part of progress bar
	r.top++;
	r.left++;
	r.bottom--;
	r.right = r.left + wid;
	FillRect(dc, &r, _ACTIVECAPTION_BRUSH);
	
	// draw "to do" part of progress bar
	r.left = r.right;
	r.right = inBounds.right - 2;
	FillRect(dc, &r, _3DFACE_BRUSH);
}

#endif /* WIN32 */
