/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "NewsSynch.h"
#include "CNewsDatabase.h"

extern CMyApplication *gApp;

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif


#pragma mark CMySearchText
CMySearchText::CMySearchText()
{
	mSearchTimer1 = nil;
	mSearchTimer2 = nil;
	
	searchText = nil;	
}

CMySearchText::~CMySearchText()
{
	UTimer::Dispose(mSearchTimer1);
	UTimer::Dispose(mSearchTimer2);
}

void CMySearchText::MakeSearchText(CContainerView *inContainerView, const SRect& inBounds)
{
	// make search box
	CBoxView *box = new CBoxView(inContainerView, inBounds, boxStyle_Etched);
	box->SetSizing(sizing_HorizSticky);
	box->Show();
		
	// make search label
	CLabelView *lbl = new CLabelView(inContainerView, SRect(inBounds.left + 4, inBounds.top + 4, inBounds.right - 4, inBounds.bottom - 4));
	lbl->SetFont(kDefaultFont, nil, 9, fontEffect_Bold);
	lbl->SetSizing(sizing_HorizSticky);
	lbl->Show();
	searchText = lbl;
}

bool CMySearchText::SearchText_KeyDown(const SKeyMsgData& inInfo)
{
	if (UText::IsPrinting(inInfo.keyChar))
	{
		Uint8 c = inInfo.keyChar;
		searchText->AppendText(&c, sizeof(c));
		
		if (!mSearchTimer1)
			mSearchTimer1 = UTimer::New(SearchTimer1, this);
		
		mSearchTimer1->Start(2000, kOnceTimer);
		
		if (!mSearchTimer2)
			mSearchTimer2 = UTimer::New(SearchTimer2, this);

		if (!mSearchTimer2->WasStarted())
			mSearchTimer2->Start(1000, kRepeatingTimer);
		
		return true;
	}
	
	if (inInfo.keyChar == '\r' && searchText->GetTextSize())
	{
		mSearchTimer1->Stop();
		SearchTimer1(this, nil, msg_Timer, nil, 0);
		
		return true;
	}
	
	return false;
}

void CMySearchText::SearchTimer1(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	#pragma unused(inObject, inData, inDataSize)
	
	if (inMsg == msg_Timer)
	{
		CMySearchText *pSearchText = (CMySearchText *)inContext;
		
		pSearchText->mSearchTimer2->Stop();
		
		Uint8 psText[256];
		psText[0] = pSearchText->searchText->GetText(psText + 1, sizeof(psText) - 1);
		if (psText[0])
		{
			pSearchText->searchText->SetText(nil, 0);
			pSearchText->SearchText(psText);
		}
	}
}

void CMySearchText::SearchTimer2(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	#pragma unused(inObject, inData, inDataSize)
	
	if (inMsg == msg_Timer)
	{
		CMySearchText *pSearchText = (CMySearchText *)inContext;
		
		Uint8 psText[256];
		psText[0] = pSearchText->searchText->GetText(psText + 1, sizeof(psText) - 1);
		if (psText[0])
			pSearchText->SearchText(psText);
	}
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark CMySynchWin

CMySynchWin::CMySynchWin()
	: CWindow(SRect(0,0,606,450), windowLayer_Standard,  windowOption_CloseBox + windowOption_Sizeable)
{
#if WIN32
	_SetWinIcon(*this, 400);
#endif

	// setup window
	SetTitle(APPLICATION_NAME);
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	SetLimits(590,430);

	// make container view for content
	CContainerView *vc = new CContainerView(this, SRect(0,0,606,450));
	vc->Show();

	// make box
	CBoxView *box = new CBoxView(vc, SRect(0,0,606,60), boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();

	CLabelView *lbl = new CLabelView(vc, SRect(36,8,84,24));
	lbl->SetText("\pServer:");
	lbl->Show();

	lbl = new CLabelView(vc, SRect(36,34,84,50));
	lbl->SetText("\pLogin:");
	lbl->Show();

	lbl = new CLabelView(vc, SRect(123,34,159,50));
	lbl->SetSizing(sizing_HorizSticky);
	lbl->SetText("\pPass:");
	lbl->Show();

	lbl = new CLabelView(vc, SRect(375,8,427,24));
	lbl->SetSizing(sizing_HorizSticky);
	lbl->SetText("\pServer:");
	lbl->Show();

	box = new CBoxView(vc, SRect(427,5,572,27), boxStyle_Sunken);
	box->SetSizing(sizing_HorizSticky);
	box->Show();

	lbl = new CLabelView(vc, SRect(431,8,568,24));
#if MACINTOSH
	lbl->SetFont(kDefaultFont, nil, 11, fontEffect_Bold);
#else
	lbl->SetFont(kDefaultFont, nil, 9);
#endif
	lbl->SetSizing(sizing_HorizSticky);
	lbl->Show();
	mViews.pServerName = lbl;

	lbl = new CLabelView(vc, SRect(375,34,427,50));
	lbl->SetSizing(sizing_HorizSticky);
	lbl->SetText("\pBundle:");
	lbl->Show();

	box = new CBoxView(vc, SRect(427,31,572,53), boxStyle_Sunken);
	box->SetSizing(sizing_HorizSticky);
	box->Show();

	lbl = new CLabelView(vc, SRect(431,34,568,50));
#if MACINTOSH
	lbl->SetFont(kDefaultFont, nil, 11, fontEffect_Bold);
#else
	lbl->SetFont(kDefaultFont, nil, 9);
#endif
	lbl->SetSizing(sizing_HorizSticky);
	lbl->Show();
	mViews.pBundleName = lbl;

	// make text box
	CScrollerView *scr = MakeTextBoxView(vc, SRect(84,4,233,28), scrollerOption_Border, &mViews.pServerAddr);
	scr->SetSizing(sizing_RightSticky);
	mViews.pServerAddr->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.pServerAddr->SetID(viewID_ServerAddr1);
	mViews.pServerAddr->SetHasChanged(true);
	scr->Show();

	scr = MakeTextBoxView(vc, SRect(84,30,119,54), scrollerOption_Border, &mViews.pUserLogin);
	scr->SetSizing(sizing_RightSticky);
	mViews.pUserLogin->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.pUserLogin->SetID(viewID_ServerAddr1);
	mViews.pUserLogin->SetHasChanged(true);
	scr->Show();

	scr = MakePasswordBoxView(vc, SRect(159,30,233,54), scrollerOption_Border, &mViews.pUserPassword);
	scr->SetSizing(sizing_HorizSticky);
	mViews.pUserPassword->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.pUserPassword->SetID(viewID_ServerAddr1);
	mViews.pUserPassword->SetHasChanged(true);
	scr->Show();
		
	// make buttons
	CIconButtonView *icb = new CIconButtonView(vc, SRect(237,4,261,28));
	icb->SetIconID(205);
	icb->SetID(viewID_Refresh);
	icb->SetTooltipMsg("\pRefresh");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();

//	icb = new CIconButtonView(vc, SRect(264,4,288,28));
//	icb->SetIconID(223);
//	icb->SetID(viewID_AddSynch);
//	icb->SetTooltipMsg("\pAdd Synch");
//	icb->SetSizing(sizing_HorizSticky);
//	icb->Show();

	icb = new CIconButtonView(vc, SRect(291,4,315,28));
	icb->SetIconID(411);
	icb->SetID(viewID_Synch);
	icb->SetTooltipMsg("\pSynch");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();

	icb = new CIconButtonView(vc, SRect(318,4,342,28));
	icb->SetIconID(419);
	icb->SetID(viewID_Options);
	icb->SetTooltipMsg("\pOptions");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();

	icb = new CIconButtonView(vc, SRect(345,4,369,28));
	icb->SetIconID(412);
	icb->SetID(viewID_StopTask);
	icb->SetTooltipMsg("\pKill Task");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();

	// make select server buttons
	icb = new CIconButtonView(vc, SRect(4,4,28,28));
	icb->SetIconID(232);
	icb->SetID(viewID_SelectServer1);
	icb->SetTooltipMsg("\pSelect News Server");
	icb->Show();

	icb = new CIconButtonView(vc, SRect(578,4,602,28));
	icb->SetIconID(232);
	icb->SetID(viewID_SelectServer2);
	icb->SetTooltipMsg("\pSelect Hotline Server");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();

	icb = new CIconButtonView(vc, SRect(578,30,602,54));
	icb->SetIconID(231);
	icb->SetID(viewID_SelectBundle2);
	icb->SetTooltipMsg("\pSelect Hotline Bundle");
	icb->SetSizing(sizing_HorizSticky);
	icb->Show();

	// make search text
#if MACINTOSH
	MakeSearchText(vc, SRect(237,32,369,52));
#else
	MakeSearchText(vc, SRect(237,31,369,53));
#endif

	// make pane view
	CPaneView *pPaneView1 = new CPaneView(vc, SRect(0,62,606,450));
	pPaneView1->SetSizing(sizing_BottomRightSticky);
	vc->SetFocusView(pPaneView1);
	pPaneView1->Show();

	mViews.pPaneView2 = new CPaneView(pPaneView1, SRect(0,0,606,338));
	mViews.pPaneView2->SetSizing(sizing_BottomRightSticky);
	pPaneView1->SetFocusView(mViews.pPaneView2);
	pPaneView1->SetViewLimits(mViews.pPaneView2, 0, 300);
	mViews.pPaneView2->Show();

	mViews.pPaneView3 = new CPaneView(mViews.pPaneView2, SRect(0,0,606,280));
	mViews.pPaneView3->SetSizing(sizing_BottomRightSticky);
	mViews.pPaneView2->SetFocusView(mViews.pPaneView3);
	mViews.pPaneView2->SetViewLimits(mViews.pPaneView3, 0, 250);
	mViews.pPaneView3->Show();

//------------------------------------------------------------------------
	// make pane view
	mViews.pPaneView4 = new CPaneView(mViews.pPaneView3, SRect(0,0,300,280));
	mViews.pPaneView4->SetSizing(sizing_BottomSticky);
	mViews.pPaneView3->SetFocusView(mViews.pPaneView4);
	mViews.pPaneView3->SetViewLimits(mViews.pPaneView4, 230, 0);
	mViews.pPaneView4->Show();
	
	mViews.pPaneView5 = new CPaneView(mViews.pPaneView4, SRect(0,0,300,189));
	mViews.pPaneView5->SetSizing(sizing_RightSticky);
	mViews.pPaneView4->SetFocusView(mViews.pPaneView5);
	mViews.pPaneView4->SetViewLimits(mViews.pPaneView5, 0, 170);
	mViews.pPaneView5->Show();

	// make group container view
	mViews.pGroupVc1 = new CContainerView(mViews.pPaneView5, SRect(0,0,300,111));
	mViews.pGroupVc1->SetSizing(sizing_RightSticky);
	mViews.pPaneView5->SetFocusView(mViews.pGroupVc1);
	mViews.pPaneView5->SetViewLimits(mViews.pGroupVc1, 0, 70);
	mViews.pGroupVc1->Show();

	// make filter label
	lbl = new CLabelView(mViews.pGroupVc1, SRect(8,8,58,24));
	lbl->SetFont(kDefaultFont, nil, 9);
	lbl->SetText("\pSearch:");
	lbl->Show();
	
	// make filter text box
	scr = MakeTextBoxView(mViews.pGroupVc1, SRect(58,4,210,28), scrollerOption_Border, &mViews.pGroupFilter1);
	scr->SetSizing(sizing_RightSticky);
	mViews.pGroupFilter1->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.pGroupFilter1->SetID(viewID_GroupFilter1);
	scr->Show();

	// make group count
#if MACINTOSH
	mViews.pGroupCount1 = MakeGroupCount(mViews.pGroupVc1, SRect(214,6,299,26));
#else
	mViews.pGroupCount1 = MakeGroupCount(mViews.pGroupVc1, SRect(214,5,299,27));
#endif

	// make group list scroller
	scr = new CScrollerView(mViews.pGroupVc1, SRect(6, 31, 300, 111), scrollerOption_Border | scrollerOption_VertBar | LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();

	// make group list
	mViews.pGroupList1 = new CMyGroupListView1(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()));
	mViews.pGroupList1->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mViews.pGroupList1->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mViews.pGroupList1->SetDragAndDropHandler(this);
	mViews.pGroupList1->SetCanFocus(true);
	mViews.pGroupList1->SetID(viewID_GroupList1);
	mViews.pGroupList1->Show();

	// make article tree scroller
	mViews.pArticleTreeScr1 = new CScrollerView(mViews.pPaneView5, SRect(6, 116, 300, 189), scrollerOption_Border | scrollerOption_VertBar | LIST_BACKGROUND_OPTION);
	mViews.pArticleTreeScr1->SetSizing(sizing_BottomRightSticky);
	mViews.pArticleTreeScr1->SetCanFocus(true);
	mViews.pPaneView5->SetViewLimits(mViews.pArticleTreeScr1, 0, 50);
	mViews.pArticleTreeScr1->Show();

	// make article tree
	mViews.pArticleTree1 = new CMyArticleTreeView1(mViews.pArticleTreeScr1, SRect(0, 0, mViews.pArticleTreeScr1->GetVisibleContentWidth(), mViews.pArticleTreeScr1->GetVisibleContentHeight()));
	mViews.pArticleTree1->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mViews.pArticleTree1->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mViews.pArticleTree1->SetDragAndDropHandler(this);
	mViews.pArticleTree1->SetCanFocus(true);
	mViews.pArticleTree1->SetID(viewID_ArticleList1);
	mViews.pArticleTree1->Show();
		
	// make article container view
	mViews.pArticleVc1 = new CContainerView(mViews.pPaneView4, SRect(6,194,300,280));
	mViews.pArticleVc1->SetSizing(sizing_BottomRightSticky);
	mViews.pPaneView4->SetViewLimits(mViews.pArticleVc1, 0, 50);
	mViews.pArticleVc1->Show();

	// make article sort button
	mViews.pSortArticlesBtn1 = new CButtonView(mViews.pArticleVc1, SRect(182,0,292,16));
	mViews.pSortArticlesBtn1->SetID(viewID_SortArticles1);
	mViews.pSortArticlesBtn1->SetSizing(sizing_HorizSticky);
	mViews.pSortArticlesBtn1->SetTooltipMsg("\pSort Articles");
	mViews.pSortArticlesBtn1->Show();
	
	// make attachment menu
	mViews.pAttachmentButton1 = MakePopupMenuView(mViews.pArticleVc1, SRect(2,0,20,16), &mViews.pAttachmentList1);
	mViews.pAttachmentList1->SetID(viewID_Attachment1);
	mViews.pAttachmentButton1->Disable();
	mViews.pAttachmentButton1->SetTooltipMsg("\pShow Attachments");
#if NEW_NEWS_FORMAT
	mViews.pAttachmentButton1->Show();
#endif

	// make article text
	scr = MakeTextBoxView(mViews.pArticleVc1, SRect(0,16,294,86), scrollerOption_Border, &mViews.pArticleText1);
	scr->SetOptions(scrollerOption_Border | scrollerOption_VertBar);
	scr->SetSizing(sizing_BottomRightSticky);
	mViews.pArticleText1->SetEnterKeyAction(enterKeyAction_None);
	mViews.pArticleText1->SetEditable(false);
	scr->Show();

//------------------------------------------------------------------------
	// make pane view
	mViews.pPaneView6 = new CPaneView(mViews.pPaneView3, SRect(306,0,606,280));
	mViews.pPaneView6->SetSizing(sizing_BottomRightSticky);
	mViews.pPaneView3->SetViewLimits(mViews.pPaneView6, 230, 0);
	mViews.pPaneView6->Show();

	mViews.pPaneView7 = new CPaneView(mViews.pPaneView6, SRect(0,0,300,189));
	mViews.pPaneView7->SetSizing(sizing_RightSticky);
	mViews.pPaneView6->SetFocusView(mViews.pPaneView7);
	mViews.pPaneView6->SetViewLimits(mViews.pPaneView7, 0, 170);
	mViews.pPaneView7->Show();

	// make group container view
	mViews.pGroupVc2 = new CContainerView(mViews.pPaneView7, SRect(0,0,300,111));
	mViews.pGroupVc2->SetSizing(sizing_RightSticky);
	mViews.pPaneView7->SetFocusView(mViews.pGroupVc2);
	mViews.pPaneView7->SetViewLimits(mViews.pGroupVc2, 0, 70);
	mViews.pGroupVc2->Show();

	// make filter label
	lbl = new CLabelView(mViews.pGroupVc2, SRect(2,8,52,24));
	lbl->SetFont(kDefaultFont, nil, 9);
	lbl->SetText("\pSearch:");
	lbl->Show();
	
	// make filter text box
	scr = MakeTextBoxView(mViews.pGroupVc2, SRect(52,4,204,28), scrollerOption_Border, &mViews.pGroupFilter2);
	scr->SetSizing(sizing_RightSticky);
	mViews.pGroupFilter2->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.pGroupFilter2->SetID(viewID_GroupFilter2);
	scr->Show();

	// make group count
#if MACINTOSH
	mViews.pGroupCount2 = MakeGroupCount(mViews.pGroupVc2, SRect(208,6,293,26));
#else
	mViews.pGroupCount2 = MakeGroupCount(mViews.pGroupVc2, SRect(208,5,293,27));
#endif

	// make group list scroller
	scr = new CScrollerView(mViews.pGroupVc2, SRect(0, 31, 294, 111), scrollerOption_Border | scrollerOption_VertBar | LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();

	// make group list
	mViews.pGroupList2 = new CMyGroupListView2(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()));
	mViews.pGroupList2->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mViews.pGroupList2->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mViews.pGroupList2->SetDragAndDropHandler(this);
	mViews.pGroupList2->SetCanFocus(true);
	mViews.pGroupList2->SetID(viewID_GroupList2);
	mViews.pGroupList2->Show();

	// make article tree scroller
	mViews.pArticleTreeScr2 = new CScrollerView(mViews.pPaneView7, SRect(0, 116, 294, 189), scrollerOption_Border | scrollerOption_VertBar | LIST_BACKGROUND_OPTION);
	mViews.pArticleTreeScr2->SetSizing(sizing_BottomRightSticky);
	mViews.pArticleTreeScr2->SetCanFocus(true);
	mViews.pPaneView7->SetViewLimits(mViews.pArticleTreeScr2, 0, 50);
	mViews.pArticleTreeScr2->Show();

	// make article tree
	mViews.pArticleTree2 = new CMyArticleTreeView2(mViews.pArticleTreeScr2, SRect(0, 0, mViews.pArticleTreeScr2->GetVisibleContentWidth(), mViews.pArticleTreeScr2->GetVisibleContentHeight()));
	mViews.pArticleTree2->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mViews.pArticleTree2->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mViews.pArticleTree2->SetDragAndDropHandler(this);
	mViews.pArticleTree2->SetCanFocus(true);
	mViews.pArticleTree2->SetID(viewID_ArticleList2);
	mViews.pArticleTree2->Show();
	
	// make article container view
	mViews.pArticleVc2 = new CContainerView(mViews.pPaneView6, SRect(0,194,294,280));
	mViews.pArticleVc2->SetSizing(sizing_BottomRightSticky);
	mViews.pPaneView6->SetViewLimits(mViews.pArticleVc2, 0, 50);
	mViews.pArticleVc2->Show();

	// make article sort button
	mViews.pSortArticlesBtn2 = new CButtonView(mViews.pArticleVc2, SRect(182,0,292,16));
	mViews.pSortArticlesBtn2->SetID(viewID_SortArticles2);
	mViews.pSortArticlesBtn2->SetSizing(sizing_HorizSticky);
	mViews.pSortArticlesBtn2->SetTooltipMsg("\pSort Articles");
	mViews.pSortArticlesBtn2->Show();

	// make attachment menu
	mViews.pAttachmentButton2 = MakePopupMenuView(mViews.pArticleVc2, SRect(2,0,20,16), &mViews.pAttachmentList2);
	mViews.pAttachmentList2->SetID(viewID_Attachment2);
	mViews.pAttachmentButton2->Disable();
	mViews.pAttachmentButton2->SetTooltipMsg("\pShow Attachments");
#if NEW_NEWS_FORMAT
	mViews.pAttachmentButton2->Show();
#endif

	// make article text
	scr = MakeTextBoxView(mViews.pArticleVc2, SRect(0,16,294,86), scrollerOption_Border, &mViews.pArticleText2);
	scr->SetOptions(scrollerOption_Border | scrollerOption_VertBar);
	scr->SetSizing(sizing_BottomRightSticky);
	mViews.pArticleText2->SetEnterKeyAction(enterKeyAction_None);
	mViews.pArticleText2->SetEditable(false);
	scr->Show();
	
//------------------------------------------------------------------------
	
	// make log list
	mViews.pLogListScr = new CScrollerView(mViews.pPaneView2, SRect(7,285,598,338), scrollerOption_PlainBorder | scrollerOption_NoFocusBorder | scrollerOption_VertBar | LIST_BACKGROUND_OPTION);
	mViews.pLogListScr->SetSizing(sizing_RightSticky | sizing_VertSticky);
	mViews.pPaneView2->SetViewLimits(mViews.pLogListScr, 0, 30);
	mViews.pLogListScr->SetCanFocus(true);
	mViews.pLogListScr->Show();

	mViews.pLogList = new CMyLogListView(mViews.pLogListScr, SRect(0, 0, mViews.pLogListScr->GetVisibleContentWidth(), mViews.pLogListScr->GetVisibleContentHeight()));
	mViews.pLogList->SetBehaviour(itemBehav_SelectOnlyOne);
	mViews.pLogList->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mViews.pLogList->SetCanFocus(true);
	mViews.pLogList->Show();

	// make task list
	mViews.pTaskListScr = new CScrollerView(pPaneView1, SRect(-1,343,607,389), scrollerOption_PlainBorder | scrollerOption_NoFocusBorder | scrollerOption_NoBkgnd | scrollerOption_VertBar | scrollerOption_GrowSpace);
	mViews.pTaskListScr->SetSizing(sizing_RightSticky | sizing_VertSticky);
	pPaneView1->SetViewLimits(mViews.pTaskListScr, 0, 10);
	mViews.pTaskListScr->SetCanFocus(true);
	mViews.pTaskListScr->Show();

	mViews.pTaskList = new CMyTaskListView(mViews.pTaskListScr, SRect(0, 0, mViews.pTaskListScr->GetVisibleContentWidth(), mViews.pTaskListScr->GetVisibleContentHeight()));
	mViews.pTaskList->SetBehaviour(itemBehav_SelectOnlyOne);
	mViews.pTaskList->SetCanFocus(true);
	mViews.pTaskList->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mViews.pTaskList->Show();

	mFilterTimer1 = UTimer::New(FilterTimerProc, this);
	mFilterTimer2 = UTimer::New(FilterTimerProc, this);
}

CMySynchWin::~CMySynchWin()
{
	UTimer::Dispose(mFilterTimer1);
	UTimer::Dispose(mFilterTimer2);

	Uint32 i = 0;
	SArticleData *pArticleData;
	
	while (mAttachmentList1.GetNext(pArticleData, i))
	{
		UMemory::Dispose((TPtr)pArticleData->pData);
		UMemory::Dispose((TPtr)pArticleData);
	}

	mAttachmentList1.Clear();
}

CLabelView *CMySynchWin::MakeGroupCount(CContainerView *inContainerView, const SRect& inBounds)
{
	// make group count box
	CBoxView *box = new CBoxView(inContainerView, inBounds, boxStyle_Etched);
	box->SetSizing(sizing_HorizSticky);
	box->Show();

	// make group count label font
	TFontDesc pFontDesc = UFontDesc::New(kDefaultFont, nil, 9);
	pFontDesc->SetAlign(textAlign_Right);

	// make group count label
	CLabelView *lbl = new CLabelView(inContainerView, SRect(inBounds.left + 4, inBounds.top + 4, inBounds.right - 4, inBounds.bottom - 4));
	lbl->SetSizing(sizing_HorizSticky);
	lbl->SetFont(pFontDesc);	// takes ownership of the pFontDesc
	lbl->SetText("\p0/0");
	lbl->Show();

	return lbl;
}

void CMySynchWin::SetInternalBounds(const SRect inViewsRect[14], const Uint8 inTabPercent[17])
{
	mViews.pPaneView2->SetBounds(inViewsRect[0]);
	mViews.pTaskListScr->SetBounds(inViewsRect[1]);
	
	mViews.pPaneView3->SetBounds(inViewsRect[2]);
	mViews.pLogListScr->SetBounds(inViewsRect[3]);
	
	mViews.pPaneView4->SetBounds(inViewsRect[4]);
	mViews.pPaneView6->SetBounds(inViewsRect[5]);

	mViews.pPaneView5->SetBounds(inViewsRect[6]);
	mViews.pArticleVc1->SetBounds(inViewsRect[7]);

	mViews.pPaneView7->SetBounds(inViewsRect[8]);
	mViews.pArticleVc2->SetBounds(inViewsRect[9]);

	mViews.pGroupVc1->SetBounds(inViewsRect[10]);
	mViews.pArticleTreeScr1->SetBounds(inViewsRect[11]);

	mViews.pGroupVc2->SetBounds(inViewsRect[12]);
	mViews.pArticleTreeScr2->SetBounds(inViewsRect[13]);

	mViews.pLogList->SetTabs(inTabPercent[0], inTabPercent[1], inTabPercent[2]);
	mViews.pArticleTree1->SetTabs(inTabPercent[3], inTabPercent[4], inTabPercent[5], inTabPercent[6], inTabPercent[7]);
	mViews.pArticleTree2->SetTabs(inTabPercent[8], inTabPercent[9], inTabPercent[10], inTabPercent[11], inTabPercent[12]);
	mViews.pGroupList1->SetTabs(inTabPercent[13], inTabPercent[14]);
	mViews.pGroupList2->SetTabs(inTabPercent[15], inTabPercent[16]);
}

void CMySynchWin::GetInternalBounds(SRect outViewsRect[14], Uint8 outTabPercent[17])
{
	mViews.pPaneView2->GetBounds(outViewsRect[0]);
	mViews.pTaskListScr->GetBounds(outViewsRect[1]);
	
	mViews.pPaneView3->GetBounds(outViewsRect[2]);
	mViews.pLogListScr->GetBounds(outViewsRect[3]);
	
	mViews.pPaneView4->GetBounds(outViewsRect[4]);
	mViews.pPaneView6->GetBounds(outViewsRect[5]);

	mViews.pPaneView5->GetBounds(outViewsRect[6]);
	mViews.pArticleVc1->GetBounds(outViewsRect[7]);

	mViews.pPaneView7->GetBounds(outViewsRect[8]);
	mViews.pArticleVc2->GetBounds(outViewsRect[9]);

	mViews.pGroupVc1->GetBounds(outViewsRect[10]);
	mViews.pArticleTreeScr1->GetBounds(outViewsRect[11]);

	mViews.pGroupVc2->GetBounds(outViewsRect[12]);
	mViews.pArticleTreeScr2->GetBounds(outViewsRect[13]);
	
	mViews.pLogList->GetTabs(outTabPercent[0], outTabPercent[1], outTabPercent[2]);
	mViews.pArticleTree1->GetTabs(outTabPercent[3], outTabPercent[4], outTabPercent[5], outTabPercent[6], outTabPercent[7]);
	mViews.pArticleTree2->GetTabs(outTabPercent[8], outTabPercent[9], outTabPercent[10], outTabPercent[11], outTabPercent[12]);
	mViews.pGroupList1->GetTabs(outTabPercent[13], outTabPercent[14]);
	mViews.pGroupList2->GetTabs(outTabPercent[15], outTabPercent[16]);
}

void CMySynchWin::SetServerInfo(SMyServerInfo *inServerInfo)
{
	if (!inServerInfo)
		return;
	
	mViews.pServerAddr->SetText(inServerInfo->psServerAddr + 1, inServerInfo->psServerAddr[0]);
	mViews.pUserLogin->SetText(inServerInfo->psUserLogin + 1, inServerInfo->psUserLogin[0]);
	mViews.pUserPassword->SetText(inServerInfo->psUserPassword + 1, inServerInfo->psUserPassword[0]);
}

Uint32 CMySynchWin::GetServerAddr(Uint8 *outServerAddr, Uint32 inMaxSize)
{
	return mViews.pServerAddr->GetText(outServerAddr, inMaxSize);
}

Uint32 CMySynchWin::GetUserLogin(Uint8 *outUserLogin, Uint32 inMaxSize)
{
	return mViews.pUserLogin->GetText(outUserLogin, inMaxSize);
}

Uint32 CMySynchWin::GetUserPassword(Uint8 *outUserPassword, Uint32 inMaxSize)
{
	return mViews.pUserPassword->GetText(outUserPassword, inMaxSize);
}

Uint32 CMySynchWin::GetFilterText1(Uint8 *outFilterText, Uint32 inMaxSize)
{
	return mViews.pGroupFilter1->GetText(outFilterText, inMaxSize);
}

Uint32 CMySynchWin::GetFilterText2(Uint8 *outFilterText, Uint32 inMaxSize)
{
	return mViews.pGroupFilter2->GetText(outFilterText, inMaxSize);
}

void CMySynchWin::AddGroup1(SMyGroupInfo1 *inGroupInfo, Uint8 *inFilterText)
{
	mViews.pGroupList1->AddGroup(inGroupInfo, inFilterText, mViews.pGroupCount1);
}

void CMySynchWin::FilterGroupList1(bool inFilter)
{
	mFilterTimer1->Stop();
				
	if (inFilter)
		FilterGroupList1();
	else
		mFilterTimer1->Start(1000);
}

void CMySynchWin::DeleteAllGroup1()
{
	mViews.pGroupList1->DeleteAll(mViews.pGroupCount1);
	mViews.pGroupList1->SetStatus("\p0 items in list.");
}

void CMySynchWin::AddGroup2(SMyGroupInfo2 *inGroupInfo, Uint8 *inFilterText)
{
	mViews.pGroupList2->AddGroup(inGroupInfo, inFilterText, mViews.pGroupCount2);
}

void CMySynchWin::FilterGroupList2(bool inFilter)
{
	mFilterTimer2->Stop();
				
	if (inFilter)
		FilterGroupList2();
	else
		mFilterTimer2->Start(1000);
}

void CMySynchWin::DeleteAllGroup2()
{
	mViews.pGroupList2->DeleteAll(mViews.pGroupCount2);
	mViews.pGroupList2->SetStatus("\p0 items in list.");
}

void CMySynchWin::UpdateSortBtn1(bool inDescending)
{
	if (inDescending)
		mViews.pSortArticlesBtn1->SetTitle("\p>     Descending");
	else
		mViews.pSortArticlesBtn1->SetTitle("\p<      Ascending");	
}

void CMySynchWin::UpdateSortBtn2(bool inDescending)
{
	if (inDescending)
		mViews.pSortArticlesBtn2->SetTitle("\p>     Descending");
	else
		mViews.pSortArticlesBtn2->SetTitle("\p<      Ascending");	
}

void CMySynchWin::DeleteAllArticle1()
{
	mViews.pArticleTree1->DeleteAll();
	mViews.pArticleTree1->SetStatus("\p0 items in list.");
}

void CMySynchWin::DeleteAllArticle2()
{
	mViews.pArticleTree2->DeleteAll();
	mViews.pArticleTree2->SetStatus("\p0 items in list.");
}

void CMySynchWin::AddAttachment1(SArticleData *inAttachmentData)
{
	mAttachmentList1.AddItem(inAttachmentData);
	mViews.pAttachmentList1->AddItem(inAttachmentData->psName);
	
	if (mViews.pAttachmentButton1->IsDisabled())
		mViews.pAttachmentButton1->Enable();
}

SArticleData *CMySynchWin::GetSelectedAttachment1()
{
	Uint32 nSelected = mViews.pAttachmentList1->GetFirstSelectedItem();
	if (!nSelected)
		return nil;
	
	Uint8 psAttachmentName[64];
	psAttachmentName[0] = mViews.pAttachmentList1->GetItemTitle(nSelected, psAttachmentName + 1, sizeof(psAttachmentName) - 1);
	if (!psAttachmentName[0])
		return nil;

	Uint32 i = 0;
	SArticleData *pArticleData;
	
	while (mAttachmentList1.GetNext(pArticleData, i))
	{
		if (!UMemory::Compare(pArticleData->psName + 1, pArticleData->psName[0], psAttachmentName + 1, psAttachmentName[0]))
			return pArticleData;
	}

	return nil;
}

void CMySynchWin::DeleteArticle1()
{
	Uint32 i = 0;
	SArticleData *pArticleData;
	
	while (mAttachmentList1.GetNext(pArticleData, i))
	{
		UMemory::Dispose((TPtr)pArticleData->pData);
		UMemory::Dispose((TPtr)pArticleData);
	}

	mAttachmentList1.Clear();

	if (mViews.pAttachmentButton1->IsEnabled())
		mViews.pAttachmentButton1->Disable();

	mViews.pAttachmentList1->Clear();
	
	mViews.pArticleText1->SetText(nil, 0);
}

void CMySynchWin::AddAttachment2(const Uint8 *inAttachmentName)
{
	mViews.pAttachmentList2->AddItem(inAttachmentName);

	if (mViews.pAttachmentButton2->IsDisabled())
		mViews.pAttachmentButton2->Enable();
}

Uint32 CMySynchWin::GetSelectedAttachment2(Uint8 *outAttachmentName, Uint32 inMaxSize)
{
	Uint32 nSelected = mViews.pAttachmentList2->GetFirstSelectedItem();
	if (!nSelected)
		return 0;
	
	return mViews.pAttachmentList2->GetItemTitle(nSelected, outAttachmentName, inMaxSize);
}

void CMySynchWin::DeleteArticle2()
{
	if (mViews.pAttachmentButton2->IsEnabled())
		mViews.pAttachmentButton2->Disable();

	mViews.pAttachmentList2->Clear();

	mViews.pArticleText2->SetText(nil, 0);
}

void CMySynchWin::SearchText(const Uint8 *inText)
{
	if (!inText || !inText[0])
		return;
	
	if (mViews.pGroupList1->IsFocus())
		mViews.pGroupList1->SearchText(inText);
	else if(mViews.pArticleTree1->IsFocus())
		mViews.pArticleTree1->SearchText(inText);
	else if (mViews.pGroupList2->IsFocus())
		mViews.pGroupList2->SearchText(inText);
	else if (mViews.pArticleTree2->IsFocus())
		mViews.pArticleTree2->SearchText(inText);
}

void CMySynchWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (UText::IsPrinting(inInfo.keyChar) &&
	   (mViews.pGroupList1->IsFocus() || mViews.pArticleTree1->IsFocus() || 
		 mViews.pGroupList2->IsFocus() || mViews.pArticleTree2->IsFocus()) &&
		 SearchText_KeyDown(inInfo))
		return;
			
	CWindow::KeyDown(inInfo);
}

void CMySynchWin::HandleDrag(CDragAndDroppable *inDD, const SMouseMsgData& inInfo)
{
	if (!UDragAndDrop::IsAvailable()) return;
	
	Uint8 nSelectedView;
	
	if (inDD == mViews.pGroupList1)
		nSelectedView = 1;
	else if (inDD == mViews.pArticleTree1)
		nSelectedView = 2;
	else if (inDD == mViews.pGroupList2)
		nSelectedView = 3;
	else if (inDD == mViews.pArticleTree2)
		nSelectedView = 4;
	else return;

	StDrag drag;
	Int32 hd, vd;
		
	switch (nSelectedView)
	{
		case 1:
			SMyGroupInfo1 *pGroupInfo1;
			if (!mViews.pGroupList1->GetSelectedItem(&pGroupInfo1))
				return;
		
			// get global mouse loc
			mViews.pGroupList1->GetScreenDelta(hd, vd);
		
			drag->AddFlavor(1, 'SGP1', pGroupInfo1->stGroupInfo.psGroupName, sizeof(pGroupInfo1->stGroupInfo.psGroupName), flavorFlag_SenderOnly);
			break;

		case 2:
			SMyArticleInfo1 *pArticleInfo1;
			mViews.pArticleTree1->GetFirstSelectedTreeItem(&pArticleInfo1);
		
			// get global mouse loc
			mViews.pArticleTree1->GetScreenDelta(hd, vd);
		
			drag->AddFlavor(1, 'SAD1', &pArticleInfo1->stArticleInfo.nArticleNumber, sizeof(pArticleInfo1->stArticleInfo.nArticleNumber), flavorFlag_SenderOnly);
			drag->AddFlavor(1, 'SAN1', &pArticleInfo1->stArticleInfo.psArticleName, sizeof(pArticleInfo1->stArticleInfo.psArticleName), flavorFlag_SenderOnly);
			break;			

		case 3:
			SMyGroupInfo2 *pGroupInfo2;
			if (!mViews.pGroupList2->GetSelectedItem(&pGroupInfo2))
				return;
		
			// get global mouse loc
			mViews.pGroupList2->GetScreenDelta(hd, vd);
		
			drag->AddFlavor(1, 'SGP2', pGroupInfo2->psGroupName, sizeof(pGroupInfo2->psGroupName), flavorFlag_SenderOnly);
			break;

		case 4:
			SMyArticleInfo2 *pArticleInfo2;
			mViews.pArticleTree2->GetFirstSelectedTreeItem(&pArticleInfo2);
		
			// get global mouse loc
			mViews.pArticleTree2->GetScreenDelta(hd, vd);
		
			drag->AddFlavor(1, 'SAD2', &pArticleInfo2->nArticleID, sizeof(pArticleInfo2->nArticleID), flavorFlag_SenderOnly);
			drag->AddFlavor(1, 'SAN2', &pArticleInfo2->psTitle, sizeof(pArticleInfo2->psTitle), flavorFlag_SenderOnly);
			break;			
	};
	
	SPoint mouseLoc;
	mouseLoc.h = inInfo.loc.h + hd;
	mouseLoc.v = inInfo.loc.v + vd;
	
	TRegion rgn = nil, imgRgn = nil;
	TImage img = nil;
	
	try
	{
		rgn = URegion::New();
			
		switch (nSelectedView)
		{
			case 1:
				mViews.pGroupList1->GetSelectedItemsRegion(rgn);	
				break;

			case 2:
				mViews.pArticleTree1->GetSelectedItemsRegion(rgn);	
				break;

			case 3:
				mViews.pGroupList2->GetSelectedItemsRegion(rgn);	
				break;

			case 4:
				mViews.pArticleTree2->GetSelectedItemsRegion(rgn);	
				break;
		};
		
		SRect r;
		rgn->GetBounds(r);
		
		#if MACINTOSH
		if ((((Uint32)r.GetWidth() * (Uint32)r.GetHeight()) <= 65536) && UDragAndDrop::HasTranslucentDrag())
		{
			imgRgn = URegion::New(rgn);
			imgRgn->GetBounds(r);
			imgRgn->Move(-r.left, -r.top);
			
			SPoint imgPt;
			switch (nSelectedView)
			{
				case 1:
					img = mViews.pGroupList1->GetSelectedItemsImage(imgPt);
					break;

				case 2:
					img = mViews.pArticleTree1->GetSelectedItemsImage(imgPt);	
					break;

				case 3:
					img = mViews.pGroupList2->GetSelectedItemsImage(imgPt);
					break;

				case 4:
					img = mViews.pArticleTree2->GetSelectedItemsImage(imgPt);	
					break;
			};
			
			drag->SetImage(img, imgRgn, imgPt, dragImageStyle_Dark);
		}
		#endif

		rgn->Move(hd, vd);
		rgn->SetOutline(2, 2);
		drag->Track(mouseLoc, rgn);
	}
	catch(...)
	{
		URegion::Dispose(rgn);
		URegion::Dispose(imgRgn);
		#if MACINTOSH
		UGraphics::DisposeImage(img);
		#endif
		
		throw;
	}

	URegion::Dispose(rgn);
	URegion::Dispose(imgRgn);
	#if MACINTOSH
	UGraphics::DisposeImage(img);	
	#endif
}

bool CMySynchWin::HandleDrop(CDragAndDroppable *inDD, const SDragMsgData& inInfo)
{
	Uint8 nSelectedView;
	
	if (inDD == mViews.pGroupList1)
		nSelectedView = 1;
	else if (inDD == mViews.pArticleTree1)
		nSelectedView = 2;
	else if (inDD == mViews.pGroupList2)
		nSelectedView = 3;
	else if (inDD == mViews.pArticleTree2)
		nSelectedView = 4;
	else 
		return false;

	switch (nSelectedView)
	{
		case 1:
			Uint8 psGroupName2[256];
			if (inInfo.drag->GetFlavorData(1, 'SGP2', psGroupName2, sizeof(psGroupName2)))
			{
				Uint8 psGroupName1[64];
				psGroupName1[0] = mViews.pGroupList1->GetDropGroupName(psGroupName1 + 1, sizeof(psGroupName1) - 1);
				
				gApp->DoSynchGroup1(psGroupName1, psGroupName2);
			}
			break;
	
		case 2:
			Uint32 nArticleID2;
			Uint8 psArticleName2[64];
			if (inInfo.drag->GetFlavorData(1, 'SAD2', &nArticleID2, sizeof(nArticleID2)) && 
				inInfo.drag->GetFlavorData(1, 'SAN2', psArticleName2, sizeof(psArticleName2)))
			{
				CPtrList<SMyDataInfo2> *pDataList = mViews.pArticleTree2->GetDataList(nArticleID2);
				if (!pDataList)
					return false;
				
				CPtrList<Uint8> pParentIDs1;
				mViews.pArticleTree1->GetDropArticleID(pParentIDs1);
								
				gApp->DoSynchArticle1(nArticleID2, psArticleName2, *pDataList, pParentIDs1);
				pParentIDs1.Clear();
			}
			break;		

		case 3:
			Uint8 psGroupName1[64];
			if (inInfo.drag->GetFlavorData(1, 'SGP1', psGroupName1, sizeof(psGroupName1)))
			{
				Uint8 psGroupName2[256];
				psGroupName2[0] = mViews.pGroupList2->GetDropGroupName(psGroupName2 + 1, sizeof(psGroupName2) - 1);
				
				gApp->DoSynchGroup2(psGroupName1, psGroupName2);
			}
			break;
	
		case 4:
			Uint32 nArticleID1;
			Uint8 psArticleName1[64];
			if (inInfo.drag->GetFlavorData(1, 'SAD1', &nArticleID1, sizeof(nArticleID1)) &&
				inInfo.drag->GetFlavorData(1, 'SAN1', psArticleName1, sizeof(psArticleName1)))
			{
				Uint32 nParentID2;
				nParentID2 = mViews.pArticleTree2->GetDropArticleID();
				
				gApp->DoSynchArticle2(nArticleID1, psArticleName1, nParentID2);
			}
			break;		
	};
	
	inInfo.drag->AcceptDrop();
	return true;
}

void CMySynchWin::FilterGroupList1()
{
	DeleteAllArticle1();
	DeleteArticle1();
	
	Uint8 psFilterText[256];
	psFilterText[0] = mViews.pGroupFilter1->GetText(psFilterText + 1, sizeof(psFilterText) - 1);

	mViews.pGroupList1->FilterGroupList(psFilterText, mViews.pGroupCount1);	
}

void CMySynchWin::FilterGroupList2()
{
	DeleteAllArticle2();
	DeleteArticle2();

	Uint8 psFilterText[256];
	psFilterText[0] = mViews.pGroupFilter2->GetText(psFilterText + 1, sizeof(psFilterText) - 1);

	mViews.pGroupList2->FilterGroupList(psFilterText, mViews.pGroupCount2);
}

void CMySynchWin::FilterTimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	#pragma unused(inMsg, inData, inDataSize)
	
	CMySynchWin *pWin = (CMySynchWin *)inContext;
	
	if (inObject == pWin->mFilterTimer1)
		pWin->FilterGroupList1();
	else if (inObject == pWin->mFilterTimer2)
		pWin->FilterGroupList2();
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark CMySynchList

CMySynchList::CMySynchList()
{
}

CMySynchList::~CMySynchList()
{
	mSynchList.Clear();
}
	
void CMySynchList::AddSynch(SMySynchInfo *inSynchInfo)
{
	if (!inSynchInfo)
		return;
		
	Uint32 i = 0;
	SMySynchInfo *pSynchInfo;
	
	while (mSynchList.GetNext(pSynchInfo, i))
	{
		if (pSynchInfo == inSynchInfo)
			return;
	}

	mSynchList.AddItem(inSynchInfo);
}

void CMySynchList::DeleteSynch(SMySynchInfo *inSynchInfo)
{
	Uint32 i = 0;
	SMySynchInfo *pSynchInfo;
	
	while (mSynchList.GetNext(pSynchInfo, i))
	{
		if (pSynchInfo == inSynchInfo)
		{
			mSynchList.RemoveItem(i);
			return;
		}
	}
}

// when synch
bool CMySynchList::GetNextSynch(SMySynchInfo*& ioSynchInfo, Uint32& ioIndex)
{
	return mSynchList.GetNext(ioSynchInfo, ioIndex);
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark CMyOptionsWin

CMyOptionsWin::CMyOptionsWin()
	: CWindow(SRect(0,0,550,450), windowLayer_Modal)
{
	CContainerView *vc;
	
	// setup window
	SetTitle("\pOptions");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,550,450));
	vc->Show();

	// make tab view
	mViews.tabs = new CTabbedView(vc, SRect(10,10,540,410));
	mViews.tabs->SetFont(UFontDesc::New(kDefaultFont, nil, 10));
	
	// add the tabs
	mViews.tabs->AddTab("\pGeneral");
	mViews.tabs->AddTab("\pSynchronization");
	
	// attach views to tabs
	mViews.tabs->SetTabView(1, MakeGeneralTab());
	mViews.tabs->SetTabView(2, MakeSynchTab());
	mViews.tabs->SetCurrentTab(2);
	mViews.tabs->Show();

	// make buttons
	SButtons btns[] = {{1, "\pSave", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(340,418,520,444), btns);

	mSynchList = nil;
	mFilterTimer = UTimer::New(FilterTimerProc, this);

#if MACINTOSH
	UMemory::Fill(mDispPassword, sizeof(mDispPassword), 0xA5);
#else
	UMemory::Fill(mDispPassword, sizeof(mDispPassword), '*');
#endif
}

CMyOptionsWin::~CMyOptionsWin()
{
	UTimer::Dispose(mFilterTimer);
}

CContainerView *CMyOptionsWin::MakeGeneralTab()
{
	CContainerView *vc;
	CScrollerView *scr;
	CCheckBoxView *chk;
	CLabelView *lbl;

	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,520,390));

	try
	{
		// make e-mail label
		lbl = new CLabelView(vc, SRect(10,19,120,35));
		lbl->SetText("\pE-mail address:");
		lbl->Show();

		// make e-mail text
		scr = MakeTextBoxView(vc, SRect(120,14,515,40), scrollerOption_Border, &mViews.emailText);
		scr->Show();

		// make organization label
		lbl = new CLabelView(vc, SRect(10,53,120,69));
		lbl->SetText("\pOrganization:");
		lbl->Show();

		// make organization text
		scr = MakeTextBoxView(vc, SRect(120,48,515,74), scrollerOption_Border, &mViews.organizationText);
		scr->Show();

		// make add text label
		lbl = new CLabelView(vc, SRect(10,143,120,159));
		lbl->SetText("\pAdd text:");
		lbl->Show();

		// make add text
		scr = MakeTextBoxView(vc, SRect(120,82,515,220), scrollerOption_Border, &mViews.addArticleText);
		scr->Show();

		// make purge days labels
		lbl = new CLabelView(vc, SRect(10,245,120,261));
		lbl->SetText("\pPurge after:");
		lbl->Show();

		lbl = new CLabelView(vc, SRect(174,245,210,261));
		lbl->SetText("\pdays");
		lbl->Show();

		// make purge days text
		scr = MakeTextBoxView(vc, SRect(120,240,170,266), scrollerOption_Border, &mViews.purgeDaysGenText);
		scr->Show();

		// make max articles label
		lbl = new CLabelView(vc, SRect(10,277,120,293));
		lbl->SetText("\pMax articles:");
		lbl->Show();

		// make max articles text
		scr = MakeTextBoxView(vc, SRect(120,272,170,298), scrollerOption_Border, &mViews.maxArticlesGenText);
		scr->Show();

		// make checkboxes
		chk = new CCheckBoxView(vc, SRect(280,248,515,266));
		chk->SetTitle("\pLog to file");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.logToFile = chk;

		chk = new CCheckBoxView(vc, SRect(280,272,515,290));
		chk->SetTitle("\pCreate old Hotline news groups");
		chk->SetAutoMark(true);
	#if NEW_NEWS_FORMAT
		chk->Show();
	#endif
		mViews.oldNewsFormat = chk;

	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

CContainerView *CMyOptionsWin::MakeSynchTab()
{
	CContainerView *vc;
	CScrollerView *scr;
	CBoxView *box;
	CCheckBoxView *chk;
	CLabelView *lbl;
	CButtonView *btn;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,520,390));

	try
	{	
		// make filter label
		lbl = new CLabelView(vc, SRect(12,10,62,26));
		lbl->SetFont(kDefaultFont, nil, 9);
		lbl->SetText("\pSearch:");
		lbl->Show();
	
		// make filter text box
		scr = MakeTextBoxView(vc, SRect(62,6,220,30), scrollerOption_Border, &mViews.pSynchFilter);
		mViews.pSynchFilter->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
		mViews.pSynchFilter->SetID(7);
		scr->Show();

		// make synch count
	#if MACINTOSH
		mViews.pSynchCount = MakeSynchCount(vc, SRect(225,8,300,28));
	#else
		mViews.pSynchCount = MakeSynchCount(vc, SRect(225,7,300,29));
	#endif

		// make news server checkbox
		chk = new CCheckBoxView(vc, SRect(305,0,515,18));
		chk->SetTitle("\pFilter selected News Server");
		chk->SetAutoMark(true);
		chk->SetID(8);
		chk->Show();
		mViews.showNewsServer = chk;

		// make hotline server checkbox
		chk = new CCheckBoxView(vc, SRect(305,18,515,36));
		chk->SetTitle("\pFilter selected Hotline Server");
		chk->SetAutoMark(true);
		chk->SetID(9);
		chk->Show();
		mViews.showHotlineServer = chk;

		// make list scroller view
		scr = new CScrollerView(vc, SRect(10,36,515,156), scrollerOption_Border | scrollerOption_VertBar | scrollerOption_NoBkgnd);
		scr->SetCanFocus(true);
		mViews.synchScroll = scr;
		scr->Show();

		// make news server label
		lbl = new CLabelView(vc, SRect(10,167,105,183));
		lbl->SetText("\pNews Server:");
		lbl->Show();

		// make news server text
		scr = MakeTextBoxView(vc, SRect(105,162,365,188), scrollerOption_Border, &mViews.serverText1);
		mViews.serverText1->SetEditable(false);
		scr->Show();
		
		// make news group label
		lbl = new CLabelView(vc, SRect(10,197,105,213));
		lbl->SetText("\pNews Group:");
		lbl->Show();

		// make news group text
		scr = MakeTextBoxView(vc, SRect(105,192,365,218), scrollerOption_Border, &mViews.groupText1);
		mViews.groupText1->SetEditable(false);
		scr->Show();
	
		// make login label
		lbl = new CLabelView(vc, SRect(369,167,417,183));
		lbl->SetText("\pLog:");
		lbl->Show();

		// make login text
		scr = MakeTextBoxView(vc, SRect(417,162,515,188), scrollerOption_Border, &mViews.loginText1);
		mViews.loginText1->SetEditable(false);
		scr->Show();
		
		// make password label
		lbl = new CLabelView(vc, SRect(369,197,417,213));
		lbl->SetText("\pPass:");
		lbl->Show();

		// make password box
		box = new CBoxView(vc, SRect(418,193,514,217), boxStyle_Sunken);
		box->Show();

		mViews.passwordLbl1 = new CLabelView(vc, SRect(423,198,511,216));
		mViews.passwordLbl1->Show();

		// make hotline server label
		lbl = new CLabelView(vc, SRect(10,227,105,243));
		lbl->SetText("\pHotline Server:");
		lbl->Show();

		// make hotline server text
		scr = MakeTextBoxView(vc, SRect(105,222,365,248), scrollerOption_Border, &mViews.serverText2);
		mViews.serverText2->SetEditable(false);
		scr->Show();
		
		// make hotline bundle label
		lbl = new CLabelView(vc, SRect(369,227,417,243));
		lbl->SetText("\pBundle:");
		lbl->Show();

		// make hotline bundle text
		scr = MakeTextBoxView(vc, SRect(417,222,515,248), scrollerOption_Border, &mViews.bundleText2);
		mViews.bundleText2->SetEditable(false);
		scr->Show();

		// make hotline group label
		lbl = new CLabelView(vc, SRect(10,257,105,273));
		lbl->SetText("\pHotline Group:");
		lbl->Show();

		// make hotline group text
		scr = MakeTextBoxView(vc, SRect(105,252,515,278), scrollerOption_Border, &mViews.groupText2);
		mViews.groupText2->SetEditable(false);
		scr->Show();
		
		// make synch hours labels
		lbl = new CLabelView(vc, SRect(10,287,105,303));
		lbl->SetText("\pSynch every:");
		lbl->Show();

		lbl = new CLabelView(vc, SRect(157,287,199,303));
		lbl->SetText("\phours");
		lbl->Show();

		// make synch hours box
		box = new CBoxView(vc, SRect(106,283,154,307), boxStyle_Sunken);
		box->Show();

		mViews.synchHoursLbl = new CLabelView(vc, SRect(109,288,151,304));
		mViews.synchHoursLbl->Show();

		// make purge days labels
		lbl = new CLabelView(vc, SRect(10,317,105,333));
		lbl->SetText("\pPurge after:");
		lbl->Show();

		lbl = new CLabelView(vc, SRect(157,317,199,333));
		lbl->SetText("\pdays");
		lbl->Show();

		// make purge days box
		box = new CBoxView(vc, SRect(106,313,154,337), boxStyle_Sunken);
		box->Show();

		mViews.purgeDaysLbl = new CLabelView(vc, SRect(109,318,151,334));
		mViews.purgeDaysLbl->Show();

		// make max articles label
		lbl = new CLabelView(vc, SRect(10,347,105,363));
		lbl->SetText("\pMax articles:");
		lbl->Show();

		// make max articles box
		box = new CBoxView(vc, SRect(106,343,154,367), boxStyle_Sunken);
		box->Show();

		mViews.maxArticlesLbl = new CLabelView(vc, SRect(109,348,151,364));
		mViews.maxArticlesLbl->Show();

		// make date box
		box = new CBoxView(vc, SRect(200,283,514,337), boxStyle_Sunken);
		box->SetSizing(sizing_HorizSticky);
		box->Show();

		// make last date
		lbl = new CLabelView(vc, SRect(208,289,283,305));
		lbl->SetText("\pLast Synch:");
		lbl->Show();

		mViews.lastSynchLbl = new CLabelView(vc, SRect(283,289,510,305));
		mViews.lastSynchLbl->Show();

		// make next date		
		lbl = new CLabelView(vc, SRect(208,315,283,331));
		lbl->SetText("\pNext Synch:");
		lbl->Show();

		mViews.nextSynchLbl = new CLabelView(vc, SRect(283,315,510,331));
		mViews.nextSynchLbl->Show();

		// make list view
		mViews.synchList = new CMySynchListView(mViews.synchScroll, SRect(0, 0, mViews.synchScroll->GetVisibleContentWidth(), mViews.synchScroll->GetVisibleContentHeight()), this);
		mViews.synchList->SetSizing(sizing_FullHeight | sizing_FullWidth);
		mViews.synchList->SetCanFocus(true);
		mViews.synchList->Show();

		// make buttons
		btn = new CButtonView(vc, SRect(199,344,275,366));
		btn->SetTitle("\pAdd");
//		btn->SetTooltipMsg("\pAdd New Synch");
		btn->SetID(3);
		btn->Show();
		btn = new CButtonView(vc, SRect(279,344,355,366));
		btn->SetTitle("\pEdit");
//		btn->SetTooltipMsg("\pEdit Selected Synch");
		btn->SetID(4);
		btn->Show();
		btn = new CButtonView(vc, SRect(359,344,435,366));
		btn->SetTitle("\pDelete");
//		btn->SetTooltipMsg("\pDelete Selected Synch");
		btn->SetID(5);
		btn->Show();
		btn = new CButtonView(vc, SRect(439,344,515,366));
		btn->SetTitle("\pSynch");
//		btn->SetTooltipMsg("\pMake Selected Synch");
		btn->SetID(6);
		btn->Show();			
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

CLabelView *CMyOptionsWin::MakeSynchCount(CContainerView *inContainerView, const SRect& inBounds)
{
	// make synch count box
	CBoxView *box = new CBoxView(inContainerView, inBounds, boxStyle_Etched);
	box->Show();

	// make synch count label font
	TFontDesc pFontDesc = UFontDesc::New(kDefaultFont, nil, 9);
	pFontDesc->SetAlign(textAlign_Right);

	// make group count label
	CLabelView *lbl = new CLabelView(inContainerView, SRect(inBounds.left + 4, inBounds.top + 4, inBounds.right - 4, inBounds.bottom - 4));
	lbl->SetSizing(sizing_HorizSticky);
	lbl->SetFont(pFontDesc);	// takes ownership of the pFontDesc
	lbl->SetText("\p0/0");
	lbl->Show();

	return lbl;
}

void CMyOptionsWin::SetSynchInfo(const SMySynchInfo *inSynchInfo)
{
	if (!inSynchInfo || !inSynchInfo->pHotlineFile)
		return;
	
	mViews.serverText1->SetText(inSynchInfo->stServerInfo.psServerAddr + 1, inSynchInfo->stServerInfo.psServerAddr[0]);
	mViews.loginText1->SetText(inSynchInfo->stServerInfo.psUserLogin + 1, inSynchInfo->stServerInfo.psUserLogin[0]);
	if (inSynchInfo->stServerInfo.psUserPassword[0])	mViews.passwordLbl1->SetText(mDispPassword, sizeof(mDispPassword));
	else												mViews.passwordLbl1->SetText(nil, 0);
	mViews.groupText1->SetText(inSynchInfo->psGroupName + 1, inSynchInfo->psGroupName[0]);

	mViews.serverText2->SetText(inSynchInfo->psHotlineServer + 1, inSynchInfo->psHotlineServer[0]);

	Uint8 psItemName[256];
	inSynchInfo->pHotlineFile->GetParentName(psItemName);
	mViews.bundleText2->SetText(psItemName + 1, psItemName[0]);
	
	inSynchInfo->pHotlineFile->GetName(psItemName);
	mViews.groupText2->SetText(psItemName + 1, psItemName[0]);
		
	// synch hours
	Uint8 psSynchHours[8];
	psSynchHours[0] = UText::IntegerToText(psSynchHours + 1, sizeof(psSynchHours) - 1, inSynchInfo->nSynchHours);
	mViews.synchHoursLbl->SetText(psSynchHours + 1, psSynchHours[0]);
	
	// purge days
	Uint8 psPurgeDays[8];
	psPurgeDays[0] = UText::IntegerToText(psPurgeDays + 1, sizeof(psPurgeDays) - 1, inSynchInfo->nPurgeDays);
	mViews.purgeDaysLbl->SetText(psPurgeDays + 1, psPurgeDays[0]);

	// max articles
	Uint8 psMaxArticles[8];
	psMaxArticles[0] = UText::IntegerToText(psMaxArticles + 1, sizeof(psMaxArticles) - 1, inSynchInfo->nMaxArticles);
	mViews.maxArticlesLbl->SetText(psMaxArticles + 1, psMaxArticles[0]);

	SetSynchDate(inSynchInfo->stLastSynch, inSynchInfo->nSynchHours);
}

void CMyOptionsWin::SetSynchDate(const SCalendarDate& inLastSynch, Uint32 inHours)
{
	// last synch date
	Uint8 psSynchDate[64];
	psSynchDate[0] = UDateTime::DateToText(inLastSynch, psSynchDate + 1, sizeof(psSynchDate) - 1, kAbbrevDateText | kTimeWithSecsText);
	mViews.lastSynchLbl->SetText(psSynchDate);

	// next synch date
	SCalendarDate stNextSynch = inLastSynch + inHours * 3600;
	psSynchDate[0] = UDateTime::DateToText(stNextSynch, psSynchDate + 1, sizeof(psSynchDate) - 1, kAbbrevDateText | kTimeWithSecsText);
	mViews.nextSynchLbl->SetText(psSynchDate);
}

void CMyOptionsWin::SetInfo(const SMyOptions& inOptions, CPtrList<SMySynchInfo> *inSynchList, const Uint8 *inNewsServer, const Uint8 *inHotlineServer)
{
	mViews.emailText->SetText(inOptions.psEmailAddress + 1, inOptions.psEmailAddress[0]);
	mViews.organizationText->SetText(inOptions.psOrganization + 1, inOptions.psOrganization[0]);
	
	if (inOptions.pAddArticleText)
		mViews.addArticleText->SetText(inOptions.pAddArticleText, inOptions.nArticleTextSize);

	// purge days
	Uint8 psPurgeDays[8];
	psPurgeDays[0] = UText::IntegerToText(psPurgeDays + 1, sizeof(psPurgeDays) - 1, inOptions.nPurgeDays);
	mViews.purgeDaysGenText->SetText(psPurgeDays + 1, psPurgeDays[0]);

	// max articles
	Uint8 psMaxArticles[8];
	psMaxArticles[0] = UText::IntegerToText(psMaxArticles + 1, sizeof(psMaxArticles) - 1, inOptions.nMaxArticles);
	mViews.maxArticlesGenText->SetText(psMaxArticles + 1, psMaxArticles[0]);

	// options
	mViews.logToFile->SetMark((inOptions.nOpts & myOpt_LogToFile) != 0);
#if NEW_NEWS_FORMAT
	mViews.oldNewsFormat->SetMark((inOptions.nOpts & myOpt_OldNewsFormat) != 0);
#endif
		
	if (inNewsServer && inNewsServer[0])
		mViews.showNewsServer->SetMark(true);

	if (inHotlineServer && inHotlineServer[0])
		mViews.showHotlineServer->SetMark(true);
	
	mSynchList = inSynchList;
	RefreshSynchList(inNewsServer, inHotlineServer);
}

void CMyOptionsWin::GetInfo(SMyOptions& outOptions)
{
	outOptions.psEmailAddress[0] = mViews.emailText->GetText(outOptions.psEmailAddress + 1, sizeof(outOptions.psEmailAddress) - 1);
	outOptions.psOrganization[0] = mViews.organizationText->GetText(outOptions.psOrganization + 1, sizeof(outOptions.psOrganization) - 1);

	Uint32 nArticleTextSize = mViews.addArticleText->GetTextSize();
	if (nArticleTextSize != outOptions.nArticleTextSize)
	{
		outOptions.nArticleTextSize = nArticleTextSize;
		
		if (outOptions.pAddArticleText)
			outOptions.pAddArticleText = UMemory::Reallocate((TPtr)outOptions.pAddArticleText, outOptions.nArticleTextSize);
		else
			outOptions.pAddArticleText = UMemory::New(outOptions.nArticleTextSize);
	}
	
	if (outOptions.pAddArticleText)
		outOptions.nArticleTextSize = mViews.addArticleText->GetText(outOptions.pAddArticleText, outOptions.nArticleTextSize);
	
	// purge days
	outOptions.nPurgeDays = GetPurgeDays();

	// max articles
	outOptions.nMaxArticles = GetMaxArticles();
	
	// options
	outOptions.nOpts = 0;
	if (mViews.logToFile->GetMark()) outOptions.nOpts |= myOpt_LogToFile;
#if NEW_NEWS_FORMAT
	if (mViews.oldNewsFormat->GetMark()) outOptions.nOpts |= myOpt_OldNewsFormat;
#endif

	MakeNewSynch();
}

void CMyOptionsWin::AddSynch(const SMySynchInfo *inSynchInfo)
{
	AddEditSynch(true, inSynchInfo);
}

void CMyOptionsWin::AddSynch(const SMySynchInfo& inSynchInfo)
{
	if (mSynchList->GetItemCount() >= kMaxSynchs)
	{
		SMySynchInfo *pSynchInfo = mSynchList->GetItem(1);
		EditSynch(inSynchInfo, pSynchInfo, true);
		return;
	}
	
	SMySynchInfo *pSynchInfo = (SMySynchInfo *)UMemory::NewClear(sizeof(SMySynchInfo));
		
	pSynchInfo->nActive = 1;
	UMemory::Copy(pSynchInfo->stServerInfo.psServerAddr, inSynchInfo.stServerInfo.psServerAddr, inSynchInfo.stServerInfo.psServerAddr[0] + 1);
	UMemory::Copy(pSynchInfo->stServerInfo.psUserLogin, inSynchInfo.stServerInfo.psUserLogin, inSynchInfo.stServerInfo.psUserLogin[0] + 1);
	UMemory::Copy(pSynchInfo->stServerInfo.psUserPassword, inSynchInfo.stServerInfo.psUserPassword, inSynchInfo.stServerInfo.psUserPassword[0] + 1);
	UMemory::Copy(pSynchInfo->psGroupName, inSynchInfo.psGroupName, inSynchInfo.psGroupName[0] + 1);
	UMemory::Copy(pSynchInfo->psHotlineServer, inSynchInfo.psHotlineServer, inSynchInfo.psHotlineServer[0] + 1);
	pSynchInfo->nSynchType = myOpt_SynchNewsBoth;
	pSynchInfo->nSynchHours = inSynchInfo.nSynchHours;
	pSynchInfo->nPurgeDays = inSynchInfo.nPurgeDays;
	pSynchInfo->nMaxArticles = inSynchInfo.nMaxArticles;
	UDateTime::GetCalendarDate(calendar_Gregorian, pSynchInfo->stLastSynch);
	pSynchInfo->pHotlineFile = inSynchInfo.pHotlineFile;
	
	mSynchList->AddItem(pSynchInfo);	// add to synch list
	mNewSynchList.AddSynch(pSynchInfo);	// add to new synch list

	RefreshList(pSynchInfo->stServerInfo.psServerAddr, pSynchInfo->psHotlineServer, pSynchInfo);
}

void CMyOptionsWin::EditSynch()
{
	SMySynchInfo *pSynchInfo = mViews.synchList->GetSelectedSynch();
	if (!pSynchInfo)
		return;

	AddEditSynch(false, pSynchInfo);
}

void CMyOptionsWin::EditSynch(const SMySynchInfo& inSynchInfo)
{
	SMySynchInfo *pSynchInfo = mViews.synchList->GetSelectedSynch();
	if (!pSynchInfo)
		return;

	EditSynch(inSynchInfo, pSynchInfo);
}

void CMyOptionsWin::EditSynch(const SMySynchInfo& inSynchInfo, SMySynchInfo *inEditSynchInfo, bool bActivate)
{
	if (!inEditSynchInfo)
		return;
	
	if (bActivate)
		inEditSynchInfo->nActive = 1;
	
	// news server
	if (UMemory::Compare(inEditSynchInfo->stServerInfo.psServerAddr + 1, inEditSynchInfo->stServerInfo.psServerAddr[0], inSynchInfo.stServerInfo.psServerAddr + 1, inSynchInfo.stServerInfo.psServerAddr[0]))
		UMemory::Copy(inEditSynchInfo->stServerInfo.psServerAddr, inSynchInfo.stServerInfo.psServerAddr, inSynchInfo.stServerInfo.psServerAddr[0] + 1);
	
	// login
	if (UMemory::Compare(inEditSynchInfo->stServerInfo.psUserLogin + 1, inEditSynchInfo->stServerInfo.psUserLogin[0], inSynchInfo.stServerInfo.psUserLogin + 1, inSynchInfo.stServerInfo.psUserLogin[0]))
		UMemory::Copy(inEditSynchInfo->stServerInfo.psUserLogin, inSynchInfo.stServerInfo.psUserLogin, inSynchInfo.stServerInfo.psUserLogin[0] + 1);

	// password
	if (UMemory::Compare(inEditSynchInfo->stServerInfo.psUserPassword + 1, inEditSynchInfo->stServerInfo.psUserPassword[0], inSynchInfo.stServerInfo.psUserPassword + 1, inSynchInfo.stServerInfo.psUserPassword[0]))
		UMemory::Copy(inEditSynchInfo->stServerInfo.psUserPassword, inSynchInfo.stServerInfo.psUserPassword, inSynchInfo.stServerInfo.psUserPassword[0] + 1);

	// news group
	if (UMemory::Compare(inEditSynchInfo->psGroupName + 1, inEditSynchInfo->psGroupName[0], inSynchInfo.psGroupName + 1, inSynchInfo.psGroupName[0]))
		UMemory::Copy(inEditSynchInfo->psGroupName, inSynchInfo.psGroupName, inSynchInfo.psGroupName[0] + 1);

	// hotline server
	if (UMemory::Compare(inEditSynchInfo->psHotlineServer + 1, inEditSynchInfo->psHotlineServer[0], inSynchInfo.psHotlineServer + 1, inSynchInfo.psHotlineServer[0]))
		UMemory::Copy(inEditSynchInfo->psHotlineServer, inSynchInfo.psHotlineServer, inSynchInfo.psHotlineServer[0] + 1);

	// synch type
	if (inSynchInfo.nSynchType)
		inEditSynchInfo->nSynchType = inSynchInfo.nSynchType;

	// synch hours
	if (inSynchInfo.nSynchHours)
		inEditSynchInfo->nSynchHours = inSynchInfo.nSynchHours;

	// purge days
	inEditSynchInfo->nPurgeDays = inSynchInfo.nPurgeDays;

	// max articles
	inEditSynchInfo->nMaxArticles = inSynchInfo.nMaxArticles;

	// hotline group
	if (inSynchInfo.pHotlineFile)
	{
		UFileSys::Dispose(inEditSynchInfo->pHotlineFile);
		inEditSynchInfo->pHotlineFile = inSynchInfo.pHotlineFile;
	}
	
	RefreshList(inEditSynchInfo->stServerInfo.psServerAddr, inEditSynchInfo->psHotlineServer, inEditSynchInfo);
}

void CMyOptionsWin::AddEditSynch(bool inAddEditSynch, const SMySynchInfo *inSynchInfo)
{
	if (mViews.tabs->GetCurrentTab() != 2)
		mViews.tabs->SetCurrentTab(2);

	Uint32 id;
	CMyAddEditSynchWin *win = new CMyAddEditSynchWin(inAddEditSynch, GetPurgeDays(), GetMaxArticles());
	
	if (inSynchInfo)
		win->SetSynchInfo(*inSynchInfo);
	
	win->Show();
	
	// go into modal loop to process modal add window
	for(;;)
	{
		win->ProcessModal();
		id = win->GetHitID();
		
		if (id == 1)		// save
		{
			SMySynchInfo stSynchInfo;
			if (!win->GetSynchInfo(stSynchInfo))
				continue;
			
			delete win;

			if (inAddEditSynch)
				AddSynch(stSynchInfo);
			else
				EditSynch(stSynchInfo);
				
			return;
		}
		else if (id == 2)	// cancel
		{
			delete win;
			return;
		}
		else if(id == 3)	// select bundle
			win->SelectBundle();
		else if(id == 4)	// select group
			win->SelectGroup();
	}

}

void CMyOptionsWin::DeleteSynch()
{
	SMySynchInfo *pSelSynchInfo = mViews.synchList->GetSelectedSynch();
	if (!pSelSynchInfo)
		return;

	Uint32 i = 0;
	SMySynchInfo *pSynchInfo;
	
	while (mSynchList->GetNext(pSynchInfo, i))
	{
		if (pSynchInfo == pSelSynchInfo)
		{
			mNewSynchList.DeleteSynch(pSynchInfo);		// delete from new synch list
			
			UFileSys::Dispose(pSynchInfo->pHotlineFile);
			UMemory::Dispose((TPtr)pSynchInfo);
			mSynchList->RemoveItem(i);
			
			pSelSynchInfo = mViews.synchList->GetNearSelectedSynch();	// select this item
			if (pSelSynchInfo)
				RefreshList(pSelSynchInfo->stServerInfo.psServerAddr, pSelSynchInfo->psHotlineServer, pSelSynchInfo);
			else
				RefreshList(nil, nil);
				
			return;
		}
	}
}

void CMyOptionsWin::MakeSynch()
{
	SMySynchInfo *pSynchInfo = mViews.synchList->GetSelectedSynch();
	if (!pSynchInfo)
		return;
	
	mNewSynchList.DeleteSynch(pSynchInfo);		// delete from new synch list
	
	try
	{
		gApp->MakeSynch(*pSynchInfo);
	}
	catch(SError& err)
	{
		UError::Alert(err);
		return;
	}
	
	SetSynchDate(pSynchInfo->stLastSynch, pSynchInfo->nSynchHours);
}

void CMyOptionsWin::MakeNewSynch()
{
	Uint32 i = 0;
	SMySynchInfo *pSynchInfo;
	
	while (mNewSynchList.GetNextSynch(pSynchInfo, i))
	{
		if (!pSynchInfo->nActive)
			continue;
		
		try
		{
			gApp->MakeSynch(*pSynchInfo);
		}
		catch(SError& err)
		{
			UError::Alert(err);
		}
	}
}

void CMyOptionsWin::FilterList()
{
	mFilterTimer->Start(1000);
	mViews.pSynchFilter->SetHasChanged(false);
}

void CMyOptionsWin::RefreshList()
{
	SMySynchInfo *pSynchInfo = mViews.synchList->GetSelectedSynch();

	const Uint8 *pNewsServer = nil;
	if (pSynchInfo && mViews.showNewsServer->GetMark())
		pNewsServer = pSynchInfo->stServerInfo.psServerAddr;

	const Uint8 *pHotlineServer = nil;
	if (pSynchInfo && mViews.showHotlineServer->GetMark())
		pHotlineServer = pSynchInfo->psHotlineServer;

	RefreshSynchList(pNewsServer, pHotlineServer, pSynchInfo);
}

void CMyOptionsWin::RefreshList(const Uint8 *inNewsServer, const Uint8 *inHotlineServer, SMySynchInfo *inSelSynchInfo)
{
	const Uint8 *pNewsServer = nil;
	if (mViews.showNewsServer->GetMark())
		pNewsServer = inNewsServer;

	const Uint8 *pHotlineServer = nil;
	if (mViews.showHotlineServer->GetMark())
		pHotlineServer = inHotlineServer;

	RefreshSynchList(pNewsServer, pHotlineServer, inSelSynchInfo);
}

void CMyOptionsWin::RefreshSynchList(const Uint8 *inNewsServer, const Uint8 *inHotlineServer, SMySynchInfo *inSelSynchInfo)
{
	ClearSynchInfo();
	mViews.synchList->ClearList(false);

	const Uint8 *pNewsServer = nil;
	const Uint8 *pHotlineServer = nil;

	Uint32 i = 0;
	SMySynchInfo *pSynchInfo;
			
	// determine existance of inNewsServer and inHotlineServer in list
	if ((inNewsServer && inNewsServer[0]) || (inHotlineServer && inHotlineServer[0]))
	{
		bool bBothServers = false;

		while (mSynchList->GetNext(pSynchInfo, i) && !bBothServers)
		{
			bool bNewsServer = false;
			if (inNewsServer && inNewsServer[0])
			{
		 		if (!UText::CompareInsensitive(pSynchInfo->stServerInfo.psServerAddr + 1, pSynchInfo->stServerInfo.psServerAddr[0], inNewsServer + 1, inNewsServer[0]))
		 		{
					bNewsServer = true;
					pNewsServer = inNewsServer;
				}
			}

			bool bHotlineServer = false;
			if (inHotlineServer && inHotlineServer[0])
			{
				if (!UText::CompareInsensitive(pSynchInfo->psHotlineServer + 1, pSynchInfo->psHotlineServer[0], inHotlineServer + 1, inHotlineServer[0]))
				{
					bHotlineServer = true;
					pHotlineServer = inHotlineServer;
				}
			}
			
			if (bNewsServer && bHotlineServer)
				bBothServers = true;
		}

		// if this combination don't exists in list keep just the Hotline Server
		if (pHotlineServer && !bBothServers)
			pNewsServer = nil;
	}
	
	if (!pNewsServer)
		mViews.showNewsServer->SetMark(false);
	
	if (!pHotlineServer)
		mViews.showHotlineServer->SetMark(false);
	
	Uint8 psFilterText[256];
	psFilterText[0] = mViews.pSynchFilter->GetText(psFilterText + 1, sizeof(psFilterText) - 1);

	i = 0;
	Uint32 nSelectIndex = 1;
	
	while (mSynchList->GetNext(pSynchInfo, i))
	{
		// if not this news server continue
		if (pNewsServer && UText::CompareInsensitive(pSynchInfo->stServerInfo.psServerAddr + 1, pSynchInfo->stServerInfo.psServerAddr[0], pNewsServer + 1, pNewsServer[0]))
			continue;

		// if not this hotline server continue
		if (pHotlineServer && UText::CompareInsensitive(pSynchInfo->psHotlineServer + 1, pSynchInfo->psHotlineServer[0], pHotlineServer + 1, pHotlineServer[0]))
			continue;
		
		// if not this filter text continue
		if (psFilterText[0] && !UText::SearchInsensitive(psFilterText + 1, psFilterText[0], pSynchInfo->psGroupName + 1, pSynchInfo->psGroupName[0]))
		{
			Uint8 psFileName[256];
			pSynchInfo->pHotlineFile->GetName(psFileName);

			if (!UText::SearchInsensitive(psFilterText + 1, psFilterText[0], psFileName + 1, psFileName[0]))
				continue;
		}
		
		Uint32 nIndex = mViews.synchList->AddItem(pSynchInfo, false);
		if (pSynchInfo == inSelSynchInfo)
			nSelectIndex = nIndex;
	}
			
	if (mViews.synchList->GetItemCount() >= nSelectIndex)
	{
		mViews.synchList->SelectItem(nSelectIndex);
		mViews.synchList->MakeItemVisible(nSelectIndex);
	}
	
	DisplaySynchCount();
}

void CMyOptionsWin::DisplaySynchCount()
{
	Uint8 psSynchCount[32];
	psSynchCount[0] = UText::Format(psSynchCount + 1, sizeof(psSynchCount) - 1, "%lu/%lu", mViews.synchList->GetItemCount(), mSynchList->GetItemCount());
		
	mViews.pSynchCount->SetText(psSynchCount);
}

void CMyOptionsWin::ClearSynchInfo()
{
	mViews.serverText1->SetText(nil, 0);
	mViews.loginText1->SetText(nil, 0);
	mViews.passwordLbl1->SetText(nil, 0);
	mViews.groupText1->SetText(nil, 0);
	
	mViews.serverText2->SetText(nil, 0);
	mViews.bundleText2->SetText(nil, 0);
	mViews.groupText2->SetText(nil, 0);

	mViews.synchHoursLbl->SetText(nil, 0);
	mViews.purgeDaysLbl->SetText(nil, 0);
	mViews.maxArticlesLbl->SetText(nil, 0);

	mViews.lastSynchLbl->SetText(nil, 0);
	mViews.nextSynchLbl->SetText(nil, 0);
}

Uint32 CMyOptionsWin::GetPurgeDays()
{
	Int32 nPurgeDays = 0;
	Uint8 psPurgeDays[8];
	
	psPurgeDays[0] = mViews.purgeDaysGenText->GetText(psPurgeDays + 1, sizeof(psPurgeDays) - 1);
	if (psPurgeDays[0])
	{
		nPurgeDays = UText::TextToInteger(psPurgeDays + 1, psPurgeDays[0]);
		if (nPurgeDays < 0)
			nPurgeDays = 0;
		else if (nPurgeDays > MAX_PURGE_DAYS)
			nPurgeDays = MAX_PURGE_DAYS;
	}

	return nPurgeDays;
}

Uint32 CMyOptionsWin::GetMaxArticles()
{
	Int32 nMaxArticles = 0;
	Uint8 psMaxArticles[8];
	
	psMaxArticles[0] = mViews.maxArticlesGenText->GetText(psMaxArticles + 1, sizeof(psMaxArticles) - 1);
	if (psMaxArticles[0])
	{
		nMaxArticles = UText::TextToInteger(psMaxArticles + 1, psMaxArticles[0]);
		if (nMaxArticles < 0)
			nMaxArticles = 0;
		else if (nMaxArticles > MAX_MAX_ARTICLES)
			nMaxArticles = MAX_MAX_ARTICLES;
	}

	return nMaxArticles;
}

void CMyOptionsWin::FilterTimerProc(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	#pragma unused(inMsg, inData, inDataSize)
	
	CMyOptionsWin *pWin = (CMyOptionsWin *)inContext;
	
	if (inObject == pWin->mFilterTimer)
		pWin->RefreshList();
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark CMyAddEditSynchWin

CMyAddEditSynchWin::CMyAddEditSynchWin(bool inAddEditSynch, Uint32 inPurgeDays, Uint32 inMaxArticles)
	: CWindow(SRect(0,0,520,220), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	CLabelView *lbl;
	CButtonView *btn;
	
	// setup window
	if (inAddEditSynch)
		SetTitle("\pAdd Synch");
	else
		SetTitle("\pEdit Synch");

	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,520,220));
	vc->Show();

	// make news server label
	lbl = new CLabelView(vc, SRect(10,11,105,27));
	lbl->SetText("\pNews Server:");
	lbl->Show();

	// make news server text
	scr = MakeTextBoxView(vc, SRect(105,6,365,32), scrollerOption_Border, &mViews.serverText1);
	vc->SetFocusView(scr);
	scr->Show();
	
	// make news group label
	lbl = new CLabelView(vc, SRect(10,41,105,57));
	lbl->SetText("\pNews Group:");
	lbl->Show();

	// make news group text
	scr = MakeTextBoxView(vc, SRect(105,36,365,62), scrollerOption_Border, &mViews.groupText1);
	scr->Show();
	
	// make log label
	lbl = new CLabelView(vc, SRect(369,11,417,27));
	lbl->SetText("\pLog:");
	lbl->Show();

	// make log text
	scr = MakeTextBoxView(vc, SRect(417,6,515,32), scrollerOption_Border, &mViews.loginText1);
	scr->Show();
	
	// make pass label
	lbl = new CLabelView(vc, SRect(369,41,417,57));
	lbl->SetText("\pPass:");
	lbl->Show();

	// make pass text
	scr = MakePasswordBoxView(vc, SRect(417,36,515,62), scrollerOption_Border, &mViews.passwordText1);
	scr->Show();

	// make hotline server label
	lbl = new CLabelView(vc, SRect(10,71,105,87));
	lbl->SetText("\pHotline Server:");
	lbl->Show();

	// make hotline server text
	scr = MakeTextBoxView(vc, SRect(105,66,365,92), scrollerOption_Border, &mViews.serverText2);
	scr->Show();

	// make hotline bundle label
	lbl = new CLabelView(vc, SRect(369,71,417,87));
	lbl->SetText("\pBundle:");
	lbl->Show();

	// make hotline bundle text
	scr = MakeTextBoxView(vc, SRect(417,66,515,92), scrollerOption_Border, &mViews.bundleText2);
	mViews.bundleText2->SetEditable(false);
	scr->Show();
	
	// make hotline group label
	lbl = new CLabelView(vc, SRect(10,101,105,117));
	lbl->SetText("\pHotline Group:");
	lbl->Show();

	// make hotline group text
	scr = MakeTextBoxView(vc, SRect(105,96,515,122), scrollerOption_Border, &mViews.groupText2);
	scr->Show();
	
	// make synch hours labels
	lbl = new CLabelView(vc, SRect(10,131,105,147));
	lbl->SetText("\pSynch every:");
	lbl->Show();

	lbl = new CLabelView(vc, SRect(157,131,200,147));
	lbl->SetText("\phours");
	lbl->Show();

	// make synch hours text
	Uint8 psSynchHours[8];
	scr = MakeTextBoxView(vc, SRect(105,126,155,152), scrollerOption_Border, &mViews.synchHoursText);
	psSynchHours[0] = UText::IntegerToText(psSynchHours + 1, sizeof(psSynchHours) - 1, DEFAULT_SYNCH_HOURS);
	mViews.synchHoursText->SetText(psSynchHours + 1, psSynchHours[0]);
	scr->Show();

	// make purge days labels
	lbl = new CLabelView(vc, SRect(10,161,105,177));
	lbl->SetText("\pPurge after:");
	lbl->Show();

	lbl = new CLabelView(vc, SRect(157,161,200,177));
	lbl->SetText("\pdays");
	lbl->Show();

	// make purge days text
	Uint8 psPurgeDays[8];
	scr = MakeTextBoxView(vc, SRect(105,156,155,182), scrollerOption_Border, &mViews.purgeDaysText);
	psPurgeDays[0] = UText::IntegerToText(psPurgeDays + 1, sizeof(psPurgeDays) - 1, inPurgeDays);
	mViews.purgeDaysText->SetText(psPurgeDays + 1, psPurgeDays[0]);
	scr->Show();

	// make max articles labels
	lbl = new CLabelView(vc, SRect(10,191,105,207));
	lbl->SetText("\pMax articles:");
	lbl->Show();

	// make max articles text
	Uint8 psMaxArticles[8];
	scr = MakeTextBoxView(vc, SRect(105,186,155,212), scrollerOption_Border, &mViews.maxArticlesText);
	psMaxArticles[0] = UText::IntegerToText(psMaxArticles + 1, sizeof(psMaxArticles) - 1, inMaxArticles);
	mViews.maxArticlesText->SetText(psMaxArticles + 1, psMaxArticles[0]);
	scr->Show();

	// make select buttons
	btn = new CButtonView(vc, SRect(297,128,395,150));
	btn->SetTitle("\pSelect Bundle");
	btn->SetTooltipMsg("\pSelect Hotline Bundle");
	btn->SetID(3);
	btn->Show();

	btn = new CButtonView(vc, SRect(417,128,515,150));
	btn->SetTitle("\pSelect Group");
	btn->SetTooltipMsg("\pSelect Hotline Group");
	btn->SetID(4);
	btn->Show();

	// make buttons
	SButtons btns[] = {{1, "\pSave", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(317,186,497,212), btns);

	mBundleRef = nil;
}

CMyAddEditSynchWin::~CMyAddEditSynchWin()
{
	UFileSys::Dispose(mBundleRef);
}

void CMyAddEditSynchWin::SetSynchInfo(const SMySynchInfo& inSynchInfo)
{
	// news server
	if (inSynchInfo.stServerInfo.psServerAddr[0])
		mViews.serverText1->SetText(inSynchInfo.stServerInfo.psServerAddr + 1, inSynchInfo.stServerInfo.psServerAddr[0]);

	// login
	if (inSynchInfo.stServerInfo.psUserLogin[0])
		mViews.loginText1->SetText(inSynchInfo.stServerInfo.psUserLogin + 1, inSynchInfo.stServerInfo.psUserLogin[0]);
	
	// password
	if (inSynchInfo.stServerInfo.psUserPassword[0])
		mViews.passwordText1->SetText(inSynchInfo.stServerInfo.psUserPassword + 1, inSynchInfo.stServerInfo.psUserPassword[0]);
	
	// news group
	if (inSynchInfo.psGroupName[0])
		mViews.groupText1->SetText(inSynchInfo.psGroupName + 1, inSynchInfo.psGroupName[0]);
	
	// hotline server
	if (inSynchInfo.psHotlineServer[0])
		mViews.serverText2->SetText(inSynchInfo.psHotlineServer + 1, inSynchInfo.psHotlineServer[0]);
	
	// hotline group
	if (inSynchInfo.pHotlineFile)
	{
		mBundleRef = inSynchInfo.pHotlineFile->GetFolder();

		Uint8 psItemName[256];
		inSynchInfo.pHotlineFile->GetName(psItemName);
		mViews.groupText2->SetText(psItemName + 1, psItemName[0]);

		if (mBundleRef)
		{
			mBundleRef->GetName(psItemName);
			mViews.bundleText2->SetText(psItemName + 1, psItemName[0]);
		}
	}	

	// synch hours
	if (inSynchInfo.nSynchHours)
	{
		Uint8 psSynchHours[8];
		psSynchHours[0] = UText::IntegerToText(psSynchHours + 1, sizeof(psSynchHours) - 1, inSynchInfo.nSynchHours);
		mViews.synchHoursText->SetText(psSynchHours + 1, psSynchHours[0]);
	}
	
	// purge days
	if (inSynchInfo.nPurgeDays)
	{
		Uint8 psPurgeDays[8];
		psPurgeDays[0] = UText::IntegerToText(psPurgeDays + 1, sizeof(psPurgeDays) - 1, inSynchInfo.nPurgeDays);
		mViews.purgeDaysText->SetText(psPurgeDays + 1, psPurgeDays[0]);
	}

	// max articles
	if (inSynchInfo.nMaxArticles)
	{
		Uint8 psMaxArticles[8];
		psMaxArticles[0] = UText::IntegerToText(psMaxArticles + 1, sizeof(psMaxArticles) - 1, inSynchInfo.nMaxArticles);
		mViews.maxArticlesText->SetText(psMaxArticles + 1, psMaxArticles[0]);
	}
}

bool CMyAddEditSynchWin::GetSynchInfo(SMySynchInfo& outSynchInfo)
{
	ClearStruct(outSynchInfo);
	
	if (!mBundleRef)
		return false;

	// news server
	outSynchInfo.stServerInfo.psServerAddr[0] = mViews.serverText1->GetText(outSynchInfo.stServerInfo.psServerAddr + 1, sizeof(outSynchInfo.stServerInfo.psServerAddr) - 1);
	if (!outSynchInfo.stServerInfo.psServerAddr[0])
		return false;

	// login/password
	outSynchInfo.stServerInfo.psUserLogin[0] = mViews.loginText1->GetText(outSynchInfo.stServerInfo.psUserLogin + 1, sizeof(outSynchInfo.stServerInfo.psUserLogin) - 1);
	outSynchInfo.stServerInfo.psUserPassword[0] = mViews.passwordText1->GetText(outSynchInfo.stServerInfo.psUserPassword + 1, sizeof(outSynchInfo.stServerInfo.psUserPassword) - 1);

	// news group
	outSynchInfo.psGroupName[0] = mViews.groupText1->GetText(outSynchInfo.psGroupName + 1, sizeof(outSynchInfo.psGroupName) - 1);
	if (!outSynchInfo.psGroupName[0])
		return false;

	// hotline server
	outSynchInfo.psHotlineServer[0] = mViews.serverText2->GetText(outSynchInfo.psHotlineServer + 1, sizeof(outSynchInfo.psHotlineServer) - 1);
	if (!outSynchInfo.psHotlineServer[0])
		return false;

	// synch hours
	Uint8 psSynchHours[8];
	psSynchHours[0] = mViews.synchHoursText->GetText(psSynchHours + 1, sizeof(psSynchHours) - 1);
	if (!psSynchHours[0])
		return false;
	
	Int32 nSynchHours = UText::TextToInteger(psSynchHours + 1, psSynchHours[0]);
	if (nSynchHours <= 0)
		return false;
	
	if (nSynchHours > MAX_SYNCH_HOURS)
		nSynchHours = MAX_SYNCH_HOURS;

	outSynchInfo.nSynchHours = nSynchHours;

	// purge days
	Int32 nPurgeDays = 0;
	
	Uint8 psPurgeDays[8];
	psPurgeDays[0] = mViews.purgeDaysText->GetText(psPurgeDays + 1, sizeof(psPurgeDays) - 1);
	if (psPurgeDays[0])
	{
		nPurgeDays = UText::TextToInteger(psPurgeDays + 1, psPurgeDays[0]);
		if (nPurgeDays < 0)
			nPurgeDays = 0;
		else if (nPurgeDays > MAX_PURGE_DAYS)
			nPurgeDays = MAX_PURGE_DAYS;
	}

	outSynchInfo.nPurgeDays = nPurgeDays;

	// max articles
	Int32 nMaxArticles = 0;
	
	Uint8 psMaxArticles[8];
	psMaxArticles[0] = mViews.maxArticlesText->GetText(psMaxArticles + 1, sizeof(psMaxArticles) - 1);
	if (psMaxArticles[0])
	{
		nMaxArticles = UText::TextToInteger(psMaxArticles + 1, psMaxArticles[0]);
		if (nMaxArticles < 0)
			nMaxArticles = 0;
		else if (nMaxArticles > MAX_MAX_ARTICLES)
			nMaxArticles = MAX_MAX_ARTICLES;
	}

	outSynchInfo.nMaxArticles = nMaxArticles;

	// hotline group
	Uint8 psGroupName[256];
	psGroupName[0] = mViews.groupText2->GetText(psGroupName + 1, sizeof(psGroupName) - 1);
	if (!psGroupName[0])
		return false;

#if MACINTOSH
	// max 31 chars for file name on MAC
	if (psGroupName[0] > 31)
		psGroupName[0] = 31;
#endif
	
	outSynchInfo.pHotlineFile = UFileSys::New(mBundleRef, nil, psGroupName);
	if (!outSynchInfo.pHotlineFile)
		return false;
	
	return true;
}

void CMyAddEditSynchWin::SelectBundle()
{
	TFSRefObj* pBundleRef = UFileSys::UserSelectFolder();
	if (!pBundleRef) 
		return;
		
	UFileSys::Dispose(mBundleRef);
	mBundleRef = pBundleRef;
	
	Uint8 psBundleName[256];
	mBundleRef->GetName(psBundleName);
	mViews.bundleText2->SetText(psBundleName + 1, psBundleName[0]);
	
	Uint8 psServerName[32];
	psServerName[0] = gApp->GetServerNameBundle(mBundleRef, psServerName + 1, sizeof(psServerName) - 1);
	
	if (psServerName[0])
		mViews.serverText2->SetText(psServerName + 1, psServerName[0]);	
	else
		mViews.serverText2->SetText(nil, 0);
}

void CMyAddEditSynchWin::SelectGroup()
{
	TFSRefObj* pGroupFile = UFileSys::UserSelectFile();
	if (!pGroupFile) 
		return;
	
	scopekill(TFSRefObj, pGroupFile);
	
	UFileSys::Dispose(mBundleRef);
	mBundleRef = pGroupFile->GetFolder();
	if (!mBundleRef)
		return;
	
	Uint8 psItemName[256];
	pGroupFile->GetName(psItemName);
	mViews.groupText2->SetText(psItemName + 1, psItemName[0]);

	mBundleRef->GetName(psItemName);
	mViews.bundleText2->SetText(psItemName + 1, psItemName[0]);
		
	Uint8 psServerName[32];
	psServerName[0] = gApp->GetServerNameBundle(mBundleRef, psServerName + 1, sizeof(psServerName) - 1);

	if (psServerName[0])
		mViews.serverText2->SetText(psServerName + 1, psServerName[0]);		
	else
		mViews.serverText2->SetText(nil, 0);
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark CMySelectServerWin1

CMySelectServerWin1::CMySelectServerWin1()
	: CWindow(SRect(0,0,280,240), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	
	// setup window
	SetTitle("\pSelect News Server");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,280,240));
	vc->Show();

	// make box
	CBoxView *box = new CBoxView(vc, SRect(10,4,270,24), boxStyle_Sunken);
	box->Show();

	// make label font
#if MACINTOSH
	TFontDesc pFontDesc = UFontDesc::New(kDefaultFont, nil, 11, fontEffect_Bold);
#else
	TFontDesc pFontDesc = UFontDesc::New(kDefaultFont, nil, 9, fontEffect_Bold);
#endif
	pFontDesc->SetAlign(textAlign_Center);

	// make label
	CLabelView *lbl = new CLabelView(vc, SRect(12,6,268,22));
	lbl->SetFont(pFontDesc);	// takes ownership of the pFontDesc
	lbl->SetText("\pNews Servers");
	lbl->Show();

	// make list scroller view
	scr = new CScrollerView(vc, SRect(10,28,270,200), scrollerOption_Border | scrollerOption_VertBar | scrollerOption_NoBkgnd);
	scr->SetCanFocus(true);
	scr->Show();

	// make list view
	mViews.serverList = new CMyServerListView1(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()));
	mViews.serverList->SetSizing(sizing_FullHeight | sizing_FullWidth);
	mViews.serverList->SetCanFocus(true);
	mViews.serverList->Show();

	// make buttons
	SButtons btns[] = {{1, "\pSelect", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(90,206,270,232), btns);
}

/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -
#pragma mark CMySelectServerWin2

CMySelectServerWin2::CMySelectServerWin2()
	: CWindow(SRect(0,0,360,240), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	
	// setup window
	SetTitle("\pSelect Hotline Server");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,360,240));
	vc->Show();

	// make box
	CBoxView *box = new CBoxView(vc, SRect(10,4,180,24), boxStyle_Sunken);
	box->Show();

	// make label font
#if MACINTOSH
	TFontDesc pFontDesc = UFontDesc::New(kDefaultFont, nil, 11, fontEffect_Bold);
#else
	TFontDesc pFontDesc = UFontDesc::New(kDefaultFont, nil, 9, fontEffect_Bold);
#endif
	pFontDesc->SetAlign(textAlign_Center);

	// make label
	CLabelView *lbl = new CLabelView(vc, SRect(12,6,178,22));
	lbl->SetFont(pFontDesc);	// takes ownership of the pFontDesc
	lbl->SetText("\pHotline Servers");
	lbl->Show();

	// make list scroller view
	scr = new CScrollerView(vc, SRect(10,28,180,200), scrollerOption_Border | scrollerOption_VertBar | scrollerOption_NoBkgnd);
	scr->SetCanFocus(true);
	scr->Show();

	// make list view
	mViews.serverList = new CMyServerListView2(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), this);
	mViews.serverList->SetSizing(sizing_FullHeight | sizing_FullWidth);
	mViews.serverList->SetCanFocus(true);
	mViews.serverList->Show();

	// make box
	box = new CBoxView(vc, SRect(190,4,350,24), boxStyle_Sunken);
	box->Show();

	// make label font
#if MACINTOSH
	pFontDesc = UFontDesc::New(kDefaultFont, nil, 11, fontEffect_Bold);
#else
	pFontDesc = UFontDesc::New(kDefaultFont, nil, 9, fontEffect_Bold);
#endif
	pFontDesc->SetAlign(textAlign_Center);

	// make label
	lbl = new CLabelView(vc, SRect(192,6,348,22));
	lbl->SetFont(pFontDesc);	// takes ownership of the pFontDesc
	lbl->SetText("\pHotline Bundles");
	lbl->Show();

	// make list scroller view
	scr = new CScrollerView(vc, SRect(190,28,350,200), scrollerOption_Border | scrollerOption_VertBar | scrollerOption_NoBkgnd);
	scr->SetCanFocus(true);
	scr->Show();

	// make list view
	mViews.bundleList = new CMyBundleListView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()));
	mViews.bundleList->SetSizing(sizing_FullHeight | sizing_FullWidth);
	mViews.bundleList->SetCanFocus(true);
	mViews.bundleList->Show();

	// make buttons
	SButtons btns[] = {{1, "\pSelect", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(170,206,350,232), btns);
}

void CMySelectServerWin2::SetBundleList(SMyServerBundleInfo *inServerBundleInfo)
{
	mViews.bundleList->ClearList(false);
	
	Uint32 i = 0;
	TFSRefObj* pBundle;
		
	while (inServerBundleInfo->lBundleList.GetNext(pBundle, i))
		mViews.bundleList->AddItem(pBundle);

	mViews.bundleList->DeselectAll();
	mViews.bundleList->SelectItem(1);
}

bool CMySelectServerWin2::GetSelectedServer(SMyServerBundleInfo*& outServerBundleInfo, TFSRefObj*& outBundle)
{
	return (mViews.serverList->GetSelectedItem(&outServerBundleInfo) && mViews.bundleList->GetSelectedItem(&outBundle));
}
