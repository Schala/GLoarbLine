#if WIN32

/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "UScrollBar.h"

extern HPEN _3DSHADOW_PEN, _3DHILIGHT_PEN, _ACTIVECAPTION_PEN, _3DDKSHADOW_PEN;
extern HBRUSH _3DDKSHADOW_BRUSH, _ACTIVECAPTION_BRUSH, _3DSHADOW_BRUSH, _3DFACE_BRUSH, _HALFTONE_BRUSH;

static void _SBDrawBox(HDC inDC, const RECT *inRect);
static void _SBDrawBoxHilited(HDC inDC, const RECT *inRect);
static void _SBDrawBkgndBox(HDC inDC, const RECT *inRect, bool inHilited);
static void _SBDrawDownArrow(HDC inDC, const RECT *inRect, HPEN inPen);
static void _SBDrawUpArrow(HDC inDC, const RECT *inRect, HPEN inPen);
static void _SBDrawLeftArrow(HDC inDC, const RECT *inRect, HPEN inPen);
static void _SBDrawRightArrow(HDC inDC, const RECT *inRect, HPEN inPen);

HDC _ImageToDC(TImage inImage);

/* -------------------------------------------------------------------------- */

void UScrollBar_Draw(TImage inImage, const SRect& inBounds, const SScrollBarInfo& inInfo, Uint32 inHilitedPart)
{
	SRect partRects[kScrollBarPartCount];
	HDC dc = _ImageToDC(inImage);
	bool isHoriz = inBounds.GetWidth() > inBounds.GetHeight();
	
	if (isHoriz)
		UScrollBar::CalcHorizRects(inBounds, 16, 16, 16, inInfo.val, inInfo.max, inInfo.visVal, inInfo.visMax, partRects, kScrollBarPartCount);
	else
		UScrollBar::CalcVertRects(inBounds, 16, 16, 16, inInfo.val, inInfo.max, inInfo.visVal, inInfo.visMax, partRects, kScrollBarPartCount);
	
	if (partRects[sbIndex_Up].IsEmpty())
	{
		_SBDrawBox(dc, (RECT *)&inBounds);
	}
	else
	{
		// sbPart_PageUp
		if (!partRects[sbIndex_PageUp].IsEmpty())
			_SBDrawBkgndBox(dc, (RECT *)(partRects + sbIndex_PageUp), inHilitedPart == sbPart_PageUp);
		
		// sbPart_PageDown
		if (!partRects[sbIndex_PageDown].IsEmpty())
			_SBDrawBkgndBox(dc, (RECT *)(partRects + sbIndex_PageDown), inHilitedPart == sbPart_PageDown);

		// sbPart_Up
		if (inHilitedPart == sbPart_Up)
			_SBDrawBoxHilited(dc, (RECT *)(partRects + sbIndex_Up));
		else
			_SBDrawBox(dc, (RECT *)(partRects + sbIndex_Up));
		if (isHoriz)
			_SBDrawLeftArrow(dc, (RECT *)(partRects + sbIndex_Up), _3DDKSHADOW_PEN);
		else
			_SBDrawUpArrow(dc, (RECT *)(partRects + sbIndex_Up), _3DDKSHADOW_PEN);

		// sbPart_Down
		if (inHilitedPart == sbPart_Down)
			_SBDrawBoxHilited(dc, (RECT *)(partRects + sbIndex_Down));
		else
			_SBDrawBox(dc, (RECT *)(partRects + sbIndex_Down));
		if (isHoriz)
			_SBDrawRightArrow(dc, (RECT *)(partRects + sbIndex_Down), _3DDKSHADOW_PEN);
		else
			_SBDrawDownArrow(dc, (RECT *)(partRects + sbIndex_Down), _3DDKSHADOW_PEN);

		// sbPart_SecondUp
		if (!partRects[sbIndex_SecondUp].IsEmpty())
		{
			if (inHilitedPart == sbPart_SecondUp)
				_SBDrawBoxHilited(dc, (RECT *)(partRects + sbIndex_SecondUp));
			else
				_SBDrawBox(dc, (RECT *)(partRects + sbIndex_SecondUp));
			if (isHoriz)
				_SBDrawLeftArrow(dc, (RECT *)(partRects + sbIndex_SecondUp), _3DDKSHADOW_PEN);
			else
				_SBDrawUpArrow(dc, (RECT *)(partRects + sbIndex_SecondUp), _3DDKSHADOW_PEN);
		}
		
		// sbPart_SecondDown
		if (!partRects[sbIndex_SecondDown].IsEmpty())
		{
			if (inHilitedPart == sbPart_SecondDown)
				_SBDrawBoxHilited(dc, (RECT *)(partRects + sbIndex_SecondDown));
			else
				_SBDrawBox(dc, (RECT *)(partRects + sbIndex_SecondDown));
			if (isHoriz)
				_SBDrawRightArrow(dc, (RECT *)(partRects + sbIndex_SecondDown), _3DDKSHADOW_PEN);
			else
				_SBDrawDownArrow(dc, (RECT *)(partRects + sbIndex_SecondDown), _3DDKSHADOW_PEN);
		}
		
		// sbPart_Thumb
		if (!partRects[sbIndex_Thumb].IsEmpty())
			_SBDrawBox(dc, (RECT *)(partRects + sbIndex_Thumb));
	}
}

void UScrollBar_DrawDisabled(TImage inImage, const SRect& inBounds, const SScrollBarInfo& inInfo)
{
	SRect partRects[kScrollBarPartCount];
	HDC dc = _ImageToDC(inImage);
	bool isHoriz = inBounds.GetWidth() > inBounds.GetHeight();
	
	if (isHoriz)
		UScrollBar::CalcHorizRects(inBounds, 16, 16, 16, inInfo.val, inInfo.max, inInfo.visVal, inInfo.visMax, partRects, kScrollBarPartCount);
	else
		UScrollBar::CalcVertRects(inBounds, 16, 16, 16, inInfo.val, inInfo.max, inInfo.visVal, inInfo.visMax, partRects, kScrollBarPartCount);
	
	if (partRects[sbIndex_Up].IsEmpty())
	{
		_SBDrawBox(dc, (RECT *)&inBounds);
	}
	else
	{
		// sbPart_PageUp and Down
		SRect r = partRects[sbIndex_PageUp];
		if (!r.IsEmpty())
		{
			if (isHoriz)
			{
				if (partRects[sbIndex_PageDown].right > r.right)
					r.right = partRects[sbIndex_PageDown].right;
			}
			else
			{
				if (partRects[sbIndex_PageDown].bottom > r.bottom)
					r.bottom = partRects[sbIndex_PageDown].bottom;
			}
			_SBDrawBkgndBox(dc, (RECT *)&r, false);
		}

		// sbPart_Up
		_SBDrawBox(dc, (RECT *)(partRects + sbIndex_Up));
		if (isHoriz)
			_SBDrawLeftArrow(dc, (RECT *)(partRects + sbIndex_Up), _3DSHADOW_PEN);
		else
			_SBDrawUpArrow(dc, (RECT *)(partRects + sbIndex_Up), _3DSHADOW_PEN);

		// sbPart_Down
		_SBDrawBox(dc, (RECT *)(partRects + sbIndex_Down));
		if (isHoriz)
			_SBDrawRightArrow(dc, (RECT *)(partRects + sbIndex_Down), _3DSHADOW_PEN);
		else
			_SBDrawDownArrow(dc, (RECT *)(partRects + sbIndex_Down), _3DSHADOW_PEN);

		// sbPart_SecondUp
		if (!partRects[sbIndex_SecondUp].IsEmpty())
		{
			_SBDrawBox(dc, (RECT *)(partRects + sbIndex_SecondUp));
			if (isHoriz)
				_SBDrawLeftArrow(dc, (RECT *)(partRects + sbIndex_SecondUp), _3DSHADOW_PEN);
			else
				_SBDrawUpArrow(dc, (RECT *)(partRects + sbIndex_SecondUp), _3DSHADOW_PEN);
		}
		
		// sbPart_SecondDown
		if (!partRects[sbIndex_SecondDown].IsEmpty())
		{
			_SBDrawBox(dc, (RECT *)(partRects + sbIndex_SecondDown));
			if (isHoriz)
				_SBDrawRightArrow(dc, (RECT *)(partRects + sbIndex_SecondDown), _3DSHADOW_PEN);
			else
				_SBDrawDownArrow(dc, (RECT *)(partRects + sbIndex_SecondDown), _3DSHADOW_PEN);
		}
	}
}

/* -------------------------------------------------------------------------- */
#pragma mark -

static void _SBDrawBox(HDC inDC, const RECT *inRect)
{
	RECT r = *inRect;
	HPEN savedPen;
	COLORREF col;
	
	// draw frame
	::FrameRect(inDC, inRect, _3DDKSHADOW_BRUSH);
	
	// inset rect
	r.left++;
	r.top++;
	r.right--;
	r.bottom--;
	
	// draw light
	savedPen = (HPEN)SelectObject(inDC, _3DHILIGHT_PEN);
	::MoveToEx(inDC, r.right-2, r.top, NULL);
	::LineTo(inDC, r.left, r.top);
	::LineTo(inDC, r.left, r.bottom-1);
	
	// draw dark
	SelectObject(inDC, _3DSHADOW_PEN);
	::MoveToEx(inDC, r.left+1, r.bottom-1, NULL);
	::LineTo(inDC, r.right-1, r.bottom-1);
	::LineTo(inDC, r.right-1, r.top);
	
	// restore pen
	SelectObject(inDC, savedPen);

	// draw corners
	col = GetSysColor(COLOR_3DFACE);
	SetPixel(inDC, r.left, r.bottom-1, col);
	SetPixel(inDC, r.right-1, r.top, col);
	
	// inset rect again
	r.left++;
	r.top++;
	r.right--;
	r.bottom--;
	
	// draw face
	FillRect(inDC, &r, _3DFACE_BRUSH);
}

static void _SBDrawBoxHilited(HDC inDC, const RECT *inRect)
{
	RECT r = *inRect;
	
	// draw frame
	::FrameRect(inDC, inRect, _3DDKSHADOW_BRUSH);
	
	// inset rect
	r.left++;
	r.top++;
	r.right--;
	r.bottom--;
		
	// draw face
	FillRect(inDC, &r, _3DSHADOW_BRUSH);
}

static void _SBDrawBkgndBox(HDC inDC, const RECT *inRect, bool inHilited)
{
/*
	RECT r = *inRect;

	// draw frame
	::FrameRect(inDC, inRect, _3DDKSHADOW_BRUSH);
	
	// inset rect
	r.left++;
	r.top++;
	r.right--;
	r.bottom--;

	// fill rect
	FillRect(inDC, &r, _SCROLLBAR_BRUSH);
*/
	
	COLORREF savedBackColor, savedTextColor;
	
	if (inHilited)
	{
		savedBackColor = SetBkColor(inDC, RGB(0,0,0));
		savedTextColor = SetTextColor(inDC, GetSysColor(COLOR_3DSHADOW));
	}
	else
	{
		savedBackColor = SetBkColor(inDC, RGB(0xFF,0xFF,0xFF));
		savedTextColor = SetTextColor(inDC, GetSysColor(COLOR_SCROLLBAR));
	}
	
	HPEN savedPen = (HPEN)SelectObject(inDC, _3DDKSHADOW_PEN);
	HBRUSH savedBrush = (HBRUSH)SelectObject(inDC, _HALFTONE_BRUSH);
	
	Rectangle(inDC, inRect->left, inRect->top, inRect->right, inRect->bottom);
	
	SelectObject(inDC, savedBrush);
	SelectObject(inDC, savedPen);
	
	SetTextColor(inDC, savedTextColor);
	SetBkColor(inDC, savedBackColor);
}

static void _SBDrawDownArrow(HDC inDC, const RECT *inRect, HPEN inPen)
{
	Int32 h = inRect->left + (((inRect->right - inRect->left) - 7) / 2);
	Int32 v = inRect->top + (((inRect->bottom - inRect->top) - 4) / 2);
	
	HPEN savedPen = (HPEN)SelectObject(inDC, inPen);
	
	MoveToEx(inDC, h, v, NULL);
	LineTo(inDC, h+8, v);
	MoveToEx(inDC, h+1, v+1, NULL);
	LineTo(inDC, h+7, v+1);
	MoveToEx(inDC, h+2, v+2, NULL);
	LineTo(inDC, h+6, v+2);
	MoveToEx(inDC, h+3, v+3, NULL);
	LineTo(inDC, h+5, v+3);
	
	SelectObject(inDC, savedPen);
}

static void _SBDrawUpArrow(HDC inDC, const RECT *inRect, HPEN inPen)
{
	Int32 h = inRect->left + (((inRect->right - inRect->left) - 7) / 2);
	Int32 v = inRect->top + (((inRect->bottom - inRect->top) - 4) / 2);

	HPEN savedPen = (HPEN)SelectObject(inDC, inPen);

	MoveToEx(inDC, h+3, v, NULL);
	LineTo(inDC, h+5, v);
	MoveToEx(inDC, h+2, v+1, NULL);
	LineTo(inDC, h+6, v+1);
	MoveToEx(inDC, h+1, v+2, NULL);
	LineTo(inDC, h+7, v+2);
	MoveToEx(inDC, h, v+3, NULL);
	LineTo(inDC, h+8, v+3);
	
	SelectObject(inDC, savedPen);
}

static void _SBDrawLeftArrow(HDC inDC, const RECT *inRect, HPEN inPen)
{
	Int32 h = inRect->left + (((inRect->right - inRect->left) - 4) / 2);
	Int32 v = inRect->top + (((inRect->bottom - inRect->top) - 7) / 2);

	HPEN savedPen = (HPEN)SelectObject(inDC, inPen);
	
	MoveToEx(inDC, h+3, v, NULL);
	LineTo(inDC, h+3, v+8);
	
	MoveToEx(inDC, h+2, v+1, NULL);
	LineTo(inDC, h+2, v+7);
	
	MoveToEx(inDC, h+1, v+2, NULL);
	LineTo(inDC, h+1, v+6);
	
	MoveToEx(inDC, h, v+3, NULL);
	LineTo(inDC, h, v+5);
	
	SelectObject(inDC, savedPen);
}

static void _SBDrawRightArrow(HDC inDC, const RECT *inRect, HPEN inPen)
{
	Int32 h = inRect->left + (((inRect->right - inRect->left) - 4) / 2);
	Int32 v = inRect->top + (((inRect->bottom - inRect->top) - 7) / 2);

	HPEN savedPen = (HPEN)SelectObject(inDC, inPen);

	MoveToEx(inDC, h, v, NULL);
	LineTo(inDC, h, v+8);
	
	MoveToEx(inDC, h+1, v+1, NULL);
	LineTo(inDC, h+1, v+7);
	
	MoveToEx(inDC, h+2, v+2, NULL);
	LineTo(inDC, h+2, v+6);
	
	MoveToEx(inDC, h+3, v+3, NULL);
	LineTo(inDC, h+3, v+5);
	
	SelectObject(inDC, savedPen);
}





#endif /* WIN32 */
