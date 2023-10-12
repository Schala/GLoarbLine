/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "HotlineServ.h"
#include "HotlineServIDTranslate.h"

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

CMyOptionsWin::CMyOptionsWin()
	: CWindow(SRect(0,0,590,336), windowLayer_Modal)
{
	// setup window
	SetTitle("\pOptions");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	mViews.vc = new CContainerView(this, SRect(0,0,590,336));
	mViews.vc->Show();

	// make tab view
	mViews.tabs = new CTabbedView(mViews.vc, SRect(10,10,580,290));
	mViews.tabs->SetFont(UFontDesc::New(kDefaultFont, nil, 10));
	
	// add the tabs
	mViews.tabs->AddTab("\pGeneral");
	mViews.tabs->AddTab("\pInfo");
#if BANNER_SERVER
	mViews.tabs->AddTab("\pBanner");
#endif
	mViews.tabs->AddTab("\pBan");
	mViews.tabs->AddTab("\pTrackers");
	mViews.tabs->AddTab("\pAdmin");
	mViews.tabs->AddTab("\pIP");
	mViews.tabs->AddTab("\pMisc");
	mViews.tabs->AddTab("\pLogin Message");
	// attach views to tabs
	Uint16 nTabNum = 1; 
	mViews.tabs->SetTabView(nTabNum++, MakeGeneralTab());
	mViews.tabs->SetTabView(nTabNum++, MakeServInfoTab());
#if BANNER_SERVER
	mViews.tabs->SetTabView(nTabNum++, MakeBannerTab());
#endif
	mViews.tabs->SetTabView(nTabNum++, MakeBanTab());
	mViews.tabs->SetTabView(nTabNum++, MakeTrackersTab());
	mViews.tabs->SetTabView(nTabNum++, MakeAdminTab());
	mViews.tabs->SetTabView(nTabNum++, MakeAdvancedTab());
	mViews.tabs->SetTabView(nTabNum++, MakeMiscTab());
	mViews.tabs->SetTabView(nTabNum++, MakeLoginMessageTab());
	mViews.tabs->SetCurrentTab(1);
	mViews.tabs->Show();

	// make buttons

	SButtons btns[] = {{1, "\pSave", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(mViews.vc, SRect(220,300,470,316), btns);
}

CContainerView *CMyOptionsWin::MakeGeneralTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CCheckBoxView *chk;
	CScrollerView *scr;
	CBoxView *box;

	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,550,280));

	try
	{
		// make box
		box = new CBoxView(vc, SRect(10,10,550,136), boxStyle_Etched);
		box->Show();

		// make labels
		lbl = new CLabelView(vc, SRect(20,23,300,39));
		lbl->SetText("\pServer port number:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(20,51,300,67));
		lbl->SetText("\pMaximum simultaneous downloads:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(20,79,300,95));
		lbl->SetText("\pMaximum simultaneous downloads/client:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(20,107,300,123));
		lbl->SetText("\pMaximum simultaneous connections/IP:");
		lbl->Show();

		// make text boxes
		scr = MakeTextBoxView(vc, SRect(368,18,430,44), scrollerOption_Border, &mViews.portNumText);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(368,46,430,72), scrollerOption_Border, &mViews.maxDownText);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(368,74,430,100), scrollerOption_Border, &mViews.maxDownClientText);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(368,102,430,128), scrollerOption_Border, &mViews.maxConnectionsIPText);
		scr->Show();

		// make box
		box = new CBoxView(vc, SRect(10,146,550,242), boxStyle_Etched);
		box->Show();

		// make checkboxes
		chk = new CCheckBoxView(vc, SRect(20,156,230,174));
		chk->SetTitle("\pLog Downloads to file");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.logDownloads = chk;
		
		chk = new CCheckBoxView(vc, SRect(20,176,230,194));
		chk->SetTitle("\pLog Uploads to file");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.logUploads = chk;
		
		chk = new CCheckBoxView(vc, SRect(20,196,230,214));
		chk->SetTitle("\pLog Connects to file");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.logConnects = chk;
		chk = new CCheckBoxView(vc, SRect(20,216,230,234));
		chk->SetTitle("\pLog Account Changes to file");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.logAccountChanges = chk;
		
		chk = new CCheckBoxView(vc, SRect(250,156,400,174));
		chk->SetTitle("\pConfirm Quit");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.confirmQuit = chk;
		
		chk = new CCheckBoxView(vc, SRect(250,176,400,194));
		chk->SetTitle("\pCSV Logs");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.csvMode = chk;
		
		chk = new CCheckBoxView(vc, SRect(250,196,400,214));
		chk->SetTitle("\pBot compatibility");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.botMode = chk;
		
		
		chk = new CCheckBoxView(vc, SRect(250,216,440,234));
		chk->SetTitle("\pShow Kick/Ban on public chat");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.showKick = chk;
		
		
		
		chk = new CCheckBoxView(vc, SRect(430,156,540,174));
		chk->SetTitle("\pLog Chats to file");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.logChats = chk;
		
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

CContainerView *CMyOptionsWin::MakeServInfoTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,460,280));

	try
	{
		// make labels
		lbl = new CLabelView(vc, SRect(20,20,320,36));
		lbl->SetText("\pInformation about this Server (for Trackers):");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(20,55,116,71));
		lbl->SetText("\pName:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(20,116,116,132));
		lbl->SetText("\pDescription:");
		lbl->Show();
	#if NETWORK_SERVER
		lbl = new CLabelView(vc, SRect(20,183,116,199));
		lbl->SetText("\pSerial Number:");
		lbl->Show();
	#endif
		
		// make text boxes
		scr = MakeTextBoxView(vc, SRect(120,50,430,76), scrollerOption_Border, &mViews.nameText);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(120,80,430,168), scrollerOption_Border, &mViews.descText);
		scr->Show();
		
	#if NETWORK_SERVER
		scr = MakeTextBoxView(vc, SRect(120,178,430,204), scrollerOption_Border, &mViews.serialNumberText);
		scr->Show();
		mViews.serialNumberScroll = scr;
	#endif
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
		
	return vc;
}

#if BANNER_SERVER
CContainerView *CMyOptionsWin::MakeBannerTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CCheckBoxView *chk;
	CScrollerView *scr;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,460,280));

	try
	{
		// make checkbox
		chk = new CCheckBoxView(vc, SRect(30,36,110,54));
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->SetTitle("\pLocal");
		chk->SetID(4);
		chk->Show();
		mViews.agreemLocal = chk;
		
		// make text box
		scr = MakeTextBoxView(vc, SRect(120,60,430,86), scrollerOption_Border, &mViews.agreementBannerText);
		scr->Show();
		mViews.agreementBannerScroll = scr;
		scr = MakeTextBoxView(vc, SRect(120,90,430,116), scrollerOption_Border, &mViews.agreementBannerUrlText);
		scr->Show();
		mViews.agreementBannerUrlScroll = scr;
		
		// make checkboxes
		chk = new CCheckBoxView(vc, SRect(30,140,110,158));
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->SetTitle("\pRemote");
		chk->SetID(5);
		chk->Show();
		mViews.agreemRemote = chk;

		// make text box
		scr = MakeTextBoxView(vc, SRect(120,164,430,190), scrollerOption_Border, &mViews.agreementUrlText);
		scr->Show();
		mViews.agreementUrlScroll = scr;

		mViews.agreemLocal->SetExclusiveNext(mViews.agreemRemote);
		mViews.agreemRemote->SetExclusiveNext(mViews.agreemLocal);

		// make labels
		lbl = new CLabelView(vc, SRect(64,65,120,81));
		lbl->SetText("\pBanner:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(64,95,120,111));
		lbl->SetText("\pLink:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(64,169,120,185));
		lbl->SetText("\pURL:");
		lbl->Show();		
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}
#endif

CContainerView *CMyOptionsWin::MakeBanTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CScrollerView *scr;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,460,280));

	try
	{	
		// make labels view
		lbl = new CLabelView(vc, SRect(10,193,32,209));
		lbl->SetText("\pIP:");
		lbl->Show();

		lbl = new CLabelView(vc, SRect(170,193,204,209));
		lbl->SetText("\pInfo:");
		lbl->Show();

		// make list scroller view
		scr = new CScrollerView(vc, SRect(10,10,444,180), scrollerOption_Border | scrollerOption_VertBar | scrollerOption_NoBkgnd);
		scr->SetCanFocus(true);
		mViews.permBanScroll = scr;
		scr->Show();

		// make text view
		scr = MakeTextBoxView(vc, SRect(32,188,162,214), scrollerOption_Border, &mViews.banUrlText);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(204,188,444,214), scrollerOption_Border, &mViews.banDescrText);
		scr->Show();

		// make list view
		mViews.permBanList = new CMyPermBanListView(mViews.permBanScroll, SRect(0,0,mViews.permBanScroll->GetVisibleContentWidth(), mViews.permBanScroll->GetVisibleContentHeight()), mViews.banUrlText, mViews.banDescrText);
		mViews.permBanList->SetSizing(sizing_FullHeight);
		mViews.permBanList->SetCanFocus(true);
		mViews.permBanList->Show();

		// make buttons
		btn = new CButtonView(vc, SRect(32,222,108,244));
		btn->SetTitle("\pAdd");
		btn->SetID(6);
		btn->Show();
		btn = new CButtonView(vc, SRect(122,222,198,244));
		btn->SetTitle("\pDelete");
		btn->SetID(7);
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

CContainerView *CMyOptionsWin::MakeTrackersTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	CCheckBoxView *chk;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,460,280));

	try
	{
		// make checkboxes
		chk = new CCheckBoxView(vc, SRect(10,1,240,15));
		chk->SetTitle("\pList with Hotline Trackers:");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.useTracker = chk;

		// make labels
	#if NEW_TRACKER
		lbl = new CLabelView(vc, SRect(10,60,220,76));
		lbl->SetText("\pAddress");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(230,60,330,76));
		lbl->SetText("\pLogin");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(340,60,440,76));
		lbl->SetText("\pPassword");
		lbl->Show();
	#else
		lbl = new CLabelView(vc, SRect(10,15,300,31));
		lbl->SetText("\pAddress");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(310,15,440,31));
		lbl->SetText("\pPassword");
		lbl->Show();
	#endif

		// make tracker line 1
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,30,220,56), scrollerOption_Border, mViews.trackerAddress + 0);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,30,330,56), scrollerOption_Border, mViews.trackerLogin + 0);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,30,440,56), scrollerOption_Border, mViews.trackerPassword + 0);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,30,300,56), scrollerOption_Border, mViews.trackerAddress + 0);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,30,440,56), scrollerOption_Border, mViews.trackerPassword + 0);
		scr->Show();
	#endif
		
		// make tracker line 2
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,51,220,77), scrollerOption_Border, mViews.trackerAddress + 1);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,51,330,77), scrollerOption_Border, mViews.trackerLogin + 1);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,51,440,77), scrollerOption_Border, mViews.trackerPassword + 1);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,51,300,77), scrollerOption_Border, mViews.trackerAddress + 1);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,51,440,77), scrollerOption_Border, mViews.trackerPassword + 1);
		scr->Show();
	#endif
	
		// make tracker line 3
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,72,220,98), scrollerOption_Border, mViews.trackerAddress + 2);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,72,330,98), scrollerOption_Border, mViews.trackerLogin + 2);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,72,440,98), scrollerOption_Border, mViews.trackerPassword + 2);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,72,300,98), scrollerOption_Border, mViews.trackerAddress + 2);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,72,440,98), scrollerOption_Border, mViews.trackerPassword + 2);
		scr->Show();
	#endif
	
		// make tracker line 4
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,93,220,119), scrollerOption_Border, mViews.trackerAddress + 3);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,93,330,119), scrollerOption_Border, mViews.trackerLogin + 3);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,93,440,119), scrollerOption_Border, mViews.trackerPassword + 3);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,93,300,119), scrollerOption_Border, mViews.trackerAddress + 3);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,93,440,119), scrollerOption_Border, mViews.trackerPassword + 3);
		scr->Show();
	#endif
	
		// make tracker line 5
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,114,220,140), scrollerOption_Border, mViews.trackerAddress + 4);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,114,330,140), scrollerOption_Border, mViews.trackerLogin + 4);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,114,440,140), scrollerOption_Border, mViews.trackerPassword + 4);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,114,300,140), scrollerOption_Border, mViews.trackerAddress + 4);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,114,440,140), scrollerOption_Border, mViews.trackerPassword + 4);
		scr->Show();
	#endif
	
	// make tracker line 6
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,135,220,161), scrollerOption_Border, mViews.trackerAddress + 5);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,135,330,161), scrollerOption_Border, mViews.trackerLogin + 5);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,135,440,161), scrollerOption_Border, mViews.trackerPassword + 5);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,135,300,161), scrollerOption_Border, mViews.trackerAddress + 5);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,135,440,161), scrollerOption_Border, mViews.trackerPassword + 5);
		scr->Show();
	#endif
	
	// make tracker line 7
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,156,220,182), scrollerOption_Border, mViews.trackerAddress + 6);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,156,330,182), scrollerOption_Border, mViews.trackerLogin + 6);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,156,440,182), scrollerOption_Border, mViews.trackerPassword + 6);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,156,300,182), scrollerOption_Border, mViews.trackerAddress + 6);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,156,440,182), scrollerOption_Border, mViews.trackerPassword + 6);
		scr->Show();
	#endif
	
	// make tracker line 8
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,177,220,203), scrollerOption_Border, mViews.trackerAddress + 7);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,177,330,203), scrollerOption_Border, mViews.trackerLogin + 7);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,177,440,203), scrollerOption_Border, mViews.trackerPassword + 7);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,177,300,203), scrollerOption_Border, mViews.trackerAddress + 7);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,177,440,203), scrollerOption_Border, mViews.trackerPassword + 7);
		scr->Show();
	#endif
	// make tracker line 9
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,198,220,224), scrollerOption_Border, mViews.trackerAddress + 8);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,198,330,224), scrollerOption_Border, mViews.trackerLogin + 8);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,198,440,224), scrollerOption_Border, mViews.trackerPassword + 8);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,198,300,224), scrollerOption_Border, mViews.trackerAddress + 8);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,198,440,224), scrollerOption_Border, mViews.trackerPassword + 8);
		scr->Show();
	#endif
	
	// make tracker line 10
	#if NEW_TRACKER
		scr = MakeTextBoxView(vc, SRect(10,219,220,245), scrollerOption_Border, mViews.trackerAddress + 9);
		scr->Show();
		scr = MakeTextBoxView(vc, SRect(230,219,330,245), scrollerOption_Border, mViews.trackerLogin + 9);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(340,219,440,245), scrollerOption_Border, mViews.trackerPassword + 9);
		scr->Show();
	#else
		scr = MakeTextBoxView(vc, SRect(10,219,300,245), scrollerOption_Border, mViews.trackerAddress + 9);
		scr->Show();
		scr = MakePasswordBoxView(vc, SRect(310,219,440,245), scrollerOption_Border, mViews.trackerPassword + 9);
		scr->Show();
	#endif
	
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

CContainerView *CMyOptionsWin::MakeAdminTab()
{
	CContainerView *vc;
	CBoxView *box;
	CLabelView *lbl;
	CButtonView *btn;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,460,280));

	try
	{
		// make box
		box = new CBoxView(vc, SRect(10,10,440,147), boxStyle_Etched);
		box->Show();

		// make label view
		lbl = new CLabelView(vc, SRect(20,15,430,114));
		const Int8 *pText = "The Admin Account is the the account which is treated as the administrator of the Server. Any user logged into this account has full access to the Server and you should not reveal this account's password to others. You should generally leave the account as \"admin\". Note that if you change this value, the old Admin Account is still active until you delete it.";
		lbl->SetText(pText, strlen(pText));
		lbl->Show();

		// make admin labels view
		lbl = new CLabelView(vc, SRect(20,119,160,135));
		lbl->SetText("\pAdmin Account Login:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(164,119,344,135));
		lbl->Show();
		mViews.adminLbl = lbl;
	
		// make select button
		btn = new CButtonView(vc, SRect(350,114,430,140));
		btn->SetTitle("\pSelect...");
		btn->SetID(8);
		btn->Show();

		// make box
		box = new CBoxView(vc, SRect(10,157,440,247), boxStyle_Etched);
		box->Show();

		// make label view
		lbl = new CLabelView(vc, SRect(20,162,430,214));
		lbl->SetText("\pIf you forgot the password to this account, you can reset it here. This button also resets all the privileges for this account providing the administrator with full access.");
		lbl->Show();
	
		// make reset button
		btn = new CButtonView(vc, SRect(20,214,186,240));
		btn->SetTitle("\pReset Admin Account...");
		btn->SetID(9);
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

CContainerView *CMyOptionsWin::MakeAdvancedTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,460,280));

	try
	{
		// make label view
		lbl = new CLabelView(vc, SRect(20,20,180,36));
		lbl->SetText("\pServer IP:");
		lbl->Show();

		// make IP text views
		scr = MakeTextBoxView(vc, SRect(240,15,280,41), scrollerOption_Border, &mViews.ip1Text);
		scr->Show();
		lbl = new CLabelView(vc, SRect(282,25,288,41));
		lbl->SetText("\p.");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(290,15,330,41), scrollerOption_Border, &mViews.ip2Text);
		scr->Show();
		lbl = new CLabelView(vc, SRect(332,25,338,41));
		lbl->SetText("\p.");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(340,15,380,41), scrollerOption_Border, &mViews.ip3Text);
		scr->Show();
		lbl = new CLabelView(vc, SRect(382,25,388,41));
		lbl->SetText("\p.");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(390,15,430,41), scrollerOption_Border, &mViews.ip4Text);
		scr->Show();
		
		// make label view
		lbl = new CLabelView(vc, SRect(20,55,430,200));
		lbl->SetText("\pIf you have multihoming/IP-aliasing configured on your computer, you can specify which address to bind to here.\r\rIf you have only set up a single IP address, or if you wish to use your default address, leave the IP set to 0.0.0.0.");
		lbl->Show();
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}


CContainerView *CMyOptionsWin::MakeMiscTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	CCheckBoxView *chk;
	CBoxView *box;
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,550,280));

	try
	{
		// make box
		box = new CBoxView(vc, SRect(10,10,550,106), boxStyle_Etched);
		box->Show();
		//Ban Delay
		
		lbl = new CLabelView(vc, SRect(20,22,288,80));
		lbl->SetText("\pTemp Ban Delay : ");
		lbl->Show();
		
		
		lbl = new CLabelView(vc, SRect(180,22,388,80));
		lbl->SetText("\pMin.");
		lbl->Show();
		
		
		
		scr = MakeTextBoxView(vc, SRect(124,17,175,40), scrollerOption_Border, &mViews.bandelay);
		scr->Show();
		
		
		//Away Delay
		
		lbl = new CLabelView(vc, SRect(20,52,288,110));
		
		lbl->SetText("\pAway delay : ");
		lbl->Show();
		
		lbl = new CLabelView(vc, SRect(180,52,388,110));
		lbl->SetText("\pSec.");
		lbl->Show();
		
		scr = MakeTextBoxView(vc, SRect(124,47,175,70), scrollerOption_Border, &mViews.awaydelay);
		scr->Show();
		
		
		//Kick Unnamed Users
		chk = new CCheckBoxView(vc, SRect(20,82,290,100));
		chk->SetTitle("\pKick Banned Users Names :");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.kickunnamedusers = chk;
		
		//mWrongUserName
		
		scr = MakeTextBoxView(vc, SRect(280,60,530,100), scrollerOption_Border, &mViews.wrongusername);
		scr->Show();
		
		
		// make box
		box = new CBoxView(vc, SRect(10,116,550,240), boxStyle_Etched);
		box->Show();
		
		
		//Incorrect login msg
		
		lbl = new CLabelView(vc, SRect(20,125,288,145));
		
		lbl->SetText("\pIncorrect login message : ");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(280,120,530,145), scrollerOption_Border, &mViews.incorrectlogin);
		scr->Show();
		
		
		//Send message before downloading
		
		lbl = new CLabelView(vc, SRect(20,155,288,170));
		
		lbl->SetText("\pChat before downloading : ");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(280,150,530,175), scrollerOption_Border, &mViews.messagebeforedl);
		scr->Show();
		
		//Post message before downloading
		
		lbl = new CLabelView(vc, SRect(20,185,288,200));
		
		lbl->SetText("\pPost before downloading : ");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(280,180,530,205), scrollerOption_Border, &mViews.postbeforedl);
		scr->Show();
		
		//Accept private message and private chats
		
		lbl = new CLabelView(vc, SRect(20,215,288,230));
		
		lbl->SetText("\pAccept private messages and private chats : ");
		lbl->Show();
		scr = MakeTextBoxView(vc, SRect(280,210,530,235), scrollerOption_Border, &mViews.acceptprivate);
		scr->Show();
		
		
		
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

CContainerView *CMyOptionsWin::MakeLoginMessageTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	CCheckBoxView *chk;
	//CBoxView *box;
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,560,280));





		

	try
	{
	
	// make box
		//box = new CBoxView(vc, SRect(10,10,440,136), boxStyle_Etched);
		//box->Show();
		
		//Login Message check
		
		chk = new CCheckBoxView(vc, SRect(20,10,280,36));
		chk->SetTitle("\pUse login message");
		chk->SetAutoMark(true);
		chk->Show();
		mViews.loginmessage = chk;
		
		
		
		lbl = new CLabelView(vc, SRect(20,40,288,76));
		lbl->SetText("\pLogin Message : ");
		lbl->Show();
		
		scr = MakeTextBoxView(vc, SRect(120,60,530,148), scrollerOption_Border, &mViews.loginmessagetext);
		scr->Show();
		
		
		
		// make checkboxes
		
		
		chk = new CCheckBoxView(vc, SRect(20,170,120,188));
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->SetTitle("\pChat Message");
		chk->SetID(10);
		chk->Show();
		mViews.logmsgchat = chk;
		
		
		chk = new CCheckBoxView(vc, SRect(20,190,220,208));
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->SetTitle("\pPrivate Message with Reply/Dismiss");
		chk->SetID(11);
		chk->Show();
		mViews.logmsgpvmsg = chk;
		
		chk = new CCheckBoxView(vc, SRect(20,210,190,228));
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->SetTitle("\pPrivate Message with Dismiss");
		chk->SetID(12);
		chk->Show();
		mViews.logmsgpvmsg2 = chk;
		
		chk = new CCheckBoxView(vc, SRect(20,230,190,248));
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->SetTitle("\pBroadcast Message");
		chk->SetID(13);
		chk->Show();
		mViews.logmsgbroad = chk;
		
		mViews.logmsgchat->SetExclusiveNext(mViews.logmsgpvmsg);
		mViews.logmsgpvmsg->SetExclusiveNext(mViews.logmsgpvmsg2);
		mViews.logmsgpvmsg2->SetExclusiveNext(mViews.logmsgbroad);
		mViews.logmsgbroad->SetExclusiveNext(mViews.logmsgchat);
		
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

#if BANNER_SERVER & NETWORK_SERVER
void CMyOptionsWin::SetInfo(const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl, const Uint8 *inWrongUserName, const Uint8 *inAcceptPrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc, 
							Uint8* inSerialNumber, const Uint8 *inAdminLogin, 
							Uint8 *inAgreementBanner, Uint8 *inAgreementBannerUrl, 
							Uint8 *inAgreementUrl, CPtrList<BanRecord> *inPermBanList, 
							const SMyOptions& inOptions)
#elif BANNER_SERVER
void CMyOptionsWin::SetInfo(const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl, const Uint8 *inWrongUserName, const Uint8 *inAcceptPrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc, 
							const Uint8 *inAdminLogin, Uint8 *inAgreementBanner, 
							Uint8 *inAgreementBannerUrl, Uint8 *inAgreementUrl, 
							CPtrList<BanRecord> *inPermBanList, 
							const SMyOptions& inOptions)
#elif NETWORK_SERVER
void CMyOptionsWin::SetInfo(const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl, const Uint8 *inWrongUserName, const Uint8 *inAcceptPrivate, const Uint8 *inRefusePrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc, 
							Uint8* inSerialNumber, const Uint8 *inAdminLogin, 
							CPtrList<BanRecord> *inPermBanList, 
							const SMyOptions& inOptions)
#else
void CMyOptionsWin::SetInfo(const Uint8 *inName, const Uint8 *inIncorrectlogin, const Uint8 *inMessagebeforedl, const Uint8 *inPostbeforedl,const Uint8 *inWrongUserName,  const Uint8 *inAcceptPrivate, const Uint8 *inRefusePrivate, const Uint8 *inLoginMessageText, const Uint8 *inDesc, 
							const Uint8 *inAdminLogin, 
							CPtrList<BanRecord> *inPermBanList, 
							const SMyOptions& inOptions)
#endif
{
	Uint8 str[32];
	
	// name
	if (inName) 
		mViews.nameText->SetText(inName + 1, inName[0]);
	
	// Incorrect login
	if (inIncorrectlogin) 
		mViews.incorrectlogin->SetText(inIncorrectlogin + 1, inIncorrectlogin[0]);

	// ChatBefore DL
	if (inMessagebeforedl) 
		mViews.messagebeforedl->SetText(inMessagebeforedl + 1, inMessagebeforedl[0]);

	// PostBefore DL
	if (inPostbeforedl) 
		mViews.postbeforedl->SetText(inPostbeforedl + 1, inPostbeforedl[0]);
		
		//WrongUserName
		
	if (inWrongUserName) 
		mViews.wrongusername->SetText(inWrongUserName + 1, inWrongUserName[0]);
		
	// RefusePrivate
	if (inAcceptPrivate) 
		mViews.acceptprivate->SetText(inAcceptPrivate + 1, inAcceptPrivate[0]);
		
	// LoginMessage
	if (inLoginMessageText) 
		mViews.loginmessagetext->SetText(inLoginMessageText + 1, inLoginMessageText[0]);


	
	// description	
	if (inDesc) 
		mViews.descText->SetText(inDesc + 1, inDesc[0]);
		
#if NETWORK_SERVER
	// serial number
	if (inSerialNumber)
		mViews.serialNumberText->SetText(inSerialNumber + 1, inSerialNumber[0]);
#endif

#if BANNER_SERVER
	// server banner	
	if (inAgreementBanner)
		mViews.agreementBannerText->SetText(inAgreementBanner + 1, inAgreementBanner[0]);
	
	if (inAgreementBannerUrl)
		mViews.agreementBannerUrlText->SetText(inAgreementBannerUrl + 1, inAgreementBannerUrl[0]);
	
	if (inAgreementUrl)
		mViews.agreementUrlText->SetText(inAgreementUrl + 1, inAgreementUrl[0]);
#endif
		
	// administration
	SetAdminInfo(inAdminLogin);

	// IP address
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.stIPAddress.un_IP.stB_IP.nB_IP1);
	mViews.ip1Text->SetText(str+1, str[0]);
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.stIPAddress.un_IP.stB_IP.nB_IP2);
	mViews.ip2Text->SetText(str+1, str[0]);
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.stIPAddress.un_IP.stB_IP.nB_IP3);
	mViews.ip3Text->SetText(str+1, str[0]);
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.stIPAddress.un_IP.stB_IP.nB_IP4);
	mViews.ip4Text->SetText(str+1, str[0]);
	
	// port number
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.nBasePortNum);
	mViews.portNumText->SetText(str+1, str[0]);

	// max downloads
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.nMaxDownloads);
	mViews.maxDownText->SetText(str+1, str[0]);
	
	// Delay away
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.nawaydelay);
	mViews.awaydelay->SetText(str+1, str[0]);
	
	
	// Delay ban
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.nbandelay);
	mViews.bandelay->SetText(str+1, str[0]);
	
	// max downloads per Client
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.nMaxDownloadsClient);
	mViews.maxDownClientText->SetText(str+1, str[0]);

	// max connections per IP
	str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inOptions.nMaxConnectionsIP);
	mViews.maxConnectionsIPText->SetText(str+1, str[0]);
	
	
	
	
	// misc flags
	mViews.useTracker->SetMark((inOptions.nOpts & myOpt_UseTracker) != 0);
	mViews.logDownloads->SetMark((inOptions.nOpts & myOpt_LogDownloads) != 0);
	mViews.logUploads->SetMark((inOptions.nOpts & myOpt_LogUploads) != 0);
	mViews.logChats->SetMark((inOptions.nOpts & myOpt_LogChats) != 0);
	mViews.logConnects->SetMark((inOptions.nOpts & myOpt_LogConnects) != 0);
	mViews.logAccountChanges->SetMark((inOptions.nOpts & myOpt_LogAccountChanges) != 0);
	mViews.confirmQuit->SetMark((inOptions.nOpts & myOpt_ConfirmQuit) != 0);
	mViews.csvMode->SetMark((inOptions.nOpts & myOpt_CsvMode) != 0);
	mViews.botMode->SetMark((inOptions.nOpts & myOpt_BotMode) != 0);
	mViews.showKick->SetMark((inOptions.nOpts & myOpt_showKick) != 0);
	mViews.kickunnamedusers->SetMark((inOptions.nOpts & myOpt_kickunnamedusers) != 0);
	mViews.loginmessage->SetMark((inOptions.nOpts & myOpt_loginmessage) != 0);
	mViews.logmsgchat->SetMark((inOptions.nOpts & myOpt_logmsgchat) != 0);
	mViews.logmsgpvmsg->SetMark((inOptions.nOpts & myOpt_logmsgpvmsg) != 0);
	mViews.logmsgpvmsg2->SetMark((inOptions.nOpts & myOpt_logmsgpvmsg2) != 0);
	mViews.logmsgbroad->SetMark((inOptions.nOpts & myOpt_logmsgbroad) != 0);
	

#if BANNER_SERVER
	mViews.agreemLocal->SetMark((inOptions.nOpts & myOpt_AgreementImageUrl) != 0);
	mViews.agreemRemote->SetMark((inOptions.nOpts & myOpt_AgreementImageUrl) == 0);	

	UpdateBannerTab();
#endif
	
	// permanent ban
	mViews.permBanList->SetItemsFromList(inPermBanList);
}

#if BANNER_SERVER & NETWORK_SERVER
void CMyOptionsWin::GetInfo(Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outWrongUserName, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc, 
							Uint8* outSerialNumber, Uint8 *outAdminLogin, 
							Uint8 *outAgreementBanner,  Uint8 *outAgreementBannerUrl, 
							Uint8 *outAgreementUrl, 
							CPtrList<BanRecord> *outPermBanList, 
							SMyOptions& ioOptions)
#elif BANNER_SERVER
void CMyOptionsWin::GetInfo(Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outWrongUserName, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc, 
							Uint8 *outAdminLogin, Uint8 *outAgreementBanner,  
							Uint8 *outAgreementBannerUrl, Uint8 *outAgreementUrl, 
							CPtrList<BanRecord> *outPermBanList, 
							SMyOptions& ioOptions)
#elif NETWORK_SERVER
void CMyOptionsWin::GetInfo(Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outWrongUserName, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc, 
							Uint8* outSerialNumber, Uint8 *outAdminLogin, 
							CPtrList<BanRecord> *outPermBanList, 
							SMyOptions& ioOptions)
#else
void CMyOptionsWin::GetInfo(Uint8 *outName, Uint8 *outIncorrectlogin, Uint8 *outMessagebeforedl, Uint8 *outPostbeforedl, Uint8 *outWrongUserName, Uint8 *outAcceptPrivate, Uint8 *outLoginMessageText, Uint8 *outDesc, 
							Uint8 *outAdminLogin, 
							CPtrList<BanRecord> *outPermBanList, 
							SMyOptions& ioOptions)
#endif
{
	Uint8 str[32];

	// name
	if (outName) 
		outName[0] = mViews.nameText->GetText(outName + 1, 31);
	
	//Incorrect login
	if (outIncorrectlogin) 
		outIncorrectlogin[0] = mViews.incorrectlogin->GetText(outIncorrectlogin + 1, 255);
	
	// ChatBefore DL
	if (outMessagebeforedl) 
		outMessagebeforedl[0] = mViews.messagebeforedl->GetText(outMessagebeforedl + 1, 255);
	
	// PostBefore DL
	if (outPostbeforedl) 
		outPostbeforedl[0] = mViews.postbeforedl->GetText(outPostbeforedl + 1, 255);
		
	// WrongUserName
	if (outWrongUserName) 
		outWrongUserName[0] = mViews.wrongusername->GetText(outWrongUserName + 1, 255);
		
	// RefusePrivate
	if (outAcceptPrivate) 
		outAcceptPrivate[0] = mViews.acceptprivate->GetText(outAcceptPrivate + 1, 255);
	
	// LoginMessageText
	if (outLoginMessageText) 
		outLoginMessageText[0] = mViews.loginmessagetext->GetText(outLoginMessageText + 1, 255);
	
	
	// description
	if (outDesc) 
		outDesc[0] = mViews.descText->GetText(outDesc + 1, 255);
	
#if NETWORK_SERVER
	// serial number
	GetSerialNumber(outSerialNumber);
#endif

#if BANNER_SERVER
	// server banner
	if (outAgreementBanner)
	 	outAgreementBanner[0] = mViews.agreementBannerText->GetText(outAgreementBanner + 1, 255);

	if (outAgreementBannerUrl)
	 	outAgreementBannerUrl[0] = mViews.agreementBannerUrlText->GetText(outAgreementBannerUrl + 1, 255);

	if (outAgreementUrl)
	{
	 	Uint8 agreementUrl[256];
	 	agreementUrl[0] = mViews.agreementUrlText->GetText(agreementUrl + 1, 255);
	 	outAgreementUrl[0] = UTransport::CleanUpAddressText(kInternetNameAddressType, agreementUrl + 1, agreementUrl[0], outAgreementUrl + 1, 255);
	}
#endif

	// administration
	GetAdminInfo(outAdminLogin);

	// IP address
	Int32 nIPAddress;
	ioOptions.stIPAddress.un_IP.stDW_IP.nDW_IP = 0;
		
	str[0] = mViews.ip1Text->GetText(str+1, sizeof(str)-1);
	nIPAddress = UText::TextToInteger(str+1, str[0]);
	if (nIPAddress >= 0 && nIPAddress < 256) 
		ioOptions.stIPAddress.un_IP.stB_IP.nB_IP1 = nIPAddress;
		
	str[0] = mViews.ip2Text->GetText(str+1, sizeof(str)-1);
	nIPAddress = UText::TextToInteger(str+1, str[0]);
	if (nIPAddress >= 0 && nIPAddress < 256)
		ioOptions.stIPAddress.un_IP.stB_IP.nB_IP2 = nIPAddress;

	str[0] = mViews.ip3Text->GetText(str+1, sizeof(str)-1);
	nIPAddress = UText::TextToInteger(str+1, str[0]);
	if (nIPAddress >= 0 && nIPAddress < 256)
		ioOptions.stIPAddress.un_IP.stB_IP.nB_IP3 = nIPAddress;
		
	str[0] = mViews.ip4Text->GetText(str+1, sizeof(str)-1);
	nIPAddress = UText::TextToInteger(str+1, str[0]);
	if (nIPAddress >= 0 && nIPAddress < 256)
		ioOptions.stIPAddress.un_IP.stB_IP.nB_IP4 = nIPAddress;

	// port number
	str[0] = mViews.portNumText->GetText(str+1, sizeof(str)-1);
	Int32 nBasePortNum = UText::TextToInteger(str+1, str[0]);
	
	if (nBasePortNum > 0 && nBasePortNum < max_Uint16)
		ioOptions.nBasePortNum = nBasePortNum;

	// max downloads
	str[0] = mViews.maxDownText->GetText(str+1, sizeof(str)-1);
	Int32 nMaxDownloads = UText::TextToInteger(str+1, str[0]);
		
	if (nMaxDownloads > 0 && nMaxDownloads < max_Uint16)
		ioOptions.nMaxDownloads = nMaxDownloads;
	
	// Away Delay
	str[0] = mViews.awaydelay->GetText(str+1, sizeof(str)-1);
	Int32 nawaydelay = UText::TextToInteger(str+1, str[0]);
		
	if (nawaydelay > 0 && nawaydelay < max_Uint16)
		ioOptions.nawaydelay = nawaydelay;
	
	
	// Ban Delay
	str[0] = mViews.bandelay->GetText(str+1, sizeof(str)-1);
	Int32 nbandelay = UText::TextToInteger(str+1, str[0]);
		
	if (nbandelay > 0 && nbandelay < max_Uint16)
		ioOptions.nbandelay = nbandelay;
	
	
	// max downloads per Client
	str[0] = mViews.maxDownClientText->GetText(str+1, sizeof(str)-1);
	Int32 nMaxDownloadsClient = UText::TextToInteger(str+1, str[0]);
		
	if (nMaxDownloadsClient > 0 && nMaxDownloadsClient < max_Uint16)
		ioOptions.nMaxDownloadsClient = nMaxDownloadsClient;
	
	// max connections per IP
	str[0] = mViews.maxConnectionsIPText->GetText(str+1, sizeof(str)-1);
	Int32 nMaxConnectionsIP = UText::TextToInteger(str+1, str[0]);
		
	if (nMaxConnectionsIP >= 0 && nMaxConnectionsIP < max_Uint16)
		ioOptions.nMaxConnectionsIP = nMaxConnectionsIP;

	// misc flags
	Uint32 nOpts = 0;
		
	if (mViews.useTracker->GetMark()) nOpts |= myOpt_UseTracker;
	if (mViews.logDownloads->GetMark()) nOpts |= myOpt_LogDownloads;
	if (mViews.logUploads->GetMark()) nOpts |= myOpt_LogUploads;
	if (mViews.logChats->GetMark()) nOpts |= myOpt_LogChats;
	if (mViews.logConnects->GetMark()) nOpts |= myOpt_LogConnects;
	if (mViews.logAccountChanges->GetMark()) nOpts |= myOpt_LogAccountChanges;
	if (mViews.confirmQuit->GetMark()) nOpts |= myOpt_ConfirmQuit;
	if (mViews.csvMode->GetMark()) nOpts |= myOpt_CsvMode;
	if (mViews.botMode->GetMark()) nOpts |= myOpt_BotMode;
	if (mViews.showKick->GetMark()) nOpts |= myOpt_showKick;
	if (mViews.kickunnamedusers->GetMark()) nOpts |= myOpt_kickunnamedusers;
	if (mViews.loginmessage->GetMark()) nOpts |= myOpt_loginmessage;
	if (mViews.logmsgchat->GetMark()) nOpts |= myOpt_logmsgchat;
	if (mViews.logmsgpvmsg->GetMark()) nOpts |= myOpt_logmsgpvmsg;
	if (mViews.logmsgpvmsg2->GetMark()) nOpts |= myOpt_logmsgpvmsg2;
	if (mViews.logmsgbroad->GetMark()) nOpts |= myOpt_logmsgbroad;

	
#if BANNER_SERVER
	if (mViews.agreemLocal->GetMark()) nOpts |= myOpt_AgreementImageUrl;
#endif

	ioOptions.nOpts = nOpts;
	
	// permanent ban
	mViews.permBanList->ModifySelectedPermBan();
	mViews.permBanList->GetItemsFromList(outPermBanList);
}

#if NETWORK_SERVER
void CMyOptionsWin::GetSerialNumber(Uint8 *outSerialNumber)
{
	if (outSerialNumber)
		outSerialNumber[0] = mViews.serialNumberText->GetText(outSerialNumber + 1, 31);
}

void CMyOptionsWin::FocusSerialNumber()
{
	mViews.vc->SetFocusView(mViews.tabs);
	mViews.tabs->SetCurrentTab(2);
	
	CContainerView *pContainerView = dynamic_cast<CContainerView *>(mViews.tabs->GetTabView(2));
	if (pContainerView)
	{
		pContainerView->SetFocusView(mViews.serialNumberScroll);
		mViews.serialNumberText->SetSelection(0, max_Uint32);
	}
}
#endif

#if BANNER_SERVER
void CMyOptionsWin::UpdateBannerTab()
{
	if (mViews.agreemLocal->GetMark())
	{
		mViews.agreementBannerScroll->Enable();
		mViews.agreementBannerUrlScroll->Enable();
		mViews.agreementUrlScroll->Disable();
	}
	else if (mViews.agreemRemote->GetMark())
	{
		mViews.agreementBannerScroll->Disable();
		mViews.agreementBannerUrlScroll->Disable();
		mViews.agreementUrlScroll->Enable();
	}

	mViews.agreementBannerScroll->Refresh();
	mViews.agreementBannerUrlScroll->Refresh();
	mViews.agreementUrlScroll->Refresh();
}
#endif

void CMyOptionsWin::SetAdminInfo(const Uint8 *inAdminLogin)
{
	if (inAdminLogin && inAdminLogin[0])
		mViews.adminLbl->SetText(inAdminLogin + 1, inAdminLogin[0]);
	else
		mViews.adminLbl->SetText("admin", 5);
}

void CMyOptionsWin::GetAdminInfo(Uint8 *outAdminLogin)
{
	if (outAdminLogin)
	{
	 	outAdminLogin[0] = mViews.adminLbl->GetText(outAdminLogin + 1, 31);
	 	
	 	if (outAdminLogin[0] == 0)
	 		pstrcpy(outAdminLogin, "\padmin");
	}
}

void CMyOptionsWin::AddPermBan()
{
	Uint32 nIndex = mViews.permBanList->AddPermBan();
	if (!nIndex)
		return;
	
	if (nIndex == 1)
		mViews.permBanScroll->ScrollToTop();
	else
		mViews.permBanScroll->ScrollToBottom();
}

void CMyOptionsWin::DeletePermBan()
{
	mViews.permBanList->DeleteSelectedPermBan();
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyToolbarWin::CMyToolbarWin()
	: CWindow(SRect(0,0,200,200), windowLayer_Float, windowOption_CloseBox)
{
#if WIN32
	_SetWinIcon(*this, 210);
#endif

	enum {
		buttonHeight = 24,
		buttonWidth = 146,
		space = 3
	};
	
	CContainerView *vc;
	CIconButtonView *btn;
	SRect r;

	// setup window
	SetTitle("\pGLoarbLine Server");

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,200,200));
	vc->Show();
	
	const Uint8 *btnNames[] = {
		"\pOptions", "\pBroadcast", "\pConnect as Admin", "\pReload", "\pLog", "\pStatistics", "\pQuit"
	};
	const Int16 btnIcons[] = {
		301, 302, 410, 205, 300, 300, 303
	};
	const Uint32 btnIDs[] = {
		viewID_Options, viewID_Broadcast, viewID_Connect, viewID_Reload, viewID_ShowLog, viewID_ShowStats, viewID_Quit
	};
	enum { btnCount = sizeof(btnIcons) / sizeof(Int16) };
	CIconButtonView *btns[btnCount];
	
	r.top = 1;
	r.left = space;
	r.right = r.left + buttonWidth;

#if SHOW_SERVER_INFO_IN_TOOLBAR
#if WIN32
	r.bottom = r.top + 32;
#else
	r.bottom = r.top + 23;
#endif

	(new CBoxView(vc, SRect(0, 0, r.right + 4, r.bottom + 1), boxStyle_Sunken))->Show();
	
	mInfoLbl = new CMyClickLabelView(vc, r);
	mInfoLbl->SetFont(kDefaultFont, nil, 9);
	mInfoLbl->SetID(viewID_ToolbarInfo);
	mInfoLbl->Show();

	r.top = r.bottom + space;

#endif

	for (Uint16 i=0; i<btnCount; i++)
	{
		r.bottom = r.top + buttonHeight;
		
		btn = new CIconButtonView(vc, r);
		btn->SetOptions(iconBtn_MediumTitleLeft);
	#if WIN32
		btn->SetCanFocus(true);
	#endif
		btn->SetIconID(btnIcons[i]);
		btn->SetTitle(btnNames[i]);
		btn->SetID(btnIDs[i]);
		btn->Show();
		
		btns[i] = btn;
		r.top += buttonHeight + space;
	}

	// resize window
	r.top = r.left = 0;
	r.bottom += space;
	r.right = space + buttonWidth + space - 1;
	vc->SetBounds(r);
	SetBounds(r);
	
#if SHOW_SERVER_INFO_IN_TOOLBAR
	mTrueHeight = r.GetHeight();
#endif
}

#if SHOW_SERVER_INFO_IN_TOOLBAR
void CMyToolbarWin::ToggleState()
{
	SRect bounds;
	GetBounds(bounds);
	
	if (bounds.GetHeight() < mTrueHeight)
		bounds.bottom = bounds.top + mTrueHeight;
	else
	#if WIN32
		bounds.bottom = bounds.top + 32;
	#else
		bounds.bottom = bounds.top + 25;
	#endif
	
	SetBounds(bounds);
	SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
}
#endif

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySelectAdminWin::CMySelectAdminWin()
	: CWindow(SRect(0,0,320,300), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	CLabelView *lbl;
	CButtonView *btn;

	// setup window
	SetTitle("\pSelect Head Admin Account");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,320,300));
	vc->Show();
	
	// make text label
	lbl = new CLabelView(vc, SRect(10,10,310,46));
	lbl->SetText("\pSelect the account you wish to set as the head Admin account.");
	lbl->Show();

	// make list scroller
	scr = new CScrollerView(vc, SRect(10,50,310,230));
	scr->SetOptions(scrollerOption_Border | scrollerOption_VertBar | LIST_BACKGROUND_OPTION);
	scr->SetCanFocus(true);
	scr->Show();
	vc->SetFocusView(scr);

	// make account list
	mViews.accountListView = new CMyAccountListView(scr, SRect(0,0,scr->GetVisibleContentWidth(),scr->GetVisibleContentHeight()));
	mViews.accountListView->SetBehaviour(itemBehav_SelectOnlyOne | itemBehav_DoubleClickAction);
	mViews.accountListView->SetSizing(sizing_FullHeight);
	mViews.accountListView->SetCanFocus(true);
	mViews.accountListView->SetID(4);
	mViews.accountListView->Show();
	
	// make buttons
	btn = new CButtonView(vc, SRect(10,236,110,262));
	btn->SetTitle("\pCreate New...");
	btn->SetID(3);
	btn->Show();

	SButtons btns[] = {{1, "\pOK", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(130,264,310,290), btns);
}

void CMySelectAdminWin::SetInfo(const Uint8 *inAdminLogin)
{
	mViews.accountListView->SetInfo(inAdminLogin);	
}

bool CMySelectAdminWin::GetInfo(Uint8 *outAdminLogin)
{
	return mViews.accountListView->GetInfo(outAdminLogin);	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewAdminAccountWin::CMyNewAdminAccountWin()
	: CWindow(SRect(0,0,390,200), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	CBoxView *box;
	CLabelView *lbl;

	// setup window
	SetTitle("\pCreate New Admin Account");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,390,200));
	vc->Show();
	mViews.containerView = vc;
	
	// make text label
	lbl = new CLabelView(vc, SRect(10,10,380,46));
	lbl->SetText("\pPlease enter a login and password for the new head Admin account.");
	lbl->Show();

	// make box
	box = new CBoxView(vc, SRect(10,50,380,158));
	box->SetStyle(boxStyle_Sunken);
	box->SetSizing(sizing_VertSticky);
	box->Show();

	// make admin text
	lbl = new CLabelView(vc, SRect(20,63,134,79));
	lbl->SetText("\pLogin:");
	lbl->Show();
	scr = MakeTextBoxView(vc, SRect(138,58,366,84), scrollerOption_Border, &mViews.loginText);
	mViews.loginText->SetID(10);
	scr->Show();
	vc->SetFocusView(scr);

	// make pasword boxes
	lbl = new CLabelView(vc, SRect(20,93,134,109));
	lbl->SetText("\pPassword:");
	lbl->Show();
	scr = MakePasswordBoxView(vc, SRect(138,88,366,114), scrollerOption_Border, &mViews.passwordText1);
	mViews.passwordText1->SetID(10);
	scr->Show();
	mViews.passwordScr1 = scr;
	
	lbl = new CLabelView(vc, SRect(20,123,134,139));
	lbl->SetText("\pVerify Password:");
	lbl->Show();
	scr = MakePasswordBoxView(vc, SRect(138,118,366,144), scrollerOption_Border, &mViews.passwordText2);
	mViews.passwordText2->SetID(10);
	scr->Show();
	
	// make buttons
	SButtons btns[] = {{1, "\pCreate", btnOpt_Default, &mViews.createBtn}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(200,168,380,194), btns);
	mViews.createBtn->Disable();
}

void CMyNewAdminAccountWin::SetInfo(const Uint8 *inAdminLogin)
{
	if (inAdminLogin && inAdminLogin[0])
		mViews.loginText->SetText(inAdminLogin + 1, inAdminLogin[0]);
}

bool CMyNewAdminAccountWin::GetInfo(Uint8 *outAdminLogin, Uint8 *outAdminPassword)
{
	if (outAdminLogin)
		outAdminLogin[0] = mViews.loginText->GetText(outAdminLogin + 1, 31);
	
	if (outAdminPassword) 
	{
		if (mViews.passwordText1->IsEmpty() || mViews.passwordText2->IsEmpty())
		{
			outAdminPassword[0] = 0;
			return false;
		}
		
		if (mViews.passwordText1->GetPasswordSize() > 31 || mViews.passwordText2->GetPasswordSize() > 31)
		{
			mViews.passwordText1->SetText(nil, 0);
			mViews.passwordText2->SetText(nil, 0);
			mViews.containerView->SetFocusView(mViews.passwordScr1);
			mViews.createBtn->Disable();

			gApp->DisplayStandardMessage("\pPassword too long", "\pPlease select a password that is no longer than 31 characters.", icon_Stop, 1);
			
			outAdminPassword[0] = 0;
			return false;
		}
		
		Uint8 psPassword1[32];
		psPassword1[0] = mViews.passwordText1->GetText(psPassword1 + 1, sizeof(psPassword1) - 1);
			
		Uint8 psPassword2[32];
		psPassword2[0] = mViews.passwordText2->GetText(psPassword2 + 1, sizeof(psPassword2) - 1);

		if (UMemory::Compare(psPassword1 + 1, psPassword1[0], psPassword2 + 1, psPassword2[0]))
		{
			outAdminPassword[0] = 0;
			return false;
		}
		
		UMemory::Copy(outAdminPassword, psPassword1, psPassword1[0] + 1);
	}
	
	return true;
}

void CMyNewAdminAccountWin::UpdateButtons()
{
	if (mViews.loginText->IsEmpty() || mViews.passwordText1->IsEmpty() || mViews.passwordText2->IsEmpty())
		mViews.createBtn->Disable();
	else
	{
		Uint8 psPassword1[32];
		psPassword1[0] = mViews.passwordText1->GetText(psPassword1 + 1, sizeof(psPassword1) - 1);
		
		Uint8 psPassword2[32];
		psPassword2[0] = mViews.passwordText2->GetText(psPassword2 + 1, sizeof(psPassword2) - 1);

		if (!UMemory::Compare(psPassword1 + 1, psPassword1[0], psPassword2 + 1, psPassword2[0]))
			mViews.createBtn->Enable();
		else
			mViews.createBtn->Disable();
	}
	
	mViews.loginText->SetHasChanged(false);
	mViews.passwordText1->SetHasChanged(false);
	mViews.passwordText2->SetHasChanged(false);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySetAdminPassWin::CMySetAdminPassWin()
	: CWindow(SRect(0,0,390,200), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	CBoxView *box;
	CLabelView *lbl;
	CButtonView *btn;

	// setup window
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,390,200));
	vc->Show();
	mViews.containerView = vc;
	
	// make text label
	lbl = new CLabelView(vc, SRect(10,10,380,40));
	lbl->Show();
	mViews.messageLbl = lbl;

	// make box
	box = new CBoxView(vc, SRect(10,44,380,154));
	box->SetStyle(boxStyle_Sunken);
	box->SetSizing(sizing_VertSticky);
	box->Show();

	// make admin label
	lbl = new CLabelView(vc, SRect(20,59,170,75));
	lbl->SetText("\pAdmin Account Login:");
	lbl->SetSizing(sizing_VertSticky);
	lbl->Show();
	lbl = new CLabelView(vc, SRect(174,59,366,75));
	lbl->SetSizing(sizing_VertSticky);
	lbl->Show();
	mViews.loginLbl = lbl;

	// make pasword boxes
	lbl = new CLabelView(vc, SRect(20,89,170,105));
	lbl->SetText("\pNew Admin Password:");
	lbl->SetSizing(sizing_VertSticky);
	lbl->Show();
	scr = MakePasswordBoxView(vc, SRect(174,84,366,110), scrollerOption_Border, &mViews.passwordText1);
	scr->SetSizing(sizing_VertSticky);
	mViews.passwordText1->SetID(10);
	scr->Show();
	mViews.passwordScr1 = scr;
	vc->SetFocusView(scr);
	
	lbl = new CLabelView(vc, SRect(20,119,170,135));
	lbl->SetText("\pVerify New Password:");
	lbl->SetSizing(sizing_VertSticky);
	lbl->Show();
	scr = MakePasswordBoxView(vc, SRect(174,114,366,140), scrollerOption_Border, &mViews.passwordText2);
	scr->SetSizing(sizing_VertSticky);
	mViews.passwordText2->SetID(10);
	scr->Show();
	
	// make buttons
	SButtons btns[] = {{1, "\pOK", btnOpt_Default, &mViews.saveBtn}, {2, "\pCancel", btnOpt_Cancel, &btn}};
	CButtonView::BuildButtons(vc, SRect(200,164,380,190), btns);
	
	btn->SetSizing(sizing_VertSticky);
	mViews.saveBtn->SetSizing(sizing_VertSticky);
	mViews.saveBtn->Disable();
}

void CMySetAdminPassWin::SetInfo(const Uint8 *inAdminLogin, const void *inMessage, Uint32 inMessageSize)
{
	if (inAdminLogin) 
		mViews.loginLbl->SetText(inAdminLogin);
		
	if (inMessage && inMessageSize)
	{
		mViews.messageLbl->SetText(inMessage, inMessageSize);
		
		SRect stMessageBounds;
		mViews.messageLbl->GetBounds(stMessageBounds);

		Int32 nMessageHeight = mViews.messageLbl->GetTextHeight() + 6;
		Int32 nVertDepl = nMessageHeight - stMessageBounds.GetHeight();

		if (nVertDepl)
		{
			// set window bounds
			SRect stBounds;
			GetBounds(stBounds);
			stBounds.bottom += nVertDepl;
			SetBounds(stBounds);
			
			// set label bounds
			stMessageBounds.bottom += nVertDepl;
			mViews.messageLbl->SetBounds(stMessageBounds);
		}
	}
}

bool CMySetAdminPassWin::GetInfo(Uint8 *outAdminPassword)
{
	if (outAdminPassword) 
	{
		if (mViews.passwordText1->IsEmpty() || mViews.passwordText2->IsEmpty())
		{
			outAdminPassword[0] = 0;
			return false;
		}
		
		if (mViews.passwordText1->GetPasswordSize() > 31 || mViews.passwordText2->GetPasswordSize() > 31)
		{
			mViews.passwordText1->SetText(nil, 0);
			mViews.passwordText2->SetText(nil, 0);
			mViews.containerView->SetFocusView(mViews.passwordScr1);
			mViews.saveBtn->Disable();
			
			gApp->DisplayStandardMessage("\pPassword too long", "\pPlease select a password that is no longer than 31 characters.", icon_Stop, 1);
			
			outAdminPassword[0] = 0;
			return false;
		}

		Uint8 psPassword1[32];
		psPassword1[0] = mViews.passwordText1->GetText(psPassword1 + 1, sizeof(psPassword1) - 1);
			
		Uint8 psPassword2[32];
		psPassword2[0] = mViews.passwordText2->GetText(psPassword2 + 1, sizeof(psPassword2) - 1);

		if (UMemory::Compare(psPassword1 + 1, psPassword1[0], psPassword2 + 1, psPassword2[0]))
		{
			outAdminPassword[0] = 0;
			return false;
		}	
			
		UMemory::Copy(outAdminPassword, psPassword1, psPassword1[0] + 1);	
	}
	
	return true;
}

void CMySetAdminPassWin::UpdateButtons()
{
	if (mViews.passwordText1->IsEmpty() || mViews.passwordText2->IsEmpty())
		mViews.saveBtn->Disable();
	else
	{
		Uint8 psPassword1[32];
		psPassword1[0] = mViews.passwordText1->GetText(psPassword1 + 1, sizeof(psPassword1) - 1);
		
		Uint8 psPassword2[32];
		psPassword2[0] = mViews.passwordText2->GetText(psPassword2 + 1, sizeof(psPassword2) - 1);

		if (!UMemory::Compare(psPassword1 + 1, psPassword1[0], psPassword2 + 1, psPassword2[0]))
			mViews.saveBtn->Enable();
		else
			mViews.saveBtn->Disable();
	}
	
	mViews.passwordText1->SetHasChanged(false);
	mViews.passwordText2->SetHasChanged(false);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySetAdminPrivsWin::CMySetAdminPrivsWin(const Uint8 *inLogin)
	: CWindow(SRect(0,0,400,260), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CIconView *icn;

	// setup window
	SetTitle("\pUpdate Privileges");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,400,260));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Caution);
	icn->Show();

	// make label
	Uint8 bufMessage[512];
	Uint32 nMessageSize = UText::Format(bufMessage, sizeof(bufMessage), "The account you have selected as your head administrator account does not currently have full admin privileges. Would you like to enable all of this account's privileges so that it can be used as the head admin account?\r\rSelecting Cancel will return you to the Select Head Admin Account window. In order to select an account as a head administrator account, you must enable all of the privileges.\r\rAccount Login: %#s", inLogin);

	lbl = new CLabelView(vc, SRect(50,10,390,218));
	lbl->SetText(bufMessage, nMessageSize);
	lbl->Show();
	
	// make buttons
	SButtons btns[] = {{1, "\pUpdate", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(210,224,390,250), btns);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyTextWin::CMyTextWin()
	: CWindow(SRect(0,0,520,200), windowLayer_Standard, windowOption_CloseBox + windowOption_Sizeable + windowOption_WhiteBkgnd)
{
#if WIN32
	_SetWinIcon(*this, 215);
#endif

	CContainerView *vc;

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,520,200));
	vc->Show();
	
	// make text scroller
	mViews.scrollerView = MakeTextBoxView(vc, SRect(-2,-2,522,202), scrollerOption_PlainBorder + scrollerOption_VertBar + scrollerOption_GrowSpace + scrollerOption_NoBkgnd, &mViews.textView);
	mViews.scrollerView->SetSizing(sizing_BottomRightSticky);
	mViews.scrollerView->Show();
	mViews.textView->SetSizing(sizing_FullHeight + sizing_RightSticky);
#if MACINTOSH
	mViews.textView->SetFont(kFixedFont, nil, 9);
#else
	mViews.textView->SetFont(kFixedFont, nil, 12);
#endif
	mViews.textView->SetEditable(false);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyBroadcastWin::CMyBroadcastWin()
	: CWindow(SRect(0,0,370,216), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CScrollerView *scr;
	CIconView *icn;

	// setup window
	SetTitle("\pBroadcast");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,370,216));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();

	// make labels
	lbl = new CLabelView(vc, SRect(50,10,345,44));
	lbl->SetText("\pEnter a message to display to all connected users and click the ÔSendÕ button.");
	lbl->Show();
	
	// make text boxes
	scr = MakeTextBoxView(vc, SRect(10,50,360,170), scrollerOption_Border /*+ scrollerOption_VertBar*/, &mViews.msgText);
	scr->Show();
	vc->SetFocusView(scr);

	// make buttons
	btn = new CButtonView(vc, SRect(220,183,280,203));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);

	btn = new CButtonView(vc, SRect(294,180,360,206));
	btn->SetTitle("\pSend");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyDownloadClientWin::CMyDownloadClientWin()
	: CWindow(SRect(0,0,350,106), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CIconView *icn;

	// setup window
	SetTitle("\pHotline Client Not Installed");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,350,106));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();

	// make label
	lbl = new CLabelView(vc, SRect(50,10,340,64));
	lbl->SetText("\pHotline Client is not installed on this system.\rYou can download it at:\rhttp://www.lorbac.net");
	lbl->Show();
	
	// make buttons
	btn = new CButtonView(vc, SRect(124,73,208,93));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);

	btn = new CButtonView(vc, SRect(224,70,340,96));
	btn->SetTitle("\pDownload Now");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyStatsWin::CMyStatsWin()
	: CWindow(SRect(0,0,234,246), windowLayer_Standard, windowOption_CloseBox)
{
#if WIN32
	_SetWinIcon(*this, 215);
#endif

	CContainerView *vc;
	CLabelView *lbl;
	Uint32 i;
	Uint8 str[64];

	mStatValues.connected = mStatValues.invisible = mStatValues.downloading = mStatValues.uploading = mStatValues.waiting = mStatValues.connPeak = mStatValues.connCounter = mStatValues.dlCounter = mStatValues.ulCounter = 0;

	// setup window
	SetTitle("\pStatistics");

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,240,230));
	vc->Show();

	// make labels
	const Uint8 *labelText[] = {
		"\pCurrently Connected:", "\pCurrently Invisible:", "\pDownloads in progress:", "\pUploads in progress:", "\pWaiting downloads:", "\pConnection Peak:",
		"\pConnection Counter:", "\pDownload Counter:", "\pUpload Counter:", "\pSince:"
	};

	const SRect labelRects[] = {
		SRect(10,10,156,26), SRect(10,30,166,46), SRect(10,50,152,66),
		SRect(10,70,166,86), SRect(10,100,130,116), SRect(10,120,150,136),
		SRect(10,140,143,156), SRect(10,160,125,176), SRect(10,180,156,196), SRect(10,210,58,236)
	};

	enum { lblCount = sizeof(labelText) / sizeof(Uint8 *) };
	for (i=0; i<lblCount; i++)
	{
		lbl = new CLabelView(vc, labelRects[i]);
		lbl->SetText(labelText[i]);
		lbl->Show();
	}

	TFontDesc fnt = nil;
	try
	{
		fnt = UFontDesc::New(kDefaultFont, nil, 9);
		fnt->SetAlign(textAlign_Right);
		
		// make value labels
		lbl = new CLabelView(vc, SRect(168,10,224,26));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.connected = lbl;
		
		
		lbl = new CLabelView(vc, SRect(168,30,224,46));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.invisible = lbl;
		
		
		lbl = new CLabelView(vc, SRect(168,50,224,66));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.downloading = lbl;
		lbl = new CLabelView(vc, SRect(168,70,224,86));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.uploading = lbl;
		lbl = new CLabelView(vc, SRect(168,100,224,116));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.waiting = lbl;
		lbl = new CLabelView(vc, SRect(168,120,224,136));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.connPeak = lbl;
		lbl = new CLabelView(vc, SRect(168,140,224,156));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.connCounter = lbl;
		lbl = new CLabelView(vc, SRect(168,160,224,176));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.dlCounter = lbl;
		lbl = new CLabelView(vc, SRect(168,180,224,196));
		lbl->SetFont(fnt->Clone());
		lbl->SetText("\p0");
		lbl->Show();
		mStatLabels.ulCounter = lbl;
		
		// make date label
		str[0] = UDateTime::DateToText(str+1, sizeof(str)-1, kShortDateText + kTimeWithSecsText);
		lbl = new CLabelView(vc, SRect(70,210,224,226));
		lbl->SetFont(fnt->Clone());
		lbl->SetText(str);
		lbl->Show();
	}
	catch(...)
	{
		if(fnt)
			delete fnt;

		throw;
	}
	
	delete fnt;
}

void CMyStatsWin::SetStats(Uint32 inConnected, Uint32 inInvisible, Uint32 inDownloading, Uint32 inUploading, Uint32 inWaiting, Uint32 inConnPeak, Uint32 inConnCounter, Uint32 inDlCounter, Uint32 inUlCounter)
{
	Uint8 str[32];

	if (inConnected != mStatValues.connected)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inConnected);
		mStatLabels.connected->SetText(str);
		mStatValues.connected = inConnected;
	}
	
	if (inInvisible != mStatValues.invisible)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inInvisible);
		mStatLabels.invisible->SetText(str);
		mStatValues.invisible = inInvisible;
	}

	if (inDownloading != mStatValues.downloading)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inDownloading);
		mStatLabels.downloading->SetText(str);
		mStatValues.downloading = inDownloading;
	}

	if (inUploading != mStatValues.uploading)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inUploading);
		mStatLabels.uploading->SetText(str);
		mStatValues.uploading = inUploading;
	}

	if (inWaiting != mStatValues.waiting)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inWaiting);
		mStatLabels.waiting->SetText(str);
		mStatValues.waiting = inWaiting;
	}

	if (inConnPeak != mStatValues.connPeak)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inConnPeak);
		mStatLabels.connPeak->SetText(str);
		mStatValues.connPeak = inConnPeak;
	}

	if (inConnCounter != mStatValues.connCounter)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inConnCounter);
		mStatLabels.connCounter->SetText(str);
		mStatValues.connCounter = inConnCounter;
	}

	if (inDlCounter != mStatValues.dlCounter)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inDlCounter);
		mStatLabels.dlCounter->SetText(str);
		mStatValues.dlCounter = inDlCounter;
	}

	if (inUlCounter != mStatValues.ulCounter)
	{
		str[0] = UText::IntegerToText(str+1, sizeof(str)-1, inUlCounter);
		mStatLabels.ulCounter->SetText(str);
		mStatValues.ulCounter = inUlCounter;
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyQuittingWin::CMyQuittingWin()
	: CWindow(SRect(0,0,250,70), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;

	// setup window
	SetTitle("\pQuitting");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,250,70));
	vc->Show();
	
	// make labels
	lbl = new CLabelView(vc, SRect(10,10,250,26));
	lbl->SetText("\pWaiting for clients to disconnect...");
	lbl->Show();
	
	// make buttons
	btn = new CButtonView(vc, SRect(130,40,210,60));
	btn->SetTitle("\pQuit Now");
	btn->SetID(viewID_QuitNow);
	btn->Show();
	btn = new CButtonView(vc, SRect(40,40,120,60));
	btn->SetTitle("\pCancel");
	btn->SetID(viewID_CancelQuit);
	btn->Show();
	vc->SetCancelView(btn);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyQuitWin::CMyQuitWin()
	: CWindow(SRect(0,0,350,196), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CScrollerView *scr;
	CIconView *icn;

	// setup window
	SetTitle("\pQuit");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,550,196));
	vc->Show();
	
	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Caution);
	icn->Show();

	// make labels
	lbl = new CLabelView(vc, SRect(50,10,337,76));
	lbl->SetText("\pThere are people connected to this server.  If you quit, they will be disconnected.  To disconnect them with a message, click ÒPolite QuitÓ.");
	lbl->Show();
	
	// make text boxes
	scr = MakeTextBoxView(vc, SRect(10,80,340,150), scrollerOption_Border, &mViews.msgText);
	scr->Show();
	vc->SetFocusView(scr);
	
	// make buttons
	btn = new CButtonView(vc, SRect(244,160,340,186));
	btn->SetTitle("\pPolite Quit");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);
	btn = new CButtonView(vc, SRect(141,163,231,183));
	btn->SetTitle("\pQuit Now");
	btn->SetID(2);
	btn->Show();
	btn = new CButtonView(vc, SRect(40,163,130,183));
	btn->SetTitle("\pCancel");
	btn->SetID(3);
	btn->Show();
	vc->SetCancelView(btn);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

