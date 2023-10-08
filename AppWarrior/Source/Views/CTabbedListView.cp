/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CTabbedListView.h"

/* -------------------------------------------------------------------------- */


CTabbedListView::CTabbedListView(CViewHandler *inHandler, const SRect& inBounds, bool inShowTabLine)
	: CListView(inHandler, inBounds), CTabbedItemsView(inShowTabLine)
{
}

/* -------------------------------------------------------------------------- */
#pragma mark -

bool CTabbedListView::SetBounds(const SRect& inBounds)
{
	if (!CListView::SetBounds(inBounds))
		return false;

	SetTabRectList();

	return true;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CTabbedListView::MouseDown(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseDown(inInfo);
	CListView::MouseDown(inInfo);
	
	if (mMoveTabIndex && !mHasMouseCapture)
		CaptureMouse();
}

void CTabbedListView::MouseUp(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseUp(inInfo);
	CListView::MouseUp(inInfo);

	if (!mMoveTabIndex && mHasMouseCapture)
		ReleaseMouse();
}

void CTabbedListView::MouseMove(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseMove(inInfo);
	CListView::MouseMove(inInfo);
}

void CTabbedListView::MouseLeave(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseLeave(inInfo);
	CListView::MouseLeave(inInfo);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CTabbedListView::ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions)
{
	#pragma unused(inUpdateRect)
	
	if (inListIndex)
	{
		Uint32 i = 0;
		SRect *pTabRect;

		while (mTabRectList.GetNext(pTabRect, i))
		{
			pTabRect->top = inBounds.top;
			pTabRect->bottom = inBounds.bottom;

			if (mShowTabLine && i > 1)
			{
				Int32 nLineHoriz = pTabRect->left - 2;
				if (nLineHoriz > inBounds.right)
					break;
				
				// draw tab line
				inImage->SetInkColor(mTabLineColor);
				inImage->DrawLine(SLine(nLineHoriz, pTabRect->top, nLineHoriz, pTabRect->bottom));
			}		
		}
	
		ItemDraw(inListIndex, inImage, inBounds, mTabRectList, inOptions);	
	}
	
	if (inOptions != kDrawItemForDrag)
		DrawTabHeader(inImage, inBounds);
}

// derived classes can provide implementation
void CTabbedListView::ItemDraw(Uint32, TImage, const SRect&, const CPtrList<SRect>&, Uint32){}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CTabbedListView::ItemMouseDown(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
			
	CListView::ItemMouseDown(inListIndex, inInfo);
}

void CTabbedListView::ItemMouseUp(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
		
	CListView::ItemMouseUp(inListIndex, inInfo);
}

void CTabbedListView::ItemMouseEnter(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
		
	CListView::ItemMouseEnter(inListIndex, inInfo);
}

void CTabbedListView::ItemMouseMove(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
	
	CListView::ItemMouseMove(inListIndex, inInfo);
}

void CTabbedListView::ItemMouseLeave(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
	
	CListView::ItemMouseLeave(inListIndex, inInfo);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CTabbedListView::RefreshTabView()
{
	Refresh(mBounds);
}

void CTabbedListView::GetTabViewBounds(SRect& outBounds)
{
	outBounds = mBounds;
}

void CTabbedListView::GetTabViewScrollPos(Uint32& outHorizPos, Uint32& outVertPos)
{
	outHorizPos = 0;
	outVertPos = 0;
	 
	if (dynamic_cast<CScrollerView *>(mHandler))
		(dynamic_cast<CScrollerView *>(mHandler))->GetScroll(outHorizPos, outVertPos);
}

Uint32 CTabbedListView::GetTabHeight() const
{
	return mTabHeight;
}
