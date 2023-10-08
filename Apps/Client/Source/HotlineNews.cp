/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "Hotline.h"

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

Uint32 _LookUpNewsMimeFlav(const Uint8 inFlav[])
{
	if (!inFlav)
		return 0;
		
	if (!pstrcmp(inFlav, "\ptext/plain"))
		return hlNewsFlav_plain_text;
	else if (!pstrcmp(inFlav, "\pimage/jpeg"))
		return hlNewsFlav_image_jpeg;
	else if (!pstrcmp(inFlav, "\pimage/gif"))
		return hlNewsFlav_image_gif;
	else
		return 0;
}

TIcon CMyNewsArticleTreeView::mThreadRootIcon = nil;

CMyNewsArticleTreeView::CMyNewsArticleTreeView(CViewHandler *inHandler, const SRect &inBounds, CNZReadList *inReadList)
	: CMyTreeStatusView(inHandler, inBounds)
{
	if (!mThreadRootIcon)
	
	if (gApp->mIconSet == 2){
		mThreadRootIcon = UIcon::Load(3040);
	}else if (gApp->mIconSet == 1){
		mThreadRootIcon = UIcon::Load(223);
		}
		
			
	mReadList = inReadList;
	mKeyDownTime = 0;

	// set headers
	AddTab("\pTitle", 80, 80);
	AddTab("\pSize", 1, 1, align_CenterHoriz);
	AddTab("\pPoster");
	AddTab("\pDate", 1, 1, align_CenterHoriz);
	AddTab("\pTime", 1, 1, align_CenterHoriz);
	SetTabs(40,5,25,15,15);
}

CMyNewsArticleTreeView::~CMyNewsArticleTreeView()
{
	DeleteAll();
}

void CMyNewsArticleTreeView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2, Uint8 inTabPercent3, Uint8 inTabPercent4, Uint8 inTabPercent5)
{
	if (!inTabPercent1 && !inTabPercent2 && !inTabPercent3 && !inTabPercent4 && !inTabPercent5)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	lPercentList.AddItem(&inTabPercent3);
	lPercentList.AddItem(&inTabPercent4);
	lPercentList.AddItem(&inTabPercent5);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyNewsArticleTreeView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2, Uint8& outTabPercent3, Uint8& outTabPercent4, Uint8& outTabPercent5)
{
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
	outTabPercent3 = GetTabPercent(3);
	outTabPercent4 = GetTabPercent(4);
	outTabPercent5 = GetTabPercent(5);
}

bool CMyNewsArticleTreeView::SetItemsFromData(const Uint8 *inData, Uint32 inDataSize)
{
	CUnflatten unflat(inData, inDataSize);
	if (unflat.NotEnufData(8))
		return false;
	
	unflat.SkipLong();	// don't care about ID
	Uint32 nCount = unflat.ReadLong();
	
	if (nCount > 10000)
		nCount = 10000;	// can't get crazy here...
	
	if (!unflat.SkipPString())	// name
		return false;
		
	if (!unflat.SkipPString())	// desc
		return false;
		
	SMyNewsArticle *pNewsArticle = nil;

	try
	{
		DeleteAll();
		
		if (!nCount)
			return false;

	#if USE_NEWS_HISTORY
		if (mReadList)
			mReadList->SetTotalCount(nCount);
	#endif

		// now for the individual articles
		while (nCount--)
		{
			pNewsArticle = (SMyNewsArticle *)UMemory::NewClear(sizeof(SMyNewsArticle));
			
			if (unflat.NotEnufData(4 + 8 + 4 + 4 + 2 + 2))
			{
				// corrupt
				Fail(errorType_Misc, error_Corrupt);
			}
			
			pNewsArticle->id = unflat.ReadLong();
			
		#if USE_NEWS_HISTORY
			if (mReadList)
				pNewsArticle->read = mReadList->CheckRead(pNewsArticle->id);
		#endif		
			
			unflat.ReadDateTimeStamp(pNewsArticle->dts);
			Uint32 nParentID = unflat.ReadLong();
			Uint32 nParentIndex = 0;
			
			if (nParentID)
			{
				Uint32 i = 0;
				SMyNewsArticle *pTmpNewsArticle;
				
				while (GetNextTreeItem(pTmpNewsArticle, i))
				{
					if (pTmpNewsArticle->id == nParentID)
					{
						nParentIndex = i;
						
						if (GetTreeItemLevel(nParentIndex) == 1)
							SetDisclosure(nParentIndex, optTree_Collapsed);
						else
							SetDisclosure(nParentIndex, optTree_Disclosed);
							
						break;
					}
				}
				
			#if DEBUG
				if (!nParentIndex)
					DebugBreak("Corrupt data - it says I have a parent, but none of that ID exists!");
			#endif
			}
			
			pNewsArticle->flags = unflat.ReadLong();
			Uint16 flavCount = unflat.ReadWord();
			pNewsArticle->flavCount = flavCount;
			
			// title
			Uint8 pstrLen = unflat.ReadByte();
			
			if (unflat.NotEnufData(pstrLen + 1))
			{
				// corrupt!
				Fail(errorType_Misc, error_Corrupt);
			}
			
			pNewsArticle->title[0] = UMemory::Copy(pNewsArticle->title + 1, unflat.GetPtr(), min((Uint32)pstrLen, (Uint32)31));
			unflat.Skip(pstrLen);
			
			
			
			// poster
			pstrLen = unflat.ReadByte();
			
			if (unflat.NotEnufData(pstrLen))
			{
				// corrupt!
				Fail(errorType_Misc, error_Corrupt);
			}
			
			pNewsArticle->poster[0] = UMemory::Copy(pNewsArticle->poster + 1, unflat.GetPtr(), min((Uint32)pstrLen, (Uint32)31));
			unflat.Skip(pstrLen);			
			
			pNewsArticle->flavors = 0;
			pNewsArticle->size = 0;
			while (flavCount--)
			{
				Uint8 *flav = unflat.ReadPString();
				if (!flav || unflat.NotEnufData(sizeof(Uint16)))
				{
					// corrupt
					Fail(errorType_Misc, error_Corrupt);
				}
				
				pNewsArticle->flavors |= _LookUpNewsMimeFlav(flav);
				pNewsArticle->size += unflat.ReadWord();
			}
			
			AddTreeItem(nParentIndex, pNewsArticle, false);
			pNewsArticle = nil;
		}
		
	#if USE_NEWS_HISTORY
		if (mReadList)
			mReadList->PurgeUnchecked();
	#endif		
	}
	catch(...)
	{
		UMemory::Dispose((TPtr)pNewsArticle);
		return GetTreeCount() != 0;
	}
	
	return (GetTreeCount() != 0);
}

Uint32 CMyNewsArticleTreeView::SearchNames(const Uint8 *inStr)
{
	Uint8 str[256];
	Uint8 searchStr[256];
		
	UMemory::Copy(searchStr, inStr, inStr[0] + 1);
	UText::MakeLowercase(searchStr + 1, searchStr[0]);

	Uint32 i = 0;
	SMyNewsArticle *pNewsArticle;
	
	while (GetNextVisibleTreeItem(pNewsArticle, i))
	{
		UMemory::Copy(str, pNewsArticle->title, pNewsArticle->title[0] + 1);
		UText::MakeLowercase(str + 1, str[0]);
		
		if (UMemory::Search(searchStr + 1, searchStr[0], str + 1, str[0]))
			return i;
	}
	
	return 0;
}

void CMyNewsArticleTreeView::DeleteAll()
{
	Uint32 i = 0;
	SMyNewsArticle *pNewsArticle;
	
	while (GetNextTreeItem(pNewsArticle, i))
		UMemory::Dispose((TPtr)pNewsArticle);
	
	ClearTree();
}

bool CMyNewsArticleTreeView::AddListFromFields(TFieldData inFields)
{
	Uint32 nSize = inFields->GetFieldSize(myField_NewsArtListData);
	
	if (nSize)
	{
		StPtr data(nSize);
		nSize = inFields->GetField(myField_NewsArtListData, BPTR(data), nSize);
		return SetItemsFromData(BPTR(data), nSize);
	}
	
	return false;
}

Uint32 CMyNewsArticleTreeView::GetSelectedItemNameAndID(Uint8 *outItemName)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	if (!nSelected)
		return 0;
		
	SMyNewsArticle *pNewsArticle = GetTreeItem(nSelected);
	if (!pNewsArticle)
		return 0;
	
	if (outItemName)
		UMemory::Copy(outItemName, pNewsArticle->title, pNewsArticle->title[0] + 1);
		
	return pNewsArticle->id;
}

// same as above, but marks the item as read
Uint32 CMyNewsArticleTreeView::ReadSelectedItemNameAndID(Uint8 *outItemName)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	if (!nSelected)
		return 0;
		
	SMyNewsArticle *pNewsArticle = GetTreeItem(nSelected);
	if (!pNewsArticle)
		return 0;
	
	pNewsArticle->read = true;
	
	if (outItemName)
		UMemory::Copy(outItemName, pNewsArticle->title, pNewsArticle->title[0] + 1);
	
	return pNewsArticle->id;
}

void CMyNewsArticleTreeView::SetSelectedItem(Uint32 inID)
{
	DeselectAllTreeItems();

	Uint32 i = 0;
	SMyNewsArticle *pNewsArticle;
	
	while (GetNextTreeItem(pNewsArticle, i))
	{
		if (pNewsArticle->id == inID)
		{
			MakeTreeItemVisible(i);
			SelectTreeItem(i);
			MakeTreeItemVisibleInList(i, align_InsideHoriz + align_InsideVert);
			return;
		}
	}
}

// same as above, but also sets this as read
void CMyNewsArticleTreeView::SetCurrentItem(Uint32 inID)
{
	DeselectAllTreeItems();

	Uint32 i = 0;
	SMyNewsArticle *pNewsArticle;
	
	while (GetNextTreeItem(pNewsArticle, i))
	{
		if (pNewsArticle->id == inID)
		{
			MakeTreeItemVisible(i);
			SelectTreeItem(i);
			MakeTreeItemVisibleInList(i, align_InsideHoriz + align_InsideVert);
			pNewsArticle->read = true;
			return;
		}
	}
}

bool CMyNewsArticleTreeView::KeyDown(const SKeyMsgData& inInfo)
{
	if (inInfo.keyCode == key_Left || inInfo.keyCode == key_Right || inInfo.keyCode == key_Up || inInfo.keyCode == key_Down)
		mKeyDownTime = UDateTime::GetMilliseconds();
		
	return CMyTreeStatusView::KeyDown(inInfo);
}

void CMyNewsArticleTreeView::SelectionChanged(Uint32 inTreeIndex, SMyNewsArticle *inTreeItem, bool inIsSelected)
{
	#pragma unused(inTreeIndex, inTreeItem, inIsSelected)

	CMyNewsCategoryExplWin *win = (CMyNewsCategoryExplWin *)GetDragAndDropHandler();
		
	if (win)
	{
		win->SetAccess();

		if (UDateTime::GetMilliseconds() - mKeyDownTime > 500)
			win->GetContentArticleText();
		else
			win->GetContentArticleTextTimer();
	}
}

void CMyNewsArticleTreeView::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyNewsArticle *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inOptions)
		
	Uint32 nSize;
	Uint8 bufText[32];

	SRect stRect;
	SColor stTextCol;
	bool bIsActive = IsFocus() && mIsEnabled && inTreeViewItem->bIsSelected;
	
	if (bIsActive)
		UUserInterface::GetSysColor(sysColor_InverseHighlight, stTextCol);
	else
		UUserInterface::GetSysColor(sysColor_Label, stTextCol);
	
	// set color
	inImage->SetInkColor(stTextCol);

	// set font
	inImage->SetFont(kDefaultFont, nil, 9);
	inImage->SetFontEffect(fontEffect_Plain);
		
	// draw time
	const SRect* pBounds = inTabRectList.GetItem(5);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 1;
		stRect.right -= 2;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
			
		nSize = UDateTime::DateToText(inTreeItem->dts, bufText, sizeof(bufText), kTimeText);
		inImage->DrawTruncText(stRect, bufText, nSize, nil, textAlign_Right);
	}
	
	// draw date
	pBounds = inTabRectList.GetItem(4);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 1;
		stRect.right -= 1;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;

		nSize = UDateTime::DateToText(inTreeItem->dts, bufText, sizeof(bufText), kShortDateText);
		inImage->DrawTruncText(stRect, bufText, nSize, nil, textAlign_Right);
	}
	
	// draw poster
	pBounds = inTabRectList.GetItem(3);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 1;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
	
		inImage->DrawTruncText(stRect, inTreeItem->poster + 1, inTreeItem->poster[0], nil, textAlign_Left);
	}
	
	// draw size
	pBounds = inTabRectList.GetItem(2);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 1;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;

		nSize = UText::Format(bufText, sizeof(bufText), "%luK", (inTreeItem->size + 1023) / 1024);
		inImage->DrawTruncText(stRect, bufText, nSize, nil, align_CenterHoriz | align_CenterVert);
	}
	
	// draw icon and title
	pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		if (inTreeLevel == 1)
		{
			// set rect
			stRect = *pBounds;
			stRect.top += 2;
			stRect.bottom = stRect.top + 16;
			stRect.left += 2;
			stRect.right = stRect.left + 16;
	
			// draw icon
			if (stRect.right < pBounds->right)
				mThreadRootIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
		}

		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 24;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;

		// if the item has no data, make it gray
		if (inTreeItem->flavCount == 0)
			inImage->SetInkColor(color_Gray);
	
		// if it's unread, make it bold
		if (!inTreeItem->read)
			inImage->SetFontEffect(fontEffect_Bold);
		else if (inTreeViewItem->nDisclosure == optTree_Collapsed && GetChildTreeCount(inTreeIndex))
		{
			while (GetTreeItemLevel(++inTreeIndex) > inTreeLevel)
			{		
				SMyNewsArticle *pNewsArticle = GetTreeItem(inTreeIndex);
				if (!pNewsArticle)
					break;
			
				// if a child it's unread, make it bold
				if (!pNewsArticle->read)
				{
					inImage->SetFontEffect(fontEffect_Bold);
					break;
				}
			}
		}
			
		// draw title
		inImage->DrawTruncText(stRect, inTreeItem->title + 1, inTreeItem->title[0], nil, textAlign_Left);	
	}
}

void CMyNewsArticleTreeView::TreeItemMouseDown(Uint32 inTreeIndex, const SMouseMsgData& inInfo)
{
	CTabbedTreeView::TreeItemMouseDown(inTreeIndex, inInfo);

	if (inInfo.mods & modKey_Option)
	{
		SMyNewsArticle *pNewsArticle = GetTreeItem(inTreeIndex);
		
		if (pNewsArticle)
		{
			pNewsArticle->read = !pNewsArticle->read;
			
			if(mReadList)
				mReadList->SetRead(pNewsArticle->id, pNewsArticle->read);
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsArticleTreeWin::CMyNewsArticleTreeWin(CWindow *inParent, CNZReadList *inReadList, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsTreeWin(inParent, SRect(0, 0, 500, 200), inPathData, inPathSize, inPathSum),
	  CMySearchText(mVC, SRect(60, 10, 500 - 34, 24))
{
#if WIN32
	_SetWinIcon(*this, 413);
#endif

	gApp->GetNewsBoundsInfo();
	gApp->GetArticlesBoundsInfo();
	gApp->mArticleTreeWinList.AddItem(this);
		
	SRect cBounds(0, 0, 500, 200);
	CIconButtonView *icb;
	
	// Std stuff
	SetTitle("\pArticles");
	SetLimits(420,150);
	
	SRect r = cBounds;
	
	r.top += 3;
	r.bottom = r.top + 24;
	r.left += 3;
	r.right = r.left + 24;
	
	// buttons
	icb = new CIconButtonView(mVC, r);
	
	//iconset
	if (gApp->mIconSet == 2){
			icb->SetIconID(3039);
		}else if (gApp->mIconSet == 1){
			icb->SetIconID(223);
		}
	
	icb->SetID(viewID_NewArticle);
	icb->SetTooltipMsg("\pNew Article");
	icb->Show();
	mViews.newArt = icb;
		
	// refresh list
	r.left = r.right + 3;
	r.right = r.left + 24;

	icb = new CIconButtonView(mVC, r);
	if (gApp->mIconSet == 2){
			icb->SetIconID(3026);
		}else if (gApp->mIconSet == 1){
			icb->SetIconID(205);
		}
	
	icb->SetID(viewID_Refresh);
	icb->SetTooltipMsg("\pRefresh");
	icb->Show();
	mViews.refresh = icb;
		
	// trash article
	r.right = cBounds.right - 3;
	r.left = r.right - 24;
		
	icb = new CIconButtonView(mVC, r);
	if (gApp->mIconSet == 2){
			icb->SetIconID(3000);
		}else if (gApp->mIconSet == 1){
			icb->SetIconID(212);
		}
	
	icb->SetSizing(sizing_HorizSticky);
	icb->SetID(viewID_Delete);
	icb->SetTooltipMsg("\pDelete");
	icb->Show();
	mViews.trash = icb;

    // help
    /*
	r.right = r.left - 3;
	r.left = r.right - 24;
	icb = new CIconButtonView(mVC, r);
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpNews);
	icb->SetTooltipMsg("\pHelp");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();
	*/
	
	// make scroller view
	CScrollerView *scr = new CScrollerView(mVC, SRect(-1, 30, 501, 201), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + scrollerOption_GrowSpace + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	mVC->SetFocusView(scr);

	mTreeView = new CMyNewsArticleTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), inReadList);
	mTreeView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction);
	mTreeView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mTreeView->SetCanFocus(true);
	mTreeView->SetID(viewID_NewsCatTree);
	mTreeView->Show();

	SetAccess();
	SetBoundsInfo();
}

CMyNewsArticleTreeWin::~CMyNewsArticleTreeWin()
{
	gApp->mArticleTreeWinList.RemoveItem(this);
}

void CMyNewsArticleTreeWin::GetContent()
{
	TFieldData pListData = gApp->mCacheList.SearchArticleList(mPathData, mPathSize);

	if (pListData)
		SetContent(mPathData, mPathSize, pListData, false);
	else
		new CMyGetNewsArtListTask(mPathData, mPathSize);
}

void CMyNewsArticleTreeWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	GetNewsArticleTreeView()->DeleteAll();
	bool added = GetNewsArticleTreeView()->AddListFromFields(inData);

	SetStatus(added ? listStat_Hide : listStat_0Items);
	SetAccess();
	
	if (inCache)
		gApp->mCacheList.AddArticleList(inPathData, inPathSize, inData);
}

void CMyNewsArticleTreeWin::SetAccess()
{
	if (!mPathData || !mPathSize)
	{
		mViews.newArt->Disable();
		mViews.trash->Hide();
			
		return;
	}
	
	if (gApp->HasBundlePriv(myAcc_NewsPostArt)) 
		mViews.newArt->Enable();
	else 
		mViews.newArt->Disable();
	
	if (gApp->HasBundlePriv(myAcc_NewsDeleteArt)) 
		mViews.trash->Show();
	else 
		mViews.trash->Hide();
}

void CMyNewsArticleTreeWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stArticles.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stArticles);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	(dynamic_cast<CMyNewsArticleTreeView*>(mTreeView))->SetTabs(gApp->mOptions.stWinTabs.nArticlesTab1, gApp->mOptions.stWinTabs.nArticlesTab2, gApp->mOptions.stWinTabs.nArticlesTab3, gApp->mOptions.stWinTabs.nArticlesTab4, gApp->mOptions.stWinTabs.nArticlesTab5);
}

void CMyNewsArticleTreeWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stArticles);
	
	(dynamic_cast<CMyNewsArticleTreeView*>(mTreeView))->GetTabs(gApp->mOptions.stWinTabs.nArticlesTab1, gApp->mOptions.stWinTabs.nArticlesTab2, gApp->mOptions.stWinTabs.nArticlesTab3, gApp->mOptions.stWinTabs.nArticlesTab4, gApp->mOptions.stWinTabs.nArticlesTab5);
}

void CMyNewsArticleTreeWin::DoNewArticle()
{
	void *pPathData = GetPathPtr();
	Uint32 nPathSize = GetPathSize();
	Uint32 nPathChecksum = GetPathChecksum();

	void *pNewPathData = nil;
	if (nPathSize)
		pNewPathData = UMemory::New(pPathData, nPathSize);
	
	if (!pNewPathData)
		return;

	CNZReadList *pReadList = GetNewsArticleTreeView()->GetReadList();

	SRect stBounds;
	GetBounds(stBounds);

	gApp->DoNewsCatNewArticle(pNewPathData, nPathSize, nPathChecksum, pReadList, stBounds);
}

void CMyNewsArticleTreeWin::DoOpenArticle()
{
	Uint8 psArticleName[256];
	Uint32 nArticleID = GetNewsArticleTreeView()->ReadSelectedItemNameAndID(psArticleName);
	if (!nArticleID || !psArticleName[0])
		return;
	
	void *pPathData = GetPathPtr();
	Uint32 nPathSize = GetPathSize();
	Uint32 nPathChecksum = UMemory::Checksum(pPathData, nPathSize);

	void *pNewPathData = UMemory::New(pPathData, nPathSize);
	if (!pNewPathData)
		return;
	
	CNZReadList *pReadList = GetNewsArticleTreeView()->GetReadList();

	SRect stBounds;
	GetBounds(stBounds);

	gApp->DoNewsCatOpen(pNewPathData, nPathSize, nPathChecksum, nArticleID, psArticleName, pReadList, stBounds);
}

void CMyNewsArticleTreeWin::DoRefresh()
{
	GetNewsArticleTreeView()->DeleteAll();
	SetStatus(listStat_Loading);

	gApp->DoNewsCatRefresh(GetPathPtr(), GetPathSize());
}

void CMyNewsArticleTreeWin::DoDelete()
{
	Uint8 psArticleName[256];
	Uint32 nArticleID = GetNewsArticleTreeView()->GetSelectedItemNameAndID(psArticleName);
	if (!nArticleID)
		return;
	DebugBreak("ok");	
	gApp->DoNewsCatTrash(dynamic_cast<CMyItemsWin *>(this), GetPathPtr(), GetPathSize(), nArticleID, psArticleName);

}

void CMyNewsArticleTreeWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (SearchText_KeyDown(inInfo))
		return;
		
	CMyItemsTreeWin::KeyDown(inInfo);
}

void CMyNewsArticleTreeWin::SearchText(const Uint8 *inText)
{
	CMyNewsArticleTreeView *pArticleTreeView = GetNewsArticleTreeView();
	Uint32 nIndex = pArticleTreeView->SearchNames(inText);

	if (nIndex != pArticleTreeView->GetFirstSelectedTreeItem())
	{
		pArticleTreeView->DeselectAllTreeItems();
	
		if (nIndex)
		{
			pArticleTreeView->SelectTreeItem(nIndex);
			pArticleTreeView->MakeTreeItemVisibleInList(nIndex);
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsCategoryListView::CMyNewsCategoryListView(CViewHandler *inHandler, const SRect &inBounds)
	: CMyListStatusView(inHandler, inBounds)
{
	// set headers
	AddTab("\pName", inBounds.GetWidth() - 40, 60);
	AddTab("\pSize", 1, 1, align_CenterHoriz);
}

CMyNewsCategoryListView::~CMyNewsCategoryListView()
{
	DeleteAll();
}

void CMyNewsCategoryListView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
	if (!inTabPercent1 && !inTabPercent2)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyNewsCategoryListView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}

bool CMyNewsCategoryListView::AddListFromFields(TFieldData inData)
{
	Uint16 i, n;
	Uint32 s, startCount, addCount;
	
	union
	{
		Uint8 data[300];
		struct
		{
			Uint16 type;	// 2
			Uint16 count;
			Uint8 nameSize;
			Uint8 name[];
		
		} bundleInfo;
		
		struct
		{
			Uint16 type;	// 3
			Uint16 count;
			SGUID guid;
			Uint32 addSN;
			Uint32 delSN;
			Uint8 nameSize;
			Uint8 name[];
		
		} catInfo;
	};

	SNewsCatItm *itm;
	
	startCount = mList.GetItemCount();
	addCount = 0;
	n = inData->GetFieldCount();
	
	if (n && inData->GetFieldID(1) == myField_NewsCatListData15)
	{
		for (i=1; i<=n; i++)
		{
			if (inData->GetFieldID(i) == myField_NewsCatListData15)
			{
				s = inData->GetFieldSizeByIndex(i);
			
				if (s > 300 || s < 5)
					continue;
					
				inData->GetFieldByIndex(i, data, s);
				
				if (bundleInfo.type == TB((Uint16)2))
				{
					// we're a bundle
					Uint32 nameSize = bundleInfo.nameSize;
					itm = (SNewsCatItm *)UMemory::NewClear(sizeof(SNewsCatItm) + 1 + nameSize);
					itm->type = 2;
					
					if (gApp->mIconSet == 2){
						itm->icon = UIcon::Load(3041);
					}else if (gApp->mIconSet == 1){
						itm->icon = UIcon::Load(231);
					}
					
					itm->name[0] = UMemory::Copy(itm->name + 1, bundleInfo.name, nameSize);
					itm->count = FB(bundleInfo.count);
				}
				else if (catInfo.type == TB((Uint16)3))
				{
					// we're a category
					Uint32 nameSize = catInfo.nameSize;
					itm = (SNewsCatItm *)UMemory::New(sizeof(SNewsCatItm) + 1 + nameSize);
					
					itm->type = 3;
					
					if (gApp->mIconSet == 2){
						itm->icon = UIcon::Load(3023);
					}else if (gApp->mIconSet == 1){
						itm->icon = UIcon::Load(413);
					}
					
					UMemory::Copy(&itm->guid, &catInfo.guid, sizeof(SGUID));
					itm->addSN = FB(catInfo.addSN);
					itm->delSN = FB(catInfo.delSN);
					itm->count = FB(catInfo.count);
					itm->unread = itm->count && gApp->mNZHist->IsNewAddSN(itm->guid, itm->addSN);
					itm->name[0] = UMemory::Copy(itm->name + 1, catInfo.name, nameSize);
				}
				else
					continue;	// don't know wtf we are
				
				mList.AddItem(itm);
				addCount++;
			}
		}
	}
	else	// use an old version of this function - this can be deleted after april 15th
	{
		for (i=1; i<=n; i++)
		{
			if (inData->GetFieldID(i) == myField_NewsCatListData)
			{
				s = inData->GetFieldSizeByIndex(i);
				
				if (s > 256)
					s = 256;
				else if (s < 2)		// houston, we have a problem
					return false;
				
				itm = (SNewsCatItm *)UMemory::New(sizeof(SNewsCatItm) - 1 + 1 + s);
				
				inData->GetFieldByIndex(i, itm->name, s);
				itm->type = itm->name[0];
				itm->name[0] = s - 1;
				
				try
				{
					if (itm->type == 1)			// if category folder
					
						if (gApp->mIconSet == 2){
							itm->icon = UIcon::Load(3043);
						}else if (gApp->mIconSet == 1){
							itm->icon = UIcon::Load(231);
						}
						
					else if (itm->type == 10)	// if category
					
					if (gApp->mIconSet == 2){
							itm->icon = UIcon::Load(3040);
						}else if (gApp->mIconSet == 1){
							itm->icon = UIcon::Load(413);
						}
						
					else if (itm->type == 255)
						itm->icon = UIcon::Load(422);
					else
					{
						DebugBreak("unknown type in category listing");
						itm->icon = UIcon::Load(422);
					}
					
					mList.AddItem(itm);
				}
				catch(...)
				{
					UMemory::Dispose((TPtr)itm);
					throw;
				}
				
				addCount++;
			}
		}
	}

	mList.Sort(CompareNames);
	ItemsInserted(startCount+1, addCount);
	
	return addCount != 0;
}

Uint32 CMyNewsCategoryListView::SelectNames(const Uint8 *inStr)
{
	Uint32 i = 0;
	SNewsCatItm *pNewsCatItm;

	while (mList.GetNext(pNewsCatItm, i))
	{
		if (!UText::CompareInsensitive(inStr + 1, inStr[0], pNewsCatItm->name + 1, pNewsCatItm->name[0]))
		{
			DeselectAll();
			SelectItem(i);
			MakeItemVisible(i);	

			return i;
		}
	}
	
	return 0;
}

Uint32 CMyNewsCategoryListView::SearchNames(const Uint8 *inStr)
{
	Uint8 str[256];
	Uint8 searchStr[256];
	
	UMemory::Copy(searchStr, inStr, inStr[0] + 1);
	UText::MakeLowercase(searchStr + 1, searchStr[0]);
	
	Uint32 i = 0;
	SNewsCatItm *pNewsCatItm = nil;

	while (mList.GetNext(pNewsCatItm, i))
	{
		UMemory::Copy(str, pNewsCatItm->name, pNewsCatItm->name[0] + 1);
		UText::MakeLowercase(str + 1, str[0]);
		
		if (UMemory::Search(searchStr + 1, searchStr[0], str + 1, str[0]))
			return i;
	}
	
	return 0;
}

Uint16 CMyNewsCategoryListView::GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID)
{
	outItemName[0] = 0;
	if (outGUID) ClearStruct(*outGUID);

	Uint32 nSelected = GetFirstSelectedItem();
	if (!nSelected) 
		return 0;
	
	SNewsCatItm *pNewsCatItm = mList.GetItem(nSelected);
	if (!pNewsCatItm) 
		return 0;
	
	UMemory::Copy(outItemName, pNewsCatItm->name, pNewsCatItm->name[0] + 1);
	if(outGUID && pNewsCatItm->type == 3)	// if it's a category
		UMemory::Copy(outGUID, &pNewsCatItm->guid, sizeof(SGUID));
	
	return pNewsCatItm->type;
}

void CMyNewsCategoryListView::DeleteAll()
{
	Uint32 i = 0;
	SNewsCatItm *pNewsCatItm = nil;
	
	while (mList.GetNext(pNewsCatItm, i))
	{
		UIcon::Release(pNewsCatItm->icon);
		UMemory::Dispose((TPtr)pNewsCatItm);
	}
	
	Uint32 nCount = mList.GetItemCount();
	mList.Clear();
	ItemsRemoved(1, nCount);
}

Uint32 CMyNewsCategoryListView::GetItemCount() const
{
	return mList.GetItemCount();
}

void CMyNewsCategoryListView::SetItemSelect(Uint32 inItem, bool inSelect)
{
	CMyListStatusView::SetItemSelect(inItem, inSelect);
	
	((CMyNewsCategoryListWin*)GetDragAndDropHandler())->SetAccess();
}

void CMyNewsCategoryListView::ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inOptions)
	
	// get news item
	SNewsCatItm *pNewsCatItm = mList.GetItem(inListIndex);
	if (!pNewsCatItm)
		return;
	
	SRect stRect;
	SColor stHiliteCol, stTextCol;
	
	// get info
	bool bIsSelected = mSelectData.GetItem(inListIndex);
	bool bIsActive = IsFocus() && mIsEnabled;

	if (bIsSelected)
	{
		UUserInterface::GetHighlightColor(&stHiliteCol, &stTextCol);
		inImage->SetInkColor(stHiliteCol);
		
		stRect.Set(inBounds.left + 1, inBounds.top + 1, inBounds.right - 1, inBounds.bottom);
		
		if (bIsActive)
			inImage->FillRect(stRect);
		else
		{
			inImage->FrameRect(stRect);
			UUserInterface::GetSysColor(sysColor_Label, stTextCol);
		}		
	}
	else
		UUserInterface::GetSysColor(sysColor_Label, stTextCol);
		
	// draw light lines around this item
	stRect = inBounds; stRect.bottom++;
	UUserInterface::GetSysColor(sysColor_Light, stHiliteCol);
	inImage->SetInkColor(stHiliteCol);
	inImage->FrameRect(stRect);
	
	// set color
	inImage->SetInkColor(stTextCol);

	// set font
	inImage->SetFont(kDefaultFont, nil, 9);
	inImage->SetFontEffect(fontEffect_Plain);

	// draw item count
	if (pNewsCatItm->type == 2 || pNewsCatItm->type == 3)	// can remove this condition after apr 15
	{
		const SRect* pBounds = inTabRectList.GetItem(2);
		if (pBounds && pBounds->GetWidth())
		{
			// set rect
			stRect = *pBounds;
			stRect.top += 3;
			stRect.bottom -= 2;
			stRect.left += 1;
			stRect.right -= 2;
			if (stRect.right < stRect.left)
				stRect.right = stRect.left;
			
			Uint8 psText[16];
			psText[0] = UText::Format(psText + 1, sizeof(psText) - 1, "(%lu)", pNewsCatItm->count);
			
			inImage->DrawTruncText(stRect, psText + 1, psText[0], 0, align_Right | align_CenterVert);
		}
	}

	// draw item icon and name
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += 2;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
			pNewsCatItm->icon->Draw(inImage, stRect, align_Center, (bIsActive && bIsSelected) ? transform_Dark : transform_None);

		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 24;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;

		if(pNewsCatItm->type != 3 || pNewsCatItm->unread)
			inImage->SetFontEffect(fontEffect_Bold);

		// draw item name
		inImage->DrawTruncText(stRect, pNewsCatItm->name + 1, pNewsCatItm->name[0], 0, align_Left | align_CenterVert);
	}	
}

Int32 CMyNewsCategoryListView::CompareNames(void *inPtrA, void *inPtrB, void *inRef)
{
	#pragma unused(inRef)
	
	Uint8 *pNameA = ((SNewsCatItm *)inPtrA)->name;
	Uint8 *pNameB = ((SNewsCatItm *)inPtrB)->name;

	Int32 outVal = UText::CompareInsensitive(pNameA + 1, pNameB + 1, min(pNameA[0], pNameB[0]));
	if (outVal == 0 && pNameA[0] != pNameB[0])
		outVal = pNameA[0] > pNameB[0] ? 1 : -1;
	
	return outVal;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsCategoryTreeView::CMyNewsCategoryTreeView(CViewHandler *inHandler, const SRect& inBounds, TPtr *inPathData, Uint32 *inPathSize, bool inTreeExpl)
	: CMyTreeStatusView(inHandler, inBounds)
{	
	mPathData = inPathData;
	mPathSize = inPathSize;

	mTreeExpl = inTreeExpl;
	mKeyDownTime = 0;

	mRefreshBundleIndex = 0;
	
	// set headers
	AddTab("\pName", inBounds.GetWidth() - 40, 80);
	AddTab("\pSize", 1, 1, align_CenterHoriz);
}

CMyNewsCategoryTreeView::~CMyNewsCategoryTreeView()
{
	DeleteAll();
}

void CMyNewsCategoryTreeView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
	if (!inTabPercent1 && !inTabPercent2)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyNewsCategoryTreeView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}

bool CMyNewsCategoryTreeView::AddListFromFields(const void *inPathData, Uint32 inPathSize, TFieldData inData)
{
	Uint16 i, n;
	Uint32 s, addCount;
	
	union
	{
		Uint8 data[300];
		struct
		{
			Uint16 type;	// 2
			Uint16 count;
			Uint8 nameSize;
			Uint8 name[];
		
		} bundleInfo;
		
		struct
		{
			Uint16 type;	// 3
			Uint16 count;
			SGUID guid;
			Uint32 addSN;
			Uint32 delSN;
			Uint8 nameSize;
			Uint8 name[];
		
		} catInfo;
	};

	Uint32 nBundleIndex;
	if (!GetBundleIndex(inPathData, inPathSize, nBundleIndex))
		return false;

	SNewsCatItm *itm;
	
	addCount = 0;
	n = inData->GetFieldCount();
	
	if (n && inData->GetFieldID(1) == myField_NewsCatListData15)
	{
		for (i=1; i<=n; i++)
		{
			if (inData->GetFieldID(i) == myField_NewsCatListData15)
			{
				s = inData->GetFieldSizeByIndex(i);
			
				if (s > 300 || s < 5)
					continue;
					
				inData->GetFieldByIndex(i, data, s);
				
				if (bundleInfo.type == TB((Uint16)2))
				{
					// we're a bundle
					Uint32 nameSize = bundleInfo.nameSize;
					itm = (SNewsCatItm *)UMemory::NewClear(sizeof(SNewsCatItm) + 1 + nameSize);
					itm->type = 2;
					
					if (gApp->mIconSet == 2){
						itm->icon = UIcon::Load(3041);
					}else if (gApp->mIconSet == 1){
						itm->icon = UIcon::Load(231);
					}
					
					itm->name[0] = UMemory::Copy(itm->name + 1, bundleInfo.name, nameSize);
					itm->count = FB(bundleInfo.count);
				}
				else if (catInfo.type == TB((Uint16)3))
				{
					// we're a category
					Uint32 nameSize = catInfo.nameSize;
					itm = (SNewsCatItm *)UMemory::New(sizeof(SNewsCatItm) + 1 + nameSize);
					
					itm->type = 3;
					
					if (gApp->mIconSet == 2){
						itm->icon = UIcon::Load(3040);
					}else if (gApp->mIconSet == 1){
						itm->icon = UIcon::Load(413);
					}
					
					UMemory::Copy(&itm->guid, &catInfo.guid, sizeof(SGUID));
					itm->addSN = FB(catInfo.addSN);
					itm->delSN = FB(catInfo.delSN);
					itm->count = FB(catInfo.count);
					itm->unread = itm->count && gApp->mNZHist->IsNewAddSN(itm->guid, itm->addSN);
					itm->name[0] = UMemory::Copy(itm->name + 1, catInfo.name, nameSize);
				}
				else
					continue;	// don't know wtf we are
				
				AddTreeItem(nBundleIndex, itm, bundleInfo.type == TB((Uint16)2));
				addCount++;
			}
		}
	}
	else	// use an old version of this function - this can be deleted after april 15th
	{
		for (i=1; i<=n; i++)
		{
			if (inData->GetFieldID(i) == myField_NewsCatListData)
			{
				s = inData->GetFieldSizeByIndex(i);
				
				if (s > 256)
					s = 256;
				else if (s < 2)		// houston, we have a problem
					return false;
				
				itm = (SNewsCatItm *)UMemory::New(sizeof(SNewsCatItm) - 1 + 1 + s);
				
				inData->GetFieldByIndex(i, itm->name, s);
				itm->type = itm->name[0];
				itm->name[0] = s - 1;
				
				try
				{
					if (itm->type == 1)			// if category folder
						if (gApp->mIconSet == 2){
							itm->icon = UIcon::Load(3041);
						}else if (gApp->mIconSet == 1){
							itm->icon = UIcon::Load(231);
						}
						
					else if (itm->type == 10)	// if category
					
						if (gApp->mIconSet == 2){
							itm->icon = UIcon::Load(3040);
						}else if (gApp->mIconSet == 1){
							itm->icon = UIcon::Load(413);
						}
						
					else if (itm->type == 255)
						itm->icon = UIcon::Load(422);
					else
					{
						DebugBreak("unknown type in category listing");
						itm->icon = UIcon::Load(422);
					}
					
					AddTreeItem(nBundleIndex, itm, itm->type == 1);
				}
				catch(...)
				{
					UMemory::Dispose((TPtr)itm);
					throw;
				}
				
				addCount++;
			}
		}
	}
	
	// sort
	Sort(nBundleIndex, CompareNames);
	
	// update bundle count
	if (nBundleIndex)
	{
		SNewsCatItm *pBundle = GetTreeItem(nBundleIndex);
		if (pBundle)
		{
			Uint32 nCount = GetChildTreeCount(nBundleIndex);
			if (pBundle->count != nCount)
				pBundle->count = nCount;
		}
	}

	return (addCount != 0);
}

Uint32 CMyNewsCategoryTreeView::SelectNames(const Uint8 *inStr)
{
	Uint32 i = 0;
	SNewsCatItm *pNewsCatItm;
	
	while (GetNextVisibleTreeItem(pNewsCatItm, i))
	{
		if (!UText::CompareInsensitive(inStr + 1, inStr[0], pNewsCatItm->name + 1, pNewsCatItm->name[0]))
		{
			DeselectAllTreeItems();
			SelectTreeItem(i);
			MakeTreeItemVisibleInList(i);	

			return i;
		}
	}
	
	return 0;
}

Uint32 CMyNewsCategoryTreeView::SearchNames(const Uint8 *inStr)
{
	Uint8 str[256];
	Uint8 searchStr[256];
		
	UMemory::Copy(searchStr, inStr, inStr[0] + 1);
	UText::MakeLowercase(searchStr + 1, searchStr[0]);

	Uint32 i = 0;
	SNewsCatItm *pNewsCatItm;
	
	while (GetNextVisibleTreeItem(pNewsCatItm, i))
	{
		UMemory::Copy(str, pNewsCatItm->name, pNewsCatItm->name[0] + 1);
		UText::MakeLowercase(str + 1, str[0]);
		
		if (UMemory::Search(searchStr + 1, searchStr[0], str + 1, str[0]))
			return i;
	}
	
	return 0;
}

void *CMyNewsCategoryTreeView::RefreshSelectedBundlePath(Uint32& outPathSize, bool *outCategory)
{
	if (outCategory) *outCategory = false;
	void *pPathData;
	
	if (!mTreeExpl)
	{
		pPathData = GetSelectedCategoryPath(outPathSize);
		if (pPathData)
		{
			if (outCategory) *outCategory = true;
			return pPathData;
		}
	}
	
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	pPathData = GetBundlePath(nSelected, outPathSize);
	DeletePath(pPathData, outPathSize);

	if (nSelected)
	{
		SelectTreeItem(nSelected);
		
		if (GetDisclosure(nSelected) != optTree_Disclosed)
		{
			mRefreshBundleIndex = nSelected;
			SetDisclosure(nSelected, optTree_Disclosed);
		}
	}
		
	return pPathData;
}

void *CMyNewsCategoryTreeView::GetSelectedBundlePath(Uint32& outPathSize)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	return GetBundlePath(nSelected, outPathSize);
}

void *CMyNewsCategoryTreeView::GetSelectedParentBundlePath(Uint32& outPathSize)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SNewsCatItm *pNewsCatItm = GetTreeItem(nSelected);
	if (pNewsCatItm && pNewsCatItm->type == 2)		// if it's a bundle
		nSelected = GetParentTreeIndex(nSelected);
	
	return GetBundlePath(nSelected, outPathSize);
}

void *CMyNewsCategoryTreeView::GetSelectedCategoryPath(Uint32& outPathSize, SGUID *outGUID)
{
	outPathSize = 0;

	Uint8 psItemName[256];
	Uint16 nType = GetSelectedItemName(psItemName, outGUID);
	if (psItemName[0] == 0 || nType != 3) 
		return nil;

	void *pTempPathData = GetSelectedParentBundlePath(outPathSize);
	
	void *pPathData = UFileSys::MakePathData(pTempPathData, outPathSize, psItemName, outPathSize);
	UMemory::Dispose((TPtr)pTempPathData);

	return pPathData;
}

Uint16 CMyNewsCategoryTreeView::GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID)
{
	outItemName[0] = 0;
	if (outGUID) ClearStruct(*outGUID);

	Uint32 nSelected = GetFirstSelectedTreeItem();
	if (!nSelected) 
		return 0;
	
	SNewsCatItm *pNewsCatItm = GetTreeItem(nSelected);
	if (!pNewsCatItm) 
		return 0;
	
	UMemory::Copy(outItemName, pNewsCatItm->name, pNewsCatItm->name[0] + 1);
	if (outGUID && pNewsCatItm->type == 3)	// if it's a category
		UMemory::Copy(outGUID, &pNewsCatItm->guid, sizeof(SGUID));
	
	return pNewsCatItm->type;
}

bool CMyNewsCategoryTreeView::CollapseSelectedBundle()
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SNewsCatItm *pNewsCatItm = GetTreeItem(nSelected);
	if (!pNewsCatItm || pNewsCatItm->type != 2)
		return false;
	
	return Collapse(nSelected);
}

bool CMyNewsCategoryTreeView::DiscloseSelectedBundle()
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SNewsCatItm *pNewsCatItm = GetTreeItem(nSelected);
	if (!pNewsCatItm || pNewsCatItm->type != 2)
		return false;
	
	return Disclose(nSelected);
}

void CMyNewsCategoryTreeView::DeletePath(const void *inPathData, Uint32 inPathSize)
{
	Uint32 nBundleIndex;
	if (!GetBundleIndex(inPathData, inPathSize, nBundleIndex))
		return;

	if (!nBundleIndex)
	{
		DeleteAll();
		return;
	}
	
	Uint32 i = nBundleIndex;
	SNewsCatItm *pNewsCatItm;
	Uint32 nBundleLevel = GetTreeItemLevel(nBundleIndex);
	
	while (GetNextTreeItem(pNewsCatItm, i))
	{
		if (nBundleLevel >= GetTreeItemLevel(i))
			break;
		
		UIcon::Release(pNewsCatItm->icon);
		UMemory::Dispose((TPtr)pNewsCatItm);
	}

	RemoveChildTree(nBundleIndex);
}

void CMyNewsCategoryTreeView::DeleteAll()
{
	Uint32 i = 0;
	SNewsCatItm *pNewsCatItm;
	
	while (GetNextTreeItem(pNewsCatItm, i))
	{
		UIcon::Release(pNewsCatItm->icon);
		UMemory::Dispose((TPtr)pNewsCatItm);
	}
	
	ClearTree();
}

bool CMyNewsCategoryTreeView::KeyDown(const SKeyMsgData& inInfo)
{
	if (!mTreeExpl && (inInfo.keyCode == key_Left || inInfo.keyCode == key_Right || inInfo.keyCode == key_Up || inInfo.keyCode == key_Down))
		mKeyDownTime = UDateTime::GetMilliseconds();
		
	return CMyTreeStatusView::KeyDown(inInfo);
}

void CMyNewsCategoryTreeView::SelectionChanged(Uint32 inTreeIndex, SNewsCatItm *inTreeItem, bool inIsSelected)
{
	#pragma unused(inTreeIndex, inTreeItem, inIsSelected)

	if (mTreeExpl)
		((CMyNewsCategoryTreeWin*)GetDragAndDropHandler())->SetAccess();
	else
	{
		((CMyNewsCategoryExplWin*)GetDragAndDropHandler())->SetAccess();
	
		if (UDateTime::GetMilliseconds() - mKeyDownTime > 500)
			((CMyNewsCategoryExplWin*)GetDragAndDropHandler())->GetContentArticleList();
		else
			((CMyNewsCategoryExplWin*)GetDragAndDropHandler())->GetContentArticleListTimer();
	}
}

void CMyNewsCategoryTreeView::DisclosureChanged(Uint32 inTreeIndex, SNewsCatItm *inTreeItem, Uint8 inDisclosure)
{
	#pragma unused(inTreeItem)
	
	if (inTreeIndex != mRefreshBundleIndex && inDisclosure == optTree_Disclosed && !GetChildTreeCount(inTreeIndex))
	{
		Uint32 nPathSize;
		void *pPathData = GetBundlePath(inTreeIndex, nPathSize);
		if (!pPathData)
			return;
		
		DeletePath(pPathData, nPathSize);
		if (mTreeExpl)
			((CMyNewsCategoryTreeWin*)GetDragAndDropHandler())->GetNewsContent(pPathData, nPathSize);
		else
			((CMyNewsCategoryExplWin*)GetDragAndDropHandler())->GetNewsContent(pPathData, nPathSize);
			
		UMemory::Dispose((TPtr)pPathData);
	}
	
	if (mRefreshBundleIndex)
		mRefreshBundleIndex = 0;
}

bool CMyNewsCategoryTreeView::GetBundleIndex(const void *inPathData, Uint32 inPathSize, Uint32& outBundleIndex)
{
	outBundleIndex = 0;
	if (!inPathData || !inPathSize)
	{
		if (!*mPathData || !*mPathSize)
			return true;
		
		return false;
	}
	
	if (inPathSize < 6)
		return false;
					
	Uint32 nIndex = 0;
	Uint32 nCount = FB(*(Uint16 *)inPathData);
	Uint8 *p = ((Uint8 *)inPathData) + sizeof(Uint16);
	Uint8 *ep = ((Uint8 *)inPathData) + inPathSize;
	
	if (*mPathData && *mPathSize)
	{
		if (inPathSize < *mPathSize || UMemory::Compare((Uint8 *)inPathData + 2, (Uint8 *)(*mPathData) + 2, *mPathSize - 2))
			return false;
						
		nCount -= FB(*(Uint16 *)*mPathData);
		p += *mPathSize - 2;
	}
	
	while (nCount--)
	{
		if (!GetChildTreeCount(nIndex))
			return false;
		
		p += sizeof(Uint16);
		if (p >= ep)						// check for corruption
			return false;					
		
		Uint32 nSize = *p++;
		if (nSize == 0 || p + nSize > ep) 	// check for corruption
			return false;
					
		nIndex++;
		SNewsCatItm *pNewsCatItm = GetTreeItem(nIndex);
		if (!pNewsCatItm)
			return false;
	
		bool bFound = false;
		do
		{
			if (pNewsCatItm->type == 2 && pNewsCatItm->name[0] == nSize && !UMemory::Compare(pNewsCatItm->name + 1, p, nSize))
			{
				bFound = true;
				break;
			}
			
		} while (GetNextTreeItem(pNewsCatItm, nIndex, true));
		
		if (!bFound)
			return false;
			
		p += nSize;
	}
	
	outBundleIndex = nIndex;
	return true;
}

// if inBundleIndex is a category index return parent bundle path
void *CMyNewsCategoryTreeView::GetBundlePath(Uint32 &ioBundleIndex, Uint32& outPathSize)
{
	outPathSize = 0;
	
	if (!ioBundleIndex)
	{
		if (*mPathData && *mPathSize)
		{
			outPathSize = *mPathSize;
			return UMemory::New(*mPathData, *mPathSize);
		}
		
		return nil;
	}
	
	SNewsCatItm *pNewsCatItm = GetTreeItem(ioBundleIndex);
	if (!pNewsCatItm)
		return nil;
	
	if (pNewsCatItm->type != 2)
	{
		ioBundleIndex = GetParentTreeIndex(ioBundleIndex);
		if (!ioBundleIndex)
		{
			if (*mPathData && *mPathSize)
			{
				outPathSize = *mPathSize;
				return UMemory::New(*mPathData, *mPathSize);
			}
			
			return nil;
		}
	
		pNewsCatItm = GetTreeItem(ioBundleIndex);
		if (!pNewsCatItm)
			return nil;
	}
	
	Uint32 nIndex = ioBundleIndex;
	Uint32 nPathSize = 2;
	
	if (*mPathData && *mPathSize)
		nPathSize = *mPathSize;
	
	do
	{
		nPathSize += pNewsCatItm->name[0] + 3;
		
	} while ((pNewsCatItm = GetParentTreeItem(nIndex, &nIndex)) != nil);
	
	if (nPathSize < 6)
		return nil;
	
	void *pPathData = UMemory::NewClear(nPathSize);
	if (!pPathData)
		return nil;
	
	if (*mPathData && *mPathSize)
		UMemory::Copy(pPathData, *mPathData, *mPathSize);
	
	Uint8 *pEndPath = (Uint8 *)pPathData + nPathSize;

	nIndex = ioBundleIndex;
	pNewsCatItm = GetTreeItem(nIndex);
	
	do
	{
		if (pEndPath - pNewsCatItm->name[0] - 3 < (Uint8 *)pPathData + (*mPathSize ? *mPathSize : 2))
		{
			UMemory::Dispose((TPtr)pPathData);
			return nil;
		}
		
		pEndPath -= pNewsCatItm->name[0] + 1;
		UMemory::Copy(pEndPath, pNewsCatItm->name, pNewsCatItm->name[0] + 1);
		pEndPath -= 2;
		
	#if MACINTOSH
		(*(Uint16 *)pPathData)++;
	#else
		*(Uint16 *)pPathData = TB( (Uint16)(FB(*(Uint16 *)pPathData) + 1));
	#endif
	
	} while ((pNewsCatItm = GetParentTreeItem(nIndex, &nIndex)) != nil);

	outPathSize = nPathSize;	
	return pPathData;
}

void CMyNewsCategoryTreeView::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SNewsCatItm *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inTreeIndex, inTreeLevel, inOptions)
		
	SRect stRect;
	SColor stTextCol;
	bool bIsActive = IsFocus() && mIsEnabled && inTreeViewItem->bIsSelected;
	
	if (bIsActive)
		UUserInterface::GetSysColor(sysColor_InverseHighlight, stTextCol);
	else
		UUserInterface::GetSysColor(sysColor_Label, stTextCol);

	// set color
	inImage->SetInkColor(stTextCol);

	// set font
	inImage->SetFont(kDefaultFont, nil, 9);
	inImage->SetFontEffect(fontEffect_Plain);

	// draw item count
	if (inTreeItem->type == 2 || inTreeItem->type == 3)	// can remove this condition after apr 15
	{
		const SRect* pBounds = inTabRectList.GetItem(2);
		if (pBounds && pBounds->GetWidth())
		{
			// set rect
			stRect = *pBounds;
			stRect.top += 3;
			stRect.bottom -= 2;
			stRect.left += 1;
			stRect.right -= 2;
			if (stRect.right < stRect.left)
				stRect.right = stRect.left;
			
			Uint8 psText[16];
			psText[0] = UText::Format(psText + 1, sizeof(psText) - 1, "(%lu)", inTreeItem->count);
			
			inImage->DrawTruncText(stRect, psText + 1, psText[0], 0, align_Right | align_CenterVert);
		}
	}

	// draw item icon and name
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += 2;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
			inTreeItem->icon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);

		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 24;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;

		if(inTreeItem->type != 3 || inTreeItem->unread)
			inImage->SetFontEffect(fontEffect_Bold);

		// draw item name
		inImage->DrawTruncText(stRect, inTreeItem->name + 1, inTreeItem->name[0], 0, align_Left | align_CenterVert);
	}
}

Int32 CMyNewsCategoryTreeView::CompareNames(void *inItemA, void *inItemB, void *inRef)
{
	#pragma unused(inRef)

	if (!inItemA || !inItemB)
		return 0;

	Uint8 *pNameA = ((SNewsCatItm *)inItemA)->name;
	Uint8 *pNameB = ((SNewsCatItm *)inItemB)->name;

	Int32 nOutVal = UText::CompareInsensitive(pNameA + 1, pNameB + 1, min(pNameA[0], pNameB[0]));
	if (nOutVal == 0 && pNameA[0] != pNameB[0])
		nOutVal = pNameA[0] > pNameB[0] ? 1 : -1;
	
	return nOutVal;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsCategoryWin::CMyNewsCategoryWin(CContainerView *inContainerView, const SRect& inBounds)
	: CMySearchText(inContainerView, SRect(116, 10, inBounds.right - 34, 24))
{

}

void CMyNewsCategoryWin::BuildButtons(const SRect& inBounds, CContainerView *inContainerView)
{
	SRect r = inBounds;
	r.top += 3;
	r.bottom = r.top + 24;
	r.left += 3;
	r.right = r.left + 24;
	
	// buttons
	CIconButtonView *icb = new CIconButtonView(inContainerView, r);
	if (gApp->mIconSet == 2){
		icb->SetIconID(3029);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(221);
	}
	
	icb->SetID(viewID_OpenParent);
	icb->SetTooltipMsg("\pOpen Parent");
	icb->Show();
	mViews.parentFolder = icb;

	// new category
	r.left = r.right + 3;
	r.right = r.left + 24;
		
	icb = new CIconButtonView(inContainerView, r);
	if (gApp->mIconSet == 2){
		icb->SetIconID(3023);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(413);
	}
	
	icb->SetID(viewID_NewsFldrNewCat);
	icb->SetTooltipMsg("\pCreate New Category");
	icb->Show();
	mViews.newCategory = icb;
		
	// new bundle
	r.left = r.right + 3;
	r.right = r.left + 24;

	icb = new CIconButtonView(inContainerView, r);
	if (gApp->mIconSet == 2){
		icb->SetIconID(3041);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(231);
	}
	
	icb->SetID(viewID_New);
	icb->SetTooltipMsg("\pCreate New Bundle");
	icb->Show();
	mViews.newFolder = icb;

	// refresh list
	r.left = r.right + 3;
	r.right = r.left + 24;

	icb = new CIconButtonView(inContainerView, r);
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3026);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(205);
	}
						
	
	icb->SetID(viewID_Refresh);
	icb->SetTooltipMsg("\pRefresh");
	icb->Show();
	mViews.refresh = icb;
		
	// trash article
	r.right = inBounds.right - 3;
	r.left = r.right - 24;
		
	icb = new CIconButtonView(inContainerView, r);
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3000);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(212);
	}
	
	
	icb->SetSizing(sizing_HorizSticky);
	icb->SetID(viewID_Delete);
	icb->SetTooltipMsg("\pDelete");
	icb->Show();
	mViews.trash = icb;

    // help
    /*
	r.right = r.left - 3;
	r.left = r.right - 24;
	icb = new CIconButtonView(inContainerView, r);
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpNews);
	icb->SetTooltipMsg("\pHelp");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();
	*/
}

void CMyNewsCategoryWin::GetNewsContent(const void *inPathData, Uint32 inPathSize)
{
	TFieldData pListData = gApp->mCacheList.SearchBundleList(inPathData, inPathSize);

	if (pListData)
		SetContent(inPathData, inPathSize, pListData, false);
	else
		new CMyGetNewsCatListTask(inPathData, inPathSize);
}

void CMyNewsCategoryWin::SetNewsAccess(const Uint8 *inItemName, const void *inPathData, Uint32 inPathSize, Uint16 inType)
{
	const Uint8 *pBundleName = nil;
	if (!inPathData || !inPathSize)
	{
		if (inType == 2)		// bundle
			pBundleName = inItemName;
	}
	else
		pBundleName = (Uint8 *)inPathData + 4;
	
	if (gApp->HasBundlePriv(myAcc_NewsCreateCat)) 
		mViews.newCategory->Enable();
	else 
		mViews.newCategory->Disable();

	if (gApp->HasBundlePriv(myAcc_NewsCreateFldr)) 
		mViews.newFolder->Enable();
	else 
		mViews.newFolder->Disable();
	
	if (inType == 2) 			// bundle
	{
		if (gApp->HasBundlePriv(myAcc_NewsDeleteFldr))
			mViews.trash->Show();
		else 
			mViews.trash->Hide();
	}
	else if (inType == 3) 		// category
	{
		if (gApp->HasBundlePriv(myAcc_NewsDeleteCat)) 
			mViews.trash->Show();
		else 
			mViews.trash->Hide();
	}
	else
		mViews.trash->Hide();
}

void CMyNewsCategoryWin::DoOpenParent(Uint16 inMods)
{
	gApp->DoNewsFldrOpenParent(dynamic_cast<CMyItemsWin *>(this), inMods);
}

void CMyNewsCategoryWin::DoDoubleClick(Uint16 inMods)
{
	SGUID stGuid;
	Uint8 psItemName[256];
	
	Uint16 nType = GetSelectedItemName(psItemName, &stGuid);
	if (!psItemName[0]) 
		return;
	
	Uint32 nPathSize;
	bool bDisposePath;
	void *pPathData = GetSelectedParentBundlePath(nPathSize, &bDisposePath);

	gApp->DoNewsFldrOpen(dynamic_cast<CMyItemsWin*>(this), psItemName, pPathData, nPathSize, nType, stGuid, inMods);

	if (bDisposePath) 
		UMemory::Dispose((TPtr)pPathData);
}

bool CMyNewsCategoryWin::News_KeyDown(const SKeyMsgData& inInfo)
{
	return SearchText_KeyDown(inInfo);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsCategoryListWin::CMyNewsCategoryListWin(CWindow *inParent, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsListWin(inParent, SRect(0, 0, 300, 300), inPathData, inPathSize, inPathSum), CMyNewsCategoryWin(mVC, SRect(0, 0, 300, 300))
{
#if WIN32
	_SetWinIcon(*this, 413);
#endif

	gApp->GetNewsBoundsInfo();
	gApp->mCategoryWinList.AddItem(this);
		
	SetTitle("\pNews");
	SetLimits(230,150);
	
	BuildButtons(SRect(0, 0, 300, 300), mVC);
	
	// make scroller view
	CScrollerView *scr = new CScrollerView(mVC, SRect(-1, 30, 301, 301), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + scrollerOption_GrowSpace + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	mVC->SetFocusView(scr);

	mListView = new CMyNewsCategoryListView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()));
	mListView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction);
	mListView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mListView->SetCanFocus(true);
	mListView->SetID(viewID_NewsFldrList);
	mListView->SetDragAndDropHandler(this);
	mListView->Show();

	SetBoundsInfo();
}

CMyNewsCategoryListWin::~CMyNewsCategoryListWin()
{
	gApp->mCategoryWinList.RemoveItem(this);
}

void CMyNewsCategoryListWin::GetContent()
{
	GetNewsCategoryListView()->DeleteAll();
	GetNewsCategoryListView()->SetStatus(listStat_Loading);

	GetNewsContent(mPathData, mPathSize);
}

void CMyNewsCategoryListWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	CMyNewsCategoryListView *pNewsCategoryListView = GetNewsCategoryListView();

	pNewsCategoryListView->DeleteAll();
	bool bAdded = pNewsCategoryListView->AddListFromFields(inData);

	SetStatus(bAdded ? listStat_Hide : listStat_0Items);
	SetAccess();

	if (!mPathData) 
		mViews.parentFolder->Disable();
	else
		mViews.parentFolder->Enable();
		
	if (inCache)
		gApp->mCacheList.AddBundleList(inPathData, inPathSize, inData);
}

void CMyNewsCategoryListWin::SetAccess()
{
	Uint8 psItemName[256];
	Uint16 nType = GetNewsCategoryListView()->GetSelectedItemName(psItemName);

	SetNewsAccess(psItemName, mPathData, mPathSize, nType);
}

void CMyNewsCategoryListWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stNews1.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stNews1);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	(dynamic_cast<CMyNewsCategoryListView*>(mListView))->SetTabs(gApp->mOptions.stWinTabs.nNewsTab1, gApp->mOptions.stWinTabs.nNewsTab2);
}

void CMyNewsCategoryListWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stNews1);
	
	(dynamic_cast<CMyNewsCategoryListView*>(mListView))->GetTabs(gApp->mOptions.stWinTabs.nNewsTab1, gApp->mOptions.stWinTabs.nNewsTab2);
}

Uint16 CMyNewsCategoryListWin::GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID)
{
	return GetNewsCategoryListView()->GetSelectedItemName(outItemName, outGUID);
}

void *CMyNewsCategoryListWin::GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath)
{
	if (outDisposePath)
		*outDisposePath = false;

	outPathSize = mPathSize;
	return mPathData;
}

void CMyNewsCategoryListWin::DoNewCategory()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentBundlePath(nPathSize);

	gApp->DoNewsFldrNewCat(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
}

void CMyNewsCategoryListWin::DoNewFolder()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentBundlePath(nPathSize);

	gApp->DoNewsFldrNewFldr(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
}

void CMyNewsCategoryListWin::DoRefresh(Uint16 inMods)
{
	#pragma unused(inMods)

	GetNewsCategoryListView()->DeleteAll();
	SetStatus(listStat_Loading);
	
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentBundlePath(nPathSize);

	gApp->DoNewsFldrRefresh(pPathData, nPathSize);
}

void CMyNewsCategoryListWin::DoDelete()
{
	Uint8 psItemName[256];	
	Uint16 nType = GetSelectedItemName(psItemName);
	if (!psItemName[0] || !nType)
		return;

	Uint32 nPathSize;
	void *pPathData = GetSelectedParentBundlePath(nPathSize);
		
	gApp->DoNewsFldrTrash(dynamic_cast<CMyItemsWin *>(this), psItemName, pPathData, nPathSize, nType);
}

void CMyNewsCategoryListWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (News_KeyDown(inInfo))
		return;
		
	CMyItemsListWin::KeyDown(inInfo);
}

void CMyNewsCategoryListWin::SearchText(const Uint8 *inText)
{
	CMyNewsCategoryListView *pNewsListView = GetNewsCategoryListView();
	Uint32 nIndex = pNewsListView->SearchNames(inText);
	
	if (nIndex != pNewsListView->GetFirstSelectedItem())
	{
		pNewsListView->DeselectAll();
	
		if (nIndex)
		{
			pNewsListView->SelectItem(nIndex);
			pNewsListView->MakeItemVisible(nIndex);
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsCategoryTreeWin::CMyNewsCategoryTreeWin(CWindow *inParent, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsTreeWin(inParent, SRect(0, 0, 300, 300), inPathData, inPathSize, inPathSum), CMyNewsCategoryWin(mVC, SRect(0, 0, 300, 300))
{
#if WIN32
	_SetWinIcon(*this, 413);
#endif

	gApp->GetNewsBoundsInfo();
	gApp->mCategoryWinList.AddItem(this);
	
	SetTitle("\pNews");
	SetLimits(230,150);

	BuildButtons(SRect(0, 0, 300, 300), mVC);

	// make scroller view
	CScrollerView *scr = new CScrollerView(mVC, SRect(-1, 30, 301, 301), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + scrollerOption_GrowSpace + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	mVC->SetFocusView(scr);

	mTreeView = new CMyNewsCategoryTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), &mPathData, &mPathSize, true);
	mTreeView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction);
	mTreeView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mTreeView->SetCanFocus(true);
	mTreeView->SetID(viewID_NewsFldrTree);
	mTreeView->SetDragAndDropHandler(this);
	mTreeView->Show();

	SetBoundsInfo();
}

CMyNewsCategoryTreeWin::~CMyNewsCategoryTreeWin()
{
	gApp->mCategoryWinList.RemoveItem(this);
}

void CMyNewsCategoryTreeWin::GetContent()
{
	GetNewsCategoryTreeView()->DeleteAll();
	GetNewsCategoryTreeView()->SetStatus(listStat_Loading);

	GetNewsContent(mPathData, mPathSize);
}

void CMyNewsCategoryTreeWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	CMyNewsCategoryTreeView *pNewsCategoryTreeView = GetNewsCategoryTreeView();
	
	pNewsCategoryTreeView->DeletePath(inPathData, inPathSize);
	pNewsCategoryTreeView->AddListFromFields(inPathData, inPathSize, inData);
	
	SetStatus(pNewsCategoryTreeView->GetTreeCount() ? listStat_Hide : listStat_0Items);
	SetAccess();
		
	if (!mPathData) 
		mViews.parentFolder->Disable();
	else
		mViews.parentFolder->Enable();

	if (inCache)
		gApp->mCacheList.AddBundleList(inPathData, inPathSize, inData);
}

void CMyNewsCategoryTreeWin::SetAccess()
{
	CMyNewsCategoryTreeView *pNewsCategoryTreeView = GetNewsCategoryTreeView();

	Uint8 psItemName[256];	
	Uint16 nType = pNewsCategoryTreeView->GetSelectedItemName(psItemName);
	
	Uint32 nPathSize;
	void *pPathData = pNewsCategoryTreeView->GetSelectedParentBundlePath(nPathSize);
	
	SetNewsAccess(psItemName, pPathData, nPathSize, nType);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryTreeWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stNews1.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stNews1);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	(dynamic_cast<CMyNewsCategoryTreeView*>(mTreeView))->SetTabs(gApp->mOptions.stWinTabs.nNewsTab1, gApp->mOptions.stWinTabs.nNewsTab2);
}

void CMyNewsCategoryTreeWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stNews1);
	
	(dynamic_cast<CMyNewsCategoryTreeView*>(mTreeView))->GetTabs(gApp->mOptions.stWinTabs.nNewsTab1, gApp->mOptions.stWinTabs.nNewsTab2);
}

Uint16 CMyNewsCategoryTreeWin::GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID)
{
	return GetNewsCategoryTreeView()->GetSelectedItemName(outItemName, outGUID);
}

void *CMyNewsCategoryTreeWin::GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath)
{
	if (outDisposePath)
		*outDisposePath = true;

	return GetNewsCategoryTreeView()->GetSelectedParentBundlePath(outPathSize);
}

void *CMyNewsCategoryTreeWin::GetSelectedBundlePath(Uint32& outPathSize)
{
	return GetNewsCategoryTreeView()->GetSelectedBundlePath(outPathSize);
}

void CMyNewsCategoryTreeWin::DoNewCategory()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedBundlePath(nPathSize);
	
	gApp->DoNewsFldrNewCat(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryTreeWin::DoNewFolder()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedBundlePath(nPathSize);
	
	gApp->DoNewsFldrNewFldr(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryTreeWin::DoRefresh(Uint16 inMods)
{
	CMyNewsCategoryTreeView *pNewsCategoryTreeView = GetNewsCategoryTreeView();
	
	if (inMods & modKey_Option)
		pNewsCategoryTreeView->DeselectAllTreeItems();

	Uint32 nPathSize;
	void *pPathData = pNewsCategoryTreeView->RefreshSelectedBundlePath(nPathSize);
	
	if (!pNewsCategoryTreeView->GetTreeCount())
		SetStatus(listStat_Loading);

	gApp->DoNewsFldrRefresh(pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryTreeWin::DoDelete()
{
	Uint8 psItemName[256];
	Uint16 nType = GetSelectedItemName(psItemName);
	if (!psItemName[0] || !nType)
		return;

	Uint32 nPathSize;
	void *pPathData = GetSelectedParentBundlePath(nPathSize);

	gApp->DoNewsFldrTrash(dynamic_cast<CMyItemsWin *>(this), psItemName, pPathData, nPathSize, nType);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryTreeWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (News_KeyDown(inInfo))
		return;
		
	CMyItemsTreeWin::KeyDown(inInfo);
}

void CMyNewsCategoryTreeWin::SearchText(const Uint8 *inText)
{
	CMyNewsCategoryTreeView *pNewsTreeView = GetNewsCategoryTreeView();
	Uint32 nIndex = pNewsTreeView->SearchNames(inText);

	if (nIndex != pNewsTreeView->GetFirstSelectedTreeItem())
	{
		pNewsTreeView->DeselectAllTreeItems();
	
		if (nIndex)
		{
			pNewsTreeView->SelectTreeItem(nIndex);
			pNewsTreeView->MakeTreeItemVisibleInList(nIndex);
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsCategoryExplWin::CMyNewsCategoryExplWin(CWindow *inParent, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsTreeWin(inParent, SRect(0,0,640,320), inPathData, inPathSize, inPathSum), CMyNewsCategoryWin(mVC, SRect(0,0,640,320))
{
#if WIN32
	_SetWinIcon(*this, 413);
#endif

	gApp->GetNewsBoundsInfo();
	gApp->mCategoryWinList.AddItem(this);
	
	SetTitle("\pNews");
	SetLimits(500,200);
		
	SRect r;
 	searchText->GetBounds(r);
 	r.right = r.left + 60;
 	searchText->SetBounds(r);
	searchText->SetSizing(0);
	
	// make pane view
	mNewViews.paneView1 = new CPaneView(mVC, SRect(0, 0, 640, 320));
	mNewViews.paneView1->SetSizing(sizing_BottomRightSticky);
	mNewViews.paneView1->Show();
	mVC->SetFocusView(mNewViews.paneView1);
	
	// make scroller view
	CScrollerView *scr = new CScrollerView(mNewViews.paneView1, SRect(-1, 30, 250, 321), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	mNewViews.paneView1->SetFocusView(scr);
	mNewViews.paneView1->SetViewLimits(scr, 180, 0);

	// make news tree
	mTreeView = new CMyNewsCategoryTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), &mPathData, &mPathSize, false);
	mTreeView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction);
	mTreeView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mTreeView->SetCanFocus(true);
	mTreeView->SetID(viewID_NewsFldrExplTree);
	mTreeView->SetDragAndDropHandler(this);
	mTreeView->Show();
	
	// make a container for the right side
	CContainerView *rCont = new CContainerView(mNewViews.paneView1, SRect(255, 0, 640, 320));
	rCont->SetSizing(sizing_HorizSticky | sizing_BottomSticky);
	rCont->Show();
	mNewViews.paneView1->SetViewLimits(rCont, 300, 0);
	
	// make buttons
	// left side and delete
	BuildButtons(SRect(0,0,640,320), mVC);
	
	// right side
	r.top = 3;
	r.bottom = r.top + 24;
	r.left = 3;
	r.right = r.left + 24;

	CIconButtonView *icb = new CIconButtonView(rCont, r);
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3040);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(223);
	}
	
	icb->SetID(viewID_NewArticle);
	icb->SetTooltipMsg("\pNew Article");
	icb->Show();
	mNewViews.newArt = icb;

	r.left = r.right + 3;
	r.right = r.left + 24;

	icb = new CIconButtonView(rCont, r);
	if (gApp->mIconSet == 2){
		icb->SetIconID(3039);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(220);
	}
	
	icb->SetID(viewID_NewsArticReply);
	icb->SetTooltipMsg("\pReply");
	icb->Show();
	mNewViews.replArt = icb;
	
	// make pane view for right side
	mNewViews.paneView2 = new CPaneView(rCont, SRect(0, 30, 385, 320));
	mNewViews.paneView2->SetSizing(sizing_BottomRightSticky);
	mNewViews.paneView2->Show();

	// make scroller view
	scr = new CScrollerView(mNewViews.paneView2, SRect(0, 0, 386, 150), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	mNewViews.paneView2->SetViewLimits(scr, 0, 60);
	
	// make article tree
	mArticleTreeView = new CMyNewsArticleTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), nil);
	mArticleTreeView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction);
	mArticleTreeView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mArticleTreeView->SetCanFocus(true);
	mArticleTreeView->SetID(viewID_NewsFldrExplList);
	mArticleTreeView->SetDragAndDropHandler(this);
	mArticleTreeView->Show();

	scr = MakeTextBoxView(mNewViews.paneView2, SRect(-1, 155, 387, 292), scrollerOption_Border | scrollerOption_NoFocusBorder | scrollerOption_VertBar + scrollerOption_GrowSpace, &mNewViews.articleText);
	mNewViews.articleText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mNewViews.articleText->SetEditable(false);
	mNewViews.articleText->SetTabSelectText(false);
	scr->SetSizing(sizing_RightSticky | sizing_VertSticky);
	scr->Show();
	mNewViews.paneView2->SetViewLimits(scr, 0, 80);
	mNewViews.articleScroll = scr;

	mArticleListTimer = nil;
	mArticleTextTimer = nil;
	
	SetTextColor(gApp->mOptions.ColorNews);
	SetBoundsInfo();
}

CMyNewsCategoryExplWin::~CMyNewsCategoryExplWin()
{
	UTimer::Dispose(mArticleListTimer);
	UTimer::Dispose(mArticleTextTimer);
		
	gApp->mCategoryWinList.RemoveItem(this);
}

void CMyNewsCategoryExplWin::GetContent()
{
	GetNewsCategoryTreeView()->DeleteAll();
	GetNewsCategoryTreeView()->SetStatus(listStat_Loading);

	mArticleTreeView->DeleteAll();
	mArticleTreeView->SetStatus(listStat_0Items);

#if USE_NEWS_HISTORY
	mArticleTreeView->SetReadList(nil);
#endif

	UMemory::Dispose(mNewViews.articleText->DetachTextHandle());

	GetNewsContent(mPathData, mPathSize);
}

void CMyNewsCategoryExplWin::GetContentArticleList()
{
	if (mArticleListTimer)
		mArticleListTimer->Stop();

	mArticleTreeView->DeleteAll();
	UMemory::Dispose(mNewViews.articleText->DetachTextHandle());

#if USE_NEWS_HISTORY
	mArticleTreeView->SetReadList(nil);
#endif

	SGUID stGuid;
	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize, &stGuid);
	if (!pPathData) 
	{
		mArticleTreeView->SetStatus(listStat_0Items);
		return;
	}

	mArticleTreeView->SetStatus(listStat_Loading);
	try
	{
		mArticleTreeView->SetReadList(gApp->mNZHist->GetReadList(stGuid));
	}
	catch(...)
	{
		// don't throw
	}
	TFieldData pListData = gApp->mCacheList.SearchArticleList(pPathData, nPathSize);

	if (pListData)
	{
	#if USE_NEWS_HISTORY
		try
		{
			mArticleTreeView->SetReadList(gApp->mNZHist->GetReadList(stGuid));
		}
		catch(...)
		{
			// don't throw
		}
	#endif

		bool bAdded = mArticleTreeView->AddListFromFields(pListData);
		
		// set Guid
		mArticleTreeView->SetStatus(bAdded ? listStat_Hide : listStat_0Items);
	}
	else
		new CMyGetNewsArtListTask(pPathData, nPathSize);
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::GetContentArticleListTimer()
{
	if (!mArticleListTimer)
		mArticleListTimer = NewTimer();
		
	mArticleListTimer->Start(500, kOnceTimer);
}

void CMyNewsCategoryExplWin::GetContentArticleText()
{
	UMemory::Dispose(mNewViews.articleText->DetachTextHandle());

	Uint8 psArticleName[256];
	Uint32 nArticleID = mArticleTreeView->ReadSelectedItemNameAndID(psArticleName);
	if (!nArticleID || !psArticleName[0])
		return;
 
	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;

	gApp->DoNewsCatOpen(pPathData, nPathSize, nArticleID, psArticleName);
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::GetContentArticleTextTimer()
{
	if (!mArticleTextTimer)
		mArticleTextTimer = NewTimer();
		
	mArticleTextTimer->Start(500, kOnceTimer);
}

void CMyNewsCategoryExplWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	CMyNewsCategoryTreeView *pNewsCategoryTreeView = GetNewsCategoryTreeView();
	
	pNewsCategoryTreeView->DeletePath(inPathData, inPathSize);
	pNewsCategoryTreeView->AddListFromFields(inPathData, inPathSize, inData);
	
	SetStatus(pNewsCategoryTreeView->GetTreeCount() ? listStat_Hide : listStat_0Items);
	SetAccess();
		
	if (!mPathData) 
		mViews.parentFolder->Disable();
	else
		mViews.parentFolder->Enable();

	if (inCache)
		gApp->mCacheList.AddBundleList(inPathData, inPathSize, inData);
}

void CMyNewsCategoryExplWin::SetContentArticleList(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	SGUID stGuid;
	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize, &stGuid);
	
	if (pPathData) 
	{
		if (!UMemory::Compare(pPathData, nPathSize, inPathData, inPathSize))	
		{
			mArticleTreeView->DeleteAll();
			UMemory::Dispose(mNewViews.articleText->DetachTextHandle());

		#if USE_NEWS_HISTORY
			try
			{
				mArticleTreeView->SetReadList(gApp->mNZHist->GetReadList(stGuid));
			}
			catch(...)
			{
				// don't throw
			}
		#endif

			bool bAdded = mArticleTreeView->AddListFromFields(inData);
			mArticleTreeView->SetStatus(bAdded ? listStat_Hide : listStat_0Items);			
			
			SetAccess();
		}
		
		UMemory::Dispose((TPtr)pPathData);
	}
	
	if (inCache)
		gApp->mCacheList.AddArticleList(inPathData, inPathSize, inData);
}

void CMyNewsCategoryExplWin::SetContentArticleText(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID, TFieldData inData)
{
	Uint32 nArticleID = mArticleTreeView->GetSelectedItemNameAndID(nil);
	if (nArticleID != inArticleID)
		return;
 
	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;

	if (!UMemory::Compare(pPathData, nPathSize, inPathData, inPathSize))
	{
		Int8 flav[256];
		inData->GetCString(myField_NewsArtDataFlav, flav, sizeof(flav));
		
		if (!strcmp(flav, "text/plain"))
		{	
			Uint32 nTextSize = inData->GetFieldSize(myField_NewsArtData);
			if (nTextSize > 65536)
				nTextSize = 65536;
	
			THdl hText = mNewViews.articleText->GetTextHandle();
			hText->Reallocate(nTextSize);
	
			{
				void *pText;
				StHandleLocker lock(hText, pText);
				inData->GetField(myField_NewsArtData, pText, nTextSize);
			}
	
			mNewViews.articleText->SetTextHandle(hText);

			// mark this item as read
			CNZReadList *readList = mArticleTreeView->GetReadList();
			if(readList)
				readList->SetRead(inArticleID);
			else
				DebugBreak("no ReadList");
				
			mArticleTreeView->SetReadListItem(nArticleID);
		}
		
		SetAccess();
	}
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::SetCurrentArticle(const void *inPathData, Uint32 inPathSize, Uint32 inCurrentID, Uint32 inNewID)
{
	Uint32 nArticleID = mArticleTreeView->GetSelectedItemNameAndID(nil);
	if (nArticleID != inCurrentID)
		return;

	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;

	if (!UMemory::Compare(pPathData, nPathSize, inPathData, inPathSize))
	{
		mArticleTreeView->SetCurrentItem(inNewID);
		SetAccess();
	}
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::SelectArticle(const void *inPathData, Uint32 inPathSize, Uint32 inArticleID)
{
	Uint32 nArticleID = mArticleTreeView->GetSelectedItemNameAndID(nil);
	if (nArticleID == inArticleID)
		return;

	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;

	if (!UMemory::Compare(pPathData, nPathSize, inPathData, inPathSize))
	{
		mArticleTreeView->SetSelectedItem(inArticleID);
		SetAccess();
	}
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::SetTextColor(const SMyColorInfo& inColorInfo) 
{
	mNewViews.articleScroll->SetContentColor(inColorInfo.backColor);

	mNewViews.articleText->SetColor(inColorInfo.textColor);
	mNewViews.articleText->SetFontSize(inColorInfo.textSize);
}

void CMyNewsCategoryExplWin::SetAccess()
{
	CMyNewsCategoryTreeView *pNewsCategoryTreeView = GetNewsCategoryTreeView();

	Uint8 psItemName[256];	
	Uint16 nType = pNewsCategoryTreeView->GetSelectedItemName(psItemName);
		
	Uint32 nPathSize;
	void *pPathData = pNewsCategoryTreeView->GetSelectedParentBundlePath(nPathSize);
	
	SetNewsAccess(psItemName, pPathData, nPathSize, nType);
	
	if (nType == 2)
	{
		mNewViews.newArt->Disable();
		mNewViews.replArt->Disable();
	}
	else
	{
		if (gApp->HasBundlePriv(myAcc_NewsPostArt)) 
		{
			mNewViews.newArt->Enable();
			
			if (mArticleTreeView->GetSelectedItemNameAndID(nil))
				mNewViews.replArt->Enable();
			else
				mNewViews.replArt->Disable();
		}
		else 
		{
			mNewViews.newArt->Disable();
			mNewViews.replArt->Disable();
		}
	
		if (IsActiveArticleView())
		{
			if (gApp->HasBundlePriv(myAcc_NewsDeleteArt)) 
				mViews.trash->Show();
			else 
				mViews.trash->Hide();
		}
	}
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stNews2.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stNews2);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	mNewViews.paneView1->SetPanePercent(gApp->mOptions.stWinPane.nNewsPane1);
	mNewViews.paneView2->SetPanePercent(gApp->mOptions.stWinPane.nNewsPane2);
	
	(dynamic_cast<CMyNewsCategoryTreeView*>(mTreeView))->SetTabs(gApp->mOptions.stWinTabs.nNewsTab1, gApp->mOptions.stWinTabs.nNewsTab2);
	(dynamic_cast<CMyNewsArticleTreeView*>(mArticleTreeView))->SetTabs(gApp->mOptions.stWinTabs.nArticlesTab1, gApp->mOptions.stWinTabs.nArticlesTab2, gApp->mOptions.stWinTabs.nArticlesTab3, gApp->mOptions.stWinTabs.nArticlesTab4, gApp->mOptions.stWinTabs.nArticlesTab5);
}

void CMyNewsCategoryExplWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stNews2);
	
	gApp->mOptions.stWinPane.nNewsPane1 = mNewViews.paneView1->GetPanePercent();
	gApp->mOptions.stWinPane.nNewsPane2 = mNewViews.paneView2->GetPanePercent();

	(dynamic_cast<CMyNewsCategoryTreeView*>(mTreeView))->GetTabs(gApp->mOptions.stWinTabs.nNewsTab1, gApp->mOptions.stWinTabs.nNewsTab2);
	(dynamic_cast<CMyNewsArticleTreeView*>(mArticleTreeView))->GetTabs(gApp->mOptions.stWinTabs.nArticlesTab1, gApp->mOptions.stWinTabs.nArticlesTab2, gApp->mOptions.stWinTabs.nArticlesTab3, gApp->mOptions.stWinTabs.nArticlesTab4, gApp->mOptions.stWinTabs.nArticlesTab5);
}

Uint16 CMyNewsCategoryExplWin::GetSelectedItemName(Uint8 *outItemName, SGUID *outGUID)
{
	return GetNewsCategoryTreeView()->GetSelectedItemName(outItemName, outGUID);
}

void *CMyNewsCategoryExplWin::GetSelectedCategoryPath(Uint32& outPathSize, SGUID *outGUID)
{
	return GetNewsCategoryTreeView()->GetSelectedCategoryPath(outPathSize, outGUID);
}

void *CMyNewsCategoryExplWin::GetSelectedParentBundlePath(Uint32& outPathSize, bool *outDisposePath)
{
	if (outDisposePath)
		*outDisposePath = true;

	return GetNewsCategoryTreeView()->GetSelectedParentBundlePath(outPathSize);
}

void *CMyNewsCategoryExplWin::GetSelectedBundlePath(Uint32& outPathSize)
{
	return GetNewsCategoryTreeView()->GetSelectedBundlePath(outPathSize);
}

void CMyNewsCategoryExplWin::DoNewArticle()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;
	
	Uint32 nPathChecksum = UMemory::Checksum(pPathData, nPathSize);
	CNZReadList *pReadList = GetNewsArticleTreeView()->GetReadList();

	SRect stBounds;
	GetBounds(stBounds);

	gApp->DoNewsCatNewArticle(pPathData, nPathSize, nPathChecksum, pReadList, stBounds);
	// don't dispose pPathData
}

void CMyNewsCategoryExplWin::DoOpenArticle()
{
	Uint8 psArticleName[256];
	Uint32 nArticleID = GetNewsArticleTreeView()->ReadSelectedItemNameAndID(psArticleName);
	if (!nArticleID || !psArticleName[0])
		return;
	
	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;
	
	Uint32 nPathChecksum = UMemory::Checksum(pPathData, nPathSize);
	CNZReadList *pReadList = GetNewsArticleTreeView()->GetReadList();

	SRect stBounds;
	GetBounds(stBounds);

	gApp->DoNewsCatOpen(pPathData, nPathSize, nPathChecksum, nArticleID, psArticleName, pReadList, stBounds);
	// don't dispose pPathData
}

void CMyNewsCategoryExplWin::DoReplyArticle()
{
	Uint8 psArticleName[256];
	Uint32 nArticleID = GetNewsArticleTreeView()->GetSelectedItemNameAndID(psArticleName);
	if (!nArticleID || !psArticleName[0])
		return;

	Uint32 nPathSize;
	void *pPathData = GetSelectedCategoryPath(nPathSize);
	if (!pPathData)
		return;
	
	Uint32 nPathChecksum = UMemory::Checksum(pPathData, nPathSize);
	CNZReadList *pReadList = GetNewsArticleTreeView()->GetReadList();

	gApp->DoNewsArticReply(pPathData, nPathSize, nPathChecksum, nArticleID, psArticleName, pReadList);
	// don't dispose pPathData
}

void CMyNewsCategoryExplWin::DoNewCategory()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedBundlePath(nPathSize);
	
	gApp->DoNewsFldrNewCat(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::DoNewFolder()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedBundlePath(nPathSize);
	
	gApp->DoNewsFldrNewFldr(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::DoRefresh(Uint16 inMods)
{
	if (IsActiveArticleView())
	{
		if (inMods & modKey_Option)
			inMods &= ~modKey_Option;
		else
		{
			GetContentArticleText();
			return;
		}
	}
	
	CMyNewsCategoryTreeView *pNewsCategoryTreeView = GetNewsCategoryTreeView();
	CMyNewsArticleTreeView *pNewsArticleTreeView = GetNewsArticleTreeView();

	pNewsArticleTreeView->DeleteAll();
	if (inMods & modKey_Option)
		pNewsCategoryTreeView->DeselectAllTreeItems();

	bool bCategory;
	Uint32 nPathSize;
	void *pPathData = pNewsCategoryTreeView->RefreshSelectedBundlePath(nPathSize, &bCategory);
	
	if (bCategory)
	{
		pNewsArticleTreeView->SetStatus(listStat_Loading);
		gApp->DoNewsCatRefresh(pPathData, nPathSize);
	}
	else
	{
		pNewsArticleTreeView->SetStatus(listStat_0Items);
		if (!pNewsCategoryTreeView->GetTreeCount())
			SetStatus(listStat_Loading);
			
		gApp->DoNewsFldrRefresh(pPathData, nPathSize);
	}
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyNewsCategoryExplWin::DoDelete()
{
	if (IsActiveArticleView())
	{
		Uint8 psArticleName[256];
		Uint32 nArticleID = GetNewsArticleTreeView()->GetSelectedItemNameAndID(psArticleName);
		if (!nArticleID)
			return;
	
		Uint32 nPathSize;
		void *pPathData = GetSelectedCategoryPath(nPathSize);
		if (!pPathData)
			return;
		
		gApp->DoNewsCatTrash(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize, nArticleID, psArticleName);
		UMemory::Dispose((TPtr)pPathData);
	}
	else
	{
		Uint8 psItemName[256];
		Uint16 nType = GetSelectedItemName(psItemName);
		if (!psItemName[0] || !nType)
			return;

		Uint32 nPathSize;
		void *pPathData = GetSelectedParentBundlePath(nPathSize);

		gApp->DoNewsFldrTrash(dynamic_cast<CMyItemsWin *>(this), psItemName, pPathData, nPathSize, nType);
		UMemory::Dispose((TPtr)pPathData);
	}
}

void CMyNewsCategoryExplWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (!((inInfo.mods & (modKey_Command | modKey_Control)) && (inInfo.keyChar == 'c' || inInfo.keyChar == 'C')) && News_KeyDown(inInfo))
		return;

	CMyItemsTreeWin::KeyDown(inInfo);
}

void CMyNewsCategoryExplWin::Timer(TTimer inTimer)
{
	if (inTimer == mArticleListTimer)
		GetContentArticleList();
	else if (inTimer == mArticleTextTimer)
		GetContentArticleText();
}

void CMyNewsCategoryExplWin::SearchText(const Uint8 *inText)
{
	if (mTreeView->IsFocus())
	{
		CMyNewsCategoryTreeView *pNewsTreeView = GetNewsCategoryTreeView();
		Uint32 nIndex = pNewsTreeView->SearchNames(inText);
		
		if (nIndex != pNewsTreeView->GetFirstSelectedTreeItem())
		{
			pNewsTreeView->DeselectAllTreeItems();
		
			if (nIndex)
			{
				pNewsTreeView->SelectTreeItem(nIndex);
				pNewsTreeView->MakeTreeItemVisibleInList(nIndex);
			}
		}
	}
	else if (mArticleTreeView->IsFocus())
	{
		Uint32 nIndex = mArticleTreeView->SearchNames(inText);
		
		if (nIndex != mArticleTreeView->GetFirstSelectedTreeItem())
		{
			mArticleTreeView->DeselectAllTreeItems();
		
			if (nIndex)
			{
				mArticleTreeView->SelectTreeItem(nIndex);
				mArticleTreeView->MakeTreeItemVisibleInList(nIndex);
			}
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewsArticTextWin::CMyNewsArticTextWin(CWindow *inParent, CNZReadList *inReadList, bool inIsComposing, Uint32 inID, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum, Uint32 inParentID)
	: CWindow(SRect(0, 0, 300, 300), windowLayer_Standard, windowOption_CloseBox | windowOption_Sizeable, 0, inParent),
	  CMyPathData(inPathData, inPathSize, inPathSum)
{
#if WIN32
	_SetWinIcon(*this, 223);
#endif

	mReadList = inReadList;
	mIsComposing = inIsComposing;
	mID = inID;
	mPrevID = mNextID = 0;
	mParentID = inParentID;
		
	gApp->mNewsArticTxtWinList.AddItem(this);
	SetLimits(300, 250);
	
#if USE_NEWS_HISTORY
	if (!inIsComposing && mReadList)
		mReadList->SetRead(mID);
#endif		
	
	SRect cBounds(0, 0, 300, 300);

	Uint8 catTxt[256];
	catTxt[0] = UFS::GetPathTargetName(inPathData, inPathSize, catTxt + 1, sizeof(catTxt) - 1);

	CContainerView *vc = new CContainerView(this, cBounds);
	vc->Show();
	mViews.vc = vc;
	
	SRect r(3, 3, 27, 27);
	
	// buttons
	CIconButtonView *icb;
	if (inIsComposing)
	{
		icb = new CIconButtonView(vc, r);
	if (gApp->mIconSet == 2){
		icb->SetIconID(3024);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(222);
	}
		
		icb->SetID(viewID_NewsArticSend);
		vc->SetDefaultView(icb);
		mViews.send = icb;
		mViews.repl = nil;
		icb->SetTooltipMsg("\pSend");
		icb->Show();
	}
	else
	{
		r.right = r.left + 12;
		r.bottom -= 5;
		
		CSimpleIconButtonView *sicb = new CSimpleIconButtonView(vc, r);
		
		if (gApp->mIconSet == 2){
		sicb->SetIconID(218);
	}else if (gApp->mIconSet == 1){
		sicb->SetIconID(218);
	}
		
		sicb->SetID(viewID_NewsArticGoParent);
		sicb->SetEnable(false);
		sicb->SetTooltipMsg("\pOpen Parent");
		sicb->Show();
		mViews.goParent = sicb;
		
////
		r.left = r.right + 3;
		r.bottom = r.top + 11;
		r.right = r.left + 11;
		
		sicb = new CSimpleIconButtonView(vc, r);
		if (gApp->mIconSet == 2){
			sicb->SetIconID(224);
		}else if (gApp->mIconSet == 1){
			sicb->SetIconID(224);
		}
		
		sicb->SetID(viewID_NewsArticGoPrev);
		sicb->SetEnable(true);
		sicb->SetTooltipMsg("\pOpen Previous in Level");
		sicb->Show();
		mViews.goPrev = sicb;
				
		r.top = r.bottom + 2;
		r.bottom = 27;
		
		sicb = new CSimpleIconButtonView(vc, r);
		if (gApp->mIconSet == 2){
			sicb->SetIconID(225);
		}else if (gApp->mIconSet == 1){
			sicb->SetIconID(225);
		}
		
		sicb->SetID(viewID_NewsArticGoNext);
		sicb->SetEnable(true);
		sicb->SetTooltipMsg("\pOpen Next in Level");
		sicb->Show();
		mViews.goNext = sicb;

		r.top = 3;
////		
		r.left = r.right + 3;
		r.right = r.left + 12;
		
		sicb = new CSimpleIconButtonView(vc, r);
		if (gApp->mIconSet == 2){
			sicb->SetIconID(219);
		}else if (gApp->mIconSet == 1){
			sicb->SetIconID(219);
		}
		
		sicb->SetID(viewID_NewsArticGo1stChild);
		sicb->SetEnable(false);
		sicb->SetTooltipMsg("\pOpen First Reply");
		sicb->Show();
		mViews.goChild = sicb;

		r.left = r.right + 10;
		r.right = r.left + 24;
	
		icb = new CIconButtonView(vc, r);
		
		if (gApp->mIconSet == 2){
			icb->SetIconID(3039);
		}else if (gApp->mIconSet == 1){
			icb->SetIconID(220);
		}
		
		icb->SetID(viewID_NewsArticReply);
		mViews.repl = icb;
		mViews.send = nil;
		icb->SetTooltipMsg("\pReply");
		icb->Show();
	}

	/*
	r.right = cBounds.right - 3;
	r.left = r.right - 24;
	
	icb = new CIconButtonView(vc, r);
	icb->SetIconID(212);
	icb->SetID(viewID_NewsArticTrash);
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();
	mViews.trash = icb;
	*/
	r.top = r.bottom + 5;
	r.left = cBounds.left + 5;
	r.right = cBounds.right - 5;
	r.bottom = r.top + (5 + 16) * 4 + 5;
	
	// box
	CBoxView *box = new CBoxView(vc, r, boxStyle_Sunken);
	box->SetSizing(sizing_RightSticky);
	box->Show();
	mViews.box = box;
	
	// labels
	r.left += 5;
	r.top += 5;
	r.right = r.left + 70;
	r.bottom = r.top + 16;
	Int16 lblTop = r.top;
	
	(new CLabelView(vc, r, "\pTitle:"))->Show();

	r.top = r.bottom + 5;
	r.bottom = r.top + 16;
	
	(new CLabelView(vc, r, "\pPoster:"))->Show();

	r.top = r.bottom + 5;
	r.bottom = r.top + 16;
	
	(new CLabelView(vc, r, "\pDate:"))->Show();

	r.top = r.bottom + 5;
	r.bottom = r.top + 16;
	
	(new CLabelView(vc, r, "\pCategory:"))->Show();
	
	// label values
	r.left = r.right + 10;
	r.right = cBounds.right - 5 - 5;
	
	CScrollerView *scr;
	if (inIsComposing)
	{
		r.top = lblTop;
		
	#if MACINTOSH
		r.bottom = r.top + 16;
	#else
		r.bottom = r.top + 20;
	#endif
		
		r.Enlarge(3);
		scr = MakeTextBoxView(vc, r, scrollerOption_Border, &mViews.titleTxt);
		vc->SetFocusView(scr);
		r.Enlarge(-3);
		scr->SetSizing(sizing_RightSticky);
		scr->Show();
		mViews.titleTxt->SetID(viewID_NewsArticTitle);
		mViews.titleTxt->SetFont(kDefaultFont, nil, 9);
		mViews.titleTxt->SetEnterKeyAction(enterKeyAction_Hit);
		mViews.titleLbl = nil;
		
	#if MACINTOSH
		r.bottom += 3;
	#else
		r.bottom -= 3;
	#endif
	}
	else
	{
	#if MACINTOSH
		r.top = lblTop + 3;
	#else
		r.top = lblTop;
	#endif

		r.bottom = r.top + 16;

		CLabelView *lbl = new CLabelView(vc, r);
		lbl->SetFont(kDefaultFont, nil, 9);
		lbl->SetSizing(sizing_RightSticky);
		lbl->Show();
		mViews.titleLbl = lbl;
		mViews.titleTxt = nil;
	}	
		
	r.top = r.bottom + 5;
	r.bottom = r.top + 16;
	
	CLabelView *lbl = new CLabelView(vc, r);
	lbl->SetFont(kDefaultFont, nil, 9);
	lbl->SetSizing(sizing_RightSticky);
	lbl->Show();
	mViews.poster = lbl;
	
	r.top = r.bottom + 5;
	r.bottom = r.top + 16;
	
	lbl = new CLabelView(vc, r);
	lbl->SetFont(kDefaultFont, nil, 9);
	lbl->SetSizing(sizing_RightSticky);
	lbl->Show();
	mViews.date = lbl;
	
	r.top = r.bottom + 5;
	r.bottom = r.top + 16;
	
	lbl = new CLabelView(vc, r);
	lbl->SetFont(kDefaultFont, nil, 9);
	lbl->SetSizing(sizing_RightSticky);
	lbl->SetText(catTxt);
	lbl->Show();
	mViews.category = lbl;
		
	// textbox
	r.top = r.bottom + 5 + 5;
	
	if (inIsComposing)
	{
		r.left = cBounds.left - 3;
		r.right = cBounds.right + 3;
		r.bottom = cBounds.bottom + 3;
		mViews.dataScroller = MakeTextBoxView(vc, r, scrollerOption_Border + scrollerOption_VertBar + scrollerOption_GrowSpace, &mViews.data);
		//mViews.data->SetEnterKeyAction(enterKeyAction_Hit);
		//mViews.data->SetID(viewID_NewsArticText);
	}
	else
	{
		r.left = cBounds.left - 2;
		r.right = cBounds.right + 2;
		r.bottom = cBounds.bottom + 2;
		mViews.dataScroller = MakeTextBoxView(vc, r, scrollerOption_Border + scrollerOption_VertBar | scrollerOption_NoFocusBorder | scrollerOption_GrowSpace, &mViews.data);
		mViews.data->SetEditable(false);
		vc->SetFocusView(mViews.dataScroller);
	}
	
	mViews.data->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.data->SetTabSelectText(false);
	mViews.dataScroller->SetSizing(sizing_BottomRightSticky);
	mViews.dataScroller->Show();

	SetAccess();
}

CMyNewsArticTextWin::~CMyNewsArticTextWin()
{
	gApp->mNewsArticTxtWinList.RemoveItem(this);
}

bool CMyNewsArticTextWin::CanClose()
{
	if (mIsComposing && !mViews.data->IsEmpty())
	{	
		switch (MsgBox(161))
		{
			case 1:
				if (IsTitleAndMsgValid())
				{
					gApp->DoNewsArticSend(this);
					return false;	// this will have been deleted by the send above
				}
				else
				{
					// display a message saying title/content invalid
					USound::Beep();
					return false;
				}
				break;
			case 2:
				return true;
				break;
			case 3:
				return false;
				break;
		
		}
	}
	
	return true;
}

void CMyNewsArticTextWin::KeyDown(const SKeyMsgData& inInfo)
{
	CWindow::KeyDown(inInfo);

	if (mIsComposing)	// these key cmds only apply if not composing
		return;
	
	if (UText::tolower(inInfo.keyChar) == 'r')
		mViews.repl->HitButton();
	else if (UText::tolower(inInfo.keyChar) == 's' && inInfo.mods == modKey_Shortcut)
		gApp->SaveTextAs(mViews.data);
	
	if (inInfo.mods & modKey_Shortcut)
	{
		switch (inInfo.keyCode)
		{
			case key_Up:
				gApp->DoNewsArticGoPrev(this, inInfo.mods & ~modKey_Shortcut);
				break;
			case key_Down:
				gApp->DoNewsArticGoNext(this, inInfo.mods & ~modKey_Shortcut);
				break;
			case key_Left:
				gApp->DoNewsArticGoParent(this, inInfo.mods & ~modKey_Shortcut);
				break;
			case key_Right:
				gApp->DoNewsArticGoFirstChild(this, inInfo.mods & ~modKey_Shortcut);
				break;
		}
	}
}

void CMyNewsArticTextWin::DoReplyArticle()
{
	Uint8 psArticleName[256];
	psArticleName[0] = GetTitle(psArticleName + 1, sizeof(psArticleName) - 1);

	void *pPathData = GetPathPtr();
	Uint32 nPathSize = GetPathSize();
	Uint32 nPathChecksum = GetPathChecksum();
	Uint32 nArticleID = GetArticID();

	void *pNewPathData = nil;
	if (nPathSize)
		pNewPathData = UMemory::New(pPathData, nPathSize);
	
	if (!pNewPathData)
		return;

	CNZReadList *pReadList = GetReadList();

	gApp->DoNewsArticReply(pNewPathData, nPathSize, nPathChecksum, nArticleID, psArticleName, pReadList);
}
void CMyNewsArticTextWin::DoDelete()
{
	Uint8 psArticleName[256];
	psArticleName[0] = GetTitle(psArticleName + 1, sizeof(psArticleName) - 1);

	void *pPathData = GetPathPtr();
	Uint32 nPathSize = GetPathSize();
	Uint32 nPathChecksum = GetPathChecksum();
	Uint32 nArticleID = GetArticID();

	void *pNewPathData = nil;
	if (nPathSize)
		pNewPathData = UMemory::New(pPathData, nPathSize);
	
	if (!pNewPathData)
		return;

	CNZReadList *pReadList = GetReadList();
gApp->DoNewsCatTrash(dynamic_cast<CMyItemsWin *>(this), GetPathPtr(), GetPathSize(), nArticleID, psArticleName);

	//gApp->DoNewsArticReply(pNewPathData, nPathSize, nPathChecksum, nArticleID, psArticleName, pReadList);
}

CMyNewsArticTextWin *CMyNewsArticTextWin::FindWindowByPathAndID(Uint32 inID, const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum)
{
	// only searches through non-composing windows (since ids are not valid anyway otherwise)

	CMyNewsArticTextWin *win;
	Uint32 i = 0;
	
	if (inCheckSum == 0)
		inCheckSum = UMemory::Checksum(inPathData, inPathSize);
	
	while (gApp->mNewsArticTxtWinList.GetNext(win, i))
	{
		if (!win->IsComposing() && win->GetArticID() == inID && win->GetPathChecksum() == inCheckSum && win->IsPathEqual(inPathData, inPathSize))
			return win;
	}
	
	return nil;
}

void CMyNewsArticTextWin::TitleChanged()
{
	if (mIsComposing)
	{
		Uint8 title[32];
		title[0] = mViews.titleTxt->GetText(title + 1, sizeof(title) - 1);
		
		SetTitle(title);
		mViews.titleTxt->SetHasChanged(false);
	}
}

void CMyNewsArticTextWin::StateChanged()
{
	CWindow::StateChanged();
	
	if (mIsComposing)
		return;
		
	Uint16 state = UWindow::GetState(mWindow);
	
	// find my parent if it exists
	// if so, set its selected to me.
	if (state == windowState_Focus)
		gApp->SelectArticle(mPathData, mPathSize, mPathSum, mID);
}

void CMyNewsArticTextWin::SetContentsFromFields(TFieldData inFieldData)
{
	if (mIsComposing)
		return;			// don't allow the server to set the data on an article I am composing
		
	// the fields I get will be prolly flavour, title, date, poster, and data
	union
	{
		Int8 flav[256];
		Uint8 str[256];
	};	
	SDateTimeStamp dts;
	
	inFieldData->GetPString(myField_NewsArtTitle, str);
	SetMyTitle(str);
	
	inFieldData->GetPString(myField_NewsArtPoster, str);
	mViews.poster->SetText(str);
	
	dts.year = dts.msecs = dts.seconds = 0;
	inFieldData->GetField(myField_NewsArtDate, &dts, sizeof(dts));
#if CONVERT_INTS
	dts.year = FB(dts.year);
	dts.msecs = FB(dts.msecs);
	dts.seconds = FB(dts.seconds);
#endif
	str[0] = UDateTime::DateToText(dts, str+1, sizeof(str)-1, kAbbrevDateText | kTimeWithSecsText);
	mViews.date->SetText(str);
	
	SetPrevArticID(inFieldData->GetInteger(myField_NewsArtPrevArt));
	SetNextArticID(inFieldData->GetInteger(myField_NewsArtNextArt));
	SetParentID(inFieldData->GetInteger(myField_NewsArtParentArt));
	SetFirstChildID(inFieldData->GetInteger(myField_NewsArt1stChildArt));
	
	inFieldData->GetCString(myField_NewsArtDataFlav, flav, sizeof(flav));
	
	if (strcmp(flav, "text/plain"))
		return;
	
	Uint32 s = inFieldData->GetFieldSize(myField_NewsArtData);
	if (s > 65536)
		s = 65536;
	
	THdl h = mViews.data->GetTextHandle();
	if (!h)
		DebugBreak("hdl doesn't exist in SetContentsFromFields CMyNewsArticTextWin");
	
	h->Reallocate(s);
	{
		void *p;
		StHandleLocker lock(h, p);
		
		inFieldData->GetField(myField_NewsArtData, p, s);
	}
	
	mViews.data->SetTextHandle(h);
	mViews.dataScroller->ScrollToTop();
}

void CMyNewsArticTextWin::SetMyTitle(const Uint8 inTitle[])
{
	SetTitle(inTitle);

	if (mIsComposing)
	{
		mViews.titleTxt->SetText(inTitle + 1, inTitle[0]);
		
		// set the focus to the data
		mViews.vc->SetFocusView(mViews.dataScroller);
	}
	else
		mViews.titleLbl->SetText(inTitle);
}

void CMyNewsArticTextWin::SetAccess()
{
	if (!mViews.repl)
		return;
	
	if (mPathData && gApp->HasBundlePriv(myAcc_NewsPostArt))
		mViews.repl->Enable();
	else 
		mViews.repl->Disable();
}

void CMyNewsArticTextWin::SetArticID(Uint32 inID)
{
	// get my parent win
	// if it exists, get the selected ID.
	// if it equals my ID, set selection to inID
	gApp->SetCurrentArticle(mPathData, mPathSize, mPathSum, mID, inID);

	mID = inID;

#if USE_NEWS_HISTORY
	if (!mIsComposing && mReadList)
		mReadList->SetRead(mID);
#endif		
}

void CMyNewsArticTextWin::SetPrevArticID(Uint32 inID)
{
	mPrevID = inID;
	mViews.goPrev->SetEnable(inID != 0);
}

void CMyNewsArticTextWin::SetNextArticID(Uint32 inID)
{
	mNextID = inID;
	mViews.goNext->SetEnable(inID != 0);
}

void CMyNewsArticTextWin::SetParentID(Uint32 inID)
{
	mParentID = inID;
	mViews.goParent->SetEnable(inID != 0);
}

void CMyNewsArticTextWin::SetFirstChildID(Uint32 inID)
{
	mFirstChildID = inID;
	mViews.goChild->SetEnable(inID != 0);
}

void CMyNewsArticTextWin::SetTextColor(const SMyColorInfo& colorInfo) 
{
	mViews.dataScroller->SetContentColor(colorInfo.backColor);

	mViews.data->SetColor(colorInfo.textColor);
	mViews.data->SetFontSize(colorInfo.textSize);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewNewsCatWin::CMyNewNewsCatWin()
	: CWindow(SRect(0,0,300,146), windowLayer_Modal)
{
	CContainerView *vc;
	CButtonView *btn;
	CScrollerView *scr;
	CIconView *icn;
	CLabelView *lbl;
	CBoxView *box;

	// setup window
	SetTitle("\pCreate News Category");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,300,146));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();
	
	// make label
	lbl = new CLabelView(vc, SRect(50,17,290,33));
	lbl->SetText("\pEnter a name for the new category:");
	lbl->Show();

	// make box
	box = new CBoxView(vc, SRect(10,50,290,100));
	box->SetStyle(boxStyle_Sunken);
	box->Show();

	// make text box
	scr = MakeTextBoxView(vc, SRect(50,62,250,88), scrollerOption_Border, &mViews.nameText);
	scr->Show();
	vc->SetFocusView(scr);
	mViews.nameText->SetID(3);
	
	// make buttons
	btn = new CButtonView(vc, SRect(214,110,290,136));
	btn->SetTitle("\pCreate");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Disable();
	btn->Show();
	vc->SetDefaultView(btn);
	mViews.createBtn = btn;
	btn = new CButtonView(vc, SRect(124,113,194,133));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);
}

void CMyNewNewsCatWin::UpdateCreateButton()
{
	mViews.createBtn->SetEnable(!mViews.nameText->IsEmpty());
	mViews.nameText->SetHasChanged(false);
}

