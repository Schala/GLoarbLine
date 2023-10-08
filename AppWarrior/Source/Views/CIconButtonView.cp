/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CIconButtonView.h"
#include "UIconButton.h"

#pragma options align=packed

typedef struct {
	Int16 titleStyle;
	Int16 iconID;
	Uint16 options;
	Int16 script;
	Uint16 titleLen;
	Uint8 titleData[];
} SIconButtonView;

#pragma options align=reset

/* -------------------------------------------------------------------------- */

CIconButtonView::CIconButtonView(CViewHandler *inHandler, const SRect& inBounds)
	: CView(inHandler, inBounds)
{
	mFont = nil;
#if MACINTOSH
	mIcon = nil;
#endif
	mOptions = iconBtn_Medium;
	mIsFocused = mIsHilited = mIsFlashHilited = mIsAltHit = mIsDrawDisabled = false;
	mTitle = nil;
	mFlashTimer = nil;
}

CIconButtonView::CIconButtonView(CViewHandler *inHandler, const SRect& inBounds, Uint32 inCmdID, const Uint8 *inTitle, Int32 inIconID, Uint32 inOptions, TFontDesc inFont)
	: CView(inHandler, inBounds)
{
	mCommandID = inCmdID;
	mFont = inFont;
	mOptions = inOptions;
	mIsFocused = mIsHilited = mIsFlashHilited = mIsAltHit = mIsDrawDisabled = false;
	mTitle = inTitle ? UMemory::NewHandle(inTitle+1, inTitle[0]) : nil;
	mFlashTimer = nil;
	try
	{
	#if MACINTOSH
		mIcon = UIcon::Load(inIconID);
	#else
		mRez.Reload('ICON', inIconID);
	#endif
	}
	catch(...)
	{
		UMemory::Dispose(mTitle);
		throw;
	}
}

CIconButtonView::~CIconButtonView()
{
	delete mFont;
	UMemory::Dispose(mTitle);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CIconButtonView::SetTitle(const void *inText, Uint32 inSize)
{
	if (mTitle == nil)
		mTitle = UMemory::NewHandle(inText, inSize);
	else
		UMemory::Set(mTitle, inText, inSize);
	
	Refresh();
}

// takes ownership of the TFontDesc
void CIconButtonView::SetFont(TFontDesc inFont)
{
	delete mFont;
	mFont = inFont;
	Refresh();
}

#if MACINTOSH
void CIconButtonView::SetIconID(Int32 inID)
{
	SetIcon(UIcon::Load(inID));
}

void CIconButtonView::SetIcon(TIcon inIcon)
{
	if (inIcon == mIcon)
	{
		UIcon::Release(inIcon);
	}
	else
	{
		UIcon::Release(mIcon);
		mIcon = inIcon;
		Refresh();
	}
}

TIcon CIconButtonView::GetIcon() const
{
	return mIcon;
}
#endif

void CIconButtonView::SetOptions(Uint16 inOptions)
{
	if (mOptions != inOptions)
	{
		mOptions = inOptions;
		Refresh();
	}
}


void CIconButtonView::Flash()
{
	if (!mIsFlashHilited)
	{
		mFlashTimer = StartNewTimer(150);
		
		mIsFlashHilited = true;
		if (!mIsHilited)
			Refresh();
	}
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CIconButtonView::Draw(TImage inImage, const SRect& /* inUpdateRect */, Uint32 inDepth)
{
	SIconButtonInfo info;

	if (mTitle == nil)
	{
		info.title = nil;
		info.titleSize = 0;
	}
	else
	{
		info.titleSize = UMemory::GetSize(mTitle);
		info.title = UMemory::Lock(mTitle);
	}
	
	info.encoding = 0;

	try
	{
#if MACINTOSH
		info.icon = mIcon;
#else
		info.icon = mRez.GetHdl();
		info.iconLayer = 0;		// use best/nearest
#endif
		info.options = mOptions;
		if (inDepth < 4) info.options |= 0x8000;
		
		inImage->SetFont(mFont);

		if (mIsDrawDisabled)
			UIconButton::DrawDisabled(inImage, mBounds, info);
		else if (mIsHilited || mIsFlashHilited)
			UIconButton::DrawHilited(inImage, mBounds, info);
		else if (mIsFocused)
			UIconButton::DrawFocused(inImage, mBounds, info);
		else
			UIconButton::Draw(inImage, mBounds, info);
	}
	catch(...)
	{
		// don't throw
	}
	
	if (mTitle) 
		UMemory::Unlock(mTitle);
}

void CIconButtonView::MouseDown(const SMouseMsgData& inInfo)
{
	inherited::MouseDown(inInfo);
	
	if (IsEnabled() && IsActive())
	{
		mIsHilited = true;
		Refresh();
	}
}

void CIconButtonView::MouseUp(const SMouseMsgData& inInfo)
{
	inherited::MouseUp(inInfo);
	
	if (mIsHilited)
	{
		mIsHilited = false;
		Refresh();
		
		if (IsMouseWithin())
			Hit(hitType_Standard, inInfo.button, inInfo.mods);
	}
}

void CIconButtonView::MouseEnter(const SMouseMsgData& inInfo)
{
	inherited::MouseEnter(inInfo);
	
	if (!mIsHilited && IsAnyMouseBtnDown() && IsEnabled() && IsActive())
	{
		mIsHilited = true;
		Refresh();
	}
}

void CIconButtonView::MouseLeave(const SMouseMsgData& inInfo)
{
	inherited::MouseLeave(inInfo);
	
	if (mIsHilited)
	{
		mIsHilited = false;
		Refresh();
	}
}

bool CIconButtonView::KeyDown(const SKeyMsgData& inInfo)
{
	if (IsEnabled() && IsActive())
	{
		if (inInfo.keyCode == key_nEnter || inInfo.keyCode == key_Return || inInfo.keyCode == key_Escape || (inInfo.keyChar == '.' && (inInfo.mods & modKey_Command)))
		{
			if (!mIsFlashHilited)
			{
				mIsAltHit = (inInfo.mods & modKey_Option) != 0;
				Flash();
			}
			
			return true;
		}
	}
	
	return false;
}

void CIconButtonView::UpdateActive()
{
	bool bStateChanged = false;
	
	bool bIsFocused = IsEnabled() && IsFocus();
	if (bIsFocused != mIsFocused)
	{
		mIsFocused = bIsFocused;
		if (mIsFocused) mIsDrawDisabled = mIsHilited = mIsFlashHilited = false;
		bStateChanged = true;
	}

	bool bDrawDisab = IsDisabled() || IsInactive();
	if (bDrawDisab != mIsDrawDisabled)	// if change
	{
		mIsDrawDisabled = bDrawDisab;
		if (mIsDrawDisabled) mIsHilited = mIsFlashHilited = false;
		bStateChanged = true;
	}
	
	if (bStateChanged)
		Refresh();
}

bool CIconButtonView::ChangeState(Uint16 inState)
{
	if (mState != inState)	// if change
	{
		mState = inState;
		UpdateActive();
		return true;
	}
	
	return false;
}

bool CIconButtonView::SetEnable(bool inEnable)
{
	if (mIsEnabled != (inEnable != 0))		// if change
	{
		mIsEnabled = (inEnable != 0);
		UpdateActive();
		
		return true;
	}
	
	return false;
}

void CIconButtonView::Timer(TTimer inTimer)
{
	if (inTimer == mFlashTimer)
	{
		delete mFlashTimer;
		mFlashTimer = nil;
		
		if (mIsFlashHilited)
		{
			mIsFlashHilited = false;
			Refresh();
			Hit(mIsAltHit ? hitType_Alternate : hitType_Standard);
		}
	}
}

