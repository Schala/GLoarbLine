/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */#include "CListView.h"#pragma options align=packedtypedef struct {	Uint16 cellHeight;	Uint16 rsvd;	Uint16 byteCount;	Uint32 itemCount;	Uint8 selectData[];} SListView;#pragma options align=reset/* -------------------------------------------------------------------------- */CListView::CListView(CViewHandler *inHandler, const SRect& inBounds)	: CSelectableItemsView(inHandler, inBounds){	mCellHeight = 19;}bool CListView::IsValidItem(Uint32 inItem) const{	return (inItem != 0 && inItem <= GetItemCount());}bool CListView::GetNextItem(Uint32& ioItem) const{	ioItem++;	return (ioItem <= GetItemCount());}bool CListView::GetPrevItem(Uint32& ioItem) const{	if (ioItem == 0)	{		ioItem = GetItemCount();		return (ioItem != 0);	}	else	{		ioItem--;		return (ioItem != 0 && ioItem <= GetItemCount());	}}void CListView::SetItemSelect(Uint32 inItem, bool inSelect){	Require(IsValidItem(inItem));		if (mSelectData.GetItem(inItem) != (inSelect!=0))	// if change	{		mSelectData.SetItem(inItem, inSelect);		RefreshItem(inItem);	}}bool CListView::IsItemSelected(Uint32 inItem) const{	return mSelectData.GetItem(inItem);}Uint32 CListView::GetFirstSelectedItem() const{	return mSelectData.GetFirstSet();}Uint32 CListView::GetLastSelectedItem() const{	return mSelectData.GetLastSet();}void CListView::SetItemRangeSelect(Uint32 inFirstItem, Uint32 inLastItem, bool inSelect){	Uint32 itemCount = GetItemCount();	SRect updateRect, r;	Uint32 i;		// ensure that start <= end	if (inFirstItem > inLastItem)	{		i = inFirstItem;		inFirstItem = inLastItem;		inLastItem = i;	}	// bring into range	if (itemCount == 0) return;	if (inFirstItem < 1) inFirstItem = 1;	if (inLastItem > itemCount) inLastItem = itemCount;	// by default, nothing needs to be updated	updateRect.SetEmpty();	// set each item	for (; inFirstItem <= inLastItem; inFirstItem++)	{		if (mSelectData.GetItem(inFirstItem) != (inSelect!=0))	// if change		{			mSelectData.SetItem(inFirstItem, inSelect);			SetItemSelect(inFirstItem, inSelect);						GetItemRect(inFirstItem, r);			updateRect |= r;		}	}		// refresh area that changed	Refresh(updateRect);}void CListView::GetItemRect(Uint32 inItem, SRect& outRect) const{	outRect.left = mBounds.left;	outRect.right = mBounds.right;	outRect.top = mBounds.top + GetTabHeight() + (inItem - 1) * mCellHeight;	outRect.bottom = outRect.top + mCellHeight;}void CListView::GetSelectedItemsRect(SRect& outRect) const{	Uint32 firstItem, lastItem;		firstItem = GetFirstSelectedItem();	lastItem = GetLastSelectedItem();		if (firstItem == 0 || lastItem == 0)		outRect.SetEmpty();	else	{		outRect.left = mBounds.left;		outRect.right = mBounds.right;		outRect.top = mBounds.top + GetTabHeight() + (firstItem - 1) * mCellHeight;		outRect.bottom = mBounds.top + GetTabHeight() + lastItem * mCellHeight;	}}Uint32 CListView::PointToItem(const SPoint& inPt) const{	Uint32 i, c;	Int32 v;	v = inPt.v - mBounds.top - GetTabHeight();		if (v <= 0)		i = 1;	else		i = ((Uint32)v / mCellHeight) + 1;		c = GetItemCount();	if (i > c) i = c;		return i;}void CListView::GetFullSize(Uint32& outWidth, Uint32& outHeight) const{	outHeight = GetFullHeight();	outWidth = GetFullWidth();}Uint32 CListView::GetFullHeight() const{	return GetItemCount() ? GetTabHeight() + GetItemCount() * mCellHeight : mBounds.GetHeight();}Uint32 CListView::GetFullWidth() const{	return mBounds.GetWidth();}void CListView::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 /* inDepth */){	Int32 nTabHeight = GetTabHeight();		// draw tab	SRect r;	if (nTabHeight)	{		r.SetEmpty();		ItemDraw(0, inImage, r, r);	}	// draw items	Uint32 itemCount = GetItemCount();	Uint32 i, endItem;	// calc starting item	if (inUpdateRect.top > mBounds.top + nTabHeight)	{		i = (inUpdateRect.top - mBounds.top - nTabHeight) / mCellHeight + 1;		if (i > itemCount) return;	}	else		i = 1;		// calc ending item	endItem = ((inUpdateRect.bottom <= mBounds.bottom ? inUpdateRect.bottom : mBounds.bottom) - mBounds.top - nTabHeight) / mCellHeight + 1;	if (endItem > itemCount) endItem = itemCount;		// draw each item	while (i <= endItem)	{		GetItemRect(i, r);		ItemDraw(i, inImage, r, inUpdateRect);		i++;	}}void CListView::DragEnter(const SDragMsgData& inInfo){	inherited::DragEnter(inInfo);}void CListView::DragLeave(const SDragMsgData& inInfo){	inherited::DragLeave(inInfo);}void CListView::ItemsInserted(Uint32 inAtItem, Uint32 inCount){	mSelectData.InsertItems(inAtItem, inCount, false);		SRect updateRect;	updateRect = mBounds;	updateRect.top = mBounds.top + GetTabHeight() + (inAtItem - 1) * mCellHeight;	Refresh(updateRect);		inherited::ItemsInserted(inAtItem, inCount);}void CListView::ItemsRemoved(Uint32 inAtItem, Uint32 inCount){	mSelectData.RemoveItems(inAtItem, inCount);	SRect updateRect;	updateRect = mBounds;	updateRect.top = mBounds.top + GetTabHeight() + (inAtItem - 1) * mCellHeight;	Refresh(updateRect);		inherited::ItemsRemoved(inAtItem, inCount);}void CListView::ItemsMoved(Uint32 inToItem, Uint32 inFromItem, Uint32 inCount){	SRect updateRect;	Uint32 start, end;		mSelectData.MoveItems(inToItem, inFromItem, inCount);	if (inToItem > inFromItem)	{		start = inFromItem;		end = inToItem + inCount;	}	else	{		start = inToItem;		end = inFromItem + inCount;	}	updateRect.left = mBounds.left;	updateRect.right = mBounds.right;	updateRect.top = mBounds.top + GetTabHeight() + (start - 1) * mCellHeight;	updateRect.bottom = mBounds.top + GetTabHeight() + (end - 1) * mCellHeight;		Refresh(updateRect);}void CListView::ItemsSwapped(Uint32 inItemA, Uint32 inItemB, Uint32 inCount){	SRect updateRect;	Uint32 start, end;		mSelectData.SwapItems(inItemA, inItemB, inCount);		if (inItemA > inItemB)	{		start = inItemB;		end = inItemA + inCount;	}	else	{		start = inItemA;		end = inItemB + inCount;	}	updateRect.left = mBounds.left;	updateRect.right = mBounds.right;	updateRect.top = mBounds.top + GetTabHeight() + (start - 1) * mCellHeight;	updateRect.bottom = mBounds.top + GetTabHeight() + (end - 1) * mCellHeight;		Refresh(updateRect);}