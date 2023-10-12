/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "Hotline.h"

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif
#if MACINTOSH_CARBON
Int16 _GetSystemVersion();
#endif
void _GetScreenBounds(SRect& outScreenBounds);

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

CMyOptionsWin::CMyOptionsWin()
	: CWindow(SRect(0,0,420,366), windowLayer_Modal)
{
	// setup window
	SetTitle("\pOptions");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	CContainerView *vc = new CContainerView(this, SRect(0,0,420,364));
	vc->Show();

	// make tab view
	CTabbedView *tabs = new CTabbedView(vc, SRect(10,10,410,320));
#if WIN32
	tabs->SetFont(fd_Default9);
#else
	tabs->SetFont(fd_Default10);
#endif

	// make icons view
	mViews.iconTab = new CMyOptIconTab(SPoint(40,16));
	mViews.iconTab->SetText("\pChoose an icon to appear next to your name in user lists:");

	// add the tabs
	tabs->AddTab("\pGeneral");
	tabs->AddTab("\pIcon");
	tabs->AddTab("\pDisplay");
	tabs->AddTab("\pSound");
	tabs->AddTab("\pAdvanced");
	tabs->AddTab("\pIconSet");
		
	// attach views to tabs
	tabs->SetTabView(1, MakeGeneralTab());
	tabs->SetTabView(2, mViews.iconTab);
	tabs->SetTabView(3, MakeDisplayTab());
	tabs->SetTabView(4, MakeSoundTab());
	tabs->SetTabView(5, MakeAdvancedTab());
	tabs->SetTabView(6, MakeIconSetTab());
	tabs->SetCurrentTab(1);
	tabs->Show();

	// make buttons 
	#if USE_HELP
 	CButtonView *ibh = new CButtonView(vc, SRect(120,302,200,324));
	ibh->SetTitle("\pHelp");
	ibh->SetID(viewID_HelpOptions);
	ibh->Show();
	#endif
	
	SButtons btns[] = {
		{1, "\pSave", btnOpt_Default, nil}, 
		{2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(210,300,390,386), btns);
	
}

CContainerView *CMyOptionsWin::MakeGeneralTab()
{
	// make container view to hold the views
	CContainerView *vc = new CContainerView(nil, SRect(0,0,400,270));

	try
	{
		// make labels
		CLabelView *lbl = new CLabelView(vc, SRect(20,16,102,32));
		lbl->SetText("\pYour Name:");
		lbl->Show();

		// make text box
		CScrollerView *scr = MakeTextBoxView(vc, SRect(110,10,380,36), scrollerOption_Border, &mViews.nameText);
		mViews.nameText->SetEnterKeyAction(enterKeyAction_None);
		scr->Show();
			
		// make box
		CBoxView *box = new CBoxView(vc, SRect(10,44,380,132), boxStyle_Etched);
		box->Show();

		// make checkboxes
		CCheckBoxView *chk = new CCheckBoxView(vc, SRect(20,49,370,67));
		chk->SetTitle("\pQueue File Transfers");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.queueTransfers = chk;
		chk = new CCheckBoxView(vc, SRect(20,69,205,87));
		chk->SetTitle("\pShow Join/Leave in Chat");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->SetID(4);
		chk->Show();
		mViews.showJoinLeave = chk;
		
		chk = new CCheckBoxView(vc, SRect(245,49,405,67));
		chk->SetTitle("\pShow Date/Time");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.showDateTime = chk;
		
		chk = new CCheckBoxView(vc, SRect(245,69,405,87));
		chk->SetTitle("\pShow Splash Screen");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.showSplash = chk;
		
		chk = new CCheckBoxView(vc, SRect(245,89,405,107));
		chk->SetTitle("\pShow Banner");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		//chk->Disable();
		mViews.showBanner = chk;
		
		chk = new CCheckBoxView(vc, SRect(20,89,225,107));
		chk->SetTitle("\pShow Private Messages at Back");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.showPrivMsgAtBack = chk;
		chk = new CCheckBoxView(vc, SRect(20,109,370,127));
		chk->SetTitle("\pBrowse folders using a separate window");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.browseFolders = chk;
		
		// make box
		box = new CBoxView(vc, SRect(10,142,380,277), boxStyle_Etched);
		box->Show();
		
		chk = new CCheckBoxView(vc, SRect(20,147,200,165));
		chk->SetTitle("\pRefuse private messages");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.refusePrivateMsg = chk;
		chk = new CCheckBoxView(vc, SRect(20,167,350,185));
		chk->SetTitle("\pRefuse private chat");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.refusePrivateChat = chk;
		chk = new CCheckBoxView(vc, SRect(20,187,350,205));
		chk->SetTitle("\pAutomatic response");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->SetID(5);
		chk->Show();
		mViews.automaticResponse = chk;
		
		
		chk = new CCheckBoxView(vc, SRect(200,167,350,185));
		chk->SetTitle("\pAuto Agree");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.autoAgree = chk;
		
		//mViews.refuseBroadcast = chk;
		chk = new CCheckBoxView(vc, SRect(200,187,350,205));
		chk->SetTitle("\pRefuse broadcast");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.refuseBroadcast = chk;
		
		
		chk = new CCheckBoxView(vc, SRect(200,147,350,165));
		chk->SetTitle("\pLog Messages");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.logMessages = chk;
	
		// auto response text box
		scr = MakeTextBoxView(vc, SRect(20,207,370,233), scrollerOption_Border, &mViews.automaticResponseText);
		mViews.automaticResponseText->SetEnterKeyAction(enterKeyAction_None);
		mViews.automaticResponseScroll = scr;
		
		
		// auto response cancel
		scr = MakeTextBoxView(vc, SRect(230,235,270,260), scrollerOption_Border, &mViews.cancelAutorespTime);
		mViews.cancelAutorespTime->SetEnterKeyAction(enterKeyAction_None);
		mViews.cancelAutorespTimeScroll = scr;	
		
		
		CLabelView *lbl2 = new CLabelView(vc, SRect(20,240,232,270));
		lbl2->SetText("\pDisable AutoResponse at next message for");
		mViews.lbl2 = lbl2;
		
		CLabelView *lbl3 = new CLabelView(vc, SRect(270,240,292,265));
		lbl3->SetText("\psec ");
		mViews.lbl3 = lbl3;
		
		
		
			
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}


CContainerView *CMyOptionsWin::MakeDisplayTab()
{
	enum
	{
		cellOffset = 3,
		cellHPadding = 12,
		cellVPadding = 3,
	#if MACINTOSH
		cellHeight = 27 // really CellHeight - 3 for visual appeal :)
	#else
		cellHeight = 30
	#endif
	};
	
	struct SMyFontTableViews
	{
		Uint8 *labelTxt;
		CTextView **size;
		CColorView **textCol;
		CColorView **backCol;
	};
	
	const SMyFontTableViews fontviews[] =
	{
		{	"\pPublic Chat", &mViews.fontChat.size, &mViews.fontChat.col, &mViews.fontChat.backCol				},
		{	"\pPrivate Chat", &mViews.fontPrivChat.size, &mViews.fontPrivChat.col, &mViews.fontPrivChat.backCol	},
		{	"\pView Text", &mViews.fontPrev.size, &mViews.fontPrev.col, &mViews.fontPrev.backCol					},
		{	"\pNews Posts", &mViews.fontNews.size, &mViews.fontNews.col, &mViews.fontNews.backCol				}
	
	};
	
	struct SColInfo
	{
		Uint8 *name;
		Uint16 width;
	};
	
	const SColInfo colInf[] =
	{
		{ "\p", 120 + 2 * cellHPadding					},
		{ "\pSize", 35 + 2 * cellHPadding				},
		{ "\pText Color", 40 + 2 * cellHPadding			},
		{ "\pBack Color", 40 + 2 * cellHPadding			}
	};
	
	CContainerView *vc = new CContainerView(nil, SRect(0,0,400,280));
	vc->Show();
	
	try
	{
	#if MACINTOSH
		SRect r(0, 16, 20 - cellOffset, 46);
	#else
		SRect r(0, 10, 20 - cellOffset, 40);
	#endif
		
		CLabelView *lbl;
		CTextView *txt;
		CScrollerView *scr;
		CColorView *col;
		
		// headers
		for (Uint32 i=0; i!=sizeof(colInf) / sizeof(SColInfo); i++)
		{
			r.left = r.right + cellOffset;
			r.right = r.left + colInf[i].width;
			
			lbl = new CLabelView(vc, r, colInf[i].name);
			lbl->SetFont(fd_Default9BoldCenter);
			lbl->Show();
		}
		
		r.bottom -= 15;
			
		for (Uint32 i=0; i!=sizeof(fontviews) / sizeof(SMyFontTableViews); i++)
		{
			r.left = 20 - cellHPadding;
			r.right = r.left + cellHPadding + colInf[0].width;
			r.top = r.bottom + cellOffset;
			r.bottom = r.top + cellHeight;
			
	//		box = new CBoxView(vc, r, boxStyle_Raised);
	//		box->Show();
			
			r.Enlarge(-cellHPadding, -cellVPadding);
			lbl = new CLabelView(vc, r, fontviews[i].labelTxt);
			lbl->Show();
			r.Enlarge(cellHPadding, cellVPadding);
			
			r.left = r.right + cellOffset;
			r.right = r.left + colInf[1].width;		
					
	//		box = new CBoxView(vc, r, boxStyle_Raised);
	//		box->Show();
	
			r.Enlarge(-cellHPadding, -cellVPadding);
			scr = MakeTextBoxView(vc, r, scrollerOption_Border, &txt);
			txt->SetEnterKeyAction(enterKeyAction_None);
			scr->Show();
			txt->SetFont(kDefaultFont, nil, 9);
			*fontviews[i].size = txt;
			r.Enlarge(cellHPadding, cellVPadding);
			
			r.left = r.right + cellOffset;
			r.right = r.left + colInf[2].width;		
					
	//		box = new CBoxView(vc, r, boxStyle_Raised);
	//		box->Show();
	
			r.Enlarge(-cellHPadding, -cellVPadding);
			col = new CColorView(vc, r);
			col->Show();
			r.Enlarge(cellHPadding, cellVPadding);
			
			*fontviews[i].textCol = col;
			
			r.left = r.right + cellOffset;
			r.right = r.left + colInf[3].width;		
					
	//		box = new CBoxView(vc, r, boxStyle_Raised);
	//		box->Show();
	
			r.Enlarge(-cellHPadding, -cellVPadding);
			col = new CColorView(vc, r);
			col->Show();
			r.Enlarge(2);
			
			*fontviews[i].backCol = col;	
		}
		
	#if MACINTOSH
		Uint32 nTop = r.bottom + 32;
	#else
		Uint32 nTop = r.bottom + 28;
	#endif
		
		r.left = 20;
		r.right = r.left + 164;
		r.top = nTop;
		r.bottom = r.top + 16;
		
		CBoxView *box = new CBoxView(vc, SRect(r.left - 6, nTop - 10, r.right + 6, nTop + 64), boxStyle_Etched);
		box->Show();

		CCheckBoxView *chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pToolbar Buttons on Top");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.toolbarButtonsTop = chk;
				
		r.top = r.bottom + 4;
		r.bottom = r.top + 16;

		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pMagnetic Windows");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.magneticWindows = chk;

		r.top = r.bottom + 4;
		r.bottom = r.top + 16;
		
		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pShow ToolTips");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.showTooltips = chk;

		// files
		r.left = r.right + 22;
		r.right = r.left + 74;
		r.top = nTop;
		r.bottom = r.top + 16;
		
		box = new CBoxView(vc, SRect(r.left - 6, nTop - 16, r.right + 6, nTop + 64), boxStyle_Etched);
		box->SetTitle("\pFiles");
		box->Show();

		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pList");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.filesList = chk;
	
		r.top = r.bottom + 4;
		r.bottom = r.top + 16;
	
		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pTree");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.filesTree = chk;
		
		r.top = r.bottom + 4;
		r.bottom = r.top + 16;
	
		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pPaned");
						 
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.filesExpl = chk;

		mViews.filesList->SetExclusiveNext(mViews.filesTree);
		mViews.filesTree->SetExclusiveNext(mViews.filesExpl);
		mViews.filesExpl->SetExclusiveNext(mViews.filesList);

		// news
		r.left = r.right + 22;
		r.right = r.left + 74;
		r.top = nTop;
		r.bottom = r.top + 16;

		box = new CBoxView(vc, SRect(r.left - 6, nTop - 16, r.right + 6, nTop + 64), boxStyle_Etched);
		box->SetTitle("\pNews");
		box->Show();

		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pList");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.newsList = chk;
	
		r.top = r.bottom + 4;
		r.bottom = r.top + 16;
	
		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pTree");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.newsTree = chk;
		
		r.top = r.bottom + 4;
		r.bottom = r.top + 16;
	
		chk = new CCheckBoxView(vc, r);
		chk->SetTitle("\pPaned");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.newsExpl = chk;

		mViews.newsList->SetExclusiveNext(mViews.newsTree);
		mViews.newsTree->SetExclusiveNext(mViews.newsExpl);
		mViews.newsExpl->SetExclusiveNext(mViews.newsList);		
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}

	return vc;
}
/*

CContainerView *CMyOptionsWin::MakeIconTab()
{
	CContainerView *vc;
	CLabelView *lbl;
	CMyIconPickView *icp;
	Uint16 i;

	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,400,280));

	try
	{
		// make labels
		lbl = new CLabelView(vc, SRect(10,10,311,44));
		lbl->SetText("\pChoose an icon to appear next to your name in user lists:");
		lbl->Show();
		
		// make icon pickers
		Int16 iconIDs[] = {
			0xFE61, 0xF82F, 0xF82E, 0xF82D, 0xF82C, 0xF82B, 0xF821, 0xF829, 0xF828, 0xF827, 0xF826,
			0xF825, 0xF824, 0xF823, 0xFF6F, 0xFF6E, 0xF820, 0xF81F, 0xF81E, 0xF81D, 0xFE5E, 0xFF73,
			0xFF72, 0xFF71, 0xFF70, 0xF822, 0xFF75, 0xFF6D
		};
		const SRect iconRects[] = {
			SRect(50,60,82,92),		SRect(85,60,117,92),	SRect(120,60,152,92),
			SRect(155,60,187,92),	SRect(190,60,222,92),	SRect(225,60,257,92),
			SRect(260,60,292,92),	SRect(50,95,82,127),	SRect(85,95,117,127),
			SRect(120,95,152,127),	SRect(155,95,187,127),	SRect(190,95,222,127),
			SRect(225,95,257,127),	SRect(260,95,292,127),	SRect(50,130,82,162),
			SRect(85,130,117,162),	SRect(120,130,152,162),	SRect(155,130,187,162),
			SRect(190,130,222,162),	SRect(225,130,257,162),	SRect(260,130,292,162),
			SRect(50,165,82,197),	SRect(85,165,117,197),	SRect(120,165,152,197),
			SRect(155,165,187,197),	SRect(190,165,222,197),	SRect(225,165,257,197),
			SRect(260,165,292,197)
		};
		
		for (i=0; i<kIconCount; i++)
			iconIDs[i] = ~iconIDs[i];
		
		for (i=0; i<kIconCount; i++)
		{
			icp = new CMyIconPickView(vc, iconRects[i]);
			icp->SetIcon(iconIDs[i]);
			icp->Show();
			mIcons[i] = icp;
		}
		Uint32 ix;
		for (ix=0; ix<kIconCount-1; ix++)
			mIcons[ix]->SetNext(mIcons[ix+1]);
		mIcons[ix]->SetNext(mIcons[0]);
	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

*/
CContainerView *CMyOptionsWin::MakeSoundTab()
{
	CContainerView *vc;
	// make container view to hold the views
	vc = new CContainerView(nil, SRect(0,0,400,280));

	try
	{
		// make checkboxes
		const Uint8 *checkText[] = {
			"\pPlay Sounds for:", "\pChat", /*"\pNews",*/
			"\pFile transfer complete", "\pPrivate Message", "\pJoin",
			"\pLeave", "\pLogged in", "\pError", "\pChat Invitation", "\pMagnetic Windows"
		};
		
		const SRect checkRects[] = {
			SRect(60,20,218,38),	SRect(110,50,226,68),	/*SRect(110,70,216,88),
			SRect(110,90,290,108),	SRect(110,110,250,128),	SRect(110,130,216,148),
			SRect(110,150,216,168),	SRect(110,170,216,188),	SRect(110,190,216,208),
			SRect(110,210,230,228)*/
			
			SRect(110,70,300,88),	SRect(110,90,260,108),	SRect(110,110,226,128),
			SRect(110,130,226,148),	SRect(110,150,226,168),	SRect(110,170,226,188),
			SRect(110,190,230,208), SRect(110,210,300,228)			
		};
		
		enum { checkCount = sizeof(checkText) / sizeof(Uint8 *) };

	#if DEBUG
		if ((checkCount * sizeof(CCheckBoxView*)) > sizeof(mSoundChecks))
			DebugBreak("Need to make array bigger!");
	#endif

		for (Uint16 i=0; i<checkCount; i++)
		{
			CCheckBoxView *chk = new CCheckBoxView(vc, checkRects[i]);
			chk->SetTitle(checkText[i]);
			chk->SetAutoMark(true);
			chk->SetStyle(kTickBox);
			chk->Show();
			mSoundChecks[i] = chk;
		}
		
		mSoundChecks[0]->SetID(6);
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
	// make container view to hold the views
	CContainerView *vc = new CContainerView(nil, SRect(0,0,400,280));

	try
	{
		// make box
		CBoxView *box = new CBoxView(vc, SRect(10,10,380,123), boxStyle_Etched);
		box->Show();
				
		// make labels
		CLabelView *lbl = new CLabelView(vc, SRect(20,15,150,31), "\pSOCKS firewall:");
		lbl->Show();
		lbl = new CLabelView(vc, SRect(20,65,370,118), "\pIf your network requires you to connect through a SOCKS firewall, ask your network administrator for the address.  If you're not sure, leave the box empty.");
		lbl->Show();

		// make text boxe
		CScrollerView *scr = MakeTextBoxView(vc, SRect(20,35,370,61), scrollerOption_Border, &mViews.firewallText);
		mViews.firewallText->SetEnterKeyAction(enterKeyAction_None);
		scr->Show();
	
		// make box
		box = new CBoxView(vc, SRect(10,133,380,213), boxStyle_Etched);
		box->Show();

		// make check box	
		CCheckBoxView *chk = new CCheckBoxView(vc, SRect(20,140,230,156));
		chk->SetTitle("\pTunnel through HTTP proxy");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.tunnelThroughHttp = chk;

		// make label
		lbl = new CLabelView(vc, SRect(20,160,370,208), "\pIf your network is behind a firewall, and you use an http proxy server, check this box.  (You will only be able to connect to v1.7 or newer servers)");
		lbl->Show();

		// make check box	
		chk = new CCheckBoxView(vc, SRect(20,224,170,240));
		chk->SetTitle("\pDon't save cookies");
		chk->SetAutoMark(true);
		chk->SetStyle(kTickBox);
		chk->Show();
		mViews.dontSaveCookies = chk;

	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}
CContainerView *CMyOptionsWin::MakeIconSetTab()
{
	// make container view to hold the views
	CContainerView *vc = new CContainerView(nil, SRect(0,0,400,280));

	try
	{
		// make box
		CBoxView *box = new CBoxView(vc, SRect(10,10,380,123), boxStyle_Etched);
		box->Show();
		
		CCheckBoxView *chk = new CCheckBoxView(vc, SRect(20,20,230,35));
		chk->SetTitle("\pOriginal Style");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.originalIconset = chk;
	
	
	
		
		chk = new CCheckBoxView(vc, SRect(20,40,230,55));
		chk->SetTitle("\pFamPastel Style");
		chk->SetStyle(kRoundCheckBox);
		chk->SetAutoMark(true);
		chk->Show();
		mViews.fampastelIconset = chk;
		
		CLabelView *lbl = new CLabelView(vc, SRect(20,255,350,275), "\pNote: You have to Restart client to make changes.");
		lbl->Show();
		
		mViews.fampastelIconset->SetExclusiveNext(mViews.originalIconset);
		mViews.originalIconset->SetExclusiveNext(mViews.fampastelIconset);
		

	}
	catch(...)
	{
		// clean up
		delete vc;
		throw;
	}
	
	return vc;
}

void CMyOptionsWin::SetInfo(const Uint8 *inName, const Uint8 *inFirewall, const SMyOptions& inOptions)
{
	mViews.nameText->SetText(inName+1, inName[0]);
	mViews.firewallText->SetText(inFirewall+1, inFirewall[0]);
	mViews.showPrivMsgAtBack->SetMark(inOptions.bShowPrivMsgAtBack);
	mViews.queueTransfers->SetMark(inOptions.bQueueTransfers);
	
	mViews.showJoinLeave->SetMark(inOptions.bShowJoinLeaveInChat);
		
	
	
	mViews.showDateTime->SetMark(inOptions.bShowDateTime);
	
	if (inOptions.bShowJoinLeaveInChat)
		mViews.showDateTime->Enable();

	

	mViews.browseFolders->SetMark(inOptions.bBrowseFolders);
	mViews.refusePrivateMsg->SetMark(inOptions.bRefusePrivateMsg);
	mViews.refusePrivateChat->SetMark(inOptions.bRefusePrivateChat);
	mViews.automaticResponse->SetMark(inOptions.bAutomaticResponse);
	
	
	mViews.autoAgree->SetMark(inOptions.bAutoAgree);
	mViews.showSplash->SetMark(inOptions.bShowSplash);
	mViews.showBanner->SetMark(inOptions.bShowBanner);
	
	mViews.originalIconset->SetMark(inOptions.bOriginalIconSet);
	mViews.fampastelIconset->SetMark(inOptions.bFampastelIconSet);
	
	mViews.refuseBroadcast->SetMark(inOptions.bRefuseBroadcast);
	mViews.logMessages->SetMark(inOptions.bLogMessages);
	
	
	mViews.automaticResponseText->SetText(inOptions.psAutomaticResponse + 1, inOptions.psAutomaticResponse[0]);
	
	mViews.cancelAutorespTime->SetText(inOptions.pscancelAutorespTime + 1, inOptions.pscancelAutorespTime[0]);
	
	
	if (inOptions.bAutomaticResponse){
		mViews.automaticResponseScroll->Show();
		mViews.cancelAutorespTimeScroll->Show();
		mViews.lbl2->Show();
		mViews.lbl3->Show();
		}
	
	Uint8 str[32];
	str[0] = UText::IntegerToText(str + 1, sizeof(str) - 1, inOptions.ColorPublicChat.textSize);
	mViews.fontChat.size->SetText(str + 1, str[0]);
	mViews.fontChat.col->SetColor(inOptions.ColorPublicChat.textColor);
	mViews.fontChat.backCol->SetColor(inOptions.ColorPublicChat.backColor);
	
	str[0] = UText::IntegerToText(str + 1, sizeof(str) - 1, inOptions.ColorPrivateChat.textSize);
	mViews.fontPrivChat.size->SetText(str + 1, str[0]);
	mViews.fontPrivChat.col->SetColor(inOptions.ColorPrivateChat.textColor);
	mViews.fontPrivChat.backCol->SetColor(inOptions.ColorPrivateChat.backColor);

	str[0] = UText::IntegerToText(str + 1, sizeof(str) - 1, inOptions.ColorNews.textSize);
	mViews.fontNews.size->SetText(str + 1, str[0]);
	mViews.fontNews.col->SetColor(inOptions.ColorNews.textColor);
	mViews.fontNews.backCol->SetColor(inOptions.ColorNews.backColor);

	str[0] = UText::IntegerToText(str + 1, sizeof(str) - 1, inOptions.ColorViewText.textSize);
	mViews.fontPrev.size->SetText(str + 1, str[0]);
	mViews.fontPrev.col->SetColor(inOptions.ColorViewText.textColor);
	mViews.fontPrev.backCol->SetColor(inOptions.ColorViewText.backColor);

	mViews.toolbarButtonsTop->SetMark(inOptions.bToolbarButtonsTop);
	mViews.magneticWindows->SetMark(inOptions.bMagneticWindows);
	mViews.showTooltips->SetMark(inOptions.bShowTooltips != 0);


//mViews.originalIconset->SetMark(true);

	if (inOptions.nFilesWindow == optWindow_List)
		mViews.filesList->SetMark(true);
	else if (inOptions.nFilesWindow == optWindow_Tree)
		mViews.filesTree->SetMark(true);
	else
		mViews.filesExpl->SetMark(true);
	
	if (inOptions.nNewsWindow == optWindow_List)
		mViews.newsList->SetMark(true);
	else if (inOptions.nNewsWindow == optWindow_Tree)
		mViews.newsTree->SetMark(true);
	else
		mViews.newsExpl->SetMark(true);

	if (inOptions.SoundPrefs.playSounds) mSoundChecks[0]->SetMark(true);
	if (inOptions.SoundPrefs.chat) mSoundChecks[1]->SetMark(true);
  //  if (inOptions.SoundPrefs.news) mSoundChecks[2]->SetMark(true);
	if (inOptions.SoundPrefs.fileTransfer) mSoundChecks[2]->SetMark(true);
	if (inOptions.SoundPrefs.privMsg) mSoundChecks[3]->SetMark(true);
	if (inOptions.SoundPrefs.join) mSoundChecks[4]->SetMark(true);
	if (inOptions.SoundPrefs.leave) mSoundChecks[5]->SetMark(true);
	if (inOptions.SoundPrefs.login) mSoundChecks[6]->SetMark(true);
	if (inOptions.SoundPrefs.error) mSoundChecks[7]->SetMark(true);
	if (inOptions.SoundPrefs.chatInvite) mSoundChecks[8]->SetMark(true);
	if (inOptions.SoundPrefs.magneticWindows) mSoundChecks[9]->SetMark(true);

	mViews.tunnelThroughHttp->SetMark(inOptions.bTunnelThroughHttp);
	mViews.dontSaveCookies->SetMark(inOptions.bDontSaveCookies);

	UpdateSoundChecks();
	ShowHideDateTime();
}

void CMyOptionsWin::GetInfo(Uint8 *outName, Uint8 *outFirewall, SMyOptions& outOptions)
{
	Uint8 str[256];

	if (outName) outName[0] = mViews.nameText->GetText(outName+1, 31);

	if (outFirewall)
	{
		str[0] = mViews.firewallText->GetText(str+1, 255);
		outFirewall[0] = UTransport::CleanUpAddressText(kInternetNameAddressType, str+1, str[0], outFirewall+1, 255);
	}
	
	outOptions.bShowPrivMsgAtBack = mViews.showPrivMsgAtBack->GetMark();
	outOptions.bQueueTransfers = mViews.queueTransfers->GetMark();
	outOptions.bShowJoinLeaveInChat = mViews.showJoinLeave->GetMark();
	outOptions.bShowDateTime = mViews.showDateTime->GetMark();
	
	outOptions.bBrowseFolders = mViews.browseFolders->GetMark();
	outOptions.bRefusePrivateMsg = mViews.refusePrivateMsg->GetMark();
	outOptions.bRefusePrivateChat = mViews.refusePrivateChat->GetMark();
	outOptions.bRefuseBroadcast = mViews.refuseBroadcast->GetMark();
	
	outOptions.bAutomaticResponse = mViews.automaticResponse->GetMark();
	outOptions.bAutoAgree = mViews.autoAgree->GetMark();
	outOptions.bShowSplash = mViews.showSplash->GetMark();
	outOptions.bShowBanner = mViews.showBanner->GetMark();
	
	outOptions.bOriginalIconSet = mViews.originalIconset->GetMark();
	outOptions.bFampastelIconSet = mViews.fampastelIconset->GetMark();
	
	outOptions.bLogMessages = mViews.logMessages->GetMark();
	

	if (outOptions.bAutomaticResponse){
		outOptions.psAutomaticResponse[0] = mViews.automaticResponseText->GetText(outOptions.psAutomaticResponse + 1, sizeof(outOptions.psAutomaticResponse) - 1);
		outOptions.pscancelAutorespTime[0] = mViews.cancelAutorespTime->GetText(outOptions.pscancelAutorespTime + 1, sizeof(outOptions.pscancelAutorespTime) - 1);
	}

	str[0] = mViews.fontChat.size->GetText(str + 1, sizeof(str) - 1);
	outOptions.ColorPublicChat.textSize = UText::TextToUInteger(str + 1, str[0]);
	mViews.fontChat.col->GetColor(outOptions.ColorPublicChat.textColor);
	mViews.fontChat.backCol->GetColor(outOptions.ColorPublicChat.backColor);
	
	str[0] = mViews.fontPrivChat.size->GetText(str + 1, sizeof(str) - 1);
	outOptions.ColorPrivateChat.textSize = UText::TextToUInteger(str + 1, str[0]);
	mViews.fontPrivChat.col->GetColor(outOptions.ColorPrivateChat.textColor);
	mViews.fontPrivChat.backCol->GetColor(outOptions.ColorPrivateChat.backColor);

	str[0] = mViews.fontNews.size->GetText(str + 1, sizeof(str) - 1);
	outOptions.ColorNews.textSize = UText::TextToUInteger(str + 1, str[0]);
	mViews.fontNews.col->GetColor(outOptions.ColorNews.textColor);
	mViews.fontNews.backCol->GetColor(outOptions.ColorNews.backColor);

	str[0] = mViews.fontPrev.size->GetText(str + 1, sizeof(str) - 1);
	outOptions.ColorViewText.textSize = UText::TextToUInteger(str + 1, str[0]);
	mViews.fontPrev.col->GetColor(outOptions.ColorViewText.textColor);
	mViews.fontPrev.backCol->GetColor(outOptions.ColorViewText.backColor);

	outOptions.bToolbarButtonsTop = mViews.toolbarButtonsTop->GetMark();
	outOptions.bMagneticWindows = mViews.magneticWindows->GetMark();
	outOptions.bShowTooltips = mViews.showTooltips->GetMark();

	if (mViews.filesList->GetMark())
		outOptions.nFilesWindow = optWindow_List;
	else if (mViews.filesTree->GetMark())
		outOptions.nFilesWindow = optWindow_Tree;
	else
		outOptions.nFilesWindow = optWindow_Expl;

	if (mViews.newsList->GetMark())
		outOptions.nNewsWindow = optWindow_List;
	else if (mViews.newsTree->GetMark())
		outOptions.nNewsWindow = optWindow_Tree;
	else
		outOptions.nNewsWindow = optWindow_Expl;

	ClearStruct(outOptions.SoundPrefs);
	if (mSoundChecks[0]->GetMark()) outOptions.SoundPrefs.playSounds = true;
	if (mSoundChecks[1]->GetMark()) outOptions.SoundPrefs.chat = true;
//	if (mSoundChecks[2]->GetMark()) outOptions.SoundPrefs.news = true;
	if (mSoundChecks[2]->GetMark()) outOptions.SoundPrefs.fileTransfer = true;
	if (mSoundChecks[3]->GetMark()) outOptions.SoundPrefs.privMsg = true;
	if (mSoundChecks[4]->GetMark()) outOptions.SoundPrefs.join = true;
	if (mSoundChecks[5]->GetMark()) outOptions.SoundPrefs.leave = true;
	if (mSoundChecks[6]->GetMark()) outOptions.SoundPrefs.login = true;
	if (mSoundChecks[7]->GetMark()) outOptions.SoundPrefs.error = true;
	if (mSoundChecks[8]->GetMark()) outOptions.SoundPrefs.chatInvite = true;
	if (mSoundChecks[9]->GetMark()) outOptions.SoundPrefs.magneticWindows = true;

	outOptions.bTunnelThroughHttp = mViews.tunnelThroughHttp->GetMark();
	outOptions.bDontSaveCookies = mViews.dontSaveCookies->GetMark();
}

void CMyOptionsWin::ShowHideDateTime()
{
	if (mViews.showJoinLeave->GetMark())
		mViews.showDateTime->Enable();
	else
		mViews.showDateTime->Disable();	 
}

void CMyOptionsWin::ShowHideAutomaticResponse()
{
	if (mViews.automaticResponse->GetMark()){
		mViews.automaticResponseScroll->Show();
		mViews.cancelAutorespTimeScroll->Show();
		mViews.lbl2->Show();
		mViews.lbl3->Show();
	}else{
		mViews.automaticResponseScroll->Hide();	
		mViews.cancelAutorespTimeScroll->Hide(); 
		mViews.lbl2->Hide();
		mViews.lbl3->Hide();
		}
}

void CMyOptionsWin::UpdateSoundChecks()
{
	bool mark = mSoundChecks[0]->GetMark();
	
	for (Uint32 i = 1; i!= sizeof(mSoundChecks) / sizeof(CCheckBoxView *); i++)
		mSoundChecks[i]->SetEnable(mark);
}





/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyLoginWin::CMyLoginWin()
	: CWindow(SRect(0,0,340,246), windowLayer_Modal)
{
	CContainerView *vc;
	CIconView *icn;
	CBoxView *box;
	CLabelView *lbl;
	CScrollerView *scr;

	// setup window
	SetTitle("\pConnect");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,340,246));
	vc->Show();
	mViews.container = vc;
	
	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();

	// make box
	box = new CBoxView(vc, SRect(10,70,330,200));
	box->SetStyle(boxStyle_Sunken);
	box->Show();

	// make labels
	lbl = new CLabelView(vc, SRect(50,10,335,60));
	lbl->SetText("\pEnter the Server address, and if you have an account, your login and password.  If not, leave the login and password blank.");
	lbl->Show();
	
	lbl = new CLabelView(vc, SRect(20,85,96,101));
	lbl->SetText("\pServer:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,115,96,131));
	lbl->SetText("\pLogin:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,145,96,161));
	lbl->SetText("\pPassword:");
	lbl->Show();
	
	//make crypt checkbox
	mViews.useCrypt = new CCheckBoxView(vc, SRect(20,175,190,191));
	mViews.useCrypt->SetTitle("\pUse Encryption");
	mViews.useCrypt->SetAutoMark(true);
	mViews.useCrypt->SetStyle(kTickBox);
	mViews.useCrypt->SetID(4);
	mViews.useCrypt->Show();

	// make text boxes
	scr = MakeTextBoxView(vc, SRect(100,80,320,106), scrollerOption_Border, &mViews.addressText);
	mViews.addressText->SetEnterKeyAction(enterKeyAction_None, enterKeyAction_None);
	mViews.addressText->SetID(10);
	scr->Show();
	vc->SetFocusView(scr);	
	mViews.addressScr = scr;
	scr = MakeTextBoxView(vc, SRect(100,110,320,136), scrollerOption_Border, &mViews.nameText);
	mViews.nameText->SetEnterKeyAction(enterKeyAction_None, enterKeyAction_None);
	scr->Show();
	mViews.nameScr = scr;
	scr = MakePasswordBoxView(vc, SRect(100,140,320,166), scrollerOption_Border, &mViews.passwordText);
	scr->Show();
	mViews.passwordScr = scr;
	
	// make buttons
	SButtons btnSave = {3, "\pSave...", btnOpt_Regular, &mViews.saveBtn};
	CButtonView::BuildButton(vc, SRect(10,210,80,236), &btnSave);
	mViews.saveBtn->Disable();

	SButtons btns[] = {{1, "\pConnect", btnOpt_Default, &mViews.connectBtn}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(150,210,330,236), btns);
	mViews.connectBtn->Disable();
}

void CMyLoginWin::SetInfo(const Uint8 *inAddress, const Uint8 *inLogin, const Uint8 *inPassword, bool inUseCrypt)
{
	if (inAddress && inAddress[0]) 
		mViews.addressText->SetText(inAddress + 1, inAddress[0]);
		
	if (inLogin && inLogin[0]) 
		mViews.nameText->SetText(inLogin + 1, inLogin[0]);
		
	if (inPassword && inPassword[0]) 
		mViews.passwordText->SetText(inPassword + 1, inPassword[0]);
	
	mViews.useCrypt->SetMark(inUseCrypt);	
	
	CView *pFocusView;
	if (mViews.addressText->IsEmpty())
		pFocusView = mViews.addressScr;
	else if (mViews.nameText->IsEmpty())
		pFocusView = mViews.nameScr;
	else if (mViews.passwordText->IsEmpty())
		pFocusView = mViews.passwordScr;
	else
		pFocusView = mViews.addressScr;
	
	mViews.container->SetFocusView(pFocusView);
	UpdateButtons();
}

void CMyLoginWin::GetInfo(Uint8 *outAddress, Uint8 *outLogin, Uint8 *outPassword, bool *outUseCrypt)
{
	if (outAddress) 
		outAddress[0] = mViews.addressText->GetText(outAddress + 1, 255);
		
	if (outLogin) 
		outLogin[0] = mViews.nameText->GetText(outLogin + 1, 31);
		
	if (outPassword) 
		outPassword[0] = mViews.passwordText->GetText(outPassword + 1, 31);
	
	if(outUseCrypt)
		*outUseCrypt = mViews.useCrypt->GetMark();
}

void CMyLoginWin::UpdateButtons()
{
	if (mViews.addressText->IsEmpty())
	{
		mViews.connectBtn->Disable();
		mViews.saveBtn->Disable();
	}
	else
	{
		mViews.connectBtn->Enable();
		mViews.saveBtn->Enable();
	}
	
	mViews.addressText->SetHasChanged(false);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyItemsWin::CMyItemsWin(CWindow *inParent, const SRect &inBounds, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CWindow(inBounds, windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox + windowOption_ZoomBox, 0, inParent),
	  CMyPathData(inPathData, inPathSize, inPathSum)
{	
	// make container view for content
	mVC = new CContainerView(this, inBounds);
	mVC->Show();

	// make background box
	mBoxView = new CBoxView(mVC, SRect(0, 0, inBounds.right, 30), boxStyle_Raised);
	mBoxView->SetSizing(sizing_RightSticky);
	mBoxView->Show();
}


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyItemsListWin::CMyItemsListWin(CWindow *inParent, const SRect &inBounds, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsWin(inParent, inBounds, inPathData, inPathSize, inPathSum)
{	
	mListView = nil;
}

void CMyItemsListWin::SetStatus(Uint32 inStatus)
{
	if (mListView)
		mListView->SetStatus(inStatus);
}

CMyItemsListWin *CMyItemsListWin::FindWindowByPath(CVoidPtrList &inWinList, const void *inPathData, Uint32 inPathSize, Uint32 inCheckSum)
{
	Uint32 i = 0;
	CMyItemsListWin *win;
	
	if (inCheckSum == 0)
		inCheckSum = UMemory::Checksum(inPathData, inPathSize);
	
	while (inWinList.GetNext((void*&)win, i))
	{
		if (win->GetPathChecksum() == inCheckSum && win->IsPathEqual(inPathData, inPathSize))
			return win;
	}
	
	return nil;
}

void CMyItemsListWin::UserZoom(const SMouseMsgData& /* inInfo */)
{
	SRect r;
	
	GetBounds(r);
	if (mListView)
		r.bottom = r.top + mListView->GetFullHeight() + 31;
	else
		r.bottom = r.top + 60;
	
	if (UWindow::GetZoomBounds(mWindow, r, r))
		SetBounds(r);
}


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyFileWin::CMyFileWin(CContainerView *inContainerView, const SRect& inBounds)
	: CMySearchText(inContainerView, SRect(196, 10, inBounds.right - 34, 24))
{
	mCurrentDrag = nil;

	// make buttons
	CIconButtonView *icb = new CIconButtonView(inContainerView, SRect(3,3,27,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3029);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(230);
		}
	
	icb->SetID(viewID_OpenParent);
	icb->SetTooltipMsg("\pGo Back");
	icb->Show();
	mViews.parentFolder = icb;
	
	icb = new CIconButtonView(inContainerView, SRect(30,3,54,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3028);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(210);
		}
	
	
	icb->SetID(viewID_FileDownload);
	icb->SetTooltipMsg("\pDownload");
	icb->Show();
	mViews.downloadBtn = icb;

	icb = new CIconButtonView(inContainerView, SRect(57,3,81,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3027);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(211);
		}
	
	icb->SetID(viewID_FileUpload);
	icb->SetTooltipMsg("\pUpload");
	icb->Show();
	mViews.uploadBtn = icb;

	icb = new CIconButtonView(inContainerView, SRect(84,3,108,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3044);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(245); 
		}
	 
	icb->SetID(viewID_FileView);
	icb->SetTooltipMsg("\pView");
	icb->Show();
	mViews.viewBtn = icb;

	icb = new CIconButtonView(inContainerView, SRect(111,3,135,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3007);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(215);
		}
	
	
	icb->SetID(viewID_GetInfo);
	icb->SetTooltipMsg("\pGet Info");
	icb->Show();
	mViews.infoBtn = icb;
	
	
  
  	#if USE_HELP
	icb = new CIconButtonView(inContainerView, SRect(138,3,162,27));
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpFiles);
	icb->SetTooltipMsg("\pHelp");
	icb->Show();
	#endif

	icb = new CIconButtonView(inContainerView, SRect(165,3,189,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3013);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(iconID_OpenDloadFolder);
		}
		
	
	
	icb->SetID(viewID_OpenDloadFolder);
	icb->SetTooltipMsg("\pOpen Download Folder");
	icb->Show();


	icb = new CIconButtonView(inContainerView, SRect(192,3,216,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3019);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(401);
		}
	
	
	icb->SetID(viewID_New);
	icb->SetTooltipMsg("\pNew Folder");
	icb->Show();
	mViews.newfolderBtn = icb;
 
	icb = new CIconButtonView(inContainerView, SRect(219,3,243,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3026);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(205);
		}
	
	
	icb->SetTooltipMsg("\pRefresh");
	icb->SetID(viewID_Refresh);
	icb->Show();

	icb = new CIconButtonView(inContainerView, SRect(inBounds.right-27,3,inBounds.right-3,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3000);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(212);
		}
	
	icb->SetID(viewID_Delete);
	icb->SetSizing(sizing_HorizSticky);
	icb->SetTooltipMsg("\pDelete");
	icb->Show();
	mViews.deleteBtn = icb;
}

void CMyFileWin::GetFileContent(const void *inPathData, Uint32 inPathSize)
{
	TFieldData pListData = gApp->mCacheList.SearchFileList(inPathData, inPathSize);

	if (pListData)
		SetContent(inPathData, inPathSize, pListData, false);
	else
		new CMyGetFileListTask(inPathData, inPathSize);
}

void CMyFileWin::SearchCacheFileList(const void *inPathData, Uint32 inPathSize)
{	
	if (!gApp->mCacheList.SearchFileList(inPathData, inPathSize) && !gApp->SearchGetFileListTask(inPathData, inPathSize))
	{
		CMyGetFileListTask *pGetFileListTask = new CMyGetFileListTask(inPathData, inPathSize);
	
		while (!UApplication::IsQuit() && !gApp->mCacheList.SearchFileList(inPathData, inPathSize) && gApp->IsTaskInList(pGetFileListTask))
			UApplication::ProcessAndSleep();	
	}
}

void CMyFileWin::SetFileAccess(const Uint8 *inItemName, const void *inPathData, Uint32 inPathSize, Uint32 inTypeCode, bool inIsFolder)
{
	const Uint8 *pFolderName = nil;
	if (!inPathData || !inPathSize)
	{
		if (inTypeCode)
			pFolderName = inItemName;
	}
	else
		pFolderName = (Uint8 *)inPathData + 4;
		
	if (!inTypeCode) 
		mViews.downloadBtn->Disable();
	else if (inIsFolder && inTypeCode == TB((Uint32)'fldr') && gApp->mServerVers >= 182)
	{
		if (gApp->HasFolderPriv(myAcc_DownloadFolder)) 
			mViews.downloadBtn->Enable();
		else 
			mViews.downloadBtn->Disable();
	}
	else if (gApp->HasFolderPriv(myAcc_DownloadFile)) 
		mViews.downloadBtn->Enable();
	else 
		mViews.downloadBtn->Disable();

	if (!inPathData || !inPathSize) 
		mViews.uploadBtn->Disable();
	else if (gApp->HasFolderPriv(myAcc_UploadFile)) 
		mViews.uploadBtn->Enable();
	else 
		mViews.uploadBtn->Disable();

	//if (gApp->HasFolderPriv(myAcc_CreateFolder)) 
		mViews.newfolderBtn->Enable();
	//else 
		//mViews.newfolderBtn->Disable();
	
	bool bSetDefaultViewIcon = true;
	if (!inTypeCode)
	{
		mViews.viewBtn->Disable();
		mViews.infoBtn->Disable();
		
		
		mViews.deleteBtn->Hide();
	}
	else
	{	
		mViews.infoBtn->Enable();
		
		if (inIsFolder && inTypeCode == TB((Uint32)'fldr'))
		{
			mViews.viewBtn->Disable();
			
			//if (gApp->HasFolderPriv(myAcc_DeleteFolder)) 
				mViews.deleteBtn->Show();
			//else 
				//mViews.deleteBtn->Hide();
		}
		else
		{
			// change view icon
			if (inTypeCode == TB('TEXT'))
			{
				bSetDefaultViewIcon = false;
				if (gApp->mIconSet == 2){
					mViews.viewBtn->SetIconID(3043);
				}else if (gApp->mIconSet == 1){
					mViews.viewBtn->SetIconID(247);
				}
				
			}
			else if (inTypeCode == TB('GIFf') || inTypeCode == TB('JPEG') || inTypeCode == TB('PICT') || inTypeCode == TB('BMP '))
			{
				bSetDefaultViewIcon = false;
				if (gApp->mIconSet == 2){
					mViews.viewBtn->SetIconID(3065);
				}else if (gApp->mIconSet == 1){
					mViews.viewBtn->SetIconID(246);
				}
				
			}

			if (gApp->HasFolderPriv(myAcc_DownloadFile)) 
				mViews.viewBtn->Enable();
			else 
				mViews.viewBtn->Disable();

			//if (gApp->HasFolderPriv(myAcc_DeleteFile)) 
				mViews.deleteBtn->Show();
			//else 
				//mViews.deleteBtn->Hide();
		}
	}
	
	// set default view icon
	if (bSetDefaultViewIcon)
	
	if (gApp->mIconSet == 2){
					mViews.viewBtn->SetIconID(3044);
				}else if (gApp->mIconSet == 1){
					mViews.viewBtn->SetIconID(245);
				}
		
}

void CMyFileWin::DoOpenParent(Uint16 inMods)
{
	gApp->DoFileOpenParent(dynamic_cast<CMyItemsWin *>(this), inMods);
}

void CMyFileWin::DoDownload(Uint16 inMods)
{
	Uint32 nTypeCode;
	Uint8 psFileName[256];
	
	bool bIsFolder = GetSelectedItemName(psFileName, &nTypeCode);
	if (!psFileName[0]) 
		return;

	Uint32 nPathSize;
	bool bDisposePath;
	void *pPathData = GetSelectedParentFolderPath(nPathSize, &bDisposePath);
	
	try
	{
		gApp->DoFileDownload(psFileName, pPathData, nPathSize, nTypeCode, bIsFolder, inMods);
	}
	catch(...)
	{
		if (bDisposePath) UMemory::Dispose((TPtr)pPathData);
		throw;
	}

	if (bDisposePath) 
		UMemory::Dispose((TPtr)pPathData);
}

void CMyFileWin::DoView(Uint16 inMods)
{
	Uint8 psFileName[256];
	Uint32 nTypeCode;
	Uint32 nCreatorCode;

	if (GetSelectedItemName(psFileName, &nTypeCode, &nCreatorCode)) return;
	if (!psFileName[0]) 
		return;

	Uint32 nPathSize;
	bool bDisposePath;
	void *pPathData = GetSelectedParentFolderPath(nPathSize, &bDisposePath);

	gApp->DoFileView(psFileName, pPathData, nPathSize, nTypeCode, nCreatorCode, inMods);
	
	if (bDisposePath) 
		UMemory::Dispose((TPtr)pPathData);
}

void CMyFileWin::DoGetInfo()
{
	gApp->DoFileGetInfo(this);
}


void CMyFileWin::DoDelete()
{
	Uint8 psFileName[256];
	bool bIsFolder = GetSelectedItemName(psFileName);
	if (!psFileName[0]) 
		return;

	Uint32 nPathSize;
	bool bDisposePath;
	void *pPathData = GetSelectedParentFolderPath(nPathSize, &bDisposePath);
	
	gApp->DoFileDelete(psFileName, pPathData, nPathSize, bIsFolder);
	
	if (bDisposePath) 
		UMemory::Dispose((TPtr)pPathData);
}

void CMyFileWin::HandleDrag(CDragAndDroppable *inDD, const SMouseMsgData& inInfo)
{
	if (!UDragAndDrop::IsAvailable()) 
		return;
	
	Uint8 str[256];
	SPoint mouseLoc;
	TDrag drag;
	TRegion rgn, imgRgn;
	TImage img;
	Uint32 dragArea;
	SRect r;
	Int32 hd, vd;
	SPoint imgPt;

	// get file name
	Uint32 nTypeCode;
	Uint32 nCreatorCode;
	bool isFolder = GetSelectedItemName(str, &nTypeCode, &nCreatorCode, &inInfo);
	if (str[0] == 0) return;
	
	// get file path
	bool bDisposePath;
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize, &bDisposePath);
	
	CSelectableItemsView *pItemsView = (CSelectableItemsView *)inDD;

	// get global mouse loc
	pItemsView->GetScreenDelta(hd, vd);
	mouseLoc.h = inInfo.loc.h + hd;
	mouseLoc.v = inInfo.loc.v + vd;

	// create new drag object
	drag = UDragAndDrop::New();
	scopekill(TDragObj, drag);

	// add drag data
	drag->AddFlavor(1, 'HLFN', str+1, str[0], flavorFlag_SenderOnly);
	drag->AddFlavor(1, 'ISDR', isFolder ? "\x01" : "\x00", 1, flavorFlag_SenderOnly);
	drag->AddFlavor(1, 'TYCO', &nTypeCode, 4, flavorFlag_SenderOnly);
	if (pPathData && nPathSize) drag->AddFlavor(1, 'HLFP', pPathData, nPathSize, flavorFlag_SenderOnly);

	if (bDisposePath)
		UMemory::Dispose((TPtr)pPathData);
	
	// allow drag and drop to finder to download
#if MACINTOSH
	drag->AddPromisedFileSysFlavor(1, isFolder ? "" : "mactypecode/HTftHTLC");
#elif WIN32
	drag->AddPromisedFileSysFlavor(str[0], (Int8 *)(str + 1));
#endif

	drag->SetSendHandler(DownloadDragSendProc, this);

	rgn = imgRgn = nil;
	img = nil;
	
	try
	{
		rgn = URegion::New();
		pItemsView->GetSelectedItemsRegion(rgn);
		
		rgn->GetBounds(r);
		dragArea = (Uint32)r.GetWidth() * (Uint32)r.GetHeight();
		
	#if MACINTOSH
		if ((dragArea <= 65536) && UDragAndDrop::HasTranslucentDrag())
		{
			imgRgn = URegion::New(rgn);
			imgRgn->GetBounds(r);
			imgRgn->Move(-r.left, -r.top);
			
			img = pItemsView->GetSelectedItemsImage(imgPt);
			drag->SetImage(img, imgRgn, imgPt, dragImageStyle_Dark);
		}
	#endif

		rgn->Move(hd, vd);
		rgn->SetOutline(2, 2);
		if (!bDisposePath) mCurrentDrag = drag;
		drag->Track(mouseLoc, rgn);
		mCurrentDrag = nil;
	}
	catch(...)
	{
		mCurrentDrag = nil;
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

void CMyFileWin::HandleSetDragAction(CDragAndDroppable *inDD, const SDragMsgData& inInfo)
{
	#pragma unused(inDD)

	if (!mCurrentDrag || !inInfo.drag->Equals(mCurrentDrag))
	{
		if (inInfo.drag->HasFlavor(
		#if WIN32
			flavor_WFile
		#else
			flavor_File
		#endif
			) || inInfo.drag->HasFlavor('HLFN'))
		{
			inInfo.drag->SetDragAction((inInfo.mods & modKey_Option) && (inInfo.mods & (modKey_Command | modKey_Control)) ? dragAction_Link : dragAction_Move);
			return;
		}
	}

	inInfo.drag->SetDragAction(dragAction_None);
}

bool CMyFileWin::HandleDrop(CDragAndDroppable *inDD, const SDragMsgData& inInfo)
{
	#pragma unused(inDD, inInfo)

	return false;
}

void CMyFileWin::DownloadDragSendProc(
					TDrag inDrag, Uint32 inItem, Uint32 inFlavor, 
					void *inRef, bool *outDropEnd, bool *outDropError)
{
	#pragma unused(inFlavor)
	
	if (!inRef || !gApp->mIsConnected)
		Fail(errorType_FileSys, fsError_AccessDenied);
		
	// check access
	if (!gApp->HasFolderPriv(myAcc_DownloadFile) && 
		(gApp->mServerVers < 182 || !gApp->HasFolderPriv(myAcc_DownloadFolder)))
	{
		Uint8 *psMsg = "\pYou are not allowed to download.";
		UApplication::PostMessage(1100, psMsg, psMsg[0] + 1);

		Fail(errorType_FileSys, fsError_AccessDenied);
	}

	Uint8 bIsFolder;
	inDrag->GetFlavorData(inItem, 'ISDR', &bIsFolder, sizeof(bIsFolder));

	// check access
	if (bIsFolder)
	{
		if ( gApp->mServerVers >= 182 && 
			!gApp->HasFolderPriv(myAcc_DownloadFolder))
		{			
			Uint8 *psMsg = "\pYou are not allowed to download folders.";
			UApplication::PostMessage(1100, psMsg, psMsg[0] + 1);

			Fail(errorType_FileSys, fsError_AccessDenied);
		}
	}
	else if (!gApp->HasFolderPriv(myAcc_DownloadFile))
	{
		Uint8 *psMsg = "\pYou are not allowed to download files.";
		UApplication::PostMessage(1100, psMsg, psMsg[0] + 1);

		Fail(errorType_FileSys, fsError_AccessDenied);
	}

	TFSRefObj* dlFolder = inDrag->GetDropFolder();
	scopekill(TFSRefObj, dlFolder);
		
	Uint8 psFileName[256];
	psFileName[0] = inDrag->GetFlavorData(inItem, 'HLFN', psFileName + 1, sizeof(psFileName) - 1);

	Uint8 bufPathData[2048];
	Uint32 nPathSize = inDrag->GetFlavorData(inItem, 'HLFP', bufPathData, sizeof(bufPathData));
		
	// validate name
	Uint8 psValidatedName[256];
	UMemory::Copy(psValidatedName, psFileName, psFileName[0] + 1);
	psValidatedName[0] = UFileSys::ValidateFileName(psValidatedName + 1, psValidatedName[0]);

	// take the .lnk out of the name
	if (psValidatedName[0] > 4 && 
		!UText::CompareInsensitive(psValidatedName + psValidatedName[0] - 3, ".lnk", 4))
		psValidatedName[0] -= 4;

	bool bResume = false;
	StFSRef fp(dlFolder, nil, psValidatedName);

	try
	{			
	#if WIN32
		if (gApp->FindResumableFile(fp, bIsFolder))
		{
			bResume = true;
			goto startDownload;
		}
		else if (fp->Exists())
			fp->MoveToTrash();
	#endif

		if (bIsFolder)
			fp->CreateFolder();
		else
			fp->CreateFile('HTft', 'HTLC');
	}
	catch (SError& err)
	{
		if (err.type == errorType_FileSys && err.id == fsError_ItemAlreadyExists)
		{
			Uint32 typeCode, creatorCode;
			fp->GetTypeAndCreatorCode(typeCode, creatorCode);
			if (creatorCode == 'HTLC' && typeCode == 'HTft')
				bResume = true;
			else
				throw;
		}
		else
			throw;
	}
		
startDownload:
#if MACINTOSH_CARBON
	// check if we are running under MacOS X
	if (_GetSystemVersion() >= 0x0A00)
		inDrag->CreatedPromisedFile(inItem, fp);	//??
#endif

	if (bIsFolder)
		new CMyDownloadFldrTask(bufPathData, nPathSize, psFileName, 
				fp, bResume, gApp->mOptions.bQueueTransfers, 
				false, outDropEnd, outDropError);
	else
	{		
		fp->GetName(psValidatedName);
		new CMyDownloadFileTask(bufPathData, nPathSize, psFileName, 
				psValidatedName, fp, 
				bResume, gApp->mOptions.bQueueTransfers, false, false,
				outDropEnd, outDropError);
	}
}

bool CMyFileWin::File_KeyDown(const SKeyMsgData& inInfo)
{
	if ((inInfo.mods & modKey_Command) && UText::tolower(inInfo.keyChar) == 'i')
	{
		mViews.infoBtn->HitButton();
		return true;
	}
	
	if (SearchText_KeyDown(inInfo))
		return true;
	
	return false;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
CMyFileListWin::CMyFileListWin(CWindow *inParent, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsListWin(inParent, SRect(0,0,300,260), inPathData, inPathSize, inPathSum), CMyFileWin(mVC, SRect(0,0,300,260))
{
#if WIN32
	_SetWinIcon(*this, 401);
#endif

	gApp->GetFilesBoundsInfo();
	gApp->mFileWinList.AddItem(this);
		
	// setup window
	SetTitle("\pFiles");
	SetLimits(280,150);

	// make scroller view
	CScrollerView *scr = new CScrollerView(mVC, SRect(-1, 30, 301, 261), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_GrowSpace + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
#if SPACE_DOT_COM
	scr->SetContentColor(color_Black);
#endif	
	scr->Show();
	mVC->SetFocusView(scr);

	// make file list
	mListView = new CMyFileListView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), true);
	mListView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mListView->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mListView->SetCanFocus(true);
	mListView->SetID(viewID_FileList);
	mListView->SetDragAndDropHandler(this);		// geez I love multiple inheritance
	mListView->Show();

	SetBoundsInfo();
}

CMyFileListWin::~CMyFileListWin()
{
	gApp->mFileWinList.RemoveItem(this);
}

bool CMyFileListWin::HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder)	
{	
	SearchCacheFileList(inPathData, inPathSize);
		
	return GetFileListView()->HasPartialFile(inFileName, inIsFolder);	
}

TFSRefObj* CMyFileListWin::GetItemToUpload()
{
	return GetFileListView()->GetItemToUpload();	
}

void CMyFileListWin::ClearUploadList()
{
	GetFileListView()->ClearUploadList();
}

void CMyFileListWin::GetContent()
{
	GetFileListView()->DeleteAll();
	GetFileListView()->SetStatus(listStat_Loading);
	
	GetFileContent(mPathData, mPathSize);
}

void CMyFileListWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	CMyFileListView *pFileListView = GetFileListView();
	
	pFileListView->DeleteAll();
	bool bAdded = pFileListView->AddListFromFields(inData);
	
	SetStatus(bAdded ? listStat_Hide : listStat_0Items);
	SetAccess();

	if (!mPathData) 
		mViews.parentFolder->Disable();
	else
		mViews.parentFolder->Enable();
		
	if (inCache)
		gApp->mCacheList.AddFileList(inPathData, inPathSize, inData);
}

void CMyFileListWin::SetAccess()
{
	Uint8 psItemName[256];
	Uint32 nTypeCode;
	
	bool bIsFolder = GetFileListView()->GetSelectedItemName(psItemName, &nTypeCode);

	SetFileAccess(psItemName, mPathData, mPathSize, nTypeCode, bIsFolder);
}

void CMyFileListWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stFiles1.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stFiles1);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	(dynamic_cast<CMyFileListView*>(mListView))->SetTabs(gApp->mOptions.stWinTabs.nFilesTab1, gApp->mOptions.stWinTabs.nFilesTab2);
}

void CMyFileListWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stFiles1);
	
	(dynamic_cast<CMyFileListView*>(mListView))->GetTabs(gApp->mOptions.stWinTabs.nFilesTab1, gApp->mOptions.stWinTabs.nFilesTab2);
}

bool CMyFileListWin::GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode, Uint32 *outCreatorCode, const SMouseMsgData *inInfo)
{
	return GetFileListView()->GetSelectedItemName(outItemName, outTypeCode, outCreatorCode, inInfo);
}

void *CMyFileListWin::GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath)
{
	if (outDisposePath)
		*outDisposePath = false;
	
	outPathSize = mPathSize;
	return mPathData;
}

void CMyFileListWin::DoUpload(Uint16 inMods)
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize);

	gApp->DoFileUpload(this, pPathData, nPathSize, inMods);
}

void CMyFileListWin::DoUploadFromDrop(bool inTreeList)
{
	#pragma unused(inTreeList)
	
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize);

	gApp->DoFileUploadFromDrop(this, pPathData, nPathSize);
}

void CMyFileListWin::DoNewFolder()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize);

	gApp->DoFileNewFolder(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);	
}

void CMyFileListWin::DoRefresh(Uint16 inMods)
{
	#pragma unused(inMods)
	
	GetFileListView()->DeleteAll();
	SetStatus(listStat_Loading);

	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize);
	
	gApp->DoFileRefresh(pPathData, nPathSize);
}

void CMyFileListWin::DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList)
{
	#pragma unused(inTreeList)

	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize);

	gApp->DoFileMove(dynamic_cast<CMyItemsWin *>(this), inMods, inFileName, inPathData, inPathSize, pPathData, nPathSize);
}

void CMyFileListWin::DoDoubleClick(Uint16 inMods, bool inTreeList)
{
	#pragma unused(inTreeList)
	
	Uint8 psFileName[256];
	bool bIsFolder = GetSelectedItemName(psFileName);
	if (!psFileName[0]) 
		return;
	
	if (bIsFolder)
	{
		Uint32 nPathSize;
		void *pPathData = GetSelectedParentFolderPath(nPathSize);	

		gApp->DoFileDoubleClick(dynamic_cast<CMyItemsWin *>(this), psFileName, pPathData, nPathSize, inMods);
		return;
	}
		
	DoDownload(0);
}

void CMyFileListWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (inInfo.keyCode == key_Backspace)
	{
		gApp->DoFileOpenParent(this, inInfo.mods);
		return;
	}

	if (File_KeyDown(inInfo))
		return;
		
	CWindow::KeyDown(inInfo);
}

void CMyFileListWin::SearchText(const Uint8 *inText)
{
	CMyFileListView *pFileListView = GetFileListView();
	Uint32 nIndex = pFileListView->SearchNames(inText);

	if (nIndex != pFileListView->GetFirstSelectedItem())
	{
		pFileListView->DeselectAll();
	
		if (nIndex)
		{
			pFileListView->SelectItem(nIndex);
			pFileListView->MakeItemVisible(nIndex);
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyFileTreeWin::CMyFileTreeWin(CWindow *inParent, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsTreeWin(inParent, SRect(0,0,300,260), inPathData, inPathSize, inPathSum), CMyFileWin(mVC, SRect(0,0,300,260))
{
#if WIN32
	_SetWinIcon(*this, 401);
#endif

	gApp->GetFilesBoundsInfo();
	gApp->mFileWinList.AddItem(this);
	
	// setup window
	SetTitle("\pFiles");
	SetLimits(280,150);

	// make scroller view
	CScrollerView *scr = new CScrollerView(mVC, SRect(-1, 30, 301, 261), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_GrowSpace + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
#if SPACE_DOT_COM
	scr->SetContentColor(color_Black);
#endif	
	scr->Show();
	mVC->SetFocusView(scr);

	// make file tree
	mTreeView = new CMyFileTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), &mPathData, &mPathSize, true);
	mTreeView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mTreeView->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mTreeView->SetCanFocus(true);
	mTreeView->SetID(viewID_FileTree);
	mTreeView->SetDragAndDropHandler(this);
	mTreeView->Show();

	SetBoundsInfo();
}

CMyFileTreeWin::~CMyFileTreeWin()
{
	gApp->mFileWinList.RemoveItem(this);
}

bool CMyFileTreeWin::HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder)	
{	
	SearchCacheFileList(inPathData, inPathSize);
	
	return GetFileTreeView()->HasPartialFile(inFileName, inPathData, inPathSize, inIsFolder);	
}

TFSRefObj* CMyFileTreeWin::GetItemToUpload()
{
	return GetFileTreeView()->GetItemToUpload();	
}

void CMyFileTreeWin::ClearUploadList()
{
	GetFileTreeView()->ClearUploadList();
}

void CMyFileTreeWin::GetContent()
{
	GetFileTreeView()->DeleteAll();
	GetFileTreeView()->SetStatus(listStat_Loading);

	GetFileContent(mPathData, mPathSize);
}

void CMyFileTreeWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();
	
	pFileTreeView->DeletePath(inPathData, inPathSize);
	pFileTreeView->AddListFromFields(inPathData, inPathSize, inData);
	
	SetStatus(pFileTreeView->GetTreeCount() ? listStat_Hide : listStat_0Items);
	SetAccess();
		
	if (!mPathData) 
		mViews.parentFolder->Disable();
	else
		mViews.parentFolder->Enable();

	if (inCache)
		gApp->mCacheList.AddFileList(inPathData, inPathSize, inData);
}

void CMyFileTreeWin::SetAccess()
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();

	Uint8 psItemName[256];	
	Uint32 nTypeCode;
	
	bool bIsFolder = pFileTreeView->GetSelectedItemName(psItemName, &nTypeCode);
	
	Uint32 nPathSize;
	void *pPathData = pFileTreeView->GetSelectedParentFolderPath(nPathSize);
	
	SetFileAccess(psItemName, pPathData, nPathSize, nTypeCode, bIsFolder);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileTreeWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stFiles1.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stFiles1);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	(dynamic_cast<CMyFileTreeView*>(mTreeView))->SetTabs(gApp->mOptions.stWinTabs.nFilesTab1, gApp->mOptions.stWinTabs.nFilesTab2);
}

void CMyFileTreeWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stFiles1);

	(dynamic_cast<CMyFileTreeView*>(mTreeView))->GetTabs(gApp->mOptions.stWinTabs.nFilesTab1, gApp->mOptions.stWinTabs.nFilesTab2);
}

bool CMyFileTreeWin::GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode, Uint32 *outCreatorCode, const SMouseMsgData *inInfo)
{
	return GetFileTreeView()->GetSelectedItemName(outItemName, outTypeCode, outCreatorCode, inInfo);
}

void *CMyFileTreeWin::GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath)
{
	if (outDisposePath)
		*outDisposePath = true;
	
	return GetFileTreeView()->GetSelectedParentFolderPath(outPathSize);
}

void *CMyFileTreeWin::GetSelectedFolderPath(Uint32& outPathSize)
{
	return GetFileTreeView()->GetSelectedFolderPath(outPathSize);
}

void CMyFileTreeWin::DoUpload(Uint16 inMods)
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedFolderPath(nPathSize);
	
	gApp->DoFileUpload(this, pPathData, nPathSize, inMods);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileTreeWin::DoUploadFromDrop(bool inTreeList)
{
	#pragma unused(inTreeList)
	
	Uint32 nPathSize;
	void *pPathData;
		
	if (!GetFileTreeView()->GetDropFolderPath(&pPathData, nPathSize))
	{
		ClearUploadList();
		return;
	}
	
	gApp->DoFileUploadFromDrop(this, pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);	
}

void CMyFileTreeWin::DoNewFolder()
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedFolderPath(nPathSize);

	gApp->DoFileNewFolder(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileTreeWin::DoRefresh(Uint16 inMods)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();

	if (inMods & modKey_Option)
		pFileTreeView->DeselectAllTreeItems();
	
	Uint32 nPathSize;
	void *pPathData = pFileTreeView->RefreshSelectedFolderPath(nPathSize);
	
	if (!pFileTreeView->GetTreeCount())
		SetStatus(listStat_Loading);
	
	gApp->DoFileRefresh(pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileTreeWin::DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList)
{
	#pragma unused(inTreeList)

	Uint32 nPathSize;
	void *pPathData;
	
	if (!GetFileTreeView()->GetDropFolderPath(&pPathData, nPathSize))
		return;
		
	gApp->DoFileMove(dynamic_cast<CMyItemsWin *>(this), inMods, inFileName, inPathData, inPathSize, pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileTreeWin::DoDoubleClick(Uint16 inMods, bool inTreeList)
{
	#pragma unused(inTreeList)

	Uint8 psFileName[256];
	bool bIsFolder = GetSelectedItemName(psFileName);
	if (!psFileName[0]) 
		return;

	if (bIsFolder)
	{
		Uint32 nPathSize;
		void *pPathData = GetSelectedParentFolderPath(nPathSize);	

		gApp->DoFileDoubleClick(dynamic_cast<CMyItemsWin *>(this), psFileName, pPathData, nPathSize, inMods);
		UMemory::Dispose((TPtr)pPathData);
		return;
	}
	
	DoDownload(0);
}

void CMyFileTreeWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (File_KeyDown(inInfo))
		return;
		
	CWindow::KeyDown(inInfo);
}

void CMyFileTreeWin::SearchText(const Uint8 *inText)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();
	Uint32 nIndex = pFileTreeView->SearchNames(inText);
	
	if (nIndex != pFileTreeView->GetFirstSelectedTreeItem())
	{
		pFileTreeView->DeselectAllTreeItems();
	
		if (nIndex)
		{
			pFileTreeView->SelectTreeItem(nIndex);
			pFileTreeView->MakeTreeItemVisibleInList(nIndex);
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyFileExplWin::CMyFileExplWin(CWindow *inParent, TPtr inPathData, Uint32 inPathSize, Uint32 inPathSum)
	: CMyItemsTreeWin(inParent, SRect(0,0,520,320), inPathData, inPathSize, inPathSum), CMyFileWin(mVC, SRect(0,0,520,320))
{
#if WIN32
	_SetWinIcon(*this, 401);
#endif

	gApp->GetFilesBoundsInfo();
	gApp->mFileWinList.AddItem(this);
	
	// setup window
	SetTitle("\pFiles");
	SetLimits(350,200);
	
	// make pane view
	mFilePaneView = new CPaneView(mVC, SRect(0, 30, 520, 320));
	mFilePaneView->SetSizing(sizing_BottomRightSticky);
	mFilePaneView->Show();
	mVC->SetFocusView(mFilePaneView);
	
	// make scroller view
	CScrollerView *scr = new CScrollerView(mFilePaneView, SRect(-1, 00, 250, 291), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
#if SPACE_DOT_COM
	scr->SetContentColor(color_Black);
#endif	
	scr->Show();
	mFilePaneView->SetFocusView(scr);
	mFilePaneView->SetViewLimits(scr, 150, 0);
	
	// make file tree
	mTreeView = new CMyFileTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), &mPathData, &mPathSize, false);
	mTreeView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mTreeView->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mTreeView->SetCanFocus(true);
	mTreeView->SetID(viewID_FileExplTree);
	mTreeView->SetDragAndDropHandler(this);
	mTreeView->Show();

	// make scroller view
	scr = new CScrollerView(mFilePaneView, SRect(255, 0, 521, 291), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_GrowSpace + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_HorizSticky + sizing_BottomSticky);
	scr->SetCanFocus(true);
#if SPACE_DOT_COM
	scr->SetContentColor(color_Black);
#endif	
	scr->Show();
	mFilePaneView->SetViewLimits(scr, 150, 0);

	// make file list
	mFileListView = new CMyFileListView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), false);
	mFileListView->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	mFileListView->SetSizing(sizing_RightSticky + sizing_FullHeight);
	mFileListView->SetCanFocus(true);
	mFileListView->SetID(viewID_FileExplList);
	mFileListView->SetDragAndDropHandler(this);
	mFileListView->Show();

	mFileListTimer = nil;

	SetBoundsInfo();
}

CMyFileExplWin::~CMyFileExplWin()
{
	UTimer::Dispose(mFileListTimer);
		
	gApp->mFileWinList.RemoveItem(this);
}

bool CMyFileExplWin::HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder)	
{	
	SearchCacheFileList(inPathData, inPathSize);
	
	return mFileListView->HasPartialFile(inFileName, inIsFolder);	
}

TFSRefObj* CMyFileExplWin::GetItemToUpload()
{
	TFSRefObj* fp = GetFileTreeView()->GetItemToUpload();
	if (!fp) fp = mFileListView->GetItemToUpload();

	return fp;
}

void CMyFileExplWin::ClearUploadList()
{
	GetFileTreeView()->ClearUploadList();
	mFileListView->ClearUploadList();
}

void CMyFileExplWin::GetContent()
{
	GetFileTreeView()->DeleteAll();
	GetFileTreeView()->SetStatus(listStat_Loading);

	mFileListView->DeleteAll();
	mFileListView->SetStatus(listStat_0Items);
	
	GetFileContent(mPathData, mPathSize);
}

void CMyFileExplWin::GetContentFileList()
{
	if (mFileListTimer)
		mFileListTimer->Stop();

	CMyFileTreeView *pFileTreeView = GetFileTreeView();
	
	mFileListView->DeleteAll();
	if (!pFileTreeView->GetFirstSelectedTreeItem())
	{
		mFileListView->SetStatus(listStat_0Items);
		return;
	}

	mFileListView->SetStatus(listStat_Loading);

	Uint32 nPathSize;
	void *pPathData = pFileTreeView->GetSelectedFolderPath(nPathSize);
	TFieldData pListData = gApp->mCacheList.SearchFileList(pPathData, nPathSize);

	if (pListData)
	{
		bool bAdded = mFileListView->AddListFromFields(pListData);	
		mFileListView->SetStatus(bAdded ? listStat_Hide : listStat_0Items);
	}
	else
		new CMyGetFileListTask(pPathData, nPathSize);
	
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::GetContentFileListTimer()
{
	if (!mFileListTimer)
		mFileListTimer = NewTimer();
		
	mFileListTimer->Start(500, kOnceTimer);
}

void CMyFileExplWin::SetContent(const void *inPathData, Uint32 inPathSize, TFieldData inData, bool inCache)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();
	
	pFileTreeView->DeletePath(inPathData, inPathSize);
	pFileTreeView->AddListFromFields(inPathData, inPathSize, inData);
	
	SetStatus(pFileTreeView->GetTreeCount() ? listStat_Hide : listStat_0Items);
	SetAccess();
		
	if (!mPathData) 
		mViews.parentFolder->Disable();
	else
		mViews.parentFolder->Enable();

	if (pFileTreeView->GetFirstSelectedTreeItem())
	{
		Uint32 nPathSize;
		void *pPathData = pFileTreeView->GetSelectedFolderPath(nPathSize);
		
		if (!UMemory::Compare(pPathData, nPathSize, inPathData, inPathSize))
		{
			mFileListView->DeleteAll();
			bool bAdded = mFileListView->AddListFromFields(inData);
			mFileListView->SetStatus(bAdded ? listStat_Hide : listStat_0Items);
		}
		
		UMemory::Dispose((TPtr)pPathData);
	}

	if (inCache)
		gApp->mCacheList.AddFileList(inPathData, inPathSize, inData);
}

void CMyFileExplWin::SetAccess()
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();

	Uint32 nTypeCode;
	Uint8 psItemName[256];	
	
	bool bIsFolder = GetSelectedItemName(psItemName, &nTypeCode);
	
	Uint32 nPathSize;
	void *pPathData = GetSelectedParentFolderPath(nPathSize);
	
	SetFileAccess(psItemName, pPathData, nPathSize, nTypeCode, bIsFolder);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::SetBoundsInfo()
{
	if (gApp->mOptions.stWinRect.stFiles2.IsNotEmpty())
	{
		SetBounds(gApp->mOptions.stWinRect.stFiles2);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	mFilePaneView->SetPanePercent(gApp->mOptions.stWinPane.nFilesPane1);
	
	(dynamic_cast<CMyFileTreeView*>(mTreeView))->SetTabs(gApp->mOptions.stWinTabs.nFilesTab1, gApp->mOptions.stWinTabs.nFilesTab2);
	(dynamic_cast<CMyFileListView*>(mFileListView))->SetTabs(gApp->mOptions.stWinTabs.nFilesTab3, gApp->mOptions.stWinTabs.nFilesTab4);
}

void CMyFileExplWin::GetBoundsInfo()
{
	GetBounds(gApp->mOptions.stWinRect.stFiles2);

	gApp->mOptions.stWinPane.nFilesPane1 = mFilePaneView->GetPanePercent();
	
	(dynamic_cast<CMyFileTreeView*>(mTreeView))->GetTabs(gApp->mOptions.stWinTabs.nFilesTab1, gApp->mOptions.stWinTabs.nFilesTab2);
	(dynamic_cast<CMyFileListView*>(mFileListView))->GetTabs(gApp->mOptions.stWinTabs.nFilesTab3, gApp->mOptions.stWinTabs.nFilesTab4);
}

bool CMyFileExplWin::GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode, Uint32 *outCreatorCode, const SMouseMsgData *inInfo)
{
	outItemName[0] = 0;
	if (outTypeCode) 
		*outTypeCode = 0;

	if (mTreeView->IsFocus())
		return GetFileTreeView()->GetSelectedItemName(outItemName, outTypeCode, outCreatorCode, inInfo);
	else
//	if (mFileListView->IsFocus())	// Focus event comes after MouseDown event if another window was selected
		return mFileListView->GetSelectedItemName(outItemName, outTypeCode, outCreatorCode, inInfo);

	return false;
}

void *CMyFileExplWin::GetSelectedParentFolderPath(Uint32& outPathSize, bool *outDisposePath)
{
	outPathSize = 0;

	if (outDisposePath)
		*outDisposePath = true;
	
	if (mTreeView->IsFocus())
		return GetFileTreeView()->GetSelectedParentFolderPath(outPathSize);
	
	if (mFileListView->IsFocus())
		return GetFileTreeView()->GetSelectedFolderPath(outPathSize);
	
	return nil;
}

void *CMyFileExplWin::GetSelectedFolderPath(Uint32& outPathSize)
{
	Uint8 psItemName[256];
	void *pPathData = GetFileTreeView()->GetSelectedFolderPath(outPathSize);

	if (mFileListView->IsFocus() && mFileListView->GetSelectedItemName(psItemName))
	{
		void *pTempPathData = pPathData;
		
		pPathData = UFileSys::MakePathData(pTempPathData, outPathSize, psItemName, outPathSize);
		UMemory::Dispose((TPtr)pTempPathData);
	}

	return pPathData;
}

void CMyFileExplWin::DoUpload(Uint16 inMods)
{
	Uint32 nPathSize;
	void *pPathData = GetSelectedFolderPath(nPathSize);
	
	gApp->DoFileUpload(this, pPathData, nPathSize, inMods);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::DoUploadFromDrop(bool inTreeList)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();

	Uint32 nPathSize;
	void *pPathData;
	
	if (inTreeList)
	{
		if (!pFileTreeView->GetDropFolderPath(&pPathData, nPathSize))
		{
			ClearUploadList();
			return;
		}
	}
	else
		pPathData = pFileTreeView->GetSelectedFolderPath(nPathSize);

	gApp->DoFileUploadFromDrop(this, pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::DoNewFolder()
{
	Uint32 nPathSize;
	void *pPathData = GetFileTreeView()->GetSelectedFolderPath(nPathSize);

	gApp->DoFileNewFolder(dynamic_cast<CMyItemsWin *>(this), pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::DoRefresh(Uint16 inMods)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();
	CMyFileListView *pFileListView = GetFileListView();

	pFileListView->DeleteAll();
	pFileListView->SetStatus(listStat_Loading);

	if (inMods & modKey_Option)
		pFileTreeView->DeselectAllTreeItems();
	
	Uint32 nPathSize;
	void *pPathData = pFileTreeView->RefreshSelectedFolderPath(nPathSize);
	
	if (!pFileTreeView->GetTreeCount())
	{
		SetStatus(listStat_Loading);
		pFileListView->SetStatus(listStat_0Items);
	}
	
	gApp->DoFileRefresh(pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::DoMove(Uint16 inMods, const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inTreeList)
{
	CMyFileTreeView *pFileTreeView = GetFileTreeView();

	Uint32 nPathSize;
	void *pPathData;

	if (inTreeList)
	{
		if (!pFileTreeView->GetDropFolderPath(&pPathData, nPathSize))
			return;
	}
	else
	{
		if (!pFileTreeView->GetFirstSelectedTreeItem())
			return;
		
		pPathData = pFileTreeView->GetSelectedFolderPath(nPathSize);
	}

	gApp->DoFileMove(dynamic_cast<CMyItemsWin *>(this), inMods, inFileName, inPathData, inPathSize, pPathData, nPathSize);
	UMemory::Dispose((TPtr)pPathData);
}

void CMyFileExplWin::DoDoubleClick(Uint16 inMods, bool inTreeList)
{
	Uint8 psFileName[256];
	bool bIsFolder = GetSelectedItemName(psFileName);
	if (!psFileName[0]) 
		return;

	if (inTreeList)
	{
		if (bIsFolder)
		{
			Uint32 nPathSize;
			void *pPathData = GetSelectedParentFolderPath(nPathSize);	

			gApp->DoFileDoubleClick(dynamic_cast<CMyItemsWin *>(this), psFileName, pPathData, nPathSize, inMods);
			UMemory::Dispose((TPtr)pPathData);
		}
	}
	else
	{
		if (bIsFolder)
			GetFileTreeView()->SelectChildNames(psFileName);
		else
			DoDownload(0);
	}
}

void CMyFileExplWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (File_KeyDown(inInfo))
		return;
		
	CWindow::KeyDown(inInfo);
}

void CMyFileExplWin::Timer(TTimer inTimer)
{
	if (inTimer == mFileListTimer)
		GetContentFileList();
}

void CMyFileExplWin::SearchText(const Uint8 *inText)
{
	if (mTreeView->IsFocus())
	{
		CMyFileTreeView *pFileTreeView = GetFileTreeView();
		Uint32 nIndex = pFileTreeView->SearchNames(inText);
		
		if (nIndex != pFileTreeView->GetFirstSelectedTreeItem())
		{
			pFileTreeView->DeselectAllTreeItems();
		
			if (nIndex)
			{
				pFileTreeView->SelectTreeItem(nIndex);
				pFileTreeView->MakeTreeItemVisibleInList(nIndex);
			}
		}
	}
	else if (mFileListView->IsFocus())
	{
		Uint32 nIndex = mFileListView->SearchNames(inText);
		
		if (nIndex != mFileListView->GetFirstSelectedItem())
		{
			mFileListView->DeselectAll();
		
			if (nIndex)
			{
				mFileListView->SelectItem(nIndex);
				mFileListView->MakeItemVisible(nIndex);
			}
		}
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyUserListWin::CMyUserListWin(CWindow *inParent)
	: CWindow(SRect(0,0,200,220), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox + windowOption_ZoomBox, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 237);
#endif

	CContainerView *vc;
	CScrollerView *scr;
	CIconButtonView *icb;
	CBoxView *box;
	CLabelView *lbl;

	// setup window
	SetTitle("\pOnline Users");
	SetLimits(160,150);
	
	mSearchTimer = nil;
	mCurrentDrag = nil;
	
	// make container view for content
	vc = new CContainerView(this, SRect(0,0,200,220));
	vc->Show();
	
	// make background box
	box = new CBoxView(vc, SRect(0,0,200,30));
	box->SetStyle(boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();
	mViews.box = box;
	
	// make buttons
	icb = new CIconButtonView(vc, SRect(3,3,27,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3031);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(415);
		}
	
	icb->SetID(viewID_OpenPrivateChat);
	icb->SetTooltipMsg("\pPrivate Chat", "\pCtrl-F4");
	icb->Show();
	mViews.chatBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(30,3,54,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3030);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(206);
		}
	
	icb->SetID(viewID_SendPrivMsg);
	icb->SetTooltipMsg("\pPrivate Message", "\pCtrl-F9");
	icb->Show();
	mViews.msgBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(57,3,81,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3007);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(215);
		}
	
	
	icb->SetID(viewID_GetInfo);
	icb->SetTooltipMsg("\pGet Client Info", "\pCtrl-I");
	icb->Show();
	mViews.infoBtn = icb;
	/*
	icb = new CIconButtonView(vc, SRect(84,3,108,27));
	icb->SetIconID(216);
	icb->SetID(viewID_GetAdminSpectorInfo);
	icb->SetTooltipMsg("\pGet Admin Info", "\pCtrl-D");
	icb->Show();
	mViews.infoDLBtn = icb;
	*/
	

	#if USE_HELP
	icb = new CIconButtonView(vc, SRect(84,3,108,27));
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpUsers);
	icb->SetTooltipMsg("\pHelp");
	icb->Show();
	#endif
	
	SRect rBounds;
	GetBounds(rBounds);
	icb = new CIconButtonView(vc, SRect(rBounds.right-27, 3, rBounds.right-3, 27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3005);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(412);
		}
	
	icb->SetID(viewID_Delete);
	icb->SetSizing(sizing_HorizSticky);
	icb->SetTooltipMsg("\pDisconnect", "\pCtrl-Del");
	icb->Show();
	mViews.discBtn = icb;

	// make search text
	lbl = new CLabelView(vc, SRect(86, 10, rBounds.right-32, 24));
	lbl->SetFont(fd_Default9Bold);
	lbl->SetSizing(sizing_RightSticky);
	lbl->Show();
	mViews.searchText = lbl;

	// make list scroller
	scr = new CScrollerView(vc, SRect(-1,30,201,221));
	scr->SetOptions(scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_GrowSpace + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	vc->SetFocusView(scr);
	mViews.listScroll = scr;

	// make user list
	CMyUserListView *list = new CMyUserListView(scr, SRect(0,0,scr->GetVisibleContentWidth(),scr->GetVisibleContentHeight()));
	list->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	list->SetCanFocus(true);
	list->SetSizing(sizing_RightSticky + sizing_FullHeight);
	list->SetID(viewID_UserList);
	list->SetDragAndDropHandler(this);
//	list->SetStatus(listStat_NotOnline);
	list->Show();
	mViews.list = list;

	DisableIcons();
}

CMyUserListWin::~CMyUserListWin()
{
	UTimer::Dispose(mSearchTimer);
}

void CMyUserListWin::KeyDown(const SKeyMsgData& inInfo)
{
	if ((inInfo.mods & modKey_Command) && UText::tolower(inInfo.keyChar) == 'i')
		mViews.infoBtn->HitButton();
		
		
	else if (UText::IsPrinting(inInfo.keyChar))
	{
		Uint8 c = inInfo.keyChar;
		mViews.searchText->AppendText(&c, sizeof(c));
		
		if (!mSearchTimer)
			mSearchTimer = NewTimer();
	
		UTimer::Start(mSearchTimer, 1000);
	}
	else if (inInfo.keyChar == '\r' && mViews.searchText->GetTextSize())
	{
		UTimer::Stop(mSearchTimer);
		SearchUser();
	}
	else
		CWindow::KeyDown(inInfo);
}

void CMyUserListWin::Timer(TTimer inTimer)
{
	if (inTimer == mSearchTimer)
		SearchUser();
}

void CMyUserListWin::SearchUser()
{
	Uint8 psUserName[256];	
	psUserName[0] = mViews.searchText->GetText(psUserName + 1, sizeof(psUserName) - 1);	
	if (!psUserName[0])
		return;
		
	mViews.searchText->SetText(nil, 0);
	
	Uint32 nIndex = mViews.list->SearchNames(psUserName);
	mViews.list->DeselectAll();
	if (!nIndex)
		return;
	
	mViews.list->SelectItem(nIndex);
	mViews.list->MakeItemVisible(nIndex);
}

void CMyUserListWin::UserZoom(const SMouseMsgData& /* inInfo */)
{
	SRect r;
	GetBounds(r);
	
	r.bottom = r.top + mViews.list->GetFullHeight() + 31;
	
	if (UWindow::GetZoomBounds(mWindow, r, r))
		SetBounds(r);
}

void CMyUserListWin::HandleDrag(CDragAndDroppable */* inDD */, const SMouseMsgData& inInfo)
{
	if (!UDragAndDrop::IsAvailable()) 
		return;

	StDrag drag;
	TRegion rgn, imgRgn;
	TImage img;
	SRect r;
	SPoint imgPt, mouseLoc;
	Int32 hd, vd;
	Uint8 name[256];
	
	Uint32 id = mViews.list->GetSelectedUserID(name);
	if (id == 0) return;
		
	// get global mouse loc
	mViews.list->GetScreenDelta(hd, vd);
	mouseLoc.h = inInfo.loc.h + hd;
	mouseLoc.v = inInfo.loc.v + vd;

	drag->AddFlavor(1, 'HLUI', &id, sizeof(id), flavorFlag_SenderOnly);
	drag->AddTextFlavor(1, name+1, name[0]);
	
	rgn = imgRgn = nil;
	img = nil;
	
	try
	{
		rgn = URegion::New();
		mViews.list->GetSelectedItemsRegion(rgn);
		
		rgn->GetBounds(r);
		
	#if MACINTOSH
		if ((((Uint32)r.GetWidth() * (Uint32)r.GetHeight()) <= 65536) && UDragAndDrop::HasTranslucentDrag())
		{
			imgRgn = URegion::New(rgn);
			imgRgn->GetBounds(r);
			imgRgn->Move(-r.left, -r.top);
			
			img = mViews.list->GetSelectedItemsImage(imgPt);
			drag->SetImage(img, imgRgn, imgPt, dragImageStyle_Dark);
		}
	#endif

		rgn->Move(hd, vd);
		rgn->SetOutline(2, 2);
		mCurrentDrag = drag;
		drag->Track(mouseLoc, rgn);
		mCurrentDrag = nil;
	}
	catch(...)
	{
		mCurrentDrag = nil;
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

void CMyUserListWin::DoPrivateChat()
{
	Uint32 nUserID = GetUserListView()->GetSelectedUserID();
	if (nUserID == 0) 
		return;

	gApp->DoOpenPrivateChat(nUserID);
}

void CMyUserListWin::DoPrivateMsg()
{
	Uint8 psUserName[64];
	Uint16 nUserID = GetUserListView()->GetSelectedUserID(psUserName);
	if (nUserID == 0) 
		return;
	
	gApp->DoSendPrivMsg(nUserID, psUserName);
}

void CMyUserListWin::DoGetInfo()
{
	Uint8 psUserName[64];
	Uint16 nUserID = GetUserListView()->GetSelectedUserID(psUserName);
	if (nUserID == 0) 
		return;
	
	gApp->DoGetClientInfo(nUserID, psUserName);
}



void CMyUserListWin::SetAccess()
{
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_CreateChat))
		mViews.chatBtn->Enable();
	else
		mViews.chatBtn->Disable();
	
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_SendMessage))
		mViews.msgBtn->Enable();
	else
		mViews.msgBtn->Disable();

	if (gApp->HasGeneralPriv(myAcc_GetClientInfo)) 
		mViews.infoBtn->Enable();
	else
		mViews.infoBtn->Disable();
	


		
	if (gApp->HasGeneralPriv(myAcc_DisconUser))
	{
		mViews.discBtn->Enable();
		mViews.discBtn->Show();
	}
	else
		mViews.discBtn->Hide();
}

void CMyUserListWin::DisableIcons()
{
	mViews.chatBtn->Disable();
	mViews.msgBtn->Disable();
	mViews.infoBtn->Disable();
	
	
		
	mViews.discBtn->Disable();
	mViews.discBtn->Show();
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyEnterMsgWin::CMyEnterMsgWin()
	: CWindow(SRect(0,0,420,256), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	CIconView *icn;

	// setup window
	SetTitle("\pPost Message");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,420,256));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();

	// make labels
	lbl = new CLabelView(vc, SRect(50,10,407,46));
	lbl->SetText("\pType a message below.  When you click ÔSendÕ, the message will be posted for discussion.");
	lbl->Show();
	
	// make text boxes
	scr = MakeTextBoxView(vc, SRect(10,50,410,210), scrollerOption_Border + scrollerOption_VertBar, &mViews.msgText);
	scr->Show();
	vc->SetFocusView(scr);
	mViews.msgText->SetFont(fd_Fixed9);

	// make buttons
	SButtons btns[] = {{1, "\pSend", btnOpt_Default, nil}, 
					   {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(228,220,408,246), btns);
}


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySendPrivMsgWin::CMySendPrivMsgWin()
	: CWindow(SRect(0,0,340,264), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CIconView *icn;
	//CButtonView *btn;
	// setup window
	SetTitle("\pSend Message");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,340,264));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();

	
	// make buttons
	CButtonView *btnSend, *btnCancel;
	SButtons btns[] = {{1, "\pSend", btnOpt_Default, &btnSend}, 
					   {2, "\pCancel", btnOpt_Cancel, &btnCancel}};
	CButtonView::BuildButtons(vc, SRect(150,230,330,256), btns);
	btnSend->SetSizing(sizing_VertSticky);
	btnCancel->SetSizing(sizing_VertSticky);
	
	/*
	btn = new CButtonView(vc, SRect(10,230,60,256));
	btn->SetTitle("\pFlood");
	btn->SetDefault(false);
	btn->SetID(viewID_Flood);
	btn->Show();
	vc->SetDefaultView(btn);
	*/
	
	// make labels
	lbl = new CLabelView(vc, SRect(50,10,294,44));
	lbl->SetText("\pType a message and click the 'Send' button to send it privately to:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(50,47,320,63));
	lbl->Show();
	mViews.toText = lbl;

	// make quoting box
	mViews.quotScroll = MakeTextBoxView(vc, SRect(10,70,330,133), scrollerOption_Border + scrollerOption_VertBar, &mViews.quotText);
	mViews.quotScroll->SetSizing(sizing_RightSticky);

	mViews.quotText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.quotText->SetColor(color_Gray);
	mViews.quotText->SetEditable(false);
	mViews.quotText->SetTabSelectText(false);

	// make text box
	mViews.msgScroll = MakeTextBoxView(vc, SRect(10,70,330,220), scrollerOption_Border + scrollerOption_VertBar, &mViews.msgText);
	mViews.msgScroll->SetSizing(sizing_BottomSticky);
	mViews.msgScroll->Show();
	vc->SetFocusView(mViews.msgScroll);
	mViews.msgText->SetFont(fd_Fixed9);
	mViews.msgText->SetTabSelectText(false);
	
	
	
/*
	// make buttons
	CButtonView *btnSend, *btnCancel;
	SButtons btns[] = {{1, "\pSend", btnOpt_Default, &btnSend}, 
					   {2, "\pCancel", btnOpt_Cancel, &btnCancel}};
	CButtonView::BuildButtons(vc, SRect(150,230,330,256), btns);
	btnSend->SetSizing(sizing_VertSticky);
	btnCancel->SetSizing(sizing_VertSticky);
	*/
}

void CMySendPrivMsgWin::SetQuotHandle(THdl inHdl)
{
	mViews.quotText->SetTextHandle(inHdl);

	SRect r;
	GetBounds(r);
	r.bottom += 70;
	SetBounds(r);
	
	mViews.msgScroll->GetBounds(r);
	r.top += 70;
	mViews.msgScroll->SetBounds(r);

	mViews.quotScroll->Show();
	
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyChangeIconWin::CMyChangeIconWin()
	: CWindow(SRect(0,0,310,100), windowLayer_Modal)
{

	CContainerView *vc;
	CButtonView *btn, *btn2;
	CScrollerView *scr;
	CScrollerView *scr2;
	btn2=nil;

	// setup window
	SetTitle("\pUser Changer");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	
	
	// make container view for content
	vc = new CContainerView(this, SRect(0,0,310,100));
	vc->Show();


// make labels
	CLabelView *lbl = new CLabelView(vc, SRect(10,10,130,36));
	lbl->SetText("\pEnter an Icon Number:");
	lbl->Show();
	
	
	CLabelView *lbl2 = new CLabelView(vc, SRect(10,40,130,66));
	lbl2->SetText("\pEnter a Nickname:");
	lbl2->Show();

	
	
	// make text boxes
	scr = MakeTextBoxView(vc, SRect(135,10,175,36), scrollerOption_Border, &mViews.iconText);
	mViews.iconText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	
	vc->SetFocusView(scr);
	
	
	scr2 = MakeTextBoxView(vc, SRect(135,40,300,66), scrollerOption_Border, &mViews.nickText);
	mViews.nickText->SetEnterKeyAction(enterKeyAction_None);
	scr2->Show();
	
	vc->SetFocusView(scr2);
	
	
	
	
	// make buttons
	
	btn = new CButtonView(vc, SRect(10,70,50,96));
	
	btn->SetTitle("\pGo!");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);
	
	/*
	btn = new CButtonView(vc, SRect(80,70,140,96));
	
	btn->SetTitle("\pCancel!");
	btn->SetID(viewID_Cancel);
	btn->Show();
	
	vc->SetCancelView(btn);
	
	*/
	
}
//######################################################

void CMyChangeIconWin::GetInfo(Uint8 *outIcon, Uint8 *outNick)
{
	outIcon[0] = mViews.iconText->GetText(outIcon+1, 31);
	outNick[0] = mViews.nickText->GetText(outNick+1, 31);
	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

void CMyChangeIconWin::SetInfo(const Uint8 *inIconId, const Uint8 *inNickId)
{
	
	
	if (inIconId) 
		mViews.iconText->SetText(inIconId+1, inIconId[0]);
		
		
if (inNickId) 
		mViews.nickText->SetText(inNickId+1, inNickId[0]);
	
}


#pragma mark -

CMyCouldNotSendPrivMsgWin::CMyCouldNotSendPrivMsgWin(const Uint8 *inUserName, THdl inTextHdl)
	: CWindow(SRect(0,0,340,240), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CScrollerView *scr;
	CTextView *txt;

	// setup window
	SetTitle("\pUser has Disconnected");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,340,240));
	vc->Show();

	// make label
	Uint8 psMessage[256];
	psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "Could not send the private message to user %#s because the user is no longer connected to the Server.\r\rA copy of your original message is below:", inUserName);

	lbl = new CLabelView(vc, SRect(10,10,321,94));
	lbl->SetText(psMessage);
	lbl->Show();

	// make text box
	scr = MakeTextBoxView(vc, SRect(10,96,330,196), scrollerOption_Border + scrollerOption_VertBar, &txt);
	scr->Show();

	txt->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	txt->SetColor(color_Gray);
	txt->SetEditable(false);
	txt->SetTabSelectText(false);
	txt->SetTextHandle(inTextHdl);

	// make button
	SButtons btnOK = {1, "\pOk", btnOpt_Default, &mViews.OKBtn};
	CButtonView::BuildButton(vc, SRect(244,206,330,132), &btnOK);
	mViews.OKBtn->Disable();
	
	mMsgTimer = StartNewTimer(500, false);	// 0.5 secs
}

CMyCouldNotSendPrivMsgWin::~CMyCouldNotSendPrivMsgWin()
{
	UTimer::Dispose(mMsgTimer);
}

void CMyCouldNotSendPrivMsgWin::Timer(TTimer inTimer)
{
	if (inTimer == mMsgTimer)
	{
		if (mViews.OKBtn->IsDisabled())
			mViews.OKBtn->Enable();
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyBroadcastWin::CMyBroadcastWin()
	: CWindow(SRect(0,0,340,264), windowLayer_Modal)
{
	// setup window
	SetTitle("\pBroadcast");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	CContainerView *vc = new CContainerView(this, SRect(0,0,340,264));
	vc->Show();

	// make icon
	CIconView *icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();
    
    //help
    #if USE_HELP
	CIconButtonView* ibv = new CIconButtonView(vc, SRect(295,10,320,34));
	ibv->SetIconID(iconID_HelpToolbar);
	ibv->SetID(viewID_HelpBroadcast);
	ibv->SetTooltipMsg("\pHelp");
	ibv->Show();
	#endif
	
	// make labels
	CLabelView *lbl = new CLabelView(vc, SRect(50,10,290,44));
	lbl->SetText("\pEnter a message to display to all connected users and click the 'Send' button");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(50,47,320,63));
	lbl->Show();

	// make text boxes
	CScrollerView *scr = MakeTextBoxView(vc, SRect(10,70,330,220), scrollerOption_Border + scrollerOption_VertBar, &mViews.msgText);
	scr->Show();
	vc->SetFocusView(scr);
	mViews.msgText->SetFont(fd_Fixed9);
	mViews.msgText->SetTabSelectText(false);

	// make buttons
	SButtons btns[] = {{1, "\pSend", btnOpt_Default, nil}, 
					   {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(150,230,330,256), btns);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyPrivMsgWin::CMyPrivMsgWin(CWindow *inParent, bool bIsAutomaticResponse)
	: CWindow(SRect(0,0,320,160), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 206);
#endif

	CContainerView *vc;
	CIconButtonView *icb;

	SetLimits(266, 120);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,320,160));
	vc->Show();

	// make quoting box
	mViews.quotScroll = MakeTextBoxView(vc, SRect(10,25,310,88), scrollerOption_Border + scrollerOption_VertBar, &mViews.quotText);
	mViews.quotScroll->SetSizing(sizing_RightSticky);

	mViews.quotText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.quotText->SetColor(color_Gray);
	mViews.quotText->SetEditable(false);
	mViews.quotText->SetEnterKeyAction(enterKeyAction_None, enterKeyAction_None);
	mViews.quotText->SetTabSelectText(false);

	// make text box
	mViews.msgScroll = MakeTextBoxView(vc, SRect(10,10,310,100), scrollerOption_Border + scrollerOption_VertBar, &mViews.msgText);
	mViews.msgScroll->SetSizing(sizing_BottomRightSticky);
	mViews.msgScroll->Show();
	vc->SetFocusView(mViews.msgScroll);
	
	mViews.msgText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.msgText->SetEditable(false);
	mViews.msgText->SetEnterKeyAction(enterKeyAction_None, enterKeyAction_None);
	mViews.msgText->SetTabSelectText(false);
	
	// make label
	mViews.quotLabel = new CLabelView(vc, SRect(10, 6, 310, 22));
	mViews.quotLabel->SetSizing(sizing_RightSticky);

	mViews.msgLabel = new CLabelView(vc, SRect(10, 90, 310, 106));
	mViews.msgLabel->SetSizing(sizing_RightSticky);

	Uint8 str[64];
	str[0] = UMemory::Copy(str+1,"Received: ", 10);
	str[0] += UDateTime::DateToText(str + str[0] + 1, sizeof(str) - str[0] - 1, kShortDateText + kTimeWithSecsText);
	
	CLabelView *lbl = new CLabelView(vc, SRect(10,106,310,122));
	lbl->SetSizing(sizing_RightSticky + sizing_VertSticky);
	lbl->SetText(str);
	lbl->Show();
	
	// make buttons
	icb = new CIconButtonView(vc, SRect(10,128,34,152));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3031);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(415);
		}
	
	icb->SetID(viewID_OpenPrivateChat);
	icb->SetTooltipMsg("\pPrivate Chat", "\pCtrl-F4");
	icb->SetSizing(sizing_VertSticky);
	icb->Show();
	mViews.chatBtn = icb;
		
	icb = new CIconButtonView(vc, SRect(37,128,61,152));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3007);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(215);
		}
	
	
	icb->SetID(viewID_GetInfo);
	icb->SetTooltipMsg("\pGet Client Info", "\pCtrl-I");
	icb->SetSizing(sizing_VertSticky);
	icb->Show();
	mViews.infoBtn = icb;

	mViews.replyBtn=nil;
	if (!bIsAutomaticResponse) //if is an automatic response don't show Reply button
	{
		SButtons btnReplay = {viewID_ReplyPrivMsg, "\pReply...", btnOpt_Default, &mViews.replyBtn};
		CButtonView::BuildButton(vc, SRect(124,128,194,154), &btnReplay);
		mViews.replyBtn->SetSizing(sizing_HorizSticky + sizing_VertSticky);
	}
	
	SButtons btnDismiss = {viewID_DismissPrivMsg, "\pDismiss", btnOpt_Regular, &mViews.dismissBtn};
	CButtonView::BuildButton(vc, SRect(216,128,302,154), &btnDismiss);
	mViews.dismissBtn->SetSizing(sizing_HorizSticky + sizing_VertSticky);
	mViews.dismissBtn->Disable();
	
	mUserID = 0;
	mUserName[0] = 0;

	mMsgTimer = StartNewTimer(500, false);	// 0.5 secs
}

CMyPrivMsgWin::~CMyPrivMsgWin()
{
	UTimer::Dispose(mMsgTimer);
}

// if fails, does not dispose or take ownership of handle
void CMyPrivMsgWin::SetTextHandle(THdl inHdl)
{
	Uint32 h;
	SRect r;
	
	mViews.msgText->SetTextHandle(inHdl);
	
	try
	{
		mViews.msgText->SetFullHeight();
		h = mViews.msgText->GetFullHeight();
		
		if (h > mViews.msgScroll->GetVisibleContentHeight())
		{
			if (h > 250) h = 250;
			
			GetBounds(r);
			r.bottom = r.top + h + 74;
			SetBounds(r);
		}
		
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	}
	catch(...)
	{
		mViews.msgText->DetachTextHandle();
		throw;
	}
}

void CMyPrivMsgWin::SetQuotHandle(THdl inHdl)
{
	mViews.quotText->SetTextHandle(inHdl);
	mViews.quotLabel->SetText("Your original message:", 22);

	Uint8 str[64];
	str[0] = UText::Format(str+1, sizeof(str)-1, "<%#s> reply:", mUserName);
	mViews.msgLabel->SetText(str+1, str[0]);

	SRect r;
	GetBounds(r);
	r.bottom += 100;
	SetBounds(r);
	
	mViews.msgScroll->GetBounds(r);
	r.top += 100;
	mViews.msgScroll->SetBounds(r);

	mViews.quotLabel->Show();
	mViews.quotScroll->Show();
	mViews.msgLabel->Show();

	SetLimits(266, 230);
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
}

void CMyPrivMsgWin::SetAccess()
{
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_CreateChat))
		mViews.chatBtn->Enable();
	else
		mViews.chatBtn->Disable();
	
	if (gApp->HasGeneralPriv(myAcc_GetClientInfo)) 
		mViews.infoBtn->Enable();
	else
		mViews.infoBtn->Disable();
}

void CMyPrivMsgWin::DoPrivateChat()
{
	gApp->DoOpenPrivateChat(mUserID);
}

void CMyPrivMsgWin::DoGetInfo()
{
	gApp->DoGetClientInfo(mUserID, mUserName);
}

void CMyPrivMsgWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (mViews.replyBtn && UText::tolower(inInfo.keyChar) == 'r')
	{
		mViews.replyBtn->Flash();
		return;
	}
	
	if (inInfo.keyCode == key_Escape)
	{
		SMouseMsgData mi;
		ClearStruct(mi);
		UserClose(mi);
	}
	
	CWindow::KeyDown(inInfo);
}

void CMyPrivMsgWin::Timer(TTimer inTimer)
{
	if (inTimer == mMsgTimer)
	{
		if (mViews.dismissBtn->IsDisabled())
			mViews.dismissBtn->Enable();
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyOldNewsWin::CMyOldNewsWin(CWindow *inParent)
	: CWindow(SRect(0,0,370,300), windowLayer_Standard, windowOption_CloseBox + windowOption_Sizeable, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 413);
#endif

	CContainerView *vc;
	CScrollerView *scr;

	// make container view for content
	SRect cBounds(0,0,370,300);
	vc = new CContainerView(this, cBounds);
	vc->Show();
	
	// make background box
	CBoxView *box = new CBoxView(vc, SRect(0, 0, cBounds.right, 30), boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();
	
	// make post button
	CIconButtonView *icb = new CIconButtonView(vc, SRect(3,3,27,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3024);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(223);
		}
		
	
	
	
	icb->SetID(viewID_OldNewsPost);
	icb->SetTooltipMsg("\pPost a Message");
	icb->Show();
	
	// make text scroller
	scr = MakeTextBoxView(vc, SRect(-2,28,372,302), scrollerOption_PlainBorder + scrollerOption_VertBar + scrollerOption_GrowSpace , &mTextView);
	scr->SetSizing(sizing_BottomRightSticky);
	mTextView->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mTextView->SetEditable(false);
	mTextView->SetTabSelectText(false);
	mScrollerView = scr;
	scr->Show();

	vc->SetFocusView(scr);
}

void CMyOldNewsWin::SetTextColor(const SMyColorInfo& colorInfo) 
{
	mScrollerView->SetContentColor(colorInfo.backColor);

	mTextView->SetColor(colorInfo.textColor);
	mTextView->SetFontSize(colorInfo.textSize);
}

#pragma mark -

CMyTextWin::CMyTextWin(CWindow *inParent)
	: CWindow(SRect(0,0,370,300), windowLayer_Standard, windowOption_CloseBox + windowOption_Sizeable + windowOption_WhiteBkgnd, 0, inParent)
{
	CContainerView *vc;
	CScrollerView *scr;

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,370,300));
	vc->Show();
	
	// make text scroller
	scr = MakeTextBoxView(vc, SRect(-2,-2,372,302), scrollerOption_PlainBorder + scrollerOption_VertBar + scrollerOption_GrowSpace , &mTextView);
	scr->SetSizing(sizing_BottomRightSticky);
	mTextView->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mTextView->SetEditable(false);
	mTextView->SetTabSelectText(false);
	mScrollerView = scr;
	scr->Show();

	vc->SetFocusView(scr);
}

void CMyTextWin::SetTextColor(const SMyColorInfo& colorInfo) 
{
	mScrollerView->SetContentColor(colorInfo.backColor);

	mTextView->SetColor(colorInfo.textColor);
	mTextView->SetFontSize(colorInfo.textSize);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySaveFileWin::CMySaveFileWin(CWindow *inParent, const SRect& inBounds)
	: CWindow(inBounds, windowLayer_Standard, windowOption_CloseBox + windowOption_Sizeable + windowOption_WhiteBkgnd, 0, inParent)
{
	mPathData = nil;
	mPathSize = 0;
}

CMySaveFileWin::~CMySaveFileWin()
{
	ClearPathData();
}
	
bool CMySaveFileWin::SaveFileAs()
{
	try
	{
		TFSRefObj* dlFolder = gApp->GetDownloadFolder();
		scopekill(TFSRefObj, dlFolder);
		
		mTempFile.SaveFileAs(dlFolder);
	}
	catch(...)
	{
		gApp->DisplayStandardMessage("\pSave As", "\pThe file cannot be saved because it is in use.", icon_Stop, 1);
		return false;
	}
	
	return true;
}

bool CMySaveFileWin::IsThisFile(const void *inPathData, Uint32 inPathSize, const Uint8 *inFileName)
{
	if (mPathData && inPathData)
	{
		if (UMemory::Compare(mPathData, mPathSize, inPathData, inPathSize))
			return false;
	}
	else if (mPathData || inPathData)
		return false;
	
	return mTempFile.CompareFileName(inFileName);
}
	
void CMySaveFileWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (UText::tolower(inInfo.keyChar) == 's' && inInfo.mods == modKey_Shortcut)
		SaveFileAs();
		
	CWindow::KeyDown(inInfo);
}

void CMySaveFileWin::SetPathData(const void *inPathData, Uint32 inPathSize)
{
	if (inPathData && inPathSize)
	{
		if (mPathData)
			UMemory::Dispose(mPathData);
				
		try
		{
			mPathData = UMemory::New(inPathData, inPathSize);
			mPathSize = inPathSize;
		}
		catch(...)
		{
			mPathData = nil;
			mPathSize = 0;
			// don't throw
		}		
	}
}

void CMySaveFileWin::ClearPathData()
{
	if (mPathData)
	{	
		UMemory::Dispose(mPathData); 
		mPathData = nil;
	}

	mPathSize = 0;
	mTempFile.DeleteFile();
}

bool CMySaveFileWin::SetTempFile(const Uint8 *inFileName, Uint32 inTypeCode, Uint32 inCreatorCode)
{
	return mTempFile.SetTempFile(inFileName, inTypeCode, inCreatorCode);
}

bool CMySaveFileWin::SetTempData(THdl inData)
{
	if (!inData)
		return false;

	Uint32 nDataSize = inData->GetSize();
	if (!nDataSize)
		return false;

	void *pData;
	StHandleLocker stLocker(inData, pData);

	mTempFile.WriteFileData(pData, nDataSize);

	return true;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyViewFileWin::CMyViewFileWin(CWindow *inParent)
	: CMySaveFileWin(inParent, SRect(0,0,370,300))
{
#if WIN32
	_SetWinIcon(*this, 417);
#endif
	
	gApp->mViewWinList.AddItem(this);

	mContainerView = nil;
	mScrollerView = nil;
	
	mTextView = nil;
	mImageView = nil;
	mQuickTimeView = nil;

	mDecompressImage = nil;
	mViewFileTask = nil;
		
	mBuffer = nil;
	mBufferSize = 0;
	mBufferOffset = 0;

	mNewWindow = true;
	mWinBounds.SetEmpty();
	mMustCloseFlag = false;
}

CMyViewFileWin::~CMyViewFileWin()
{
	// a CQuickTimeView must be deleted in the parent window destructor
	if (mQuickTimeView)
		delete mQuickTimeView;

	ClearData();
		
	if (mViewFileTask)
	{
		mViewFileTask->EndViewFileWin();
		delete mViewFileTask; 
	}
	
	gApp->mViewWinList.RemoveItem(this);
}

void CMyViewFileWin::KeyUp(const SKeyMsgData& inInfo)
{
	if (mImageView)
		mImageView->KeyUp(inInfo);

	CMySaveFileWin::KeyUp(inInfo);
}

bool CMyViewFileWin::SetViewFileWin(CMyViewFileTask* inViewFileTask, const void *inPathData, Uint32 inPathSize, const Uint8 *inFileName, Uint32 inTypeCode, Uint32 inCreatorCode, Uint32 inFileSize, Uint32 inTotalSize, Uint32 inRefNum, bool inNewWindow)
{
	mViewFileTask = inViewFileTask;
 	mNewWindow = inNewWindow;

	ClearViews();
	ClearData();
	
	SetPathData(inPathData, inPathSize);
	mTempFile.SetTempFile(inFileName, inTypeCode, inCreatorCode);
	
	// setup window
	Uint8 psTitle[256];
	psTitle[0] = UText::Format(psTitle + 1, sizeof(psTitle) - 1, "View: %#s", inFileName);
	SetTitle(psTitle);
 	
	if (!mNewWindow && IsCollapsed())
		Expand();
	
	if (inTypeCode == TB('TEXT') || inTypeCode == TB('JPEG') || inTypeCode == TB('GIFf') || inTypeCode == TB('PICT') || inTypeCode == TB('BMP '))
	{
		mBufferOffset = 0;
		mBufferSize = inTotalSize;
		mBuffer = UMemory::NewClear(mBufferSize);

		if (inTypeCode == TB('TEXT'))
		{
			GetBounds(mWinBounds);
			mWinBounds.right = mWinBounds.left + 370; 
			mWinBounds.bottom = mWinBounds.top + 300;
		
			SetLimits(300, 100);
			SetBounds(mWinBounds);
	
			// make container view
			mContainerView = new CContainerView(this, SRect(0, 0, mWinBounds.GetWidth(), mWinBounds.GetHeight()));
			mContainerView->Show();

			// make text scroller
			mScrollerView = MakeTextBoxView(mContainerView, SRect(-2, -2, mWinBounds.GetWidth()+2, mWinBounds.GetHeight()+2), scrollerOption_PlainBorder + scrollerOption_VertBar + scrollerOption_GrowSpace , &mTextView);
			mScrollerView->SetSizing(sizing_BottomRightSticky);
			mTextView->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
			mTextView->SetEditable(false);
			mTextView->SetTabSelectText(false);
			mScrollerView->Show();		
		} 
		else
		{
			SRect stViewBounds;
			stViewBounds.SetEmpty();

			if (mNewWindow) 
				SetBounds(stViewBounds);
			else
				GetBounds(stViewBounds);
		
			// make container view
			mContainerView = new CContainerView(this, SRect(0, 0, stViewBounds.GetWidth(), stViewBounds.GetHeight()));
			mContainerView->Show();

			// make image scroller
			mScrollerView = new CScrollerView(mContainerView, SRect(-1, -1, stViewBounds.GetWidth() + 1, stViewBounds.GetHeight() + 1), scrollerOption_HorizBar | scrollerOption_VertBar | scrollerOption_PlainBorder | scrollerOption_GrowSpace);
			mScrollerView->SetSizing(sizing_BottomRightSticky);
			mScrollerView->Show();

			if (inTypeCode == TB('JPEG'))
				mDecompressImage = new CDecompressJpeg(mBufferSize);
			else if (inTypeCode == TB('GIFf'))
				mDecompressImage = new CDecompressGif(mBufferSize);
			else if (inTypeCode == TB('PICT'))
				mDecompressImage = new CDecompressPict(mBufferSize);
			else if (inTypeCode == TB('BMP '))
				mDecompressImage = new CDecompressBitmap(mBufferSize);
				
			if (inTypeCode != TB('GIFf'))
				mImageView = new CImageView(mScrollerView, SRect(0,0,0,0), mDecompressImage);
			else
				mImageView = new CAnimatedGifView(mScrollerView, SRect(0,0,0,0), (CDecompressGif*)mDecompressImage);
			
			mImageView->Show();
		}		
	}
	else
	{
	#if WIN32
		SetLimits(50, 16);
	#else
		SetLimits(50, 10);
	#endif
	
		GetBounds(mWinBounds);

		Uint8 psTempFileName[32];
		mTempFile.GetTempName(psTempFileName, sizeof(psTempFileName));

		const Int8 *pMimeType = UMime::ConvertTypeCode_Mime(inTypeCode);
		
		// if we have file size send it otherwise just ignore this parameter
		Uint32 nAddrSize;
		Int8 csHotlineAddr[512];
		if (inFileSize)
			nAddrSize = UText::Format(csHotlineAddr, sizeof(csHotlineAddr) - 1, "hotline://%#s/%#s&port=%lu&filesize=%lu&refnum=%lu&tempfile=%#s&mimetype=%s&filetype=%lu", gApp->mAddress.name, inFileName, gApp->mBasePortNum + 1, inFileSize, inRefNum, psTempFileName, pMimeType, inTypeCode);
		else
			nAddrSize = UText::Format(csHotlineAddr, sizeof(csHotlineAddr) - 1, "hotline://%#s/%#s&port=%lu&refnum=%lu&tempfile=%#s&mimetype=%s&filetype=%lu", gApp->mAddress.name, inFileName, gApp->mBasePortNum + 1, inRefNum, psTempFileName, pMimeType, inTypeCode);
		
		csHotlineAddr[nAddrSize] = 0;	// append null char
		
		mQuickTimeView = new CMyQuickTimeView(this, SRect(0, 0, mWinBounds.GetWidth(), mWinBounds.GetHeight()), qtOption_ResizeWindow, qtOption_ShowController | qtOption_ShowSaveAs | qtOption_ResolveGrowBox);
		if (!mQuickTimeView->StreamMovie(csHotlineAddr))
			return false;
		
		mQuickTimeView->SetSaveFileWin(this);
		if (!mQuickTimeView->IsVideoTrack())
		{
			SRect stQuickTimeBounds;
			mQuickTimeView->GetBounds(stQuickTimeBounds);
			
			SetLimits(50, 10, max_Int16, stQuickTimeBounds.GetHeight());
		}
		
		mQuickTimeView->Show();		
	
		if (mNewWindow)
		{
			SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
			SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
			
		#if MACINTOSH_CARBON || WIN32
			if (dynamic_cast<CWindow *>(gApp->mToolbarWin) && dynamic_cast<CWindow *>(gApp->mToolbarWin)->IsCollapsed())
				gApp->mAuxChildrenList.AddItem(this);	
			else
		#endif
				Show();
		}
		else
		{			
			SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
			BringToFront();
		}
	}
	
	return true;
}

bool CMyViewFileWin::StopStreamMovie()
{
	if (mQuickTimeView)
	{
		mQuickTimeView->StopStreamMovie();
		return true;
	}

	return false;
}

void CMyViewFileWin::ProgressMovieProc(Str255 inFileName, Uint32 inTotalSize, Uint32 inDownloadedSize)
{
	Uint32 i = 0;
	CMyViewFileWin *pViewFileWin;
	
	while (gApp->mViewWinList.GetNext(pViewFileWin, i))
	{
		if (pViewFileWin->mTempFile.CompareTempFileName(inFileName))
		{
			if (pViewFileWin->mViewFileTask)
				pViewFileWin->mViewFileTask->SetProgress(inTotalSize, inDownloadedSize);
			
			break;
		}
	}	
}

void CMyViewFileWin::InsertText(const void *inTextBuffer, Uint32 inBufferSize)
{
	if (!mBuffer)
		return;

	Uint32 nOldBufferOffset = mBufferOffset;
	mBufferOffset += UMemory::Copy((Uint8*)mBuffer + mBufferOffset, inTextBuffer, inBufferSize > mBufferSize - mBufferOffset ? mBufferSize - mBufferOffset : inBufferSize);

	if (mTextView)
	{
		// maximum size for preview text is 70K
		if (mBufferOffset <= 71680)
			mTextView->InsertText(mTextView->GetTextSize(), inTextBuffer, mBufferOffset - nOldBufferOffset); 
		else if (nOldBufferOffset <= 71680)
		{	
			mTextView->InsertText(mTextView->GetTextSize(), inTextBuffer, 71680 - nOldBufferOffset);

			mTextView->InsertText(mTextView->GetTextSize(), "\r\r------------------------------------------------", 50);
			mTextView->InsertText(mTextView->GetTextSize(), "\rThis is just a preview. For the entire text, save or download the file.", 72);
		}
	}

	if (!nOldBufferOffset)
	{
		if (mNewWindow)
		{
			SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
			SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
			
		#if MACINTOSH_CARBON || WIN32
			if (dynamic_cast<CWindow *>(gApp->mToolbarWin) && dynamic_cast<CWindow *>(gApp->mToolbarWin)->IsCollapsed())
				gApp->mAuxChildrenList.AddItem(this);	
			else
		#endif
				Show();
		}
		else
		{			
			SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
			BringToFront();
		}
	}
	
	if (mBufferOffset >= mBufferSize)
	{
		// write to a temp file
		mTempFile.WriteFileData(mBuffer, mBufferSize);
		
		UMemory::Dispose((TPtr)mBuffer);
		mBuffer = nil;
	}
}

void CMyViewFileWin::InsertImage(const void *inImageBuffer, Uint32 inBufferSize)
{
	if (!mBuffer || !mImageView || !mDecompressImage)
		return;
	
	Uint32 nOldBufferOffset = mBufferOffset;
	mBufferOffset += UMemory::Copy((Uint8 *)mBuffer + mBufferOffset, inImageBuffer, inBufferSize > mBufferSize - mBufferOffset ? mBufferSize - mBufferOffset : inBufferSize);

	try
	{
		if (mImageView && mDecompressImage)
		{	
			bool bReadNewLines = false;
			do
			{
				bReadNewLines = mDecompressImage->Decompress(mBuffer, mBufferOffset);
				if (bReadNewLines) mImageView->SetImage();

			} while(mTempFile.GetTypeCode() == TB('GIFf') && bReadNewLines);
				
			if (mBufferOffset >= mBufferSize || mDecompressImage->IsComplete())
			{
				mImageView->FinishImage();
				
				UMemory::Dispose((TPtr)mBuffer);
				mBuffer = nil;
				
				delete mDecompressImage;
				mDecompressImage = nil;
			}
		}
	}
	catch(...)
	{
		UMemory::Dispose((TPtr)mBuffer);
		mBuffer = nil;
		
		delete mDecompressImage;	
		mDecompressImage = nil;
		
		throw;
	}
	
	if (mImageView)
	{
		SRect stViewBounds; 
		mImageView->GetBounds(stViewBounds);
		
		if (!stViewBounds.IsEmpty() && (!IsVisible() || mWinBounds.GetWidth() != stViewBounds.GetWidth() || mWinBounds.GetHeight() != stViewBounds.GetHeight()))
		{
			SetWindowSize(stViewBounds);
			mWinBounds.right = mWinBounds.left + stViewBounds.GetWidth();
			mWinBounds.bottom = mWinBounds.top + stViewBounds.GetHeight();
		}
		else if (!nOldBufferOffset && !mNewWindow)
		{
			SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);  
			BringToFront();
		}
	}
	
	// write to a temp file
	mTempFile.WriteFileData(inImageBuffer, inBufferSize);
}

void CMyViewFileWin::SetTextColor(const SMyColorInfo& colorInfo) 
{
	if (mTextView)
	{
		mScrollerView->SetContentColor(colorInfo.backColor);

		mTextView->SetColor(colorInfo.textColor);
		mTextView->SetFontSize(colorInfo.textSize);
	}
}

Uint32 CMyViewFileWin::GetTextSize()
{
	if (mTextView)
		return mTextView->GetTextSize();
	
	return 0;
}

void CMyViewFileWin::EndViewFileTask()
{
	mViewFileTask = nil;
}

bool CMyViewFileWin::IsEndViewFileTask()
{
	if (mViewFileTask)
		return false;

	return true;
}

bool CMyViewFileWin::CanCloseViewFileWin()
{
	if (mQuickTimeView)
	{
		Uint8 psTempFileName[32];
		mTempFile.GetTempName(psTempFileName, sizeof(psTempFileName));
		
		if (HL_HandlerIsReading(psTempFileName))
			return false;
	}
		
	return true;
}

void CMyViewFileWin::SetMustCloseFlag()
{
	if (mQuickTimeView)
	{
		Uint8 psTempFileName[32];
		mTempFile.GetTempName(psTempFileName, sizeof(psTempFileName));
	
		HL_HandlerCancelReading(psTempFileName);
	}

	mMustCloseFlag = true;
	gApp->mCloseWinTimer->Start(500, kOnceTimer);
}

bool CMyViewFileWin::GetMustCloseFlag()
{
	return mMustCloseFlag;
}

void CMyViewFileWin::SetWindowSize(const SRect& inBounds)
{
	SRect stWinBounds;
	stWinBounds.SetEmpty();

	if (!mNewWindow)
		GetBounds(stWinBounds);
	else
		stWinBounds = mWinBounds;
					
	Uint8 nScrollWidth = 15;	
	stWinBounds.right = stWinBounds.left + inBounds.GetWidth() + nScrollWidth;
	stWinBounds.bottom = stWinBounds.top + inBounds.GetHeight() + nScrollWidth;
				
#if WIN32	
	if (stWinBounds.GetWidth() < MIN_WINDOW_WITDH)
		stWinBounds.right = stWinBounds.left + MIN_WINDOW_WITDH;
#endif
	
#if MACINTOSH
	SetLimits(40, 40, stWinBounds.GetWidth(), stWinBounds.GetHeight());
#else
	SetLimits(10, 10, stWinBounds.GetWidth(), stWinBounds.GetHeight());
#endif

	SetBounds(stWinBounds);

	if (mNewWindow)
	{
		mNewWindow = false;
		if (!mWinBounds.left && !mWinBounds.top)
			SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);		
				
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);		
	
	#if MACINTOSH_CARBON || WIN32
		if (dynamic_cast<CWindow *>(gApp->mToolbarWin) && dynamic_cast<CWindow *>(gApp->mToolbarWin)->IsCollapsed())
			gApp->mAuxChildrenList.AddItem(this);	
		else
	#endif
			Show();
	}
	else
	{
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);  
		BringToFront();
	}
}

void CMyViewFileWin::ClearViews()
{
	if (mTextView) 
	{	delete mTextView; mTextView = nil;}
	
	if (mImageView)
	{ 	delete mImageView; mImageView = nil;}

	if (mQuickTimeView)
	{	delete mQuickTimeView; mQuickTimeView = nil;}
	
	if (mScrollerView)
	{ 	delete mScrollerView; mScrollerView = nil;}
	
	if (mContainerView)
	{	delete mContainerView; mContainerView = nil;}
}

void CMyViewFileWin::ClearData()
{
	ClearPathData();

	if (mDecompressImage)
	{	delete mDecompressImage; mDecompressImage = nil;}
	
	if (mBuffer)
	{	UMemory::Dispose((TPtr)mBuffer); mBuffer = nil;}
	
	mBufferSize = 0;
	mBufferOffset = 0;	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyServerInfoWin::CMyServerInfoWin(CWindow *inParent)
	: CMySaveFileWin(inParent, SRect(0,0,10,10))
{
#if WIN32
	_SetWinIcon(*this, 215);
#endif

	gApp->mServerInfoWinList.AddItem(this);

	mImageView = nil;
	mQuickTimeView = nil;
}

CMyServerInfoWin::~CMyServerInfoWin()
{
	// a CQuickTimeView must be deleted in the parent window destructor
	if (mQuickTimeView)
		delete mQuickTimeView;

	gApp->mServerInfoWinList.RemoveItem(this);
}

void CMyServerInfoWin::KeyUp(const SKeyMsgData& inInfo)
{
	if (mImageView)
		mImageView->KeyUp(inInfo);

	CMySaveFileWin::KeyUp(inInfo);
}

Uint32 _FSTypeCodeToCreatorCode(Uint32 inTypeCode);

bool CMyServerInfoWin::SetServerInfoWin(const Uint8 *inServerName, THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	if (mTempFile.IsValidFile() || !inServerName && !inServerName[0] || !inData)
		return false;
		
	// setup window
	SetTitle(inServerName);
	
	CImageView *pImageView = nil;
	CMyQuickTimeView *pQuickTimeView = nil;
	
	// make Image or QuickTime view
	if (inTypeCode == TB('JPEG') || inTypeCode == TB('GIFf') || inTypeCode == TB('PICT') || inTypeCode == TB('BMP '))
		pImageView = MakeImageView(inData, inTypeCode, inURL, inComment);
	else
		pQuickTimeView = MakeQuickTimeView(inData, inTypeCode, inURL, inComment);
		
	if (!pImageView && !pQuickTimeView)
		return false;		
	
	mImageView = pImageView;
	mQuickTimeView = pQuickTimeView;
	
	// resize the window
	if (mImageView)
	{
		SRect stViewBounds;
		mImageView->GetBounds(stViewBounds);
		SetWindowSize(stViewBounds);
		
		mImageView->Show();
	}
	else if (mQuickTimeView)
		mQuickTimeView->Show();

	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
		
	// get the file extension
	const Int8 *pMimeType = UMime::ConvertTypeCode_Mime(inTypeCode);
	const Int8 *pExtension = UMime::ConvertMime_Extension(pMimeType);

	// set the file name
	Uint8 psFileName[256];
	UMemory::Copy(psFileName, inServerName, inServerName[0] + 1);
	psFileName[0] += UMemory::Copy(psFileName + psFileName[0] + 1, pExtension, strlen(pExtension));
	
	// set temp file
	SetTempFile(psFileName, inTypeCode, _FSTypeCodeToCreatorCode(inTypeCode));
	mTempFile.SetFileInfo(inServerName);
	SetTempData(inData);
	
	return true;
}

bool CMyServerInfoWin::IsThisServer(const Uint8 *inServerName, THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	if (!mTempFile.IsValidFile() || !inServerName && !inServerName[0] || !inData)
		return false;
	
	// compare server name
	Uint8 psServerName[32];
	psServerName[0] = mTempFile.GetFileInfo(psServerName + 1, sizeof(psServerName) - 1);
	if (!psServerName[0])
		return false;
		
	if (UText::CompareInsensitive(psServerName + 1, psServerName[0], inServerName + 1, inServerName[0]))
		return false;
	
	// compare checksum
	void *pBannerBuffer;
	Uint32 nBufferSize = inData->GetSize();
	StHandleLocker stLocker(inData, pBannerBuffer);
	if (mTempFile.GetChecksum() != UMemory::Checksum(pBannerBuffer, nBufferSize))
		return false;
	
	// compare type code
	if (mTempFile.GetTypeCode() != inTypeCode)
		return false;

	// check URL and comment
	Uint8 psURL[256];
	Uint8 psComment[256];
	
	if (mImageView)
	{
		psURL[0] = mImageView->GetURL(psURL + 1, sizeof(psURL) - 1);
		psComment[0] = mImageView->GetComment(psComment + 1, sizeof(psComment) - 1);
		
		if ((inURL && UMemory::Compare(psURL + 1, psURL[0], inURL + 1, inURL[0])) || (inComment && UMemory::Compare(psComment + 1, psComment[0], inComment + 1, inComment[0])))
			mImageView->SetURL(inURL, inComment);
	}
	else if (mQuickTimeView)
	{
		psURL[0] = mQuickTimeView->GetURL(psURL + 1, sizeof(psURL) - 1);
		psComment[0] = mQuickTimeView->GetComment(psComment + 1, sizeof(psComment) - 1);

		if ((inURL && UMemory::Compare(psURL + 1, psURL[0], inURL + 1, inURL[0])) || (inComment && UMemory::Compare(psComment + 1, psComment[0], inComment + 1, inComment[0])))
			mQuickTimeView->SetURL(inURL, inComment);	
	}
	
	return true;
}

CImageView *CMyServerInfoWin::MakeImageView(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	// make container view
	CContainerView *vc = new CContainerView(this, SRect(0,0,0,0));
	vc->Show();

	// make image scroller
	CScrollerView	*scr = new CScrollerView(vc, SRect(-1,-1,1,1), scrollerOption_HorizBar | scrollerOption_VertBar | scrollerOption_PlainBorder | scrollerOption_GrowSpace);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->Show();

	// make image view
	void *pBannerBuffer;
	Uint32 nBufferSize = inData->GetSize();
	StHandleLocker stLocker(inData, pBannerBuffer);

	CDecompressImage *pDecompressImage = nil;;
	if (inTypeCode == TB('JPEG'))
		pDecompressImage = new CDecompressJpeg(nBufferSize, true);
	else if (inTypeCode == TB('GIFf'))
		pDecompressImage = new CDecompressGif(nBufferSize, true);
	else if (inTypeCode == TB('PICT'))
		pDecompressImage = new CDecompressPict(nBufferSize, true);
	else if (inTypeCode == TB('BMP '))
		pDecompressImage = new CDecompressBitmap(nBufferSize, true);
	
	if (!pDecompressImage)
		return nil;
			
	CImageView *pImageView;
	if (inTypeCode != TB('GIFf'))
		pImageView = new CMyImageView(scr, SRect(0,0,0,0), pDecompressImage);
	else
		pImageView = new CMyAnimatedGifView(scr, SRect(0,0,0,0), (CDecompressGif*)pDecompressImage);
	
	bool bIsCompleted = false;
	try
	{
		bool bReadNewLines = false;
		do
		{
			bReadNewLines = pDecompressImage->Decompress(pBannerBuffer, nBufferSize);
			if (bReadNewLines) pImageView->SetImage();

		} while(inTypeCode == TB('GIFf') && bReadNewLines);
		
		bIsCompleted = pDecompressImage->IsComplete();
		if (bIsCompleted)
		{
			pImageView->FinishImage();
			pImageView->SetURL(inURL, inComment);
		}
	}
	catch(...)
	{
		// nothing to do here
	}
	
	delete pDecompressImage;
	
	if (!bIsCompleted)
	{
		delete pImageView;
		return nil;
	}
	
	return pImageView;
}

CMyQuickTimeView *CMyServerInfoWin::MakeQuickTimeView(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	void *pBannerBuffer;
	Uint32 nBufferSize = inData->GetSize();
	StHandleLocker stLocker(inData, pBannerBuffer);

	CMyQuickTimeView *pQuickTimeView = new CMyQuickTimeView(this, SRect(0,0,0,0), qtOption_ResizeWindow, qtOption_ShowController | qtOption_ShowSaveAs | qtOption_ResolveGrowBox);
	if (!pQuickTimeView->SetMovie(pBannerBuffer, nBufferSize, inTypeCode))
	{
		delete pQuickTimeView;
		return nil;
	}
	
	pQuickTimeView->SetSaveFileWin(this);
	pQuickTimeView->SetURL(inURL, inComment);

	if (!pQuickTimeView->IsVideoTrack())
	{
		SRect stQuickTimeBounds;
		pQuickTimeView->GetBounds(stQuickTimeBounds);
			
		SetLimits(50, 10, max_Int16, stQuickTimeBounds.GetHeight());
	}
	else
	#if WIN32
		SetLimits(50, 16);
	#else
		SetLimits(50, 10);
	#endif
	
	return pQuickTimeView;
}

void CMyServerInfoWin::SetWindowSize(const SRect& inBounds)
{
	SRect stWinBounds;
	GetBounds(stWinBounds);
					
	Uint8 nScrollWidth = 15;	
	stWinBounds.right = stWinBounds.left + inBounds.GetWidth() + nScrollWidth;
	stWinBounds.bottom = stWinBounds.top + inBounds.GetHeight() + nScrollWidth;
				
#if WIN32	
	if (stWinBounds.GetWidth() < MIN_WINDOW_WITDH)
		stWinBounds.right = stWinBounds.left + MIN_WINDOW_WITDH;
#endif
	
#if MACINTOSH
	SetLimits(40, 40, stWinBounds.GetWidth(), stWinBounds.GetHeight());
#else
	SetLimits(10, 10, stWinBounds.GetWidth(), stWinBounds.GetHeight());
#endif

	SetBounds(stWinBounds);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyPublicChatWin::CMyPublicChatWin(CWindow *inParent)
	: CWindow(SRect(0,0,360,193), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 415);
#endif

	CScrollerView *scr;

	// setup window
	SetTitle("\pChat");
	SetLimits(300, 130);

	// make container view for content
	CContainerView* vc = new CContainerView(this, SRect(0, 0, 360, 193));
	vc->Show();
	
	// make pane view
	CPaneView *pv = new CPaneView(vc, SRect(0, 0, 360, 193));
	pv->SetSizing(sizing_BottomRightSticky);
	pv->Show();
	vc->SetFocusView(pv);
	
	// make display text box
	scr = MakeTextBoxView(pv, SRect(5,5,355,142), scrollerOption_Border + scrollerOption_VertBar, &mViews.allText);
	scr->SetSizing(sizing_BottomRightSticky);
	mViews.allText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.allText->SetEditable(false);
	mViews.allText->SetTabSelectText(false);
	mViews.allScroll = scr;
	scr->Show();
	pv->SetViewLimits(scr, 250, 70);

	CContainerView* uvc = new CContainerView(pv, SRect(5,145,355,188));
	uvc->SetSizing(sizing_TopSticky + sizing_RightSticky + sizing_BottomSticky);
	uvc->Show();
	pv->SetViewLimits(uvc, 250, 40);
	pv->SetFocusView(uvc);

	// make edit text box
	scr = MakeTextBoxView(uvc, SRect(0,0,324,43), scrollerOption_Border, &mViews.sendText);
	scr->SetSizing(sizing_BottomRightSticky);
	mViews.sendText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.sendText->SetID(viewID_ChatSend);
	mViews.sendText->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.sendScroll = scr;
	scr->Show();
	//uvc->SetViewLimits(scr, 250, 30);
	uvc->SetFocusView(scr);
	
	//help
	#if USE_HELP
	CIconButtonView *icb = new CIconButtonView(uvc, SRect(326,0,350,24));
	icb->SetSizing(sizing_HorizSticky);
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpChat);
	icb->SetTooltipMsg("\pHelp");
	icb->Show();
	#endif
	
}

void CMyPublicChatWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (inInfo.keyCode == key_Escape)
		ClearInputBox();
	else if (UText::tolower(inInfo.keyChar) == 's' && inInfo.mods == modKey_Shortcut)
		CMyApplication::SaveTextAs(mViews.allText);
	else
		CWindow::KeyDown(inInfo);
}

void CMyPublicChatWin::SetTextColor(const SMyColorInfo& colorInfo)
{
	mViews.allScroll->SetContentColor(colorInfo.backColor);
	mViews.sendScroll->SetContentColor(colorInfo.backColor);

	mViews.allText->SetColor(colorInfo.textColor);
	mViews.sendText->SetColor(colorInfo.textColor);
	mViews.allText->SetFontSize(colorInfo.textSize);
	mViews.sendText->SetFontSize(colorInfo.textSize);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyPrivateChatWin::CMyPrivateChatWin(CWindow *inParent)
	: CWindow(SRect(0,0,565,208), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox + windowOption_ZoomBox, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 415);
#endif

	CContainerView *uvc;
	CPaneView *pv, *txtpv;
	CScrollerView *scr;
	CIconButtonView *icb;
	
	// setup window
	SetTitle("\pPrivate Chat");
	SetLimits(300, 150);
	mChatID = 0;
	mCurrentDrag = nil;
	mIsListVis = true;
	
	// make container view for content
	CContainerView* vc = new CContainerView(this, SRect(0,0,565,208));
	vc->Show();
	
	pv = new CPaneView(vc, SRect(0,0,565,188));
	pv->SetSizing(sizing_BottomRightSticky);
	pv->Show();
	vc->SetFocusView(pv);
	
	// make pane view for text
	txtpv = new CPaneView(pv, SRect(0, 0, 355, 188));
	txtpv->SetSizing(sizing_BottomRightSticky);
	txtpv->Show();
	pv->SetFocusView(txtpv);
	pv->SetViewLimits(txtpv, 100, 0);
	
	// make display text box
	scr = MakeTextBoxView(txtpv, SRect(5,5,355,140), scrollerOption_Border + scrollerOption_VertBar, &mViews.allText);
	scr->SetSizing(sizing_BottomRightSticky);
	mViews.allText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.allText->SetEditable(false);
	mViews.allText->SetTabSelectText(false);
	mViews.allScroll = scr;
	scr->Show();
	txtpv->SetViewLimits(scr, 0, 70);
	
	// make edit text box
	scr = MakeTextBoxView(txtpv, SRect(5,145,355,188), scrollerOption_Border, &mViews.sendText);
	scr->SetSizing(sizing_TopSticky + sizing_RightSticky + sizing_BottomSticky);
	mViews.sendText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.sendText->SetID(viewID_PrivChatSend);
	mViews.sendText->SetEnterKeyAction(enterKeyAction_Hit, enterKeyAction_Hit);
	mViews.sendScroll = scr;
	scr->Show();
	txtpv->SetFocusView(scr);
	txtpv->SetViewLimits(scr, 0, 30);
	
	// make container for userlist and btns
	uvc = new CContainerView(pv, SRect(360, 0, 560, 188));
	uvc->SetSizing(sizing_HorizSticky | sizing_BottomSticky);
	uvc->Show();
	pv->SetViewLimits(uvc, 80, 0);
	
	// make buttons
	icb = new CIconButtonView(uvc, SRect(0,3,24,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3031);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(415);
		}
	
	icb->SetID(viewID_OpenPrivateChat);
	icb->SetTooltipMsg("\pPrivate Chat");
	icb->Show();
	mViews.chatBtn = icb;
	
	icb = new CIconButtonView(uvc, SRect(27,3,51,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3030);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(206);
		}
	
	icb->SetID(viewID_SendPrivMsg);
	icb->SetTooltipMsg("\pPrivate Message");
	icb->Show();
	mViews.msgBtn = icb;
	
	icb = new CIconButtonView(uvc, SRect(54,3,78,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3007);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(215);
		}
	
	
	icb->SetID(viewID_GetInfo);
	icb->SetTooltipMsg("\pGet Client Info");
	icb->Show();
	mViews.infoBtn = icb;

	//help
	#if USE_HELP
	icb = new CIconButtonView(uvc, SRect(81,3,105,27));
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpPrivateChat);
	icb->SetTooltipMsg("\pHelp");
	icb->Show();
	#endif

	// make user list scroller
	scr = new CScrollerView(uvc, SRect(0,28,200,188));
	scr->SetOptions(scrollerOption_VertBar + scrollerOption_Border + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
#if SPACE_DOT_COM
	scr->SetContentColor(color_Black);
#endif	
	scr->Show();
	mViews.listScroll = scr;
	
	// make user list
	CMyUserListView *list = new CMyUserListView(scr, SRect(0,0,scr->GetVisibleContentWidth(),scr->GetVisibleContentHeight()));
	list->SetBehaviour(itemBehav_SelectOnlyOne + itemBehav_DoubleClickAction + itemBehav_DragAction);
	list->SetCanFocus(true);
	list->SetSizing(sizing_RightSticky + sizing_FullHeight);
	list->SetID(viewID_ChatUserList);
	list->SetDragAndDropHandler(this);		// geez I love multiple inheritance
	list->Show();
	mViews.list = list;
	
	// make subject view
#if MACINTOSH
	CMySubjectView *sub = new CMySubjectView(vc, SRect(-1,193,551,209));
#else
	CMySubjectView *sub = new CMySubjectView(vc, SRect(-1,193,566,209));
#endif
	sub->SetSizing(sizing_TopSticky + sizing_RightSticky + sizing_BottomSticky);
	sub->SetID(viewID_ChatSubject);
	sub->SetTooltipMsg("\pSet Subject");
	sub->Show();
	mViews.subject = sub;
	
	// add this window to the list of chat windows
	gList.AddFirst(this);
}

CLinkedList CMyPrivateChatWin::gList;

CMyPrivateChatWin::~CMyPrivateChatWin()
{
	// remove this window from the list of chat windows
	gList.RemoveLink(this);
}

void CMyPrivateChatWin::DoPrivateChat()
{
	Uint32 nUserID = GetUserListView()->GetSelectedUserID();
	if (nUserID == 0) 
		return;

	gApp->DoOpenPrivateChat(nUserID);
}

void CMyPrivateChatWin::DoPrivateMsg()
{
	Uint8 psUserName[64];
	Uint16 nUserID = GetUserListView()->GetSelectedUserID(psUserName);
	if (nUserID == 0) 
		return;
	
	gApp->DoSendPrivMsg(nUserID, psUserName);
}

void CMyPrivateChatWin::DoGetInfo()
{
	Uint8 psUserName[64];
	Uint16 nUserID = GetUserListView()->GetSelectedUserID(psUserName);
	if (nUserID == 0) 
		return;
	
	gApp->DoGetClientInfo(nUserID, psUserName);
}

void CMyPrivateChatWin::KeyDown(const SKeyMsgData& inInfo)
{
	if (inInfo.keyCode == key_Escape)
		ClearInputBox();
	else if ((inInfo.mods & (modKey_Command|modKey_Control)) && inInfo.keyChar == '/')
		UserZoom(SMouseMsgData());
	else if (UText::tolower(inInfo.keyChar) == 's' && inInfo.mods == modKey_Shortcut)
		CMyApplication::SaveTextAs(mViews.allText);
	else
		CWindow::KeyDown(inInfo);
}

void CMyPrivateChatWin::UserZoom(const SMouseMsgData& inInfo)
{
	// we're now using a pane view so we don't need to use this func
	CWindow::UserZoom(inInfo);
	return;

	/*
	 * This sicko function makes it so you can click the zoom box to hide/show
	 * the user list portion of the window.
	 */

	Uint16 allSizing = mViews.allScroll->GetSizing();
	Uint16 sendSizing = mViews.sendScroll->GetSizing();
	Uint16 listSizing = mViews.listScroll->GetSizing();

	mViews.allScroll->SetSizing(sizing_Fixed);
	mViews.sendScroll->SetSizing(sizing_Fixed);
	mViews.listScroll->SetSizing(sizing_Fixed);
	
	SRect r;
	GetBounds(r);
	r.right += mIsListVis ? -205 : 205;
	SetBounds(r);
	
	mViews.allScroll->SetSizing(allSizing);
	mViews.sendScroll->SetSizing(sendSizing);
	mViews.listScroll->SetSizing(listSizing);
	
	mIsListVis = !mIsListVis;
	mViews.listScroll->SetVisible(mIsListVis);
}

void CMyPrivateChatWin::SetTextColor(const SMyColorInfo& colorInfo)
{
	mViews.allScroll->SetContentColor(colorInfo.backColor);
	mViews.sendScroll->SetContentColor(colorInfo.backColor);

	mViews.allText->SetColor(colorInfo.textColor);
	mViews.sendText->SetColor(colorInfo.textColor);
	mViews.allText->SetFontSize(colorInfo.textSize);
	mViews.sendText->SetFontSize(colorInfo.textSize);
}

void CMyPrivateChatWin::SetAccess()
{
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_CreateChat))
		mViews.chatBtn->Enable();
	else
		mViews.chatBtn->Disable();
	
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_SendMessage))
		mViews.msgBtn->Enable();
	else
		mViews.msgBtn->Disable();

	if (gApp->HasGeneralPriv(myAcc_GetClientInfo)) 
		mViews.infoBtn->Enable();
	else
		mViews.infoBtn->Disable();		
}

void CMyPrivateChatWin::SetSubject(const Uint8 *inText)
{
	if (inText[0] == 0)
		SetTitle("\pPrivate Chat");
	else
	{
		enum { maxSize = 64 };
		Uint8 str[256];
		
		pstrcpy(str, "\pPrivate Chat:  ");
		pstrcat(str, inText);
		
		if (str[0] > maxSize)
		{
			str[0] = maxSize+3;
			str[maxSize+1] = '.';
			str[maxSize+2] = '.';
			str[maxSize+3] = '.';
		}
		
		SetTitle(str);
	}
	
	mViews.subject->SetText(inText);
}

void CMyPrivateChatWin::HandleDrag(CDragAndDroppable */* inDD */, const SMouseMsgData& inInfo)
{
	if (!UDragAndDrop::IsAvailable()) 
		return;

	StDrag drag;
	TRegion rgn, imgRgn;
	TImage img;
	SRect r;
	SPoint imgPt, mouseLoc;
	Int32 hd, vd;
	Uint8 name[256];
	
	Uint32 id = mViews.list->GetSelectedUserID(name);
	if (id == 0) 
		return;
		
	// get global mouse loc
	mViews.list->GetScreenDelta(hd, vd);
	mouseLoc.h = inInfo.loc.h + hd;
	mouseLoc.v = inInfo.loc.v + vd;

	drag->AddFlavor(1, 'HLUI', &id, sizeof(id), flavorFlag_SenderOnly);
	drag->AddTextFlavor(1, name+1, name[0]);
	
	rgn = imgRgn = nil;
	img = nil;
	
	try
	{
		rgn = URegion::New();
		mViews.list->GetSelectedItemsRegion(rgn);
		
		rgn->GetBounds(r);
		
	#if MACINTOSH
		if ((((Uint32)r.GetWidth() * (Uint32)r.GetHeight()) <= 65536) && UDragAndDrop::HasTranslucentDrag())
		{
			imgRgn = URegion::New(rgn);
			imgRgn->GetBounds(r);
			imgRgn->Move(-r.left, -r.top);
			
			img = mViews.list->GetSelectedItemsImage(imgPt);
			drag->SetImage(img, imgRgn, imgPt, dragImageStyle_Dark);
		}
	#endif

		rgn->Move(hd, vd);
		rgn->SetOutline(2, 2);
		mCurrentDrag = drag;
		drag->Track(mouseLoc, rgn);
		mCurrentDrag = nil;
	}
	catch(...)
	{
		mCurrentDrag = nil;
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

void CMyPrivateChatWin::HandleSetDragAction(CDragAndDroppable *inDD, const SDragMsgData& inInfo)
{
	#pragma unused(inDD)

	if (!mCurrentDrag || !inInfo.drag->Equals(mCurrentDrag))
	{
		if (inInfo.drag->HasFlavor('HLUI'))
		{
			inInfo.drag->SetDragAction(dragAction_Copy);
			return;
		}
	}

	inInfo.drag->SetDragAction(dragAction_None);
}


void CMyPrivateChatWin::DragEnter(const SDragMsgData& inInfo)
{
	CWindow::DragEnter(inInfo);
	
	if (!inInfo.drag->GetDragAction())
	{
		if (inInfo.drag->HasFlavor('HLUI'))
			inInfo.drag->SetDragAction(dragAction_Copy);
	}
}

void CMyPrivateChatWin::DragMove(const SDragMsgData& inInfo)
{
	CWindow::DragMove(inInfo);
	
	if (!inInfo.drag->GetDragAction())
	{
		if (inInfo.drag->HasFlavor('HLUI'))
			inInfo.drag->SetDragAction(dragAction_Copy);
	}
}


bool CMyPrivateChatWin::HandleDrop(CDragAndDroppable */* inDD */, const SDragMsgData& inInfo)
{
	TDrag drag = inInfo.drag;
	
	Uint32 data[2];
	
	if (drag->GetFlavorData(1, 'HLUI', data, sizeof(Uint32)) == sizeof(Uint32))
	{
		data[1] = mChatID;
		
		mViews.list->Hit(hitType_Drop, 1, 0, data, sizeof(data));
		
		drag->AcceptDrop();
		
		return true;
	}
	
	return false;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySecretWin::CMySecretWin()
	: CWindow(SRect(0,0,180,150), windowLayer_Modal)
{
	CContainerView *vc;
	CButtonView *btn;
	CScrollerView *scr;

	// setup window
	SetTitle("\pSecret");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,180,116));
	vc->Show();

	// make text boxes
	scr = MakeTextBoxView(vc, SRect(10,10,170,36), scrollerOption_Border, &mViews.codeText);
	mViews.codeText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	vc->SetFocusView(scr);
	scr = MakeTextBoxView(vc, SRect(10,43,170,69), scrollerOption_Border, &mViews.valText);
	mViews.valText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	
	scr = MakeTextBoxView(vc, SRect(10,76,170,99), scrollerOption_Border, &mViews.extraText);
	mViews.extraText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();

	// make buttons
	btn = new CButtonView(vc, SRect(55,110,125,136));
	//btn = new CButtonView(vc, SRect(55,80,125,106));
	btn->SetTitle("\pExecute");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);
}

void CMySecretWin::GetInfo(Uint8 *outCode, Uint8 *outVal, Uint8 *outExtra )
{
	outCode[0] = mViews.codeText->GetText(outCode+1, 31);
	outVal[0] = mViews.valText->GetText(outVal+1, 31);
	outExtra[0] = mViews.extraText->GetText(outExtra+1, 31);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyBannerWin::CMyBannerWin(const SRect& inBounds, Uint16 inLayer, Uint16 inOptions, Uint16 inStyle, CWindow *inParent)
	: CWindow(inBounds, inLayer, inOptions, inStyle, inParent)
{
	mContainerView = nil;
	mBannerContainerView = nil;
	mBannerScrollerView = nil;
	
	mImageView = nil;
	mQuickTimeView = nil;
}

CMyBannerWin::~CMyBannerWin()
{
	// a CQuickTimeView must be deleted in the parent window destructor
	if (mQuickTimeView)
		delete mQuickTimeView;
}

bool CMyBannerWin::SetBanner(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment, Uint16 inDepl)
{

//#if !NOBANNER
if (!gApp->mNoBanner){


	if (!mContainerView)
		return false;

	// make scroller view
	
	
		CScrollerView *pBannerScrollerView = new CScrollerView(mContainerView, SRect(inDepl, inDepl, inDepl + eMaxHorizBannerImg + 4, inDepl + 154));

	
	pBannerScrollerView->SetOptions(scrollerOption_Border);

	// make container view
	Uint16 nScrollerWidth = pBannerScrollerView->GetVisibleContentWidth();
	Uint16 nScrollerHeight = pBannerScrollerView->GetVisibleContentHeight();
	CContainerView *pBannerContainerView = new CContainerView(pBannerScrollerView, SRect(0, 0, nScrollerWidth, nScrollerHeight));

	CImageView *pImageView = nil;
	CQuickTimeView *pQuickTimeView = nil;
	
	// make image or QuickTime view
	if (inTypeCode == TB('JPEG') || inTypeCode == TB('GIFf') || inTypeCode == TB('PICT') || inTypeCode == TB('BMP '))
		pImageView = MakeImageView(pBannerContainerView, inData, inTypeCode, inURL, inComment);
	else
		pQuickTimeView = MakeQuickTimeView(pBannerContainerView, inData, inTypeCode, inURL, inComment);
		
	if (!pImageView && !pQuickTimeView)
	{
		delete pBannerContainerView;
		delete pBannerScrollerView;
		return false;		
	}
	
	ClearViews();
	mImageView = pImageView;
	mQuickTimeView = pQuickTimeView;
	mBannerContainerView = pBannerContainerView;
	mBannerScrollerView = pBannerScrollerView;
	
	SRect stBannerBounds;
	if (mImageView)
		mImageView->GetBounds(stBannerBounds);
	else if (mQuickTimeView)
		mQuickTimeView->GetBounds(stBannerBounds);
		
	if (stBannerBounds.GetWidth() < eMinHorizBannerImg)
		stBannerBounds.right = stBannerBounds.left + eMinHorizBannerImg;

	if (stBannerBounds.bottom < eMinVertBannerImg)
		stBannerBounds.bottom = stBannerBounds.top + eMinVertBannerImg;

	// resize container view
	mBannerContainerView->SetBounds(SRect(0, 0, stBannerBounds.GetWidth(), stBannerBounds.GetHeight()));

	// check scroller view
	const Uint16 nBarThick = 16;

	Uint16 nHorizBarTick = 0;
	if (nScrollerWidth < stBannerBounds.GetWidth())
	{
		mBannerScrollerView->SetOptions(mBannerScrollerView->GetOptions() + scrollerOption_HorizBar);
		nScrollerHeight = pBannerScrollerView->GetVisibleContentHeight();
		nHorizBarTick = nBarThick;
	}

	Uint16 nVertBarTick = 0;
	if (nScrollerHeight < stBannerBounds.GetHeight())
	{
		mBannerScrollerView->SetOptions(mBannerScrollerView->GetOptions() + scrollerOption_VertBar);
		nScrollerWidth = pBannerScrollerView->GetVisibleContentWidth();
		nVertBarTick = nBarThick;
	
		if (!nHorizBarTick && nScrollerWidth < stBannerBounds.GetWidth())
		{
			mBannerScrollerView->SetOptions(mBannerScrollerView->GetOptions() + scrollerOption_HorizBar);
			nScrollerHeight = pBannerScrollerView->GetVisibleContentHeight();
			nHorizBarTick = nBarThick;
		}
	}

	// resize window
	SRect stWinBounds;
	GetBounds(stWinBounds);
	
#if WIN32
	Uint16 nExtraSpace = 2*inDepl + 2;
#else
	Uint16 nExtraSpace = 2*inDepl + 4;
#endif
	Uint16 nExtraWidth = stBannerBounds.GetWidth() + nVertBarTick + nExtraSpace;

	Uint16 nWinWidth = nExtraWidth;
	if (nWinWidth > eMaxHorizBannerImg + nExtraSpace)
		nWinWidth = eMaxHorizBannerImg + nExtraSpace;
	else if (nWinWidth < eMinHorizBannerWin)
		nWinWidth = eMinHorizBannerWin;
		
	SetBounds(SRect(stWinBounds.left, stWinBounds.top, stWinBounds.left + nWinWidth, stWinBounds.bottom));

	// resize scroller view
	if (nScrollerWidth >= stBannerBounds.GetWidth())
	{		
		SRect stScrollerBounds;
		mBannerScrollerView->GetBounds(stScrollerBounds);
		
		Uint16 nExtraBarWidth;
		if (nVertBarTick)
			nExtraBarWidth = 3;
		else
			nExtraBarWidth = 4;
		
		if (nExtraWidth >= eMinHorizBannerWin)
			mBannerScrollerView->SetBounds(SRect(inDepl, stScrollerBounds.top, inDepl + stBannerBounds.GetWidth() + nVertBarTick + nExtraBarWidth, stScrollerBounds.bottom));
		else
			mBannerScrollerView->SetBounds(SRect(nWinWidth/2 - stBannerBounds.GetWidth()/2 - stBannerBounds.GetWidth()%2 - nVertBarTick/2 - nExtraBarWidth/2 - nExtraBarWidth%2, stScrollerBounds.top, nWinWidth/2 + stBannerBounds.GetWidth()/2 + nVertBarTick/2 + nExtraBarWidth/2, stScrollerBounds.bottom));
	}

	if (nScrollerHeight >= stBannerBounds.GetHeight())
	{
		SRect stScrollerBounds;
		mBannerScrollerView->GetBounds(stScrollerBounds);

		Uint16 nExtraBarHeight;
		if (nHorizBarTick)
			nExtraBarHeight = 3;
		else
			nExtraBarHeight = 4;
		
		mBannerScrollerView->SetBounds(SRect(stScrollerBounds.left, stScrollerBounds.top, stScrollerBounds.right, stScrollerBounds.top + stBannerBounds.GetHeight() + nHorizBarTick + nExtraBarHeight));
	}

	// show all views
	mBannerScrollerView->Show();
	mBannerContainerView->Show();

	if (mImageView)
		mImageView->Show();
	else if (mQuickTimeView)
	{
		mQuickTimeView->UpdateQuickTimeBounds();
		mQuickTimeView->Show();
		mQuickTimeView->StartMovie();
	}
	
	return true;
//#endif
}
return true;
}

CImageView *CMyBannerWin::MakeImageView(CContainerView *inContainerView, THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	void *pBannerBuffer;
	Uint32 nBufferSize = inData->GetSize();
	StHandleLocker stLocker(inData, pBannerBuffer);

	CDecompressImage *pDecompressImage = nil;;
	if (inTypeCode == TB('JPEG'))
		pDecompressImage = new CDecompressJpeg(nBufferSize, true);
	else if (inTypeCode == TB('GIFf'))
		pDecompressImage = new CDecompressGif(nBufferSize, true);
	else if (inTypeCode == TB('PICT'))
		pDecompressImage = new CDecompressPict(nBufferSize, true);
	else if (inTypeCode == TB('BMP '))
		pDecompressImage = new CDecompressBitmap(nBufferSize, true);
	
	if (!pDecompressImage)
		return nil;
			
	CImageView *pImageView;
	if (inTypeCode != TB('GIFf'))
		pImageView = new CMyImageView(inContainerView, SRect(0,0,0,0), pDecompressImage);
	else
		pImageView = new CMyAnimatedGifView(inContainerView, SRect(0,0,0,0), (CDecompressGif*)pDecompressImage);
	
	bool bIsCompleted = false;
	try
	{
		bool bReadNewLines = false;
		do
		{
			bReadNewLines = pDecompressImage->Decompress(pBannerBuffer, nBufferSize);
			if (bReadNewLines) pImageView->SetImage();

		} while(inTypeCode == TB('GIFf') && bReadNewLines);
		
		bIsCompleted = pDecompressImage->IsComplete();
		if (bIsCompleted)
		{
			pImageView->FinishImage();
			pImageView->SetURL(inURL, inComment);	
		}
	}
	catch(...)
	{
		// nothing to do here
	}
	
	delete pDecompressImage;
	
	if (!bIsCompleted)
	{
		delete pImageView;
		return nil;
	}
	
	return pImageView;
}

CQuickTimeView *CMyBannerWin::MakeQuickTimeView(CContainerView *inContainerView, THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	void *pBannerBuffer;
	Uint32 nBufferSize = inData->GetSize();
	StHandleLocker stLocker(inData, pBannerBuffer);

	CQuickTimeView *pQuickTimeView = new CMyQuickTimeView(inContainerView, SRect(0,0,0,0), qtOption_ResizeView, qtOption_LoopMovie);
	pQuickTimeView->SetMaxMovieSize(eMaxHorizBannerImg, eMaxVertBannerImg);
	if (!pQuickTimeView->SetMovie(pBannerBuffer, nBufferSize, inTypeCode))
	{
		delete pQuickTimeView;
		return nil;
	}
	
	pQuickTimeView->SetURL(inURL, inComment);
	
	return pQuickTimeView;
}

void CMyBannerWin::ClearViews()
{
	if (mImageView)
	{	delete mImageView; mImageView = nil;						}
	
	if (mQuickTimeView)
	{	delete mQuickTimeView; mQuickTimeView = nil;				}

	if (mBannerContainerView)
	{	delete mBannerContainerView; mBannerContainerView = nil;	}

	if (mBannerScrollerView)
	{	delete mBannerScrollerView; mBannerScrollerView = nil;		}	
}


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyAgreementWin::CMyAgreementWin()
	: CMyBannerWin(SRect(0,0,eMinHorizBannerWin,10), windowLayer_Modal)
{
	// setup window
	SetTitle("\pAgreement");

	// make container view for content
	mContainerView = new CContainerView(this, SRect(0,0,eMinHorizBannerWin,10));
	mContainerView->Show();
	
	mTextScrollerView = nil;
	
	mAgreeBtn = nil;
	mDisagreeBtn = nil;
}

bool CMyAgreementWin::SetBannerHdl(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{

	if (!SetBanner(inData, inTypeCode, inURL, inComment, 10))
		return false;
	
	SRect stScrollerBounds;
	if (mBannerScrollerView)
		mBannerScrollerView->GetBounds(stScrollerBounds);
	else
		stScrollerBounds = kZeroRect;
	
	SRect stWinBounds;
	GetBounds(stWinBounds);
	SetBounds(SRect(stWinBounds.left, stWinBounds.top, stWinBounds.right, stWinBounds.top + stScrollerBounds.bottom + 50));
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	GetBounds(stWinBounds);
	Uint16 nMiddle = stWinBounds.GetWidth()/2;

	mAgreeBtn = new CButtonView(mContainerView, SRect(nMiddle-87, stWinBounds.GetHeight()-36, nMiddle-15, stWinBounds.GetHeight()-12));
	mAgreeBtn->SetTitle("\pAgree");
	mAgreeBtn->SetID(1);
	mAgreeBtn->Show();
	
	mDisagreeBtn = new CButtonView(mContainerView, SRect(nMiddle+15, stWinBounds.GetHeight()-36, nMiddle+87, stWinBounds.GetHeight()-12));
	mDisagreeBtn->SetTitle("\pDisagree");
	mDisagreeBtn->SetID(2);
	mDisagreeBtn->Show();
	
	return true;
	
}

void CMyAgreementWin::SetTextHdl(THdl inData)
{
	SRect stWinBounds;
	GetBounds(stWinBounds);

	// make icon
	CIconView *icn = new CIconView(mContainerView, SRect(10,8,42,40));
	icn->SetIconID(icon_Caution);
	icn->Show();
	
	// make label
	CLabelView *lbl = new CLabelView(mContainerView, SRect(50, 8, stWinBounds.GetWidth() - 10, 60));
	lbl->SetText("\pPlease read this agreement carefully.  By clicking on the ÒAgreeÓ button, you are agreeing to be bound by the terms of this agreement.");
	lbl->Show();

	Uint16 nVertDepl = 62;
	if (mBannerScrollerView)
	{
		SRect stScrollerBounds;
		mBannerScrollerView->GetBounds(stScrollerBounds);
		mBannerScrollerView->SetBounds(SRect(stScrollerBounds.left, nVertDepl, stScrollerBounds.right, nVertDepl + stScrollerBounds.GetHeight()));
		nVertDepl += stScrollerBounds.GetHeight() + 10;
		
		if (mAgreeBtn)
		{
			delete mAgreeBtn;
			mAgreeBtn = nil;
		}
	}
	
	mTextScrollerView = new CScrollerView(mContainerView, SRect(10, nVertDepl, stWinBounds.GetWidth() - 10, nVertDepl + 10));
	mTextScrollerView->SetOptions(scrollerOption_VertBar + scrollerOption_Border + scrollerOption_NoFocusBorder + scrollerOption_NoBkgnd);
	mTextScrollerView->SetCanFocus(true);
	
	Uint16 nContainerWidth = mTextScrollerView->GetVisibleContentWidth();
	CContainerView *textContainerView = new CContainerView(mTextScrollerView, SRect(0, 0, nContainerWidth, 10));
	textContainerView->SetCanFocus(true);
	textContainerView->Show();

	CTextView *textView;
	CScrollerView *scrollerView = MakeTextBoxView(textContainerView, SRect(0,0,10,10), scrollerOption_PlainBorder, &textView);
	textView->SetFont(fd_Fixed9);
	textView->SetEditable(false);
	textView->SetTabSelectText(false);
	scrollerView->SetCanFocus(true);
	scrollerView->Show();
	
	textView->SetTextHandle(inData);
	textView->SetFullHeight();

	Uint32 nTextWidth ,nTextHeight;
	scrollerView->SetBounds(SRect(-2, 0, nContainerWidth+2, 10));
	scrollerView->ScrollToBottom();
	scrollerView->GetScroll(nTextWidth, nTextHeight);
	scrollerView->ScrollToTop();
	nTextHeight += 10;

	scrollerView->SetBounds(SRect(-2, -2, nContainerWidth+2, nTextHeight));
	textContainerView->SetBounds(SRect(0, 0, nContainerWidth, nTextHeight+50));
	
	Uint16 nMiddle = stWinBounds.GetWidth()/2;
	
	mAgreeBtn = new CButtonView(textContainerView, SRect(nMiddle-48, nTextHeight+14, nMiddle+24, nTextHeight+38));
	mAgreeBtn->SetTitle("\pAgree");
	mAgreeBtn->SetID(1);
	mAgreeBtn->Show();

	Uint16 nAgreementWinHeight = eMaxVertBannerWin;
	if (nAgreementWinHeight > nVertDepl + nTextHeight + 104)
		nAgreementWinHeight = nVertDepl + nTextHeight + 104;
	
	SetBounds(SRect(stWinBounds.left, stWinBounds.top, stWinBounds.right, stWinBounds.top + nAgreementWinHeight));
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	GetBounds(stWinBounds);

	mTextScrollerView->SetBounds(SRect(11, nVertDepl, stWinBounds.GetWidth() - 10, stWinBounds.GetHeight() - 50));

	if (mDisagreeBtn)
		mDisagreeBtn->SetBounds(SRect(nMiddle-36, stWinBounds.GetHeight()-36, nMiddle+36, stWinBounds.GetHeight()-12));
	else
	{
		mDisagreeBtn = new CButtonView(mContainerView, SRect(nMiddle-36, stWinBounds.GetHeight()-36, nMiddle+36, stWinBounds.GetHeight()-12));
		mDisagreeBtn->SetTitle("\pDisagree");
		mDisagreeBtn->SetID(2);
		mDisagreeBtn->Show();
	}
	
	mTextScrollerView->Show();
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyToolbar::CMyToolbar()
{
	mToolbarViews.optionsBtn = nil;
	mToolbarViews.serversBtn = nil;
	mToolbarViews.usersBtn = nil;
	mToolbarViews.chatBtn = nil;
	mToolbarViews.newsBtn = nil;
	mToolbarViews.msgBoardBtn = nil;
	mToolbarViews.filesBtn = nil;
	mToolbarViews.tasksBtn = nil;
	mToolbarViews.broadcastBtn = nil;
	mToolbarViews.newUserBtn = nil;
	mToolbarViews.openUserBtn = nil;
	mToolbarViews.openUserListBtn = nil;
	mToolbarViews.openAdmInSpectorBtn = nil;
	mToolbarViews.disconnectBtn = nil;
	mToolbarViews.helpBtn = nil;
	mToolbarViews.quitBtn = nil;
	
	mToolbarViews.connectInfo = nil;
}

void CMyToolbar::SetAccess()
{
	if (gApp->HasGeneralPriv(myAcc_ReadChat) || gApp->HasGeneralPriv(myAcc_SendChat))
		mToolbarViews.chatBtn->Enable();
	else
		mToolbarViews.chatBtn->Disable();

	if (gApp->HasGeneralPriv(myAcc_Broadcast))
		mToolbarViews.broadcastBtn->Show();
	else
		mToolbarViews.broadcastBtn->Hide();
		
	if (gApp->HasGeneralPriv(myAcc_CreateUser))
		mToolbarViews.newUserBtn->Show();
	else
		mToolbarViews.newUserBtn->Hide();

	if (gApp->HasGeneralPriv(myAcc_OpenUser))
		mToolbarViews.openUserBtn->Show();
	else
		mToolbarViews.openUserBtn->Hide();
		
	if (gApp->HasGeneralPriv(myAcc_AdmInSpector)){
	
		if (gApp->mServerVers >= 196)
			mToolbarViews.openAdmInSpectorBtn->Show();
			//mToolbarViews.openAdmInSpectorBtn->Disable();
			
	}else{
		mToolbarViews.openAdmInSpectorBtn->Hide();
		}

	if (gApp->HasGeneralPriv(myAcc_OpenUser) || gApp->HasGeneralPriv(myAcc_CreateUser))
	{
		// only 1.8.4 server or better supports advanced admin features
		if (gApp->mServerVers >= 184)
			mToolbarViews.openUserListBtn->Show();
	}
	else
		mToolbarViews.openUserListBtn->Hide();
}


void CMyToolbar::UpdateInfoBar()
{
	// update the status in toolbar
	Uint8 str[256];
	
	Uint8 *p = str + 1;
	Uint8 *q = str + sizeof(str);
	
	// show whether connected or not
	if (gApp->mIsConnected)
	{
		Uint32 userCount = gApp->mUsersWin->GetUserListView()->GetItemCount();			
		p+= UText::Format(p, q - p, 
					"Connected to: %#s   %lu %s Online    ", 
					gApp->mServerName, 
					userCount, ((userCount==1) ? "User":"Users")
					);
//#if DEBUG
		p+= UText::Format(p, q - p, 
					" Vers %d ", 
					gApp->GetServerVersion()
					);
//#endif
	}
	else
		p+= UMemory::Copy(p, "Not Connected     ", 17);
	
	p+= UText::Format(p, q - p, "Tasks: %lu Active, %lu Queued    ", gApp->mActiveTaskCount, gApp->mQueuedTaskCount);
	str[0] = p - str - 1;
	SetStatusText(str);

	// show the first task's desc
	CMyTask *task = gApp->mTaskList.GetItem(1);
	
	if (task)
	{
		str[0]= task->GetShortDesc(str + 1, sizeof(str) - 1);
		SetStatusText(str, true);
	}
	else
		SetStatusText("\p", true);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

#if WIN32
void _ActivateNextApp();
#endif


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyBannerToolbarWin::CMyBannerToolbarWin()
: CMyToolbar()
, CMyBannerWin(SRect(0, 0, eMinHorizBannerWin, 46)
			  , windowLayer_Float, windowOption_CloseBox, 1)
{
#if WIN32
	_SetWinIcon(*this, 210);
#endif

	// setup window
	SetTitle("\pGLoarbline Connect");
	
	// make container view for content
	mContainerView = new CContainerView(this, SRect(0, 0, eMinHorizBannerWin, 46));
	mContainerView->Show();
	
	// make buttons
	const Uint8 *btnNames[] = 
	{
		"\pOptions", "\pServers", "\pUsers", "\pPublic Chat", "\pNews", "\pMsg Board", 
		"\pFiles", "\pTasks", "\pBroadcast", "\pNew Account", "\pOpen Account", 
		"\pAdminister Accounts", "\pAdmInSpector"
	};
/*
	const Int16 btnIcons[] = 
	{
	
	if (gApp->mIconSet == 2){
		419, 3020, 3022, 3025, 
		3023, 3024, 3004, 3021, 3015, 
		3008, 3017, 3018, 2018
	}else if (gApp->mIconSet == 1){
		419, 233, 237, 238, 
		236, iconID_ShowMsgBoard, 235, 416, 239, 
		234, 418, 200, 2018
	}
	};
	
	*/
//	if (gApp->mIconSet == 2){
		const Int16 btnIcons1[] = 
		{
			3034, 3020, 3022, 3025, 
			3023, 3024, 3004, 3021, 3015, 
			3008, 3017, 3018, 3066
		};
//	}else if (gApp->mIconSet == 1){
		const Int16 btnIcons2[] = 
		{
			419, 233, 237, 238, 
			236, iconID_ShowMsgBoard, 235, 416, 239, 
			234, 418, 200, 2018
		};
	
//	}

	const Uint32 btnIDs[] = 
	{
		viewID_Options, viewID_ShowServers, viewID_ShowUsers, viewID_ShowChat, 
		viewID_ShowNews, viewID_ShowMsgBoard, viewID_ShowFiles, viewID_ShowTasks, viewID_Broadcast, 
		viewID_NewUser, viewID_OpenUser, viewID_OpenUserList, viewID_AdmInSpector
	};

	enum { btnCount = sizeof(btnIcons1) / sizeof(Int16) };
	
	CIconButtonView *btns[btnCount];
	
	SRect btnRect;
	btnRect.left = eButtonDepl + 1;
	btnRect.top = eButtonSpace;
	btnRect.bottom = btnRect.top + eButtonHeight;

	CIconButtonView *btn;
	for (Uint16 i=0; i<btnCount; i++)
	{
		btnRect.right = btnRect.left + eButtonWidth;
		
		btn = new CIconButtonView(mContainerView, btnRect);
	#if WIN32
		btn->SetCanFocus(true);
	#endif
	
	if (gApp->mIconSet == 2){
		btn->SetIconID(btnIcons1[i]);
	}else if (gApp->mIconSet == 1){
		btn->SetIconID(btnIcons2[i]);
		}
		
		btn->SetTooltipMsg(btnNames[i]);
		btn->SetID(btnIDs[i]);
		btn->Show();
		
		btns[i] = btn;
		btnRect.left = btnRect.right + eButtonSpace;
	}
	
	mToolbarViews.optionsBtn = btns[0];
	mToolbarViews.serversBtn = btns[1];
	mToolbarViews.usersBtn = btns[2];
	mToolbarViews.chatBtn = btns[3];
	mToolbarViews.newsBtn = btns[4];
	mToolbarViews.msgBoardBtn = btns[5];
	mToolbarViews.filesBtn = btns[6];
	mToolbarViews.tasksBtn = btns[7];
	mToolbarViews.broadcastBtn = btns[8];
	mToolbarViews.newUserBtn = btns[9];
	mToolbarViews.openUserBtn = btns[10];
	mToolbarViews.openUserListBtn = btns[11];
	mToolbarViews.openAdmInSpectorBtn = btns[12];

	mToolbarViews.newsBtn->Disable();
	mToolbarViews.msgBoardBtn->Disable();
	mToolbarViews.filesBtn->Disable();
	mToolbarViews.broadcastBtn->Hide();
	mToolbarViews.newUserBtn->Hide();
	mToolbarViews.openUserBtn->Hide();
	mToolbarViews.openUserListBtn->Hide();
	mToolbarViews.openAdmInSpectorBtn->Hide();
	
	SRect rToolbarWinBounds;
	GetBounds(rToolbarWinBounds);
	btnRect.right = rToolbarWinBounds.GetWidth() - eButtonDepl;
	btnRect.left = btnRect.right - eButtonWidth;

	// make quit button
	btn = new CIconButtonView(mContainerView, btnRect);
	#if WIN32
		btn->SetCanFocus(true);
	#endif
	
	
	if (gApp->mIconSet == 2){
		btn->SetIconID(3006);
	}else if (gApp->mIconSet == 1){
		btn->SetIconID(213);
		}
		
	
	
	btn->SetTooltipMsg("\pQuit");
	btn->SetID(viewID_Quit);
	
	btn->Show();
	mToolbarViews.quitBtn = btn;

	btnRect.right = btnRect.left - eButtonSpace;
	btnRect.left = btnRect.right - eButtonWidth;
	
	// make help button
	#if USE_HELP
	btn = new CIconButtonView(mContainerView, btnRect);
	#if WIN32
		btn->SetCanFocus(true);
	#endif
	btn->SetIconID(iconID_HelpToolbar);
	btn->SetTooltipMsg("\pHelp");
	btn->SetID(viewID_HelpToolbar);
	btn->Show();
	mToolbarViews.helpBtn = btn;
	
	btnRect.right = btnRect.left - eButtonSpace;
	btnRect.left = btnRect.right - eButtonWidth;
	#endif

	// make button
	btn = new CIconButtonView(mContainerView, btnRect);
	#if WIN32
		btn->SetCanFocus(true);
	#endif
	
	if (gApp->mIconSet == 2){
		btn->SetIconID(3007);
	}else if (gApp->mIconSet == 1){
		btn->SetIconID(215);
		}
		
	btn->SetTooltipMsg("\pAbout");
	btn->SetID(viewID_ShowAbout);
	btn->Show();
	mViews.aboutBtn = btn;
	
	btnRect.right = btnRect.left - eButtonSpace;
	btnRect.left = btnRect.right - eButtonWidth;
	

	btnRect.right = btnRect.left - eButtonSpace;
	btnRect.left = btnRect.right - eButtonWidth;

	// make disconnect button
	btn = new CIconButtonView(mContainerView, btnRect);
	#if WIN32
		btn->SetCanFocus(true);
	#endif
	
	if (gApp->mIconSet == 2){
		btn->SetIconID(3005);
	}else if (gApp->mIconSet == 1){
		btn->SetIconID(412);
		}
	
	btn->SetTooltipMsg("\pDisconnect");
	btn->SetID(viewID_Disconnect);
	mToolbarViews.disconnectBtn = btn;

#if EXTRA_BANNER_BUTTONS
	btnRect.right = rToolbarWinBounds.GetWidth() - 1; //eButtonDepl +1;
	btnRect.left = btnRect.right - eAdButtonWidth;

	{ // bottom
		btnRect.bottom = btnRect.top;// - eButtonSpace;
		btnRect.top = btnRect.bottom - eAdButtonHeight;
		mViews.adBtns[0] = btn = new CIconButtonView(mContainerView, btnRect);
		btn->SetIconID(iconID_Securiphone);
		btn->SetTooltipMsg("\pSecuriphone");
		btn->SetID(viewID_Securiphone);
		//btn->Show();
	}
	{  // middle
		btnRect.bottom = btnRect.top;// - eButtonSpace;
		btnRect.top = btnRect.bottom - eAdButtonHeight;
		mViews.adBtns[1] = btn = new CIconButtonView(mContainerView, btnRect);
		btn->SetIconID(iconID_ISP);
		btn->SetTooltipMsg("\pISP");
		btn->SetID(viewID_ISP);
		//btn->Show();
	}
	{  // top
		btnRect.bottom = btnRect.top;// - eButtonSpace;
		btnRect.top = btnRect.bottom - eAdButtonHeight;
		mViews.adBtns[2] = btn = new CIconButtonView(mContainerView, btnRect);
		btn->SetIconID(iconID_Xsprings);    //iconID_UnusedAD
		btn->SetTooltipMsg("\pxSprings");
		btn->SetID(viewID_Xsprings);
		//btn->Show();
		//btn->Disable();
	}
#endif


	// make info view
	mToolbarViews.connectInfo = new CMyConnectedInfoView(mContainerView, SRect(0,31,eMinHorizBannerWin,46), "\pNot Connected", fd_Default9);
	mToolbarViews.connectInfo->SetID(viewID_ToolbarStatus);
	mToolbarViews.connectInfo->Show();	

}

CMyBannerToolbarWin::~CMyBannerToolbarWin()
{
}

void CMyBannerToolbarWin::SetBounds(const SRect& inBounds)
{
	CMyBannerWin::SetBounds(inBounds);
}

bool CMyBannerToolbarWin::SetBannerHdl(THdl inData, Uint32 inTypeCode, const Uint8 *inURL, const Uint8 *inComment)
{
	SRect stScrollerBounds;
	if (inData)
	{
		if (!SetBanner(inData, inTypeCode, inURL, inComment))
			return false;
	
		if (mBannerScrollerView)
			mBannerScrollerView->GetBounds(stScrollerBounds);
		else
			stScrollerBounds = kZeroRect;
	}
	else
		stScrollerBounds = kZeroRect;
	
#if MACINTOSH_CLASSIC
	if (IsCollapsed())
		Expand();
#endif

	CalculateNewBounds(&stScrollerBounds);
	return true;
}

void CMyBannerToolbarWin::SetViewsPosition()
{
	SRect stWinBounds;
	GetBounds(stWinBounds);

	SRect stOldBounds;
	if (mBannerScrollerView)
		mBannerScrollerView->GetBounds(stOldBounds);
	else
		stOldBounds = kZeroRect;

	SRect stNewButtonsBounds;
	SRect stNewBounds = stOldBounds;
	if (gApp->mOptions.bToolbarButtonsTop)
	{
		stNewButtonsBounds.top = eButtonSpace + 1;
		stNewBounds.top = stNewButtonsBounds.top + eButtonHeight + eButtonSpace;
	}
	else
	{
		stNewBounds.top = 0;
		stNewButtonsBounds.top = stOldBounds.GetHeight() + eButtonSpace + 1;
	}
	
	stNewBounds.bottom = stNewBounds.top + stOldBounds.GetHeight();
	if (stOldBounds != stNewBounds)
	{
		if (mBannerScrollerView)
		{
			mBannerScrollerView->SetBounds(stNewBounds);	
	
			if (mQuickTimeView)
				mQuickTimeView->UpdateQuickTimeBounds();
		}

	}

	stNewButtonsBounds.left = eButtonDepl + 1;
	stNewButtonsBounds.right = stNewButtonsBounds.left + eButtonWidth;
	stNewButtonsBounds.bottom = stNewButtonsBounds.top + eButtonHeight;

	SetIconButton(mToolbarViews.optionsBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.serversBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.usersBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.chatBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.newsBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.msgBoardBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.filesBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.tasksBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.broadcastBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.newUserBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.openUserBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.openUserListBtn, stNewButtonsBounds);
	SetIconButton(mToolbarViews.openAdmInSpectorBtn, stNewButtonsBounds);

	stNewButtonsBounds.right = stWinBounds.GetWidth() - eButtonDepl;
	stNewButtonsBounds.left = stNewButtonsBounds.right - eButtonWidth;
	mToolbarViews.quitBtn->SetBounds(stNewButtonsBounds);

	#if USE_HELP
	stNewButtonsBounds.right = stNewButtonsBounds.left - eButtonSpace;
	stNewButtonsBounds.left = stNewButtonsBounds.right - eButtonWidth;
	mToolbarViews.helpBtn->SetBounds(stNewButtonsBounds);	
	#endif

	stNewButtonsBounds.right = stNewButtonsBounds.left - eButtonSpace;
	stNewButtonsBounds.left = stNewButtonsBounds.right - eButtonWidth;
	mViews.aboutBtn->SetBounds(stNewButtonsBounds);	


	stNewButtonsBounds.right = stNewButtonsBounds.left - eButtonSpace;
	stNewButtonsBounds.left = stNewButtonsBounds.right - eButtonWidth;
	mToolbarViews.disconnectBtn->SetBounds(stNewButtonsBounds);
/*
#if EXTRA_BANNER_BUTTONS
	stNewButtonsBounds.right = stWinBounds.GetWidth()-1; // - eButtonDepl;
	stNewButtonsBounds.left = stNewButtonsBounds.right - eAdButtonWidth;
	stNewButtonsBounds.top  = stNewBounds.bottom-2;
	for(int k = 0; k<3; ++k)
	{
		stNewButtonsBounds.bottom = stNewButtonsBounds.top;
		stNewButtonsBounds.top = stNewButtonsBounds.bottom - eAdButtonHeight+1;
		mViews.adBtns[k]->SetBounds(stNewButtonsBounds);
	}
#endif
*/

	// info view
	Uint32 nHeight = stWinBounds.GetHeight();
#if WIN32
	SRect stBottomBar(0, nHeight - 20 + 1, stWinBounds.GetWidth() + 2, nHeight - 3);
#else
	SRect stBottomBar(0, nHeight - 15 + 1, stWinBounds.GetWidth(), nHeight);
#endif
	mToolbarViews.connectInfo->SetBounds(stBottomBar);
	
	if (mBannerScrollerView)
		Refresh();
}


void CMyBannerToolbarWin::SetAccess()
{
	CMyToolbar::SetAccess();

}

// if an external program tries to hide or resize the toolbar we must restore it
void CMyBannerToolbarWin::CheckToolbarState()
{
	Uint16 nState = GetState();
	if (nState == windowState_Hidden)
		Show();
	
#if MACINTOSH_CLASSIC

	if (IsCollapsed())
		Expand();

	CalculateNewBounds();
	
#elif MACINTOSH_CARBON

	static bool bIsCollapsed = false;
	if (bIsCollapsed != IsCollapsed())
	{
		bIsCollapsed = !bIsCollapsed;
				
		if (bIsCollapsed)
			gApp->HideAllWindows();
		else
		{
			gApp->ShowAllWindows();
			
			CalculateNewBounds();
			BringToFront();
		}
	}
	else if (!bIsCollapsed)
		CalculateNewBounds();
	
#elif WIN32

	static bool bIsFocused = true;
	if (bIsFocused != (nState == windowState_Focus))
	{			
		bIsFocused = !bIsFocused;
		
		if (bIsFocused && gApp->mAuxParentWin)
			gApp->mAuxParentWin->InsertAfter(this, false);
	}

	static bool bIsCollapsed = false;
	if (bIsCollapsed != IsCollapsed())
	{
		bIsCollapsed = !bIsCollapsed;
				
		if (bIsCollapsed)
		{
			gApp->HideAllWindows();
			
			_ActivateNextApp();
		}
		else
		{
			gApp->ShowAllWindows();
			
			CalculateNewBounds();
			BringToFront();
		}
	}
	else if (!bIsCollapsed)
		CalculateNewBounds();

#endif

	if (!IsCollapsed())
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
}

void CMyBannerToolbarWin::StateChanged()
{	
	CMyBannerWin::StateChanged();

	CheckToolbarState();
}

void CMyBannerToolbarWin::CalculateNewBounds(const SRect *inScrollerBounds)
{
	SRect stScrollerBounds;
	if (inScrollerBounds)
		stScrollerBounds = *inScrollerBounds;
	else if (mBannerScrollerView)
		mBannerScrollerView->GetBounds(stScrollerBounds);
	else
		stScrollerBounds = kZeroRect;

	SRect stBounds;
	GetBounds(stBounds);
	SRect stNewBounds = stBounds;
	
#if WIN32
	Uint16 nExtraHeight = 20;
#else
	Uint16 nExtraHeight = 16;
#endif

	if (gApp->mOptions.bToolbarButtonsTop)
		stNewBounds.bottom = stNewBounds.top + stScrollerBounds.GetHeight() + eButtonHeight + 2*eButtonSpace + nExtraHeight;
	else
		stNewBounds.top = stNewBounds.bottom - stScrollerBounds.GetHeight() - eButtonHeight - 2*eButtonSpace - nExtraHeight;
	
#if WIN32
	Uint16 nExtraSpace = 2;
#else
	Uint16 nExtraSpace = 0;
#endif

	stNewBounds.right = stNewBounds.left + stScrollerBounds.GetWidth() - nExtraSpace;
	if (stNewBounds.GetWidth() < eMinHorizBannerWin)
		stNewBounds.right = stNewBounds.left + eMinHorizBannerWin;
#if EXTRA_BANNER_BUTTONS
	stNewBounds.right += /*eButtonSpace+*/eAdButtonWidth+3; //eButtonDepl;
#endif	
	if (stNewBounds != stBounds)
		SetBounds(stNewBounds);
	
	SetViewsPosition();
	SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
}

void CMyBannerToolbarWin::SetIconButton(CIconButtonView *inButton, SRect& inBounds)
{
	SRect stOldBounds;
	inButton->GetBounds(stOldBounds);
	
	if (stOldBounds != inBounds)
		inButton->SetBounds(inBounds);

	inBounds.left = inBounds.right + eButtonSpace;
	inBounds.right = inBounds.left + eButtonWidth;
}




/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyEditUserWin::CMyEditUserWin(CWindow *inParent, bool inIsNewUser)
	: CWindow(SRect(0,0,310,386), windowLayer_Standard, 0, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 414);
#endif

	struct SMyAccessItem {
		Uint16 type;		// 1=box, 2=check
		Uint16 accessID;	// myPriv_ offset
		Uint8 *name;
	};
	const SMyAccessItem accessItems[] = {
		{ 1, nil, "\pFile System Maintenance" },
		{ 2, myAcc_DownloadFile, "\pCan Download Files" },			// 0
		{ 2, myAcc_DontQueue, "\pCan Not Queue Downloads" },			// 0
		{ 2, myAcc_DownloadFolder, "\pCan Download Folders" },		// 1
		{ 2, myAcc_SpeakBefore, "\pCan Download Without Chatting" },  //41
		{ 2, myAcc_PostBefore, "\pCan Download Without Postting" },  //41
		
		{ 2, myAcc_UploadFile, "\pCan Upload Files" },				// 2
		{ 2, myAcc_UploadFolder, "\pCan Upload Folders" },			// 3
		{ 2, myAcc_UploadAnywhere, "\pCan Upload Anywhere" },		// 4
		{ 2, myAcc_DeleteFile, "\pCan Delete Files" },				// 5
		{ 2, myAcc_RenameFile, "\pCan Rename Files" },				// 6
		{ 2, myAcc_MoveFile, "\pCan Move Files" },					// 7
		{ 2, myAcc_SetFileComment, "\pCan Comment Files" },			// 8
		{ 2, myAcc_CreateFolder, "\pCan Create Folders" },			// 9
		{ 2, myAcc_DeleteFolder, "\pCan Delete Folders" },			// 10
		{ 2, myAcc_RenameFolder, "\pCan Rename Folders" },			// 11
		{ 2, myAcc_MoveFolder, "\pCan Move Folders" },				// 12
		{ 2, myAcc_SetFolderComment, "\pCan Comment Folders" },		// 13
		{ 2, myAcc_ViewDropBoxes, "\pCan View Drop Boxes" },
		{ 2, myAcc_ViewOwnDropBox, "\pCan View Own Drop Box" },		// 14
		{ 2, myAcc_MakeAlias, "\pCan Make Aliases" },				// 15

		{ 1, nil, "\pUser Maintenance" },
		{ 2, myAcc_CreateUser, "\pCan Create Accounts" },			// 16
		{ 2, myAcc_DeleteUser, "\pCan Delete Accounts" },			// 17
		{ 2, myAcc_OpenUser, "\pCan Read Accounts" },				// 18
		{ 2, myAcc_ModifyUser, "\pCan Modify Accounts" },			// 19
		{ 2, myAcc_AdmInSpector, "\pCan Use AdmInSpector" },
		{ 2, myAcc_GetClientInfo, "\pCan Get User Info" },			// 20
		{ 2, myAcc_DisconUser, "\pCan Disconnect Users" },			// 21
		{ 2, myAcc_CannotBeDiscon, "\pCannot be Disconnected" },	// 22
		{ 2, myAcc_Canviewinvisible, "\pCan View Invisible Users" },

		{ 1, nil, "\pMessaging" },
		{ 2, myAcc_SendMessage, "\pCan Send Messages" },			// 23
		{ 2, myAcc_Broadcast, "\pCan Broadcast" },					// 24
		{ 2, myAcc_RefuseChat, "\pCan Refuse Chats and Messages" }, // 42
		{ 2, myAcc_Canflood, "\pCan Flood" },
		
		{ 1, nil, "\pNews" },
		{ 2, myAcc_NewsReadArt, "\pCan Read Articles" },			// 25
		{ 2, myAcc_NewsPostArt, "\pCan Post Articles" },			// 26
		{ 2, myAcc_NewsDeleteArt, "\pCan Delete Articles" },		// 27
		{ 2, myAcc_NewsCreateCat, "\pCan Create Categories" },		// 28
		{ 2, myAcc_NewsDeleteCat, "\pCan Delete Categories" },		// 29
		{ 2, myAcc_NewsCreateFldr, "\pCan Create News Bundles" },	// 30
		{ 2, myAcc_NewsDeleteFldr, "\pCan Delete News Bundles" },	// 31
		
		{ 1, nil, "\pChat" },
		{ 2, myAcc_CreateChat, "\pCan Initiate Private Chat" },		// 32
		{ 2, myAcc_ReadChat, "\pCan Read Chat" },					// 33
		{ 2, myAcc_SendChat, "\pCan Send Chat" },					// 34

		{ 1, nil, "\pMiscellaneous" },
		{ 2, myAcc_AnyName, "\pCan Use Any Name" },					// 35
		{ 2, myAcc_NoAgreement, "\pDon't Show Agreement" },		// 36
		
		{ 1, nil, "\pRemote Action" },
		{ 2, myAcc_FakeRed, "\pCan Set User FakeRed" },				// 37
		{ 2, myAcc_Away, "\pCan Set User Away" },				// 38
		{ 2, myAcc_ChangeNick, "\pCan Change User Name" },				// 39
		{ 2, myAcc_ChangeIcon, "\pCan Change User Icon" },			//40
		{ 2, myAcc_BlockDownload, "\pCan Block an User Download" },		//43
		{ 2, myAcc_Visible, "\pCan Set User Invisible" }
		
		
				//TOTAL = 43
	/*
	#if BETAVERS		
		{ 1, nil, "\pLivestock" },
		{ 2, 63, "\pCan Slaughter Sheep" },
		{ 2, 63, "\pCan Tip Cows" },
		{ 2, 63, "\pCan Juggle Chickens" }
	#endif
	*/
	
		// remember to update size of mAccessChecks if we add checks to this
	};

	CContainerView *vc, *avc;
	CLabelView *lbl;
	CButtonView *btn;
	CScrollerView *scr;
	CBoxView *box;
	CMyAccessCheckBoxView *chk;
	Uint32 wid;

	// setup window
	SetTitle("\pEdit Account");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	mIsNewUser = (inIsNewUser != 0);
	
	// make container view for content
	vc = new CContainerView(this, SRect(0,0,310,386));
	vc->Show();
	mViews.containerView = vc;

	// make box
	box = new CBoxView(vc, SRect(10,10,300,341));
	box->SetStyle(boxStyle_Sunken);
	box->Show();

	// make labels
	lbl = new CLabelView(vc, SRect(20,25,95,41));
	lbl->SetText("\pName:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,55,95,71));
	lbl->SetText("\pLogin:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,85,95,101));
	lbl->SetText("\pPassword:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,120,95,136));
	lbl->SetText("\pAccess:");
	lbl->Show();
	
	// make text boxes
	scr = MakeTextBoxView(vc, SRect(110,20,290,46), scrollerOption_Border, &mViews.nameText);
	mViews.nameText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	vc->SetFocusView(scr);
	scr = MakeTextBoxView(vc, SRect(110,50,290,76), scrollerOption_Border, &mViews.loginText);
	mViews.loginText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	
	// make password box
	if (inIsNewUser)
	{
		scr = MakeTextBoxView(vc, SRect(110,80,290,106), scrollerOption_Border, &mViews.passText);
		mViews.passText->SetEnterKeyAction(enterKeyAction_None,enterKeyAction_None);
		mViews.bulletPassText = nil;
	}
	else
	{
		scr = MakePasswordBoxView(vc, SRect(110,80,290,106), scrollerOption_Border, &mViews.bulletPassText);
		mViews.passText = nil;
	}
	
	scr->Show();
	mViews.passwordScr = scr;

	// make access scroller
	scr = new CScrollerView(vc, SRect(20,140,290,330));
	scr->SetOptions(scrollerOption_VertBar + scrollerOption_Border + scrollerOption_NoBkgnd);
	scr->SetCanFocus(true);
	scr->Show();
	wid = scr->GetVisibleContentWidth();
	avc = new CContainerView(scr, SRect(0,0,wid,500));
	avc->Show();
	mViews.accessContainer = avc;

	// make buttons
	btn = new CButtonView(vc, SRect(224,350,300,376));
	btn->SetTitle("\pSave");
	btn->SetDefault(true);
	btn->SetID(viewID_SaveEditUser);
	btn->Show();
	vc->SetDefaultView(btn);
	btn = new CButtonView(vc, SRect(140,353,210,373));
	btn->SetTitle("\pCancel");
	btn->SetID(viewID_CancelEditUser);
	btn->Show();
	vc->SetCancelView(btn);
	btn = new CButtonView(vc, SRect(10,353,80,373));
	btn->SetTitle("\pDelete");
	btn->SetID(viewID_DeleteEditUser);
	btn->Show();
	mViews.deleteBtn = btn;
	
	// make access items
	{
		Uint32 n = sizeof(accessItems) / sizeof(SMyAccessItem);
		const SMyAccessItem *p = accessItems;
		Uint32 top = 6;
		Uint32 chkCount = 0;
		SRect r;
		
		while (n--)
		{
			switch (p->type)
			{
				case 1:
					if (top != 6) top += 16;
					
					r.top = top;
					r.left = 4;
					r.bottom = r.top + 18;
					r.right = wid - 4;
					box = new CBoxView(avc, r);
					box->SetStyle(boxStyle_Sunken);
					box->Show();

					r.top = top + 1;
					r.left = 8;
					r.bottom = r.top + 16;
					r.right = wid - 8;
					lbl = new CLabelView(avc, r);
					lbl->SetText(p->name);
					lbl->Show();
					
					top += 20;
					break;
					
				case 2:
					r.top = top;
					r.left = 16;
					r.bottom = r.top + 18;
					r.right = 210;
					chk = new CMyAccessCheckBoxView(avc, r);
					chk->SetTitle(p->name);
					chk->SetAccessID(p->accessID);
					chk->SetAutoMark(true);
					chk->SetStyle(kTickBox);

					// don't allow user to set greater access than self
					/*
					if (!gApp->HasGeneralPriv(p->accessID) && p->accessID != myAcc_NoAgreement)
						chk->Disable();
					*/
					chk->Show();
					
					top += 20;
					mAccessChecks[chkCount] = chk;
					chkCount++;
					break;
			}
			
			p++;
		}
		
	#if DEBUG
		if ((chkCount * sizeof(CCheckBoxView*)) > sizeof(mAccessChecks))
			DebugBreak("Need to increase size of mAccessChecks !!!");
	#endif

		r.top = r.left = 0;
		r.bottom = top + 6;
		r.right = wid;
		avc->SetBounds(r);
	}
}

void CMyEditUserWin::SetAccess(const SMyUserAccess& inAccess)
{
	Uint32 nSize = sizeof(mAccessChecks) / sizeof(CMyAccessCheckBoxView*);
	
	while (nSize--)
		mAccessChecks[nSize]->SetMark(inAccess.HasPriv(mAccessChecks[nSize]->GetAccessID()));
}

void CMyEditUserWin::GetAccess(SMyUserAccess& outAccess)
{
	SMyUserAccess acc = { 0, 0 };
	Uint32 nSize = sizeof(mAccessChecks) / sizeof(CMyAccessCheckBoxView*);
	
	while (nSize--)
		acc.SetPriv(mAccessChecks[nSize]->GetAccessID(), mAccessChecks[nSize]->GetMark());

	outAccess = acc;
}

void CMyEditUserWin::SetInfo(const Uint8 inName[], const Uint8 inLogin[], const Uint8 inPass[])
{
	if (inName) 
		mViews.nameText->SetText(inName+1, inName[0]);
	
	if (inLogin) 
		mViews.loginText->SetText(inLogin+1, inLogin[0]);

	if (inPass)
	{
		if (mViews.bulletPassText)
			mViews.bulletPassText->SetText(inPass+1, inPass[0]);
		else if (mViews.passText)
			mViews.passText->SetText(inPass+1, inPass[0]);
	}
}

bool CMyEditUserWin::GetInfo(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass)
{
	if (outName) 
		outName[0] = mViews.nameText->GetText(outName+1, 31);
	
	if (outLogin) 
		outLogin[0] = mViews.loginText->GetText(outLogin+1, 31);

	if (outPass)
	{
		outPass[0] = 0;
		
		// check password
		if ((mViews.bulletPassText && !mViews.bulletPassText->IsDummyPassword() && mViews.bulletPassText->GetPasswordSize() > 31) || 
		    (mViews.passText && mViews.passText->GetTextSize() > 31))
		{
			if (mViews.bulletPassText)
				mViews.bulletPassText->SetText(nil, 0);
			else if (mViews.passText)
				mViews.passText->SetText(nil, 0);

			mViews.containerView->SetFocusView(mViews.passwordScr);
			
			gApp->DisplayStandardMessage("\pPassword too long", "\pPlease select a password that is no longer than 31 characters.", icon_Stop, 1);
			return false;
		}

		if (mViews.bulletPassText)
			outPass[0] = mViews.bulletPassText->GetText(outPass+1, 31);
		else if (mViews.passText)
			outPass[0] = mViews.passText->GetText(outPass+1, 31);
	}
	
	return true;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyTasksWin::CMyTasksWin(CWindow *inParent)
	: CWindow(SRect(0,0,250,120), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox + windowOption_ZoomBox, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 416);
#endif

	CContainerView *vc;
	CScrollerView *scr;
	CBoxView *box;

	// setup window
	SetTitle("\pTasks");
	SetLimits(270, 100);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,250,120));
	vc->Show();
	
	// make background box
	box = new CBoxView(vc, SRect(0,0,250,30));
	box->SetStyle(boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();
	mViews.box = box;
	
	// make buttons
	mViews.stopTaskIcon = new CIconButtonView(vc, SRect(3,3,27,27));
	
	if (gApp->mIconSet == 2){
		mViews.stopTaskIcon->SetIconID(3005);
	}else if (gApp->mIconSet == 1){
		mViews.stopTaskIcon->SetIconID(412);
		}
		
	
	
	mViews.stopTaskIcon->SetTooltipMsg("\pKill Task");
	mViews.stopTaskIcon->Disable();
	mViews.stopTaskIcon->Show();
	mViews.stopTaskIcon->SetID(viewID_StopTask);
	
	mViews.startTaskIcon = new CIconButtonView(vc, SRect(30,3,54,27));
	
	if (gApp->mIconSet == 2){
		mViews.startTaskIcon->SetIconID(3042);
	}else if (gApp->mIconSet == 1){
		mViews.startTaskIcon->SetIconID(216);
		}
	
	mViews.startTaskIcon->SetID(viewID_StartTask);
	mViews.startTaskIcon->SetTooltipMsg("\pBegin Task");
	mViews.startTaskIcon->Disable();
	mViews.startTaskIcon->Show();

    // help
    #if USE_HELP
	CIconButtonView *icb = new CIconButtonView(vc, SRect(57,3,81,27));
	icb->SetIconID(iconID_HelpToolbar);
	icb->SetID(viewID_HelpTasks);
	icb->SetTooltipMsg("\pHelp");
	icb->Show();
	#endif

	// make list scroller
	scr = new CScrollerView(vc, SRect(-1,30,251,121));
	scr->SetOptions(scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_GrowSpace + scrollerOption_NoFocusBorder + scrollerOption_NoBkgnd);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	vc->SetFocusView(scr);

	// make action list
	CMyTaskListView *list = new CMyTaskListView(scr, SRect(0,0,scr->GetVisibleContentWidth(),scr->GetVisibleContentHeight()));
	list->SetBehaviour(itemBehav_SelectOnlyOne);
	list->SetCanFocus(true);
	list->SetSizing(sizing_RightSticky + sizing_FullHeight);
	list->Show();
	mViews.list = list;
}

void CMyTasksWin::UpdateButtons()
{
	CMyTask *task = mViews.list->GetSelectedTask();
	
	if (task)
	{
		if (dynamic_cast<CMyConnectTask*>(task) || dynamic_cast<CMyLoginTask*>(task) || dynamic_cast<CMyAgreedTask*>(task) ||
		   (dynamic_cast<CMyGetBannerTask*>(task) && dynamic_cast<CMyGetBannerTask*>(task)->IsLoginTask()) ||
		   (dynamic_cast<CMyDummyTask*>(task) && static_cast<CMyDummyTask*>(task)->IsLoginTask()) ||
		    dynamic_cast<CMyGetTrackServListTask*>(task))
		{
			mViews.stopTaskIcon->Enable();
			mViews.startTaskIcon->Disable();
			return;
		}	
		else if (dynamic_cast<CMyDownloadFileTask*>(task) ||  dynamic_cast<CMyViewFileTask*>(task) ||  dynamic_cast<CMyDownloadFldrTask*>(task) ||
				 dynamic_cast<CMyUploadFileTask*>(task) || dynamic_cast<CMyUploadFldrTask*>(task))
		{
			mViews.stopTaskIcon->Enable();
			mViews.startTaskIcon->SetEnable(task->IsQueued());
			return;
		}
	}

	mViews.startTaskIcon->Disable();
	mViews.stopTaskIcon->Disable();
}

void CMyTasksWin::UserZoom(const SMouseMsgData& /* inInfo */)
{
	SRect r;
	GetBounds(r);
	
	r.bottom = r.top + mViews.list->GetFullHeight() + 31;
	
	if (UWindow::GetZoomBounds(mWindow, r, r))
		SetBounds(r);
}

/*
void CMyTasksWin::DisableIcons()
{
	mViews.stopTaskIcon->Disable();
	mViews.startTaskIcon->Disable();
}
*/

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyOpenUserWin::CMyOpenUserWin()
	
	 : CWindow(SRect(0,0,280,146), windowLayer_Modal)
{
	CContainerView *vc;
	CButtonView *btn;
	CScrollerView *scr;
	CIconView *icn;
	CLabelView *lbl;
	CBoxView *box;

	// setup window
	SetTitle("\pOpen Account");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,280,146));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();
	
	// make label
	lbl = new CLabelView(vc, SRect(50,10,265,44));
	lbl->SetText("\pEnter the login name of the account:");
	lbl->Show();

	// make box
	box = new CBoxView(vc, SRect(10,50,270,100));
	box->SetStyle(boxStyle_Sunken);
	box->Show();

	// make text box
	scr = MakeTextBoxView(vc, SRect(65,62,215,88), scrollerOption_Border, &mViews.loginText);
	mViews.loginText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	vc->SetFocusView(scr);
	mViews.loginText->SetID(3);
	
	// make buttons
	btn = new CButtonView(vc, SRect(194,110,270,136));
	btn->SetTitle("\pOpen");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Disable();
	btn->Show();
	vc->SetDefaultView(btn);
	mViews.openBtn = btn;
	btn = new CButtonView(vc, SRect(104,113,174,133));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);
}

void CMyOpenUserWin::UpdateOpenButton()
{
	mViews.openBtn->SetEnable(!mViews.loginText->IsEmpty());
	mViews.loginText->SetHasChanged(false);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyNewFolderWin::CMyNewFolderWin(const Uint8 inMsg[])
	: CWindow(SRect(0,0,300,146), windowLayer_Modal)
{
	CContainerView *vc;
	CScrollerView *scr;
	CIconView *icn;
	CLabelView *lbl;
	CBoxView *box;

	// setup window
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
	lbl->SetText(inMsg);
	lbl->Show();

	// make box
	box = new CBoxView(vc, SRect(10,50,290,100));
	box->SetStyle(boxStyle_Sunken);
	box->Show();

	// make text box
	scr = MakeTextBoxView(vc, SRect(50,62,250,88), scrollerOption_Border, &mViews.nameText);
	mViews.nameText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	vc->SetFocusView(scr);
	mViews.nameText->SetID(3);
	
	// make buttons
	SButtons btns[] = {{1, "\pCreate", btnOpt_Default, &mViews.createBtn}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(110,110,290,136), btns);
	mViews.createBtn->Disable();
}

void CMyNewFolderWin::UpdateCreateButton()
{
	mViews.createBtn->SetEnable(!mViews.nameText->IsEmpty());
	mViews.nameText->SetHasChanged(false);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyChatInviteWin::CMyChatInviteWin(CWindow *inParent)
	: CWindow(SRect(0,0,280,96), windowLayer_Standard, 0, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 415);
#endif

	CContainerView *vc;
	CIconView *icn;
	CLabelView *lbl;

	// setup window
	SetTitle("\pChat Invitation");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,280,96));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();
	
	// make labels
	lbl = new CLabelView(vc, SRect(50,10,270,26));
	lbl->SetText("\pYou are invited to chat with:");
	lbl->Show();
	mNameLabel = new CLabelView(vc, SRect(50,28,270,44));
	mNameLabel->Show();

	// make buttons
	SButtons btns[] = {{viewID_AcceptChatInvite, "\pChat", btnOpt_Default, nil}, {viewID_RejectChatInvite, "\pDecline", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(90,60,270,86), btns);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */


CMyChatSubjectWin::CMyChatSubjectWin()
	: CWindow(SRect(0,0,320,136), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CIconView *icn;
	CScrollerView *scr;
	
	// setup window
	SetTitle("\pChat Subject");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	// make container view for content
	vc = new CContainerView(this, SRect(0,0,320,136));
	vc->Show();
	
	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Note);
	icn->Show();
	
	// make labels
	lbl = new CLabelView(vc, SRect(50,17,245,33));
	lbl->SetText("\pEnter a subject for this chat:");
	lbl->Show();
	
	// make text boxes
	scr = MakeTextBoxView(vc, SRect(10,50,310,92), scrollerOption_Border, &mTextView);
	mTextView->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	vc->SetFocusView(scr);

	// make buttons
	SButtons btns[] = {{1, "\pSet", btnOpt_Default, nil}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(130,100,310,126), btns);
}

//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */



//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyUserInfoWin::CMyUserInfoWin(CWindow *inParent)
	: CWindow(SRect(0,0,260,380), windowLayer_Standard, windowOption_CloseBox + windowOption_Sizeable, 0, inParent)
	//: CWindow(SRect(0,0,260,280), windowLayer_Standard, windowOption_CloseBox + windowOption_Sizeable, 0, inParent)

{
#if WIN32
	_SetWinIcon(*this, 215);
#endif

	gApp->mUserInfoWinList.AddItem(this);

	CContainerView *vc;
	CScrollerView *scr;
	CIconButtonView *icb;
	CBoxView *box;

	SetLimits(180,150);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,260,290));
	vc->Show();
	
	// make background box
	box = new CBoxView(vc, SRect(0,0,260,30));
	box->SetStyle(boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();
	mViews.box = box;
	
	// make buttons
	icb = new CIconButtonView(vc, SRect(3,3,27,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3031);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(415);
		}
	
	icb->SetID(viewID_OpenPrivateChat);
	icb->SetTooltipMsg("\pPrivate Chat");
	icb->Show();
	mViews.chatBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(30,3,54,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3030);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(206);
		}
	
	icb->SetID(viewID_SendPrivMsg);
	icb->SetTooltipMsg("\pPrivate Message");
	icb->Show();
	mViews.msgBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(57,3,81,27));
	
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3026);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(205);
		}
		
	
	icb->SetID(viewID_Refresh);
	icb->SetTooltipMsg("\pRefresh");
	icb->Show();
	mViews.refreshBtn = icb;
	
	
	icb = new CIconButtonView(vc, SRect(84,3,100,27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3009);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(192);
		}
	
	icb->SetID(viewID_SendFakeRed);
	icb->SetTooltipMsg("\pFake Red");
	icb->Show();
	mViews.FakeRedBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(103,3,124,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3063);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(2056);
		}
	
	icb->SetID(viewID_SendAway);
	icb->SetTooltipMsg("\pSet Away");
	icb->Show();
	mViews.AwayBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(127,3,148,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3062);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(2048);
		}
	
	icb->SetID(viewID_ChangeIcon);
	icb->SetTooltipMsg("\pChange Icon/Name");
	icb->Show();
	mViews.ChangeIconBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(151,3,172,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3060);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(2041);
		}
	
	icb->SetID(viewID_BlockDownload);
	icb->SetTooltipMsg("\pBlock Download");
	icb->Show();
	mViews.BlockDownloadBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(175,3,196,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3057);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(2053);
		}
	
	icb->SetID(viewID_SendVisible);
	icb->SetTooltipMsg("\pVisibility");
	icb->Show();
	mViews.VisibleBtn = icb;
	
	
	SRect stBounds;
	GetBounds(stBounds);

	icb = new CIconButtonView(vc, SRect(stBounds.right-54, 3, stBounds.right-30, 27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3017);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(418);
		}
	
	icb->SetID(viewID_UserInfoOpenUser);
	icb->SetSizing(sizing_HorizSticky);
	icb->SetTooltipMsg("\pOpen Account");
	icb->Show();
	mViews.openUserBtn = icb;

	icb = new CIconButtonView(vc, SRect(stBounds.right-27, 3, stBounds.right-3, 27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3005);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(412);
		}
	
	
	icb->SetID(viewID_Delete);
	icb->SetSizing(sizing_HorizSticky);
	icb->SetTooltipMsg("\pDisconnect");
	icb->Show();
	mViews.discBtn = icb;

	// make text scroller
	scr = MakeTextBoxView(vc, SRect(-2,30,261,291), scrollerOption_PlainBorder + scrollerOption_VertBar + scrollerOption_GrowSpace + scrollerOption_NoFocusBorder, &mViews.infoText);
	scr->SetSizing(sizing_BottomRightSticky);
	mViews.infoText->SetFont(kFixedFont, nil, kMyDefaultFixedFontSize);
	mViews.infoText->SetEditable(false);
	mViews.infoText->SetTabSelectText(false);
	mViews.textScroll = scr;
	scr->Show();

	mUserID = 0;
	mUserName[0] = 0;
}
	
CMyUserInfoWin::~CMyUserInfoWin()
{
	gApp->mUserInfoWinList.RemoveItem(this);
}

void CMyUserInfoWin::SetTextColor(const SMyColorInfo& colorInfo) 
{
	mViews.textScroll->SetContentColor(colorInfo.backColor);

	mViews.infoText->SetColor(colorInfo.textColor);
	mViews.infoText->SetFontSize(colorInfo.textSize);
}

void CMyUserInfoWin::SetContent(Uint16 inUserID, const Uint8 *inUserName, TFieldData inData)
{
	mUserID = inUserID;
	UMemory::Copy(mUserName, inUserName, inUserName[0] + 1);
	
	THdl hData = nil;
	try
	{
		// set window title
		Uint8 psUserName[64];
		Uint8 psUserIconID[64];
		inData->GetPString(myField_UserName, psUserName, sizeof(psUserName));
		inData->GetPString(myField_UserIconID, psUserIconID, sizeof(psUserIconID));
		
		
		
		SetTitle(psUserName[0] == 0 ? "\pUnnamed User" : psUserName);
		//SetTitle(psUserIconID[0] == 0 ? "\p0" : psUserIconID);
		
		
		if (inData)
		{
			bool bSetBounds = false;
			if (!mViews.infoText->GetTextSize())
				bSetBounds = true;
				
			// allocate handle to store text
			Uint32 nDataSize = inData->GetFieldSize(myField_Data);
			THdl hData = UMemory::NewHandle(nDataSize);

			// read text into handle
			void *pData = UMemory::Lock(hData);
			inData->GetField(myField_Data, pData, nDataSize);
			UMemory::Unlock(hData);
			
			// attach text handle to text view
			mViews.infoText->SetTextHandle(hData);
			hData = nil;
			mViews.infoText->SetFullHeight();

			if (bSetBounds)
			{
				// position window on screen
				nDataSize = mViews.infoText->GetFullHeight();
				if (nDataSize > 280) nDataSize = 400;
			
				SRect stBounds;
				GetBounds(stBounds);
				
				stBounds.right = stBounds.left + 260;
				stBounds.bottom = stBounds.top + nDataSize + 40;
				SetBounds(stBounds);
			}
		}
	}
	catch(...)
	{
		UMemory::Dispose(hData);
		throw;
	}
}

void CMyUserInfoWin::SetAccess()
{
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_CreateChat))
		mViews.chatBtn->Enable();
	else
		mViews.chatBtn->Disable();
	
	if (gApp->mServerVers < 182 || gApp->HasGeneralPriv(myAcc_SendMessage))
		mViews.msgBtn->Enable();
	else
		mViews.msgBtn->Disable();
		
		if (gApp->mServerVers > 194 && gApp->HasGeneralPriv(myAcc_FakeRed))
		mViews.FakeRedBtn->Show();
	else
		mViews.FakeRedBtn->Hide();
		
		if (gApp->mServerVers > 194 && gApp->HasGeneralPriv(myAcc_Visible))
		mViews.VisibleBtn->Show();
	else
		mViews.VisibleBtn->Hide();
		
		if (gApp->mServerVers > 194 && gApp->HasGeneralPriv(myAcc_Away))
		mViews.AwayBtn->Show();
	else
		mViews.AwayBtn->Hide();
		
		if (gApp->mServerVers > 194 && gApp->HasGeneralPriv(myAcc_BlockDownload))
		mViews.BlockDownloadBtn->Show();
	else
		mViews.BlockDownloadBtn->Hide();
		
		if (gApp->mServerVers > 194 && gApp->HasGeneralPriv(myAcc_ChangeIcon))
		mViews.ChangeIconBtn->Show();
	else
		mViews.ChangeIconBtn->Hide();
		
		
		/// c la!
	
	if (gApp->HasGeneralPriv(myAcc_GetClientInfo)) 
		mViews.refreshBtn->Enable();
	else
		mViews.refreshBtn->Disable();
		
		
	if (gApp->HasGeneralPriv(myAcc_OpenUser))
		mViews.openUserBtn->Show();
	else
		mViews.openUserBtn->Hide();

	if (gApp->HasGeneralPriv(myAcc_DisconUser))
		mViews.discBtn->Show();
	else
		mViews.discBtn->Hide();
		
}

Uint32 CMyUserInfoWin::GetAccountName(void *outName, Uint32 inMaxSize)
{
	THdl h = mViews.infoText->GetTextHandle();	
	if (!h)
		return 0;
	
	void *p;
	StHandleLocker lock(h, p);
	Uint32 s = h->GetSize();
	
	Uint8 *start = UMemory::Search("Account:    ", 12, p, s);	
	if (!start || s < ((start + 12) - BPTR(p)))
		return 0;
	
	start += 12;
		
	Uint8 *end = UMemory::Search("\rAddress:    ", 13, start, s - (start - BPTR(p)));
	if (!end)
		return 0;
		
	return UMemory::Copy(outName, start, min((Uint32)(end - start), inMaxSize));
}

void CMyUserInfoWin::DoPrivateChat()
{
	gApp->DoOpenPrivateChat(mUserID);
}


void CMyUserInfoWin::DoPrivateMsg()
{
	gApp->DoSendPrivMsg(mUserID, mUserName);
}

void CMyUserInfoWin::DoChangeIcon()
{
	gApp->DoChangeIcon(mUserID, mUserName);
}

void CMyUserInfoWin::DoFakeRed()
{


	gApp->DoSendFakeRed(mUserID);
}

void CMyUserInfoWin::DoVisible()
{


	gApp->DoSendVisible(mUserID);
}

void CMyUserInfoWin::DoAway()
{


	gApp->DoSendAway(mUserID);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyFileInfoWin::CMyFileInfoWin(CWindow *inParent, bool inIsFolder)
	: CWindow(SRect(0,0,280,240), windowLayer_Standard, windowOption_CloseBox, 0, inParent)
{
#if WIN32
	_SetWinIcon(*this, 215);
#endif

	CContainerView *vc;
	CButtonView *btn;
	CScrollerView *scr;
	CIconView *icn;
	CLabelView *lbl;
	
	// setup window
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	mOrigName[0] = 0;
	mPathData = nil;
	mPathSize = 0;

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,280,240));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,13,26,29));
	icn->Show();
	mViews.icon = icn;
	
	// make titles label
	lbl = new CLabelView(vc, SRect(10,40,75,130));
	lbl->SetText("\pType:\rCreator:\rSize:\rCreated:\rModified:\r\rComments:");
	lbl->SetFont(fd_Default9BoldRight);
	lbl->Show();

	// make info label
	lbl = new CLabelView(vc, SRect(80,40,270,130));
	lbl->SetFont(fd_Default9);
	lbl->Show();
	mViews.infoText = lbl;

	// make text boxes
#if WIN32
	scr = MakeTextBoxView(vc, SRect(40,10,270,32+4), scrollerOption_Border, &mViews.nameText);
#else
	scr = MakeTextBoxView(vc, SRect(40,10,270,32), scrollerOption_Border, &mViews.nameText);
#endif
	mViews.nameText->SetEnterKeyAction(enterKeyAction_None);
	mViews.nameText->SetFont(fd_Default9);
	mViews.nameText->SetID(viewID_FileInfoChange);
	scr->Show();
	scr = MakeTextBoxView(vc, SRect(10,130,270,200), scrollerOption_Border + scrollerOption_VertBar, &mViews.commentText);
	mViews.commentText->SetFont(fd_Default9);
	mViews.commentText->SetID(viewID_FileInfoChange);
	scr->Show();
	
	// make button
	btn = new CButtonView(vc, SRect(210,208,268,232));
	btn->SetTitle("\pSave");
	btn->SetID(viewID_FileSaveInfo);
	btn->Disable();
	btn->Show();
	mViews.saveBtn = btn;
	vc->SetDefaultView(btn);

	mIsFolder = inIsFolder;
}

CMyFileInfoWin::~CMyFileInfoWin()
{
	UMemory::Dispose(mPathData);
}

void CMyFileInfoWin::SetInfo(const Uint8 inName[], const Uint8 inComment[], const Uint8 inInfo[], Int16 inIcon)
{
	mViews.nameText->SetText(inName+1, inName[0]);
	mViews.commentText->SetText(inComment+1, inComment[0]);
	mViews.infoText->SetText(inInfo);
	mViews.icon->SetIconID(inIcon);
}

void CMyFileInfoWin::GetInfo(Uint8 *outName, Uint8 *outComment)
{
	if (outName) outName[0] = mViews.nameText->GetText(outName+1, 255);
	if (outComment) outComment[0] = mViews.commentText->GetText(outComment+1, 255);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyConfirmDisconnectWin::CMyConfirmDisconnectWin(const Uint8 *inServerName, Uint32 inTaskCount)
	: CWindow(SRect(0,0,390,116), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CIconView *icn;

	// setup window
	SetTitle("\pConfirm Disconnect");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,390,116));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Caution);
	icn->Show();

	// make label
	Uint8 psMessage[256];
	psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "You are currently connected to \"%#s\" with %lu transfer task%#s in progress.  Disconnecting will cancel your transfer task%#s.  Are you sure you want to disconnect from this Server?", inServerName, inTaskCount, inTaskCount > 1 ? "\ps" : "\p", inTaskCount > 1 ? "\ps" : "\p");

	lbl = new CLabelView(vc, SRect(50,10,380,74));
	lbl->SetText(psMessage);
	lbl->Show();
	
	// make buttons
	btn = new CButtonView(vc, SRect(10,83,94,103));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);

	btn = new CButtonView(vc, SRect(104,83,284,103));
	btn->SetTitle("\pDisconnect when Complete");
	btn->SetID(3);
	btn->Show();

	btn = new CButtonView(vc, SRect(294,80,380,106));
	btn->SetTitle("\pDisconnect");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyConfirmQuitWin::CMyConfirmQuitWin(const Uint8 *inServerName, Uint32 inTaskCount)
	: CWindow(SRect(0,0,350,116), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CIconView *icn;

	// setup window
	SetTitle("\pConfirm Quit");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,350,116));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Caution);
	icn->Show();

	// make label
	Uint8 psMessage[256];
	if (inTaskCount)
		psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "You are currently connected to \"%#s\" with %lu task%#s in progress.  Quitting will cancel your active task%#s.  Are you sure you want to quit and disconnect from this Server?", inServerName, inTaskCount, inTaskCount > 1 ? "\ps" : "\p", inTaskCount > 1 ? "\ps" : "\p");
	else
		psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "You are currently connected to \"%#s\".  Are you sure you want to quit and disconnect from this Server?", inServerName);

	lbl = new CLabelView(vc, SRect(50,10,340,74));
	lbl->SetText(psMessage);
	lbl->Show();
	
	// make buttons
	if (inTaskCount)
		btn = new CButtonView(vc, SRect(10,83,94,103));
	else
		btn = new CButtonView(vc, SRect(156,83,240,103));

	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);

	if (inTaskCount)
	{
		btn = new CButtonView(vc, SRect(106,83,244,103));
		btn->SetTitle("\pQuit when Complete");
		btn->SetID(3);
		btn->Show();
	}

	btn = new CButtonView(vc, SRect(256,80,340,106));
	btn->SetTitle("\pQuit");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyWaitDisconnectWin::CMyWaitDisconnectWin()
	: CWindow(SRect(0,0,350,106), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CIconView *icn;

	// setup window
	SetTitle("\pWaiting for Tasks to Complete");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,350,106));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Caution);
	icn->Show();

	// make label
	lbl = new CLabelView(vc, SRect(50,10,340,64));
	lbl->Show();
	mViews.textLabel = lbl;
	
	// make buttons
	btn = new CButtonView(vc, SRect(112,73,198,93));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);

	btn = new CButtonView(vc, SRect(214,70,340,96));
	btn->SetTitle("\pDisconnect Now");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);	

	CheckTasksStatus();
	mMsgTimer = StartNewTimer(500, true);	// 0.5 secs
}

CMyWaitDisconnectWin::~CMyWaitDisconnectWin()
{
	UTimer::Dispose(mMsgTimer);
}

void CMyWaitDisconnectWin::Timer(TTimer inTimer)
{
	if (inTimer == mMsgTimer)
		CheckTasksStatus();
}

void CMyWaitDisconnectWin::CheckTasksStatus()
{
	Uint32 nTaskCount = gApp->GetTransferTaskCount();
	
	if (nTaskCount)
	{
		Uint8 psMessage[256];
		if (nTaskCount > 1)
			psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "There are %lu transfer tasks remaining.  When they are complete,  Hotline will disconnect.", nTaskCount);
		else
			psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "There is %lu transfer task remaining.  When it is complete,  Hotline will disconnect.", nTaskCount);

		mViews.textLabel->SetText(psMessage);
	}
	else
	{
		SHitMsgData info;
		ClearStruct(info);
		info.id = 1;
		
		Hit(info);
	}	
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyWaitQuitWin::CMyWaitQuitWin()
	: CWindow(SRect(0,0,350,106), windowLayer_Modal)
{
	CContainerView *vc;
	CLabelView *lbl;
	CButtonView *btn;
	CIconView *icn;

	// setup window
	SetTitle("\pWaiting for Tasks to Complete");
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

	// make container view for content
	vc = new CContainerView(this, SRect(0,0,350,106));
	vc->Show();

	// make icon
	icn = new CIconView(vc, SRect(10,10,42,42));
	icn->SetIconID(icon_Caution);
	icn->Show();

	// make label
	lbl = new CLabelView(vc, SRect(50,10,340,64));
	lbl->Show();
	mViews.textLabel = lbl;
	
	// make buttons
	btn = new CButtonView(vc, SRect(132,73,218,93));
	btn->SetTitle("\pCancel");
	btn->SetID(2);
	btn->Show();
	vc->SetCancelView(btn);

	btn = new CButtonView(vc, SRect(234,70,340,96));
	btn->SetTitle("\pQuit Now");
	btn->SetDefault(true);
	btn->SetID(1);
	btn->Show();
	vc->SetDefaultView(btn);	

	CheckTasksStatus();
	mMsgTimer = StartNewTimer(500, true);	// 0.5 secs
}

CMyWaitQuitWin::~CMyWaitQuitWin()
{
	UTimer::Dispose(mMsgTimer);
}

void CMyWaitQuitWin::Timer(TTimer inTimer)
{
	if (inTimer == mMsgTimer)
		CheckTasksStatus();
}

void CMyWaitQuitWin::CheckTasksStatus()
{
	Uint32 nTaskCount = gApp->GetSignificantTaskCount();
	
	if (nTaskCount)
	{
		Uint8 psMessage[256];
		if (nTaskCount > 1)
			psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "There are %lu tasks remaining.  When they are complete,  Hotline will quit.", nTaskCount);
		else
			psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "There is %lu task remaining.  When it is complete,  Hotline will quit.", nTaskCount);

		mViews.textLabel->SetText(psMessage);
	}
	else
	{
		SHitMsgData info;
		ClearStruct(info);
		info.id = 1;
		
		Hit(info);
	}	
}

