/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "CTreeView.h"
#include "CTabbedItemsView.h"


// template tabbed tree view class
template <class T>
class CTabbedTreeView : public CTreeView<T>, public CTabbedItemsView
{	
	public:
		// construction
		CTabbedTreeView(CViewHandler *inHandler, const SRect& inBounds, Uint32 inCollapseID, Uint32 inDiscloseID, bool inShowTabLine = false);

		// properties
		virtual bool SetBounds(const SRect& inBounds);

		// mouse events
		virtual void MouseDown(const SMouseMsgData& inInfo);
		virtual void MouseUp(const SMouseMsgData& inInfo);
		virtual void MouseMove(const SMouseMsgData& inInfo);
		virtual void MouseLeave(const SMouseMsgData& inInfo);

	protected:
		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, T *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions = 0);
		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, T *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);

		// mouse events
		virtual void ItemMouseDown(Uint32 inListIndex, const SMouseMsgData& inInfo);
		virtual void ItemMouseUp(Uint32 inListIndex, const SMouseMsgData& inInfo);
		virtual void ItemMouseEnter(Uint32 inListIndex, const SMouseMsgData& inInfo);
		virtual void ItemMouseMove(Uint32 inListIndex, const SMouseMsgData& inInfo);
		virtual void ItemMouseLeave(Uint32 inListIndex, const SMouseMsgData& inInfo);
		
		// misc
		virtual void RefreshTabView();
		virtual void GetTabViewBounds(SRect& outBounds);
		virtual void GetTabViewScrollPos(Uint32& outHorizPos, Uint32& outVertPos);
		virtual Uint32 GetTabHeight() const;
};


template<class T> CTabbedTreeView<T>::CTabbedTreeView(CViewHandler *inHandler, const SRect& inBounds, Uint32 inCollapseID, Uint32 inDiscloseID, bool inShowTabLine)
	: CTreeView(inHandler, inBounds, inCollapseID, inDiscloseID), CTabbedItemsView(inShowTabLine)
{
}

/* -------------------------------------------------------------------------- */
#pragma mark -

template<class T> bool CTabbedTreeView<T>::SetBounds(const SRect& inBounds)
{
	if (!CTreeView::SetBounds(inBounds))
		return false;

	SetTabRectList();

	return true;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

template<class T> void CTabbedTreeView<T>::MouseDown(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseDown(inInfo);
	CTreeView::MouseDown(inInfo);

	if (mMoveTabIndex && !mHasMouseCapture)
		CaptureMouse();
}

template<class T> void CTabbedTreeView<T>::MouseUp(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseUp(inInfo);
	CTreeView::MouseUp(inInfo);

	if (!mMoveTabIndex && mHasMouseCapture)
		ReleaseMouse();
}

template<class T> void CTabbedTreeView<T>::MouseMove(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseMove(inInfo);
	CTreeView::MouseMove(inInfo);
}

template<class T> void CTabbedTreeView<T>::MouseLeave(const SMouseMsgData& inInfo)
{
	CTabbedItemsView::TabMouseLeave(inInfo);
	CTreeView::MouseLeave(inInfo);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

template<class T> void CTabbedTreeView<T>::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, T *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const SRect& inBounds, const SRect&, Uint32 inOptions)
{
	if (inTreeIndex)
	{
		Uint32 nTabLeft = 0, nTabRight = 0;

		Uint32 i = 0;
		SRect *pTabRect;

		while (mTabRectList.GetNext(pTabRect, i))
		{
			pTabRect->top = inBounds.top;
			pTabRect->bottom = inBounds.bottom;

			if (i == 1)
			{
				// save left/right values for first rect
				nTabLeft = pTabRect->left;
				nTabRight = pTabRect->right;
			
				pTabRect->left = inBounds.left;
				if (pTabRect->right < pTabRect->left)
					pTabRect->right = pTabRect->left;
			}
			else if (mShowTabLine)
			{
				Int32 nLineHoriz = pTabRect->left - 2;
				if (nLineHoriz > inBounds.right)
					break;

				// draw tab line
				inImage->SetInkColor(mTabLineColor);
				inImage->DrawLine(SLine(nLineHoriz, pTabRect->top, nLineHoriz, pTabRect->bottom));
			}		
		}
	
		ItemDraw(inTreeIndex, inTreeLevel, inTreeItem, inTreeViewItem, inImage, mTabRectList, inOptions);
	
		// restore left/right values for first rect
		pTabRect = mTabRectList.GetItem(1);
		if (pTabRect)
		{
			pTabRect->left = nTabLeft;
			pTabRect->right = nTabRight;
		}
	}
	
	if (inOptions != kDrawItemForDrag)
		DrawTabHeader(inImage, inBounds);
}

// derived classes can provide implementation
template<class T> void CTabbedTreeView<T>::ItemDraw(Uint32, Uint32, T*, STreeViewItem*, TImage, const CPtrList<SRect>&, Uint32){}

/* -------------------------------------------------------------------------- */
#pragma mark -

template<class T> void CTabbedTreeView<T>::ItemMouseDown(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
			
	CTreeView::ItemMouseDown(inListIndex, inInfo);
}

template<class T> void CTabbedTreeView<T>::ItemMouseUp(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
		
	CTreeView::ItemMouseUp(inListIndex, inInfo);
}

template<class T> void CTabbedTreeView<T>::ItemMouseEnter(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
	
	CTreeView::ItemMouseEnter(inListIndex, inInfo);
}

template<class T> void CTabbedTreeView<T>::ItemMouseMove(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
	
	CTreeView::ItemMouseMove(inListIndex, inInfo);
}

template<class T> void CTabbedTreeView<T>::ItemMouseLeave(Uint32 inListIndex, const SMouseMsgData& inInfo)
{
	if (mMoveTabIndex || IsMouseOnTab(inInfo.loc))
		return;
	
	CTreeView::ItemMouseLeave(inListIndex, inInfo);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

template<class T> void CTabbedTreeView<T>::RefreshTabView()
{
	Refresh(mBounds);
}

template<class T> void CTabbedTreeView<T>::GetTabViewBounds(SRect& outBounds)
{
	outBounds = mBounds;
}

template<class T> void CTabbedTreeView<T>::GetTabViewScrollPos(Uint32& outHorizPos, Uint32& outVertPos)
{
	outHorizPos = 0;
	outVertPos = 0;
	 
	if (dynamic_cast<CScrollerView *>(mHandler))
		(dynamic_cast<CScrollerView *>(mHandler))->GetScroll(outHorizPos, outVertPos);
}

template<class T> Uint32 CTabbedTreeView<T>::GetTabHeight() const
{
	return mTabHeight;
}

