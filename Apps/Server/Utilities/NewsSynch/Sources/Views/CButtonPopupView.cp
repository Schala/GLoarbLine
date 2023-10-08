/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CButtonPopupView.h"
#include "CMenuListView.h"
#include "UPopupButton.h"


/* -------------------------------------------------------------------------- */

CButtonPopupView::CButtonPopupView(CViewHandler *inHandler, const SRect& inBounds)
	: CPopupView(inHandler, inBounds)
{
#if WIN32
	mCanFocus = true;
#endif

	mIsFocused = mIsHilited = mIsDrawDisabled = false;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CButtonPopupView::GetPopupButtonRect(SRect& outRect)
{
	outRect = mBounds;
	outRect.Enlarge(0,-1);
}

void CButtonPopupView::ShowPopupWin()
{
	if (!mIsHilited)
	{
		mIsHilited = true;
		Refresh();
	}
	
	CPopupView::ShowPopupWin();
}

void CButtonPopupView::HidePopupWin()
{
	if (mIsHilited)
	{
		mIsHilited = false;
		Refresh();
	}

	CPopupView::HidePopupWin();
}

void CButtonPopupView::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth)
{
	#pragma unused(inUpdateRect)
	SPopupButtonInfo info;
	SRect r;
	CSelectableItemsView *siv;
	Uint32 i, opts;
	
	info.title = nil;
	info.titleSize = 0;
	info.encoding = 0;
	info.options = (inDepth >= 4) ? 0 : 0x8000;
	
	if (mIsDrawDisabled)
	{
		UPopupButton::DrawDisabled(inImage, mBounds, info);
		opts = kDrawItemDisabledForButton;
	}
	else if (mIsHilited)
	{
		UPopupButton::DrawHilited(inImage, mBounds, info);
		opts = kDrawItemHilitedForButton;
	}
	else if (mIsFocused)
	{
		UPopupButton::DrawFocused(inImage, mBounds, info);
		opts = kDrawItemHilitedForButton;
	}
	else
	{
		UPopupButton::Draw(inImage, mBounds, info);
		opts = kDrawItemForButton;
	}
	
	if (UPopupButton::GetTitleRect(mBounds, r))
	{
		siv = dynamic_cast<CSelectableItemsView*>(mWindow->GetView());
		if (siv)
		{
			i = siv->GetFirstSelectedItem();
			if (i) siv->DrawOneItem(i, inImage, r, inUpdateRect, opts);
		}
	}
}

void CButtonPopupView::MouseDown(const SMouseMsgData& inInfo)
{
	CView::MouseDown(inInfo);
	/*
	 * inInfo.misc is set to 1000 if the click caused popup windows to hide.
	 * We want to make it so that clicking the button when the popup is shown
	 * causes the popup to hide (toggle).  The click outside the popup will
	 * cause the window to hide, so make sure we don't show it again here.
	 */
//	if (inInfo.misc != 1000 && IsEnabled() && IsActive())
//	{
		if (mIsHilited)
			HidePopupWin();
		else
			ShowPopupWin();
//	}
}

bool CButtonPopupView::KeyDown(const SKeyMsgData& inInfo)
{
	if (IsEnabled() && IsActive())
	{
		if (inInfo.keyCode == key_nEnter || inInfo.keyCode == key_Return)
		{
			if (mIsHilited)
				HidePopupWin();
			else
				ShowPopupWin();
			
			return true;
		}
	}
	
	return false;
}

void CButtonPopupView::UpdateActive()
{
	bool bStateChanged = false;
	
	bool bIsFocused = IsEnabled() && IsFocus();
	if (bIsFocused != mIsFocused)
	{
		mIsFocused = bIsFocused;
		bStateChanged = true;
	}

	bool bDrawDisab = IsDisabled() || IsInactive();
	if (bDrawDisab != mIsDrawDisabled)	// if change
	{
		mIsDrawDisabled = bDrawDisab;
		bStateChanged = true;
	}

	if (bStateChanged)
		Refresh();
}

bool CButtonPopupView::ChangeState(Uint16 inState)
{
	if (mState != inState)	// if change
	{
		if (inState != viewState_Active && inState != viewState_Focus)
			HidePopupWin();
		
		mState = inState;
		UpdateActive();
		return true;
	}
	
	return false;
}

bool CButtonPopupView::SetEnable(bool inEnable)
{
	if (mIsEnabled != (inEnable != 0))		// if change
	{
		mIsEnabled = (inEnable != 0);

		if (!inEnable) HidePopupWin();
		UpdateActive();
		
		return true;
	}
	
	return false;
}

CButtonPopupView *MakePopupMenuView(CViewHandler *inHandler, const SRect& inBounds, CMenuListView **outMenu)
{
	CButtonPopupView *pop = new CButtonPopupView(inHandler, inBounds);
	
	try
	{
		CMenuListView *mnu = new CMenuListView(pop->GetViewHandler(), SRect(0,0,100,100));

		mnu->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_MouseUpAction + itemBehav_MouseMoveSelect);
		mnu->Show();
		
		if (outMenu) *outMenu = mnu;
	}
	catch(...)
	{
		delete pop;
		throw;
	}
	
	return pop;
}


