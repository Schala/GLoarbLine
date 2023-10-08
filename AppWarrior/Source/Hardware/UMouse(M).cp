#if MACINTOSH

/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "UMouse.h"

#include <Threads.h>
#include <Quickdraw.h>
#include <Resources.h>
#include <Events.h>

/*
 * Global Variables
 */

static struct {
	Int16 cursorID;
	CCrsrHandle colorCursor;
	CursHandle bwCursor;
	Uint16 cursorWasLocked : 1;
} _MUData = { 0, nil, nil, false };

/* -------------------------------------------------------------------------- */

/*
 * SetImage() sets the onscreen image of the mouse/cursor/pointer to the
 * 'CURS' or 'crsr' resource with the specified ID.  Note that SetImage()
 * does not release the resource, so it should be marked purgeable.
 */
void UMouse::SetImage(Int32 inID)
{
	if (_MUData.cursorID != inID)				// if change
	{
		CCrsrHandle colorCurs = nil;
		CursHandle bwCurs = nil;
	
		// set the cursor
		if (inID == 0)							// standard cursor
		{
		#if TARGET_API_MAC_CARBON
			Cursor arrowCurs;
			::SetCursor(::GetQDGlobalsArrow(&arrowCurs));
		#else	
			::SetCursor(&qd.arrow);
		#endif
		}
		else
		{
			// look for the appropriate resource
			colorCurs = ::GetCCursor(inID);		// look for a color cursor ('crsr')
			if (colorCurs)
				::SetCCursor(colorCurs);
			else
			{
				// keep looking
				bwCurs = ::GetCursor(inID);		// look for a B&W cursor ('CURS')
				if (bwCurs)
				{
					_MUData.cursorWasLocked = (HGetState((Handle)bwCurs) & 0x80) != 0;
					if (!_MUData.cursorWasLocked)
						HLockHi((Handle)bwCurs);
					
					::SetCursor(*bwCurs);
				}
				else
				{
					// give up looking
				#if TARGET_API_MAC_CARBON
					Cursor arrowCurs;
					::SetCursor(::GetQDGlobalsArrow(&arrowCurs));
				#else	
					::SetCursor(&qd.arrow);
				#endif
				
					inID = 0;
				}
			}
		}
		
		// make the new cursor current
		if (_MUData.colorCursor) ::DisposeCCursor(_MUData.colorCursor);
		if (_MUData.bwCursor && !_MUData.cursorWasLocked) ::HUnlock((Handle)_MUData.bwCursor);
		_MUData.colorCursor = colorCurs;
		_MUData.bwCursor = bwCurs;
		_MUData.cursorID = inID;
	}
}

/*
 * GetImage() returns the ID of the current mouse/cursor/pointer image.
 */
Int32 UMouse::GetImage()
{
	return _MUData.cursorID;
}

/*
 * ResetImage() sets the image of the mouse/cursor/pointer to its standard
 * image (typically an arrow).
 */
void UMouse::ResetImage()
{
	_MUData.cursorID = -1;		// force it to change
	SetImage(0);
}

/*
 * Hide() makes the cursor invisible.
 */
void UMouse::Hide()
{
	::HideCursor();
}

/*
 * Hide() makes the cursor visible (use after a previous call to Hide()).
 */
void UMouse::Show()
{
	::ShowCursor();
}

/*
 * Obscure() temporarily hides the cursor - the cursor is redisplayed the next
 * time the user moves the mouse.
 */
void UMouse::Obscure()
{
	::ObscureCursor();
}

/*
 * GetLocation() sets <outLoc> to the location of the cursor in global
 * (screen) coordinates.
 */
void UMouse::GetLocation(SPoint& outLoc)
{
	Point pt;
	::GetMouse(&pt);
	::LocalToGlobal(&pt);
	outLoc.v = pt.v;
	outLoc.h = pt.h;
}

/*
 * SetLocation() sets the location of the cursor to <inLoc>, which should
 * be in global (screen) coordinates.
 */
void UMouse::SetLocation(const SPoint& /* inLoc */)
{
	DebugBreak("UMouse - SetLocation() is not yet implemented");
}

/*
 * IsDown() returns whether or not the specified button is currently
 * being held down by the user.
 */
bool UMouse::IsDown(Uint16 inButton)
{
	switch (inButton)
	{
		case mouseButton_Default:
		case mouseButton_First:
		case mouseButton_Left:
			return ::Button();
			break;
	}
	
	return false;
}

/*
 * IsStillDown() returns whether or not the specified button has been
 * continuously pressed since the most recent mouse down.
 */
bool UMouse::IsStillDown(Uint16 inButton)
{
	switch (inButton)
	{
		case mouseButton_Default:
		case mouseButton_First:
		case mouseButton_Left:
			return ::StillDown();
			break;
	}
	
	return false;
}

/*
 * To determine whether a sequence of mouse events constitutes a double click, measure the
 * elapsed time (in milliseconds) between a mouse-up and a mouse-down.  If the time between
 * the two mouse events is less than the value returned by GetDoubleClickTime(), the two
 * mouse events can then be interpreted as a double click.
 */
Uint32 UMouse::GetDoubleClickTime()
{
	// ask OS for click time in ticks, and then pull a swifty to convert to milliseconds :)
	return ((Uint32)::GetDblTime() * 50) / 3;
}

/*
 * WaitMouseDownMove() waits for either the mouse to move from the location
 * <inInitialLoc> or for the mouse button to be released.  If the mouse moves
 * away from the location before the mouse button is released, WaitMouseDownMove()
 * returns true.  If the mouse button is released before the mouse moved from the
 * location, WaitMouseDownMove() returns false.
 */
bool UMouse::WaitMouseDownMove(const SPoint& inInitialLoc, Uint16 inButton)
{
	Point pt;

	switch (inButton)
	{
		case mouseButton_Default:
		case mouseButton_First:
		case mouseButton_Left:
		
			while (::StillDown())
			{
				::GetMouse(&pt);
				::LocalToGlobal(&pt);
				
				if (((abs(inInitialLoc.h - pt.h) > 3) || (abs(inInitialLoc.v - pt.v) > 3) ))
					return true;
			}
			break;
	}
	
	return false;
}





#endif
