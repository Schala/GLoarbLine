/* (c)2007 GLoarbLine Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "Hotline.h"

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

CMyAdmITreeView::CMyServerTreeView(CViewHandler *inHandler, const SRect& inBounds, CMyServerWindow *inServerWin)
	: CTabbedTreeView(inHandler, inBounds, 240, 241)
{
#if WIN32
	mTabHeight = 18;
#endif

#if DISABLE_TABS
	mTabHeight = 0;
#endif

	mBehaviour = itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction;

	mServerWin = inServerWin;
	mMaxWidth = 0;
	mBigDesc[0] = 0;

	// set headers
	AddTab("\pName", 80, 80);
	AddTab("\pStatus", 1, 1, align_CenterHoriz);
	AddTab("\pDescription");
	SetTabs(70,17,13);
}

CMyServerTreeView::~CMyServerTreeView()
{
	ClearTree();
}

void CMyServerTreeView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2, Uint8 inTabPercent3)
{
	if (!inTabPercent1 && !inTabPercent2 && !inTabPercent3)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	lPercentList.AddItem(&inTabPercent3);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyServerTreeView:: SetTabs(Uint16 inTabWidth1, Uint16 inTabWidth2)
{
	SetTabWidth(1, inTabWidth1);
	SetTabWidth(2, inTabWidth2);
}

void CMyServerTreeView::GetTabs(Uint16& outTabWidth1, Uint16& outTabWidth2)
{
	outTabWidth1 = GetTabWidth(1);
	outTabWidth2 = GetTabWidth(2);
}

Uint32 CMyServerTreeView::AddTracker(SMyServerInfo *inTracker)
{
	if (inTracker->Desc[0] > mBigDesc[0])
		UMemory::Copy(mBigDesc, inTracker->Desc, inTracker->Desc[0] + 1);

	return AddTreeItem(0, inTracker);
}

Uint32 CMyServerTreeView::AddServer(Uint32 inParentIndex, SMyServerInfo *inServer)
{	
	if (inServer->Desc[0] > mBigDesc[0])
		UMemory::Copy(mBigDesc, inServer->Desc, inServer->Desc[0] + 1);
		
	return AddTreeItem(inParentIndex, inServer, false);
}

void CMyServerTreeView::RefreshTrackers()
{
	Uint32 i = 0;
	SMyServerInfo *pTrackInfo;
	
	while (GetNextTreeItem(pTrackInfo, i, true))
		RefreshTreeItem(i);
}

void CMyServerTreeView::RefreshTracker(Uint32 inTrackerID)
{
	Uint32 nIndex = GetTrackerIndex(inTrackerID);
	
	if (nIndex)
		RefreshTreeItem(nIndex);
}

Uint32 CMyServerTreeView::GetSelectedTrackerID(bool *outIsBookmark)
{
	if (outIsBookmark)
		*outIsBookmark = false;
	
	Uint32 nSelectedIndex = GetFirstSelectedTreeItem();
	if (!nSelectedIndex)
		return 0;
	
	if (nSelectedIndex == 1)
	{
		if (outIsBookmark)
			*outIsBookmark = true;
		
		return 0;
	}
	
	if (GetTreeItemLevel(nSelectedIndex) == 1)
	{
		SMyServerInfo *pTrackInfo = GetTreeItem(nSelectedIndex);
		if (pTrackInfo)
			return pTrackInfo->Flags;
	}
	
	return 0;
}

const Uint8 *CMyServerTreeView::GetSelectedBookmarkName()
{
	Uint32 nSelectedIndex = GetFirstSelectedTreeItem();
	if (!nSelectedIndex)
		return nil;

	if (GetTreeItemLevel(nSelectedIndex) == 2 && GetParentTreeIndex(nSelectedIndex) == 1)
	{
		SMyServerInfo *pTrackInfo = GetTreeItem(nSelectedIndex);
		if (pTrackInfo)
			return pTrackInfo->Name;
	}
	
	return nil;
}

SMyServerInfo *CMyServerTreeView::GetSelectedServInfo(bool &outBookmark)
{
	outBookmark = false;
	Uint32 nSelectedIndex = GetFirstSelectedTreeItem();
	
	if (nSelectedIndex && GetTreeItemLevel(nSelectedIndex) > 1)
	{
		SMyServerInfo *pServerInfo = GetTreeItem(nSelectedIndex);
		
		if (pServerInfo)
		{
			SMyServerInfo *pTrackerInfo = GetParentTreeItem(nSelectedIndex);
			if (pTrackerInfo && !pTrackerInfo->Flags)
				outBookmark = true;
			
			return pServerInfo;
		}
	}

    return nil;
}

bool CMyServerTreeView::GetSelectedServInfo(bool &outBookmark, Uint8 *outName, Uint8 *outAddress)
{
	SMyServerInfo *pServerInfo = GetSelectedServInfo(outBookmark);
	if (!pServerInfo)
		return false;
				
	if (outName)
		UMemory::Copy(outName, pServerInfo->Name, pServerInfo->Name[0] + 1);
	
	if (outAddress)
	{
		Uint8 *address = (Uint8 *)&pServerInfo->Address;
		if (pServerInfo->Port == 5500)
			outAddress[0] = UText::Format(outAddress + 1, 31, "%hu.%hu.%hu.%hu", (Uint16)address[0], (Uint16)address[1], (Uint16)address[2], (Uint16)address[3]);
		else
			outAddress[0] = UText::Format(outAddress + 1, 31, "%hu.%hu.%hu.%hu:%hu", (Uint16)address[0], (Uint16)address[1], (Uint16)address[2], (Uint16)address[3], (Uint16)pServerInfo->Port);
	}

	return true;
} 

Uint32 CMyServerTreeView::GetTotalServerCount()
{
	return GetTreeCount();
}

bool CMyServerTreeView::SetTrackerDisclosure(Uint16 inTrackID, Uint8 inDisclosure)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackInfo;
	
	while (GetNextTreeItem(pTrackInfo, i, true))
	{
		if (pTrackInfo->Flags == inTrackID)
			return SetDisclosure(i, inDisclosure);
	}

	return false;
}

Uint8 CMyServerTreeView::GetTrackerDisclosure(Uint16 inTrackID)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackInfo;
	
	while (GetNextTreeItem(pTrackInfo, i, true))
	{
		if (pTrackInfo->Flags == inTrackID)
			return GetDisclosure(i);
	}

	return optTree_NoDisclosure;
}

Uint32 CMyServerTreeView::GetTrackerIndex(Uint16 inTrackID)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackInfo;
	
	while (GetNextTreeItem(pTrackInfo, i, true))
	{
		if (pTrackInfo->Flags == inTrackID)
			return i;
	}

	return 0;
}

void CMyServerTreeView::ExpandAllTrackers()
{	
	Uint32 i = 0;
	SMyServerInfo *pTrackInfo;
	
	while (GetNextTreeItem(pTrackInfo, i, true))
		SetDisclosure(i, optTree_Disclosed);
}

void CMyServerTreeView::RemoveServers()
{
	Uint32 i = 0;
	SMyServerInfo *pTrackerInfo = nil;
		
	while (GetNextTreeItem(pTrackerInfo, i, true))
		RemoveChildTree(i);
}

void CMyServerTreeView::RemoveTracker(Uint16 inTrackerID, bool inTracker)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackerInfo = nil;
		
	while (GetNextTreeItem(pTrackerInfo, i, true))
	{
		if (pTrackerInfo->Flags == inTrackerID)
		{
			if (inTracker)
				RemoveTreeItem(i);
			else
				RemoveChildTree(i);
		}
	}
}

void CMyServerTreeView::SetStatusMsg(Uint16 inTrackerID, const Uint8 inMsg[])
{
	Uint32 nIndex = GetTrackerIndex(inTrackerID);
	if (!nIndex)
		return;
	
	SMyServerInfo *pTrackerInfo = GetTreeItem(nIndex);
	if (!pTrackerInfo)
		return;
	
	pTrackerInfo->User[0] = UMemory::Copy(pTrackerInfo->User + 1, inMsg + 1, inMsg[0] > sizeof(pTrackerInfo->User) - 1 ? sizeof(pTrackerInfo->User) - 1 : inMsg[0]);
	RefreshTreeItem(nIndex);
}

void CMyServerTreeView::SelectionChanged(Uint32 inTreeIndex, SMyServerInfo *inTreeItem, bool inIsSelected)
{
	#pragma unused (inTreeItem)
	
	if (inIsSelected)
	{
		Uint32 nTreeLevel = GetTreeItemLevel(inTreeIndex);
		mServerWin->SetEnableTrash((nTreeLevel == 1 && inTreeItem->Flags) || (nTreeLevel == 2 && GetParentTreeIndex(inTreeIndex) == 1));
	}
}

void CMyServerTreeView::DisclosureChanged(Uint32 inTreeIndex, SMyServerInfo *inTreeItem, Uint8 inDisclosure)
{
	if (inDisclosure == optTree_Disclosed && !GetChildTreeCount(inTreeIndex))
		UApplication::PostMessage(1120, &inTreeItem->Flags, sizeof(Uint16));
}

void CMyServerTreeView::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyServerInfo *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inOptions)
		
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

	if (inTreeLevel == 1 && inTreeItem->Address)
		inTreeItem->User[0] = UText::Format(inTreeItem->User + 1, sizeof(inTreeItem->User) - 1, "%lu/%lu", GetChildTreeCount(inTreeIndex), inTreeItem->Address);
	
	// draw item icon and name
	const SRect *pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += 2 + (inTreeLevel == 1 ? 0 : 10);
		stRect.right = stRect.left + 16;
			
		// draw icon
		if (stRect.right < pBounds->right)
			inTreeItem->Icon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
	
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 24 + (inTreeLevel == 1 ? 0 : 10);
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;

		// draw item name
		inImage->SetFontEffect(fontEffect_Bold);
		inImage->DrawTruncText(stRect, inTreeItem->Name + 1, inTreeItem->Name[0], 0, align_Left | align_CenterVert);
	}

	// draw status
	pBounds = inTabRectList.GetItem(2);
	if (pBounds)
	{
		if (pBounds->GetWidth())
		{
			// set rect
			stRect = *pBounds;
			stRect.top += 3;
			stRect.bottom -= 2;
			stRect.left += 1;
			if (stRect.right < stRect.left)
				stRect.right = stRect.left;
		
			bool bDrawStatus = true;
			if (inTreeLevel > 1)
			{
				SMyServerInfo *pTrackerInfo = GetParentTreeItem(inTreeIndex);
				if (pTrackerInfo && !pTrackerInfo->Flags)
					bDrawStatus = false;
			}

			if (bDrawStatus)
			{
				inImage->SetFontEffect(fontEffect_Plain);
				inImage->DrawTruncText(stRect, inTreeItem->User + 1, inTreeItem->User[0], 0, align_CenterHoriz | align_CenterVert);
			}
		}
		
		// set bounds
		bool bSetBounds = false;
		SRect stBounds = mBounds;
		Uint32 nRight = inImage->GetTextWidth(mBigDesc + 1, mBigDesc[0]) + 10;

		if (nRight > mMaxWidth)
		{
			mMaxWidth = nRight;
			stBounds.right = mMaxWidth + pBounds->right + 4;
			bSetBounds = true;		
		}
		
		CScrollerView *pScrHandler = dynamic_cast<CScrollerView *>(GetHandler());
		if (pScrHandler)
		{
			Int32 nScrWidth = pScrHandler->GetVisibleContentWidth();
		
			if (stBounds.right < nScrWidth)
			{
				stBounds.right = nScrWidth;
				bSetBounds = true;
			}
		}
		
		if (bSetBounds)
			SetBounds(stBounds);
	}
	
	// draw description
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
	
		inImage->SetFontEffect(fontEffect_Plain);
		inImage->DrawTruncText(stRect, inTreeItem->Desc + 1, inTreeItem->Desc[0], 0, align_Left | align_CenterVert);
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyServerWindow::CMyServerWindow(CWindow *inParent)
	: CWindow(SRect(0,0,330,334), windowLayer_Standard, windowOption_CloseBox | windowOption_ZoomBox | windowOption_Sizeable, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 233);
#endif
		
	// setup window
	SetTitle("\pServers");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	SetLimits(300,150);
		
	// make container view for content
	CContainerView *vc = new CContainerView(this, (SRect(0,0,330,334)));
	vc->Show();
	
	CLabelView *labl = new CLabelView(vc, SRect(88,8,135,24));
	labl->SetFont(kDefaultFont, nil, 9);
	labl->SetText("\pSearch:");
	labl->Show();
	
	mServerNumLabl = new CLabelView(vc, SRect(222,8,275,24));
	mServerNumLabl->SetFont(kDefaultFont, nil, 9);
	mServerNumLabl->SetText("\p0/0");
	mServerNumLabl->SetSizing(sizing_HorizSticky);
	mServerNumLabl->Show();
	
	CScrollerView *Scr = MakeTextBoxView(vc, SRect(140,2,216,28), scrollerOption_Border, &mFilterText);
	mFilterText->SetEnterKeyAction(enterKeyAction_Hit);
	mFilterText->SetCommandID(TrackCmd_Filter);
	mFilterText->SetFont(fd_Default9);
	mFilterText->SetSizing(sizing_RightSticky);
	mFilterText->Show();
	Scr->SetSizing(sizing_RightSticky);
	Scr->Show();
	
	CIconButtonView *icb  = new CIconButtonView(vc, SRect(3, 3,27,27), nil, nil, 1, nil);
	//DebugBreak("On lit giveiconset et affiche icone: %i",gApp->GiveIconSet());
		
	 if (gApp->GiveIconSet() == 2){
		icb  = new CIconButtonView(vc, SRect(3, 3,27,27), viewID_Connect, nil, 3032, nil);
	}else if (gApp->GiveIconSet() == 1){
		icb  = new CIconButtonView(vc, SRect(3, 3,27,27), viewID_Connect, nil, 411, nil);
	}
	icb->SetTooltipMsg("\pConnect to Server");
	icb->SetID(viewID_Connect);
	icb->Show();

 if (gApp->GiveIconSet() == 2){
		icb = new CIconButtonView(vc, SRect(30,3,54,27), TrackCmd_AddTracker, nil, 3033, nil);
	}else if (gApp->GiveIconSet() == 1){
		icb = new CIconButtonView(vc, SRect(30,3,54,27), TrackCmd_AddTracker, nil, 232, nil);
	}
	
	icb->SetTooltipMsg("\pAdd Tracker");
	icb->Show();

	
	
	//DebugBreak("%i",gApp->GiveIconSet());
	if (gApp->GiveIconSet() == 2){
		icb = new CIconButtonView(vc, SRect(57,3,81,27), viewID_Refresh, nil, 3026, nil);
	}else if (gApp->GiveIconSet() == 1){
		icb = new CIconButtonView(vc, SRect(57,3,81,27), viewID_Refresh, nil, 205, nil);
	}
		
		
	icb->SetID(viewID_Refresh);
	icb->SetTooltipMsg("\pRefresh");
	icb->Show();
	
    // help
    #if USE_HELP
	icb = new CIconButtonView(vc, SRect(276,3,300,27));
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpServers);
	icb->SetTooltipMsg("\pHelp");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();
	#endif
	  
	  if (gApp->GiveIconSet() == 2){
		icb = new CIconButtonView(vc, SRect(303,3,327,27), viewID_Delete, nil, 3000, nil);
	}else if (gApp->GiveIconSet() == 1){
		icb = new CIconButtonView(vc, SRect(303,3,327,27), viewID_Delete, nil, 212, nil);
	}
 	icb->SetID(viewID_Delete);
	icb->SetTooltipMsg("\pDelete");
	icb->SetSizing(sizing_HorizSticky);
	icb->Disable();
	icb->Show();
   	mTrash = icb;
   
    //26
  	mScrChatz = new CScrollerView(vc, SRect(-2,30,332,336));
  	mScrChatz->SetOptions(scrollerOption_VertBar + scrollerOption_HorizBar + scrollerOption_NoFocusBorder + scrollerOption_Border + scrollerOption_NoBkgnd);
   	mScrChatz->SetSizing(sizing_BottomRightSticky);
   	mScrChatz->SetCanFocus(true);
  
  	mServerTreeView = new CMyServerTreeView(mScrChatz, SRect(0, 0, mScrChatz->GetVisibleContentWidth(), mScrChatz->GetVisibleContentHeight()), this);
  	mServerTreeView->SetCanFocus(true);
  	mServerTreeView->SetSizing(sizing_FullHeight | sizing_FullWidth);
  	mServerTreeView->SetCommandID(TrackCmd_ServerConnect);
  	mServerTreeView->Show();
  	
  	mScrChatz->Show();
}	

void CMyServerWindow::UserZoom(const SMouseMsgData& /* inInfo */)
{
	SRect r;
	Uint32 h, w;
	
	mServerTreeView->GetFullSize(w, h);

	GetBounds(r);
	r.bottom = r.top + h + 46;
	r.right = r.left + w + 15;
	
	if (UWindow::GetZoomBounds(mWindow, r, r))
		SetBounds(r);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyTrackServWindow::CMyTrackServWindow(CWindow *inParent)
	: //CWindow(SRect(0,0,(409-133),282), windowLayer_Standard, 0, 0, inParent)
		CWindow(SRect(0,0,409,282), windowLayer_Standard, 0, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 232);
#endif
	
	// setup window
	SetTitle("\pNew Account");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	// make container view for content
	//CContainerView 	*vc = new CContainerView(this, (SRect(0,0,(409-133),282)));
	CContainerView 	*vc = new CContainerView(this, (SRect(0,0,409,282)));
	vc->Show();

	CBoxView *Box = new CBoxView(vc, SRect(4,5,273,82));
	Box->SetTitle("\pTracker Info");
	Box->SetStyle(boxStyle_Etched);
	Box->Show();
	
	CBoxView *Box2 = new CBoxView(vc, SRect(4,115,273,253));
	Box2->SetTitle("\pConnection");
	Box2->SetStyle(boxStyle_Etched);
	Box2->Show();
	
	CBoxView *Box3 = new CBoxView(vc, SRect(12,177,266,246));
	Box3->SetStyle(boxStyle_Sunken);
	Box3->Show();

	CScrollerView *Scr = MakeTextBoxView(vc, SRect(86,21,263,47), scrollerOption_Border, &mName);
	mName->SetEnterKeyAction(enterKeyAction_None);
	mName->SetEditable(true);
	vc->SetFocusView(Scr);
	mName->Show();
	Scr->Show();
	
	CScrollerView *Scr2 = MakeTextBoxView(vc, SRect(86,51,263,77), scrollerOption_Border, &mAddr);
	mAddr->SetEnterKeyAction(enterKeyAction_None);
	mAddr->SetEditable(true);
	mAddr->Show();
	Scr2->Show();
	
	mScrLogin = MakeTextBoxView(vc, SRect(90,183,259,209), scrollerOption_Border, &mLogin);
	mLogin->SetEnterKeyAction(enterKeyAction_None);
	mLogin->SetEditable(true);
	mLogin->Show();
	mScrLogin->Show();
	
	mScrPass = MakeTextBoxView(vc, SRect(90,213,259,239), scrollerOption_Border, &mPass);
	mPass->SetEnterKeyAction(enterKeyAction_None);
	mPass->SetEditable(true);
	mPass->Show();
	mScrPass->Show();
	
	mLogin->Disable();
	mScrLogin->Disable();
	mPass->Disable();
	mScrPass->Disable();
	
	mDisclose = new CSimpleIconBtnView(vc, SRect(4,86,29,109));
	mDisclose->SetIconID(240);
	mDisclose->SetCommandID(TrackCmd_Disclose);
	mDisclose->Show();
	//
	mGuest = new CCheckBoxView(vc, SRect(22,133,80,148));
	mGuest->SetTitle("\pGuest");
	mGuest->SetCommandID(TrackCmd_GuestHit);
	mGuest->SetMark(true);
	mGuest->SetStyle(1);
	mGuest->Show();
	
	mAccount = new CCheckBoxView(vc, SRect(22,157,95,171));
	mAccount->SetCommandID(TrackCmd_AccntHit);
	mAccount->SetTitle("\pAccount");
	mAccount->SetStyle(1);
	mAccount->Show();
	
	mAccount->SetExclusiveNext(mGuest);
	mGuest->SetExclusiveNext(mAccount);
	
	// make buttons
	CButtonView *save, *cancel;
	SButtons btns[] = {{TrackCmd_SaveTracker, "\pSave", btnOpt_CommandID | btnOpt_Default, &save}, {cmd_Cancel, "\pCancel", btnOpt_CommandID | btnOpt_Cancel, &cancel}};
	CButtonView::BuildButtons(vc, SRect(90,254,270,280), btns);
	save->SetSizing(sizing_VertSticky+sizing_HorizSticky);
	cancel->SetSizing(sizing_VertSticky+sizing_HorizSticky);

	CLabelView *name = new CLabelView(vc, SRect(38,27,79,43));
	name->SetText("\pName:");
	name->Show();
	
	CLabelView *addr = new CLabelView(vc, SRect(26,55,83,70));
	addr->SetText("\pAddress:");
	addr->Show();
	
	CLabelView *login = new CLabelView(vc, SRect(46,187,85,202));
	login->SetText("\pLogin:");
	login->Show();
	
	CLabelView *pass = new CLabelView(vc, SRect(24,217,91,233));
	pass->SetText("\pPassword:");
	pass->Show();
	
	// set default to be colapsed
	Disclose();
}

void CMyTrackServWindow::Disclose()
{
	SRect WinBounds;
	GetBounds(WinBounds);
	Uint32 height = WinBounds.bottom - WinBounds.top;

	if (height > 115)	// Show Tops, Set Small
	{
		SRect bounds = WinBounds;
		bounds.bottom = bounds.top + 115;
		SetBounds(bounds);
		SetDiscloseIcon(240);
	}
	else 				// Hide Tops, Set Big
	{
		SRect bounds = WinBounds;
		bounds.bottom = bounds.top + 282;
		SetBounds(bounds);
		SetDiscloseIcon(241);
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyTracker::CMyTracker(CWindow *inParent)
{

	mServerWin = new CMyServerWindow(inParent);
	mServerTreeView = mServerWin->GetServerTreeView();
	
	mLastTrackerID = 1;
	mTimer = UTimer::New(TimerProc, this);
	
	AddBookmarksInTree();
	RefreshBookmarks();
}

CMyTracker::~CMyTracker()
{
	// delete the window because it's dependant on the data structs we're purging now
	// should the window own this data?  it may make more sense (have a CMyTrackerWin instead of CMyTracker)
	delete mServerWin;
	mServerWin = nil;
	
	Uint32 i = 0;
	SMyTrackerInfo *pTrackerInfo;
    
   	while (mTrackerList.GetNext(pTrackerInfo, i))
   		UMemory::Dispose((TPtr)pTrackerInfo);
	    
	 mTrackerList.Clear();  

	i = 0;
	SMyServerInfo *pServerInfo = nil;
		
	while (mServerTree.GetNext(pServerInfo, i))
	{
		UIcon::Release(pServerInfo->Icon);
		UMemory::Dispose((TPtr)pServerInfo->Name);
		UMemory::Dispose((TPtr)pServerInfo->Desc);
		UMemory::Dispose((TPtr)pServerInfo->SearchName);
		UMemory::Dispose((TPtr)pServerInfo->SearchDesc);

		UMemory::Dispose((TPtr)pServerInfo);
	}
		
	mServerTree.Clear();	
}

void CMyTracker::ShowServersWindow()
{
	if (mServerWin->IsVisible())
		mServerWin->BringToFront();
	else
		mServerWin->Show();
	
	if (mServerWin->IsCollapsed())
 		mServerWin->Expand();

	mServerWin->SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
}

bool CMyTracker::CloseWindow(CWindow *inWindow)
{
	if (inWindow == mServerWin)
	{
		mServerWin->Hide();
		return true;
	}
	else if (dynamic_cast<CMyTrackServWindow *>(inWindow))
	{
		delete inWindow;
		return true;
	}

	return false;
}

bool CMyTracker::KeyCommand(Uint32 inCmd, const SKeyMsgData& /*inInfo*/)
{
	switch (inCmd)
	{
		case TrackCmd_AddTracker:
		{
			mServerTreeView->DeselectAllTreeItems();

			CMyTrackServWindow *pWindow = new CMyTrackServWindow(gApp->mAuxParentWin);				
			pWindow->Show();
		}
		break;
		
		default:
			return false;
	}
	
	return true;
}

// returns whether this is from one of my windows and I processed it
bool CMyTracker::WindowHit(CWindow *inWindow, const SHitMsgData& inInfo)
{
	switch (inInfo.cmd)
	{
		// close box
		case CWindow::ClassID:
			return CloseWindow(inWindow);
			break;
			
		case cmd_Cancel:
			delete inWindow;
			return true;
			break;
			
		case TrackCmd_ServerConnect:
			if (!EditSelectedTracker())
				gApp->DoConnectToTracked(inInfo.param & modKey_Option ? 1 : 0);			
			break;
											
		case TrackCmd_Filter:
			mTimer->Stop();
				
			if (inInfo.type == hitType_Standard)
				FilterServerList();
			else
				mTimer->Start(1000);

			inInfo.view->SetHasChanged(false);
			break;
						
		case TrackCmd_Disclose:
			if (dynamic_cast<CMyTrackServWindow *>(inWindow))
				(dynamic_cast<CMyTrackServWindow *>(inWindow))->Disclose();
			break;
			
		case TrackCmd_GuestHit:
			if (dynamic_cast<CMyTrackServWindow *>(inWindow))
				(dynamic_cast<CMyTrackServWindow *>(inWindow))->GuestHit();
			break;
			
		case TrackCmd_AccntHit:
			if (dynamic_cast<CMyTrackServWindow *>(inWindow))
				(dynamic_cast<CMyTrackServWindow *>(inWindow))->AccntHit();
			break;
						
		case TrackCmd_SaveTracker:
			CMyTrackServWindow *win = dynamic_cast<CMyTrackServWindow *>(inWindow);
			if (win)
			{		
			///////////////
			
			
			/////////////////
				Uint8 psAddr[256];
				win->GetAddr(psAddr);
				
				if (psAddr[0])
				{
					Uint8 psName[256];
					win->GetName(psName);

					Uint8 psLogin[256];
					win->GetLogin(psLogin);
				
					Uint8 psPass[256];
					win->GetPass(psPass);
				
					Uint8 psComments[256];
					pstrcpy(psComments, "\p");
				
					short bIsAccount = win->IsAccount();

					Uint32 nTrackerID = mServerTreeView->GetSelectedTrackerID();
					if (nTrackerID)
						UpdateTrackerInList(nTrackerID, psName, psComments, psAddr, psLogin, psPass, bIsAccount);
					else
						AddTrackerInList(psName, psComments, psAddr, psLogin, psPass, bIsAccount, mLastTrackerID++);

					delete inWindow;
				}
				else
					gApp->DisplayStandardMessage("\pNeed Address", "\pPlease enter the address of this tracker.", icon_Stop, 1);
			}
			break;
						
		case TrackCmd_AddTracker:
			mServerTreeView->DeselectAllTreeItems();
		
			CMyTrackServWindow *pWindow = new CMyTrackServWindow(gApp->mAuxParentWin);
			pWindow->Show();
			break;
							
		default:
			return false;
			break;
				
	}

	return true;
}

void CMyTracker::TimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	#pragma unused(inMsg, inData, inDataSize)
	
	((CMyTracker *)inContext)->Timer((TTimer)inObject);
}

void CMyTracker::Timer(TTimer inTimer)
{
	#pragma unused(inTimer)
	
	FilterServerList();
}

void CMyTracker::SetTabs(Uint16 inTabWidth1, Uint16 inTabWidth2)
{
	mServerWin->GetServerTreeView()->SetTabs(inTabWidth1, inTabWidth2);
}

void CMyTracker::GetTabs(Uint16& outTabWidth1, Uint16& outTabWidth2)
{
	mServerWin->GetServerTreeView()->GetTabs(outTabWidth1, outTabWidth2);
}

void CMyTracker::AddTrackerInList(SMyTrackerInfo *inTracker)
{
	mTrackerList.AddItem(inTracker);
	AddTrackerInTree(inTracker);
}

void CMyTracker::AddTrackerInList(Uint8 *Name, Uint8 *Comments, Uint8 *inAddress, 
								  Uint8 *Login, Uint8 *Pass, short isAccount, 
								  Uint16 inTrackerID)
{
	SMyTrackerInfo *pTrackerInfo = (SMyTrackerInfo*)UMemory::NewClear(sizeof(SMyTrackerInfo));
	
	pstrcpy(pTrackerInfo->Name, Name);
	pstrcpy(pTrackerInfo->Comments, Comments);
	pstrcpy(pTrackerInfo->Address, inAddress);
	pstrcpy(pTrackerInfo->Login, Login);
	pstrcpy(pTrackerInfo->Pass, Pass);
	pTrackerInfo->hasAccount = isAccount;
	pTrackerInfo->nTrackerID = inTrackerID;
	
	mTrackerList.AddItem(pTrackerInfo);
	AddTrackerInTree(pTrackerInfo);
}

void CMyTracker::AddBookmarksInTree()
{
	SMyServerInfo *pTrackerInfo = (SMyServerInfo *)UMemory::NewClear(sizeof(SMyServerInfo));
	
	if (gApp->GiveIconSet() == 2){
		pTrackerInfo->Icon = UIcon::Load(3003);
		}else if (gApp->GiveIconSet() == 1){
		pTrackerInfo->Icon = UIcon::Load(401);
		}
	
	pTrackerInfo->Name = (Uint8 *)UMemory::New(10);
	pTrackerInfo->Desc = (Uint8 *)UMemory::New(10);
	pTrackerInfo->SearchName = (Uint8 *)UMemory::New(10);
	pTrackerInfo->SearchDesc = (Uint8 *)UMemory::New(10);
		
	UMemory::Copy(pTrackerInfo->Name, "\pBookmarks", 10);
	UMemory::Copy(pTrackerInfo->Desc, "\pBookmarks", 10);
	UMemory::Copy(pTrackerInfo->SearchName, "\pBookmarks", 10);
	UMemory::Copy(pTrackerInfo->SearchDesc, "\pBookmarks", 10);

	mServerTree.AddItem(0, pTrackerInfo);
	mServerTreeView->AddTracker(pTrackerInfo);
}

void CMyTracker::AddTrackersInTree()
{
	SMyTrackerInfo **pInfoPtr = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	while (nCount--)
	{
		SMyTrackerInfo *pTrackerInfo = *pInfoPtr;
		AddTrackerInTree(pTrackerInfo);
	
		pInfoPtr++;
	}
}

void CMyTracker::AddTrackerInTree(SMyTrackerInfo *inTracker)
{
	SMyServerInfo *pTrackerInfo = (SMyServerInfo *)UMemory::NewClear(sizeof(SMyServerInfo));
	
	if (gApp->GiveIconSet() == 2){
		pTrackerInfo->Icon = UIcon::Load(3033);
		}else if (gApp->GiveIconSet() == 1){
		pTrackerInfo->Icon = UIcon::Load(232);
		}
		
	
	pTrackerInfo->Name = (Uint8 *)UMemory::New(inTracker->Name[0] + 1);
	pTrackerInfo->Desc = (Uint8 *)UMemory::New(inTracker->Address[0] + 1);
	pTrackerInfo->SearchName = (Uint8 *)UMemory::New(inTracker->Name[0] + 1);
	pTrackerInfo->SearchDesc = (Uint8 *)UMemory::New(inTracker->Address[0] + 1);
		
	UMemory::Copy(pTrackerInfo->Name, inTracker->Name, inTracker->Name[0] + 1);
	UMemory::Copy(pTrackerInfo->Desc, inTracker->Address, inTracker->Address[0] + 1);
	UMemory::Copy(pTrackerInfo->SearchName, inTracker->Name, inTracker->Name[0] + 1);
	UMemory::Copy(pTrackerInfo->SearchDesc, inTracker->Address, inTracker->Address[0] + 1);
	pTrackerInfo->Flags = inTracker->nTrackerID;

	mServerTree.AddItem(0, pTrackerInfo);
	mServerTreeView->AddTracker(pTrackerInfo);
}

// returns the number of servers in the list, not how many it added
Uint32 CMyTracker::AddListFromData(Uint16 inTrackerID, const Uint8 *inData, Uint32 inDataSize)
{
	Uint8 *p = (Uint8 *)inData;
	Uint8 *ep = p + inDataSize;
	Uint32 addr;
	Uint16 port, userCount;
	Uint8 *name, *desc;
	Uint32 startCount, addCount, nonDupeCount;
	
	startCount = mServerTree.GetTreeCount();
	addCount = nonDupeCount = 0;
	
	SMyTrackerInfo *pTrackerInfo = GetTrackerInfoByID(inTrackerID);
	if (!pTrackerInfo) return 0;

	Uint32 nParentIndex = GetTrackerIndex(inTrackerID);
	if (!nParentIndex) return 0;
	
	Uint32 nParentIndexView = mServerTreeView->GetTrackerIndex(inTrackerID);
	if (!nParentIndexView) return 0;
		
	while (p < ep)
	{
		if (ep - p < 12) break;
		addr = *((Uint32 *)p)++;
		port = FB( *((Uint16 *)p)++ );
		userCount = FB( *((Uint16 *)p)++ );
		*((Uint16 *)p)++;
		name = p;
		p += *p + 1;
		if (p >= ep) break;
		desc = p;
		p += *p + 1;
		if (p > ep) break;

//		if (!IsInList(addr, port))
		AddServerInTree(nParentIndex, nParentIndexView, name, desc, addr, port, userCount, 0);
		
		addCount++;
	}
		
	UpdateServersCount(inTrackerID);
	mServerTreeView->RefreshTreeItem(nParentIndexView);
	SetServerCountLabel();
	
	return addCount;
}

void CMyTracker::AddServerInTree(Uint32 inParentIndex, Uint32 inParentIndexView, Uint8 *inName, Uint8 *inDesc, Uint32 inAddress, Uint16 inPort, Uint16 inCount, Uint16 inFlags)
{
	SMyServerInfo *pServerInfo = (SMyServerInfo *)UMemory::NewClear(sizeof(SMyServerInfo));
	
	
	
		
	if (inParentIndex == 1){	// it's a bookmark
		if (gApp->GiveIconSet() == 2){
			pServerInfo->Icon = UIcon::Load(3064);
		}else if (gApp->GiveIconSet() == 1){
			pServerInfo->Icon = UIcon::Load(408);
		}
		
	}else{
	if (gApp->GiveIconSet() == 2){
			pServerInfo->Icon = UIcon::Load(3020);
		}else if (gApp->GiveIconSet() == 1){
			pServerInfo->Icon = UIcon::Load(233);
		}
		
	}
	
	pServerInfo->Name = (Uint8 *)UMemory::New(inName[0] + 1);
	pServerInfo->Desc = (Uint8 *)UMemory::New(inDesc[0] + 1);
	pServerInfo->SearchName = (Uint8 *)UMemory::New(inName[0] + 1);
	pServerInfo->SearchDesc = (Uint8 *)UMemory::New(inDesc[0] + 1);
	
	pstrcpy(pServerInfo->Name, inName);
	pstrcpy(pServerInfo->Desc, inDesc);
	
	Uint8 bufName[256] = "\p";
	Uint8 bufDesc[256] = "\p";
		    	
	pstrcpy(bufName,inName);
	pstrcpy(bufDesc,inDesc);
	UText::MakeLowercase(bufName+1, bufName[0]);
	UText::MakeLowercase(bufDesc+1, bufDesc[0]);

	pstrcpy(pServerInfo->SearchName, bufName);
	pstrcpy(pServerInfo->SearchDesc, bufDesc);
	
	pServerInfo->Address = inAddress;
	pServerInfo->Port = inPort;
	pServerInfo->uCount = inCount;
	pServerInfo->Flags = inFlags;
	
	pServerInfo->User[0] = UText::IntegerToText(pServerInfo->User + 1, sizeof(pServerInfo->User) - 1, inCount);
	
	mServerTree.AddItem(inParentIndex, pServerInfo);
	
	Uint8 FilterText[256] = "\p";
	mServerWin->GetFilterText(FilterText);
	
	if (FilterText[0])
	{
		if (FilterServer(FilterText, pServerInfo))
			mServerTreeView->AddServer(inParentIndexView, pServerInfo);
	}
	else
		mServerTreeView->AddServer(inParentIndexView, pServerInfo);
}

void CMyTracker::UpdateServersCount(Uint32 inTrackerID)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackerInfo;
	
	while (mServerTree.GetNext(pTrackerInfo, i, true))
	{
		if (pTrackerInfo->Flags == inTrackerID)
		{
			pTrackerInfo->Address = mServerTree.GetChildTreeCount(i);
			if (!pTrackerInfo->Address)
				pTrackerInfo->User[0] = UMemory::Copy(pTrackerInfo->User + 1, "0/0", 3);

			break;
		}
	}
}

void CMyTracker::UpdateTrackerInList(Uint16 inTrackerID, Uint8 *Name, Uint8 *Comments, Uint8 *inAddress, Uint8 *Login, Uint8 *Pass, short isAccount)
{
	SMyTrackerInfo *pTrackerInfo = GetTrackerInfoByID(inTrackerID);
	if (!pTrackerInfo)
		return;

	pstrcpy(pTrackerInfo->Name, Name);
	pstrcpy(pTrackerInfo->Comments, Comments);
	pstrcpy(pTrackerInfo->Address, inAddress);
	pstrcpy(pTrackerInfo->Login, Login);
	pstrcpy(pTrackerInfo->Pass, Pass);
	pTrackerInfo->hasAccount = isAccount;
	
	UpdateTrackerInTree(pTrackerInfo);
}

void CMyTracker::UpdateTrackerInTree(SMyTrackerInfo *inTracker)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackerInfo;
	
	while (mServerTree.GetNext(pTrackerInfo, i, true))
	{
		if (pTrackerInfo->Flags == inTracker->nTrackerID)
		{
			pTrackerInfo->Name = (Uint8 *)UMemory::Reallocate((TPtr)pTrackerInfo->Name, inTracker->Name[0] + 1);
			pTrackerInfo->Desc = (Uint8 *)UMemory::Reallocate((TPtr)pTrackerInfo->Desc, inTracker->Address[0] + 1);
			pTrackerInfo->SearchName = (Uint8 *)UMemory::Reallocate((TPtr)pTrackerInfo->SearchName, inTracker->Name[0] + 1);
			pTrackerInfo->SearchDesc = (Uint8 *)UMemory::Reallocate((TPtr)pTrackerInfo->SearchDesc, inTracker->Address[0] + 1);
		
			UMemory::Copy(pTrackerInfo->Name, inTracker->Name, inTracker->Name[0] + 1);
			UMemory::Copy(pTrackerInfo->Desc, inTracker->Address, inTracker->Address[0] + 1);
			UMemory::Copy(pTrackerInfo->SearchName, inTracker->Name, inTracker->Name[0] + 1);
			UMemory::Copy(pTrackerInfo->SearchDesc, inTracker->Address, inTracker->Address[0] + 1);
			
			mServerTreeView->RefreshTracker(inTracker->nTrackerID);
			break;
		}
	}
}

bool CMyTracker::EditSelectedTracker()
{
	Uint32 nTrackerID = mServerTreeView->GetSelectedTrackerID();
	if (!nTrackerID)
		return false;
	
	SMyTrackerInfo *pTrackerInfo = GetTrackerInfoByID(nTrackerID);
	if (!pTrackerInfo)
		return true;
					
	CMyTrackServWindow *pWindow = new CMyTrackServWindow(gApp->mAuxParentWin);
	pWindow->SetTitle(pTrackerInfo->Name);
				
	pWindow->SetName(pTrackerInfo->Name);
	pWindow->SetAddr(pTrackerInfo->Address);
	pWindow->SetPass(pTrackerInfo->Pass);
	pWindow->SetLogin(pTrackerInfo->Login);
				
	if (pTrackerInfo->hasAccount)
		pWindow->AccntHit();
				
	pWindow->Show();
	
	return true;
}

void CMyTracker::RemoveTrackerInList(Uint16 inTrackerID)
{
	Uint32 i = 0;
	SMyTrackerInfo *pTrackerInfo;
    
   	while (mTrackerList.GetNext(pTrackerInfo, i))
   	{
   		if (pTrackerInfo->nTrackerID == inTrackerID)
   		{
   			mTrackerList.RemoveItem(pTrackerInfo);
   			UMemory::Dispose((TPtr)pTrackerInfo);
   			
   			return;
   		}
   	}
}

void CMyTracker::RemoveTrackersInTree()
{
	Uint32 i = 0;
	SMyServerInfo *pServerInfo;
		
	while (mServerTree.GetNext(pServerInfo, i, true))
	{
		if (!pServerInfo->Flags)	// Bookmarks
			continue;
		
		SMyTrackerInfo **infoPtr = mTrackerList.GetArrayPtr();
		Uint32 nCount = mTrackerList.GetItemCount();
	
		bool bRemoveTracker = true;
		while (nCount--)
		{
			SMyTrackerInfo *pTrackerInfo = *infoPtr;	
			
			if (pTrackerInfo->nTrackerID == pServerInfo->Flags &&
			    pServerInfo->Name[0] == pTrackerInfo->Name[0] && !UMemory::Compare(pServerInfo->Name + 1, pTrackerInfo->Name + 1, pTrackerInfo->Name[0]) &&
				pServerInfo->Desc[0] == pTrackerInfo->Address[0] && !UMemory::Compare(pServerInfo->Desc + 1, pTrackerInfo->Address + 1, pTrackerInfo->Address[0]))
			{	
				bRemoveTracker = false;
				break;
			}
			
			infoPtr++;
		}
		
		if (bRemoveTracker)
		{
			RemoveTrackerInTree(pServerInfo->Flags, true);
			i = 0;
		}
	}
	
	SetServerCountLabel();
}

void CMyTracker::RemoveTrackerInTree(Uint16 inTrackerID, bool inTracker)
{
 	mServerTreeView->RemoveTracker(inTrackerID, inTracker);

	Uint32 i = 0;
	SMyServerInfo *pTrackerInfo = nil;
		
	while (mServerTree.GetNext(pTrackerInfo, i, true))
	{
		if (pTrackerInfo->Flags == inTrackerID)
		{
		 	if (inTracker)
		 	{
 				UIcon::Release(pTrackerInfo->Icon);
		 		UMemory::Dispose((TPtr)pTrackerInfo->Name);
				UMemory::Dispose((TPtr)pTrackerInfo->Desc);
				UMemory::Dispose((TPtr)pTrackerInfo->SearchName);
				UMemory::Dispose((TPtr)pTrackerInfo->SearchDesc);
	
				UMemory::Dispose((TPtr)pTrackerInfo);
		 	}
		 	else
		 		pTrackerInfo->Address = 0;
		 	
			Uint32 nTrackerIndex = i;
		 	if (mServerTree.GetChildTreeCount(i))
		 	{
				pTrackerInfo = mServerTree.GetItem(++i);
			
				do
				{
					UIcon::Release(pTrackerInfo->Icon);
					UMemory::Dispose((TPtr)pTrackerInfo->Name);
					UMemory::Dispose((TPtr)pTrackerInfo->Desc);
					UMemory::Dispose((TPtr)pTrackerInfo->SearchName);
					UMemory::Dispose((TPtr)pTrackerInfo->SearchDesc);
	
					UMemory::Dispose((TPtr)pTrackerInfo);
			
				} while (mServerTree.GetNext(pTrackerInfo, i, true));
			}
						
			if (inTracker)
				mServerTree.RemoveItem(nTrackerIndex);
			else
				mServerTree.RemoveChildTree(nTrackerIndex);
		
			SetServerCountLabel();
			return;
		}
	}
}

void CMyTracker::RemoveSelectedItem()
{
	// delete tracker
	Uint32 nTrackerID = mServerTreeView->GetSelectedTrackerID();
	if (nTrackerID)
	{
		RemoveTrackerInList(nTrackerID);
		RemoveTrackerInTree(nTrackerID, true);
		SetServerCountLabel();
		return;
	}
	
	// delete bookmark
	const Uint8 *pBookmarkName = mServerTreeView->GetSelectedBookmarkName();
	if (pBookmarkName)
	{
		Uint8 psBookmarkName[256];
		UMemory::Copy(psBookmarkName, pBookmarkName, pBookmarkName[0] + 1);
			
		// add the ".hbm" to the file name
	#if WIN32
		pstrcat(psBookmarkName, "\p.hbm");
	#endif

		StFileSysRef pBookmarkFile(kProgramFolder, "\pBookmarks", psBookmarkName, fsOption_RequireExistingFile);
		if (pBookmarkFile.IsValid())
		{
			pBookmarkFile->MoveToTrash();
			RefreshBookmarks();
		}
	}
}

void CMyTracker::SetStatusMsg(Uint16 inTrackerID, const Uint8 inMsg[])
{		
	mServerTreeView->SetStatusMsg(inTrackerID, inMsg);
}

void CMyTracker::SetServerCountLabel()
{
	Uint32 nTotalCount = mServerTree.GetTreeCount() - mServerTree.GetRootCount();
    Uint32 nListCount = mServerTreeView->GetTotalServerCount() - mServerTreeView->GetRootCount();
    
    Uint8 Status[256] = "\p";
    Uint8 LCText[256] = "\p";
    Uint8 TCText[256] = "\p";
    LCText[0] = UText::IntegerToText(LCText+1, 255, nListCount);
    TCText[0] = UText::IntegerToText(TCText+1, 255, nTotalCount);
    Status[0] = UText::Format(Status+1, 255, "%#s\/%#s", LCText, TCText);
    
    mServerWin->SetServerNum(Status);
}

bool CMyTracker::IsInList(Uint32 inAddress, Uint16 inPort)
{
	Uint32 i = 0;
	SMyServerInfo *pServerInfo = nil;
		
	while (mServerTree.GetNext(pServerInfo, i))
	{
		if (mServerTree.GetItemLevel(i) > 1 && pServerInfo->Address == inAddress && pServerInfo->Port == inPort)
			return true;		
	}
	
	return false;
}

void CMyTracker::FilterServerList()
{
	Uint8 FilterText[256];
	
	mServerWin->GetFilterText(FilterText);
	FilterServerList(FilterText);
}

short CMyTracker::FilterServer(Uint8 *inText, SMyServerInfo *inServer)
{
	if (!(*inText)) 
		return true;

    Uint8 bufSearch[256] = "\p";
    
    pstrcpy(bufSearch, inText);
    UText::MakeLowercase(bufSearch+1, bufSearch[0]);
    
    SWordPtr wrds[25];
    SWordPtr *wrd = wrds;
    SWordPtr *lastWrd = wrds + sizeof(wrds) / sizeof(SWordPtr);
    Uint8 *p = bufSearch + 1;
    Uint8 *q = bufSearch + 1 + bufSearch[0];
    
    while (wrd != lastWrd && p != q)
    {
    	if (*p == '-')
    	{
    		*p++;
    		wrd->negative = 1;
    	}
    	else
    		wrd->negative = 0;
    		
    	wrd->startWord = p;
    	Uint8 *spc = UMemory::SearchByte(' ', p, q - p);
    	
    	if (spc)
    	{
    		wrd->wordLen = spc - p;
    		wrd++;
    		
    		// skip over multi-spaces if there are any
    		while (spc != q && *spc == ' ')
    			spc++;
    		
    		p = spc;  	
    	}
    	else
    	{
    		wrd->wordLen = q - p;
    		wrd++;
    		break;
    	}
    }
    
    wrd->startWord = nil;	// so we know where we ended
    
	wrd = wrds;
    while (wrd->startWord)
    {
	    if ((UMemory::Search(wrd->startWord, wrd->wordLen, inServer->SearchName+1, inServer->SearchName[0]) ||
    		 UMemory::Search(wrd->startWord, wrd->wordLen, inServer->SearchDesc+1, inServer->SearchDesc[0]) ) == wrd->negative) // Contains it
    			goto dontAddServer;
    	wrd++;
	}
    	
    return true;
    
dontAddServer:	
	return false;
}

void CMyTracker::FilterServerList(Uint8 *inText)
{	
	Uint32 nParentIndex = 0;
	SMyServerInfo *pServerInfo = nil;
	
	mServerTreeView->RemoveServers();
	Uint32 nCount = mServerTree.GetTreeCount();

	if (*inText)
	{
		if (inText[0] == 1)
		{
			if (inText[1] == '-')
				goto displayAllServs;
				
		    Uint8 bufSearch[256] = "\p";
		    
		    pstrcpy(bufSearch, inText);
		    UText::MakeLowercase(bufSearch+1, bufSearch[0]);

		    for (Uint32 i=1; i <= nCount; i++)
		    {
		    	pServerInfo = mServerTree.GetItem(i);
		    	if (!pServerInfo)
		    		continue;
		    	
		    	if (mServerTree.GetItemLevel(i) == 1)
		    		nParentIndex = mServerTreeView->GetTrackerIndex(pServerInfo->Flags);
			    else if (UMemory::SearchByte(inText[1], pServerInfo->SearchName + 1, pServerInfo->SearchName[0]) ||
		    			 UMemory::SearchByte(inText[1], pServerInfo->SearchDesc+1, pServerInfo->SearchDesc[0]))
					mServerTreeView->AddServer(nParentIndex, pServerInfo);
		    }
		}
		else
		{
		    Uint8 bufSearch[256] = "\p";
		    
		    pstrcpy(bufSearch, inText);
		    UText::MakeLowercase(bufSearch+1, bufSearch[0]);
		    
		    // now I need to build a list of offsets for search text, or separate pstrings?
		    // let's do offsets - I could do a struct with starts and finishes
		    SWordPtr wrds[25];	// no one's gonna enter more than 25 words
		    SWordPtr *wrd = wrds;
		    SWordPtr *lastWrd = wrds + sizeof(wrds) / sizeof(SWordPtr);
		    Uint8 *p = bufSearch + 1;
		    Uint8 *q = bufSearch + 1 + bufSearch[0];
		    
		    while (wrd != lastWrd && p != q)
		    {
		    	if (*p == '-')
		    	{
		    		*p++;
		    		wrd->negative = 1;
		    	}
		    	else
		    		wrd->negative = 0;
		    		
		    	wrd->startWord = p;
		    	Uint8 *spc = UMemory::SearchByte(' ', p, q - p);
		    	
		    	if (spc)
		    	{
		    		wrd->wordLen = spc - p;
		    		wrd++;
		    		
		    		// skip over multi-spaces if there are any
		    		while (spc != q && *spc == ' ')
		    			spc++;
		    		
		    		p = spc;		    	
		    	}
		    	else
		    	{
		    		wrd->wordLen = q - p;
		    		wrd++;
		    		break;
		    	}
		    }
		    
		    wrd->startWord = nil;	// so we know where we ended
		  		    
		    for (Uint32 i=1; i <= nCount; i++)
		    {
		    	pServerInfo = mServerTree.GetItem(i);
		    	if (!pServerInfo)
		    		continue;
		    	
		    	if (mServerTree.GetItemLevel(i) == 1)
		    	{
		    		nParentIndex = mServerTreeView->GetTrackerIndex(pServerInfo->Flags);
		    	}
				else
			    {
				    wrd = wrds;
				    bool bAddServer = true;
				    				    
				    while (wrd->startWord)
				    {
					    if ((UMemory::Search(wrd->startWord, wrd->wordLen, pServerInfo->SearchName+1, pServerInfo->SearchName[0]) ||
			    			 UMemory::Search(wrd->startWord, wrd->wordLen, pServerInfo->SearchDesc+1, pServerInfo->SearchDesc[0]) ) == wrd->negative)
			    			{
			    				bAddServer = false;
			    				break;
			    			}

				    	wrd++;
		    		}
		    		
		    		if (bAddServer)	    	
		    			mServerTreeView->AddServer(nParentIndex, pServerInfo);
		    	}
	    	}
    	}
    }
    else
    {
displayAllServs:	
	    for(Uint32 i=1; i <= nCount; i++)
		{
		   	pServerInfo = mServerTree.GetItem(i);
		   	if (!pServerInfo)
		   		continue;
		    	
		   	if (mServerTree.GetItemLevel(i) == 1)
		   		nParentIndex = mServerTreeView->GetTrackerIndex(pServerInfo->Flags);
		  	else
    			mServerTreeView->AddServer(nParentIndex, pServerInfo);
    	}
    }
    
    SetServerCountLabel();
    mServerTreeView->RefreshTrackers();
}

bool CMyTracker::RefreshBookmarks()
{
	Uint16 nTrackerID = 0;
	RemoveTrackerInTree(nTrackerID, false);
	if (mServerTreeView->GetTrackerDisclosure(nTrackerID) == optTree_Collapsed)
		mServerTreeView->SetTrackerDisclosure(nTrackerID, optTree_Disclosed);

	TFSRefObj* folder = nil;
	
	try
	{
		folder = UFS::New(kProgramFolder, nil, "\pBookmarks", fsOption_PreferExistingFolder);
		if (!folder)
		{
			folder = UFS::New(kProgramFolder, nil, "\pServers", fsOption_PreferExistingFolder);
			
			if (!folder)
			{
				// create a bookmarks folder if none exists
				try
				{
					folder = UFS::New(kProgramFolder, nil, "\pBookmarks");
					if(folder)
					{
						scopekill(TFSRefObj, folder);
						folder->CreateFolder();
					}
				}
				catch(...)	{}
				
				return false;
			}
			
			folder->SetName("\pBookmarks");
		}
	}
	catch(...)
	{
		delete folder;
		throw;
	}
	
	scopekill(TFSRefObj, folder);
	
	THdl h = folder->GetListing();
	if (h == nil) 
	{
		UpdateServersCount(nTrackerID);
		return false;
	}
	
	Uint32 nParentIndex = GetTrackerIndex(nTrackerID);
	if (!nParentIndex) return false;
	
	Uint32 nParentIndexView = mServerTreeView->GetTrackerIndex(nTrackerID);
	if (!nParentIndexView) return false;
	
	try
	{
		Uint8 name[256];
		Uint32 typeCode, creatorCode, flags;
		Uint32 offset = 0;
	
		while (UFS::GetListNext(h, offset, name, &typeCode, &creatorCode, nil, nil, nil, &flags))
		{
			// loose the ".hbm" from the file name
		#if WIN32
			if (name[0] > 4)
			{
				Uint8 *p = (name + name[0]) - 3;
				if (p[0] == '.' && UText::tolower(p[1]) == 'h' && UText::tolower(p[2]) == 'b' && UText::tolower(p[3]) == 'm')
					name[0] -= 4;
			}
		#endif
		
			if (typeCode == TB((Uint32)'HTbm') && creatorCode == TB((Uint32)'HTLC') && (flags & 1) == 0)	// if visible bookmark file
				AddServerInTree(nParentIndex, nParentIndexView, name, name, 0, 0, 0, 0);
		}
	}
	catch(...)
	{
		UMemory::Dispose(h);
		throw;
	}
	
	UMemory::Dispose(h);
	
	UpdateServersCount(nTrackerID);
	SetServerCountLabel();
	
	return true;
}

void CMyTracker::RefreshTrackers()
{
	RemoveTrackersInTree();
	RefreshBookmarks();
	
	SMyTrackerInfo **infoPtr = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	while (nCount--)
	{
		SMyTrackerInfo *pTrackerInfo = *infoPtr;
				
		bool bRefreshTracker = false;
		Uint32 nTrackerIndex = GetTrackerIndex(pTrackerInfo->nTrackerID);
						
		if (nTrackerIndex)
		{
			if (!gApp->SearchTrackServTask(pTrackerInfo->nTrackerID))
			{
				bRefreshTracker = true;
				
				RemoveTrackerInTree(pTrackerInfo->nTrackerID, false);
				if (mServerTreeView->GetTrackerDisclosure(pTrackerInfo->nTrackerID) == optTree_Collapsed)
					mServerTreeView->SetTrackerDisclosure(pTrackerInfo->nTrackerID, optTree_Disclosed);
			}
		}
		else
		{
			bRefreshTracker = true;
				
			AddTrackerInTree(pTrackerInfo);
			mServerTreeView->SetTrackerDisclosure(pTrackerInfo->nTrackerID, optTree_Disclosed);
		}

		if (bRefreshTracker)
		{
			if (pTrackerInfo->hasAccount)
				new CMyGetTrackServListTask(pTrackerInfo->Address, pTrackerInfo->Name, pTrackerInfo->nTrackerID, pTrackerInfo->Login, pTrackerInfo->Pass);
			else
				new CMyGetTrackServListTask(pTrackerInfo->Address, pTrackerInfo->Name, pTrackerInfo->nTrackerID, "\p", "\p");
		
			SetStatusMsg(pTrackerInfo->nTrackerID, "\pConnecting...");
		}
		
		infoPtr++;
	}
	
	SetServerCountLabel();
}

void CMyTracker::RefreshTrackers(Uint16 inMods)
{
	if (inMods & modKey_Option)
	{
		RefreshTrackers();
		return;
	}

	bool bIsBookmark;
	Uint32 nTrackerID = mServerTreeView->GetSelectedTrackerID(&bIsBookmark);
	if (!nTrackerID && !bIsBookmark)
	{
		RefreshTrackers();
		return;
	}

	RemoveTrackersInTree();
	SetServerCountLabel();

	if (bIsBookmark)
	{
		RefreshBookmarks();
		return;
	}
	
	if (gApp->SearchTrackServTask(nTrackerID))
		return;
		
	RemoveTrackerInTree(nTrackerID, false);
	if (mServerTreeView->GetTrackerDisclosure(nTrackerID) == optTree_Collapsed)
		mServerTreeView->SetTrackerDisclosure(nTrackerID, optTree_Disclosed);

	SMyTrackerInfo **infoPtr = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	while (nCount--)
	{
		SMyTrackerInfo *pTrackerInfo = *infoPtr;
		
		if (pTrackerInfo->nTrackerID == nTrackerID)
		{
			if (pTrackerInfo->hasAccount)
				new CMyGetTrackServListTask(pTrackerInfo->Address, pTrackerInfo->Name, pTrackerInfo->nTrackerID, pTrackerInfo->Login, pTrackerInfo->Pass);
			else
				new CMyGetTrackServListTask(pTrackerInfo->Address, pTrackerInfo->Name, pTrackerInfo->nTrackerID, "\p", "\p");
		
			SetStatusMsg(nTrackerID, "\pConnecting...");
			break;
		}
	
		infoPtr++;
	}
	
	SetServerCountLabel();
}

void CMyTracker::RefreshTracker(Uint16 inTrackerID)
{
	if (!inTrackerID)
	{
		RefreshBookmarks();
		return;
	}
	
	if (gApp->SearchTrackServTask(inTrackerID))
		return;
	
	SMyTrackerInfo **infoPtr = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	while (nCount--)
	{
		SMyTrackerInfo *pTrackerInfo = *infoPtr;
		
		if (pTrackerInfo->nTrackerID == inTrackerID)
		{
			Uint32 nTrackerIndex = GetTrackerIndex(pTrackerInfo->nTrackerID);
			if (mServerTree.GetChildTreeCount(nTrackerIndex))
				return;
				
			if (pTrackerInfo->hasAccount)
				new CMyGetTrackServListTask(pTrackerInfo->Address, pTrackerInfo->Name, pTrackerInfo->nTrackerID, pTrackerInfo->Login, pTrackerInfo->Pass);
			else
				new CMyGetTrackServListTask(pTrackerInfo->Address, pTrackerInfo->Name, pTrackerInfo->nTrackerID, "\p", "\p");
			
			SetStatusMsg(pTrackerInfo->nTrackerID, "\pConnecting...");
			break;
		}
		
		infoPtr++;
	}
	
	SetServerCountLabel();
}

void CMyTracker::SetDefaultTracker()
{
	// this gets called on corrupt (or non-existent) prefs file only add if there are no other items in the list.
	if (!mTrackerList.GetItemCount() && gApp->mOptions.stTrackerList.GetItemCount())
	{
		Uint32 i = 0;
		SMyDefTrackerInfo *pTrackerInfo;
		
		while (gApp->mOptions.stTrackerList.GetNext(pTrackerInfo, i))
			AddTrackerInList(pTrackerInfo->psName, "\p", pTrackerInfo->psAddr, "\p", "\p", false, mLastTrackerID++);
	}
}

void CMyTracker::ExpandDefaultTracker()
{
	//mServerTreeView->ExpandAllTrackers();
}

bool CMyTracker::GetSelectedServInfo(Uint8 *outName, Uint8 *outAddress, Uint8 *outLogin, Uint8 *outPassword, bool *outUseCrypt)
{ 
	if (outName) outName[0] = 0;
	if (outAddress) outAddress[0] = 0;
	if (outLogin) outLogin[0] = 0;
	if (outPassword) outPassword[0] = 0;
	if (outUseCrypt) *outUseCrypt=false;

	bool bBookmark;
	if (!mServerTreeView->GetSelectedServInfo(bBookmark, outName, outAddress))
		return false;
	
	if (bBookmark)
	{
		// add the ".hbm" to the file name
	#if WIN32
		pstrcat(outName, "\p.hbm");
	#endif

		StFileSysRef pBookmarkFile(kProgramFolder, "\pBookmarks", outName, fsOption_RequireExistingFile);
		if (pBookmarkFile.IsValid())
			gApp->ReadServerFile(pBookmarkFile, outAddress, outLogin, outPassword, outUseCrypt); //////////

		// loose the ".hbm" from the file name
	#if WIN32
		outName[0] -= 4;
	#endif
	}
		
	return true;
}

Uint32 CMyTracker::GetTrackerIndex(Uint16 inTrackerID)
{
	Uint32 i = 0;
	SMyServerInfo *pTrackInfo;
	
	while (mServerTree.GetNext(pTrackInfo, i, true))
	{
		if (pTrackInfo->Flags == inTrackerID)
			return i;	
	}

	return 0;
}

SMyTrackerInfo *CMyTracker::GetTrackerInfo(Uint32 inTrakerIndex)
{
	SMyTrackerInfo **array = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	if (inTrakerIndex >= 0 && inTrakerIndex < nCount)
		return array[inTrakerIndex];
	
	return nil;
}

SMyTrackerInfo *CMyTracker::GetTrackerInfoByID(Uint16 inTrackerID)
{
	SMyTrackerInfo **array = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	for (Uint32 i = 0; i < nCount; i++)
		if (array[i]->nTrackerID == inTrackerID)
			return array[i];
		
	return nil;
}

Uint32 CMyTracker::WritePrefs(TFSRefObj* inFile, Uint32 inOffset)
{
	SMyTrackerInfo **array = mTrackerList.GetArrayPtr();
	Uint32 nCount = mTrackerList.GetItemCount();
	
	Uint32 nTotalSize = sizeof(Uint32) * 3 + sizeof(SRect)/2 + sizeof(Uint8) * 2 + sizeof(Uint32) * 2;
	for (Uint32 i = 0; i < nCount; i++)
	{
		nTotalSize += array[i]->Login[0] + 1;
		nTotalSize += array[i]->Pass[0] + 1;
		nTotalSize += array[i]->Address[0] + 1;
		nTotalSize += array[i]->Name[0] + 1;
		nTotalSize += array[i]->Comments[0] + 1;
		nTotalSize += sizeof(Uint16) * 2;
	}
		
	StPtr buf(nTotalSize);
	CFlatten Flat(buf);

	Uint32 nVerNumber = 3; // tracker list version number
	Flat.WriteLong(nVerNumber);
	Flat.WriteLong(gApp->GiveIconSet()); //write iconSet
	//Flat.ReserveLong();
	Flat.ReserveLong();

	SRect stServerWinBounds;
	mServerWin->GetBounds(stServerWinBounds);
	Flat.WriteShortRect(stServerWinBounds);
	
	Flat.ReserveByte();
	Flat.WriteByte(mServerWin->IsVisible());
	
	Flat.WriteLong(nTotalSize);
	Flat.WriteLong(nCount);
	
	for (Uint32 i = 0; i < nCount; i++)
	{
		Flat.WritePString(array[i]->Login);
		Flat.WritePString(array[i]->Pass);
		Flat.WritePString(array[i]->Address);
		Flat.WritePString(array[i]->Name);
		Flat.WritePString(array[i]->Comments);
		Flat.WriteWord(array[i]->hasAccount);
		Flat.ReserveWord();
	}
	//Flat.WriteLong(gApp->GiveIconSet());
//DebugBreak("%i",gApp->GiveIconSet());
	nTotalSize = Flat.GetSize();
	inFile->Write(inOffset, buf, nTotalSize);
	
	return nTotalSize;
}

Uint32 CMyTracker::ReadPrefs(TFSRefObj* inFile, Uint32 inOffset, Uint16 inTabWidth1, Uint16 inTabWidth2, bool iconset)
{
if (iconset){
	
	//DebugBreak("juste pour liconset");
	//SMyTrackerInfo *pTrackerInfo2 = nil;
	Uint32 nSize = inFile->GetSize() - inOffset;
	Uint32 nTotalSize = sizeof(Uint32) * 3 + sizeof(SRect)/2 + sizeof(Uint8) * 2 + sizeof(Uint32) * 2;
	
	StPtr buf(nSize);
	nSize = inFile->Read(inOffset, buf, nSize);

	CUnflatten unflat(buf, nSize);
	if (unflat.NotEnufData(nTotalSize))
		DebugBreak("je c pa");
		
	unflat.SkipLong();
	gApp->SetIconSet(unflat.ReadLong());
	
}else{
	SMyTrackerInfo *pTrackerInfo = nil;
	Uint32 nSize = inFile->GetSize() - inOffset;
	Uint32 nTotalSize = sizeof(Uint32) * 3 + sizeof(SRect)/2 + sizeof(Uint8) * 2 + sizeof(Uint32) * 2;
	
	StPtr buf(nSize);
	nSize = inFile->Read(inOffset, buf, nSize);

	CUnflatten unflat(buf, nSize);
	if (unflat.NotEnufData(nTotalSize))
		goto corrupt;
		
	Uint32 nVerNumber = unflat.ReadLong();
	if (nVerNumber != 3)	// tracker list version number
		goto corrupt;
	
	unflat.SkipLong();
	unflat.SkipLong();

	SRect stServerWinBounds;
	unflat.ReadShortRect(stServerWinBounds);
	mServerWin->SetBounds(stServerWinBounds);
	mServerWin->SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	mServerWin->GetServerTreeView()->SetTabs(inTabWidth1, inTabWidth2);

	unflat.SkipByte();
	mServerWin->SetVisible(unflat.ReadByte());
	
	unflat.SkipLong();
	Uint32 nCount = unflat.ReadLong();
	
	for (Uint32 i = 0; i < nCount; i++)
	{	
		pTrackerInfo = (SMyTrackerInfo *)UMemory::NewClear(sizeof(SMyTrackerInfo));
		
		if (!unflat.ReadPString(pTrackerInfo->Login, 255)) goto corrupt;
		if (!unflat.ReadPString(pTrackerInfo->Pass, 255)) goto corrupt;
		if (!unflat.ReadPString(pTrackerInfo->Address, 255)) goto corrupt;
		if (!unflat.ReadPString(pTrackerInfo->Name, 255)) goto corrupt;
		if (!unflat.ReadPString(pTrackerInfo->Comments, 255)) goto corrupt;
		
		
		pTrackerInfo->hasAccount = unflat.ReadWord();
		unflat.SkipWord();
		//Uint32 nCount = ;
		//DebugBreak("On lit giveiconset avant de lire pref: %i",gApp->GiveIconSet());
		//gApp->SetIconSet(unflat.ReadLong());
		
		
		pTrackerInfo->nTrackerID = mLastTrackerID++;
		
		nTotalSize += pTrackerInfo->Login[0] + pTrackerInfo->Pass[0] + pTrackerInfo->Address[0] + pTrackerInfo->Name[0] + pTrackerInfo->Comments[0] + 5;
		nTotalSize += sizeof(Uint16) * 2;
		
		mTrackerList.AddItem(pTrackerInfo);
		pTrackerInfo = nil;
	}

	AddTrackersInTree();
	return nTotalSize;
	
corrupt:
	if (pTrackerInfo)
		delete pTrackerInfo;	// don't want any mem leaks

	SetDefaultTracker();
	}
	return 0;
}


