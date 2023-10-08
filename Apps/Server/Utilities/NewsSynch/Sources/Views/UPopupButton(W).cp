#if WIN32


/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "UPopupButton.h"

#include <Windows.h>

void _DrawButton(HDC inDC, const RECT *inRect, const void *inTitle, Uint32 inTitleSize, bool inEnabled, bool inFocused = false);
void _DrawButtonHilited(HDC inDC, const RECT *inRect, const void *inTitle, Uint32 inTitleSize, bool inFocused = false);

HDC _ImageToDC(TImage inImage);
static void _DrawTri(HDC inDC, const RECT *inRect);

extern HPEN _3DHILIGHT_PEN, _3DSHADOW_PEN, _3DDKSHADOW_PEN;

/* -------------------------------------------------------------------------- */

void UPopupButton::Draw(TImage inImage, const SRect& inBounds, const SPopupButtonInfo& inInfo)
{
	HDC dc = _ImageToDC(inImage);
	
	_DrawButton(dc, (RECT *)&inBounds, inInfo.title, inInfo.titleSize, true);
	_DrawTri(dc, (RECT *)&inBounds);
}

void UPopupButton::DrawFocused(TImage inImage, const SRect& inBounds, const SPopupButtonInfo& inInfo)
{
	Draw(inImage, inBounds, inInfo);

	RECT r = *(RECT *)&inBounds;
	r.top += 2;
	r.left += 2;
	r.bottom -= 3;
	r.right -= 3;
	
	HDC dc = _ImageToDC(inImage);
	DrawFocusRect(dc, &r);
}

void UPopupButton::DrawHilited(TImage inImage, const SRect& inBounds, const SPopupButtonInfo& inInfo)
{
	RECT r = *(RECT *)&inBounds;
	HDC dc = _ImageToDC(inImage);

	_DrawButtonHilited(dc, &r, inInfo.title, inInfo.titleSize);
	
	r.left++;
	r.top++;
	r.right++;
	r.bottom++;
	
	_DrawTri(dc, &r);
}

void UPopupButton::DrawDisabled(TImage inImage, const SRect& inBounds, const SPopupButtonInfo& inInfo)
{
	RECT r = *(RECT *)&inBounds;
	HDC dc = _ImageToDC(inImage);

	_DrawButton(dc, &r, inInfo.title, inInfo.titleSize, false);

	r.left++;
	r.top++;
	r.right++;
	r.bottom++;

	HPEN savedPen = (HPEN)::SelectObject(dc, _3DHILIGHT_PEN);
	_DrawTri(dc, &r);
	
	::SelectObject(dc, _3DSHADOW_PEN);
	_DrawTri(dc, (RECT *)&inBounds);
	
	::SelectObject(dc, savedPen);
}

// returns true if you should draw the title
bool UPopupButton::GetTitleRect(const SRect& inBounds, SRect& outRect)
{
	outRect.top = inBounds.top + 2;
	outRect.bottom = inBounds.bottom - 2;
	outRect.left = inBounds.left + 2;
	outRect.right = inBounds.right - 22;
	
	return (outRect.GetHeight() >= 8) && (outRect.GetWidth() >= 16);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

static void _DrawTri(HDC inDC, const RECT *inRect)
{
	Int32 h = inRect->left + (((inRect->right - inRect->left)-8)/2) - 1;
	Int32 v = inRect->top + (((inRect->bottom - inRect->top)-5)/2);

	MoveToEx(inDC, h, v, NULL);
	LineTo(inDC, h+9, v);
	MoveToEx(inDC, h+1, v+1, NULL);
	LineTo(inDC, h+8, v+1);
	MoveToEx(inDC, h+2, v+2, NULL);
	LineTo(inDC, h+7, v+2);
	MoveToEx(inDC, h+3, v+3, NULL);
	LineTo(inDC, h+6, v+3);
	MoveToEx(inDC, h+4, v+4, NULL);
	LineTo(inDC, h+5, v+4);
}



#endif /* WIN32 */
