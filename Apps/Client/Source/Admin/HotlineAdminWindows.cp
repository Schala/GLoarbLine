/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "HotlineAdmin.h"

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

#define ADMIN_WIN_WIDTH		600 
#define ADMIN_WIN_HEIGHT 	420

#define ADMINSPECTOR_WIN_WIDTH		600 
#define ADMINSPECTOR_WIN_HEIGHT 	420

CMyAdminWin::CMyAdminWin(CWindow *inParent)
	: CWindow(SRect(0, 0, ADMIN_WIN_WIDTH, ADMIN_WIN_HEIGHT), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox + windowOption_ZoomBox, 0, inParent)
{

	// setup window
	SetTitle("\pHotline Administrator");
	SetLimits(400, 250);
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

#if WIN32
	_SetWinIcon(*this, 210);
#endif

	// make container view for content
	CContainerView *vc = new CContainerView(this, SRect(0, 0, ADMIN_WIN_WIDTH, ADMIN_WIN_HEIGHT));
	vc->Show();

	// make background box
	CBoxView *box = new CBoxView(vc, SRect(0, 0, ADMIN_WIN_WIDTH, 30));
	box->SetStyle(boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();

	// make pane view
	mViews.pPaneView = new CPaneView(vc, SRect(0, 0, ADMIN_WIN_WIDTH, ADMIN_WIN_HEIGHT - 40));
	mViews.pPaneView->SetSizing(sizing_BottomRightSticky);
	vc->SetFocusView(mViews.pPaneView);
	mViews.pPaneView->Show();


	// make icon buttons
	CIconButtonView *icb = new CIconButtonView(vc, SRect(3,3,27,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3008);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(234);
		}
	
	icb->SetID(viewID_AdminNewUser);
	icb->SetTooltipMsg("\pNew Account");
	icb->Show();
	mViews.pNewUserBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(30,3,54,27));
	if (gApp->mIconSet == 2){
		icb->SetIconID(3017);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(418);
		}
	
	icb->SetID(viewID_AdminOpenUser);
	icb->SetTooltipMsg("\pOpen Account");
	icb->Show();
	mViews.pOpenUserBtn = icb;

	// make buttons
	CButtonView *btn = new CButtonView(vc, SRect(ADMIN_WIN_WIDTH - 340, ADMIN_WIN_HEIGHT - 32, ADMIN_WIN_WIDTH - 250, ADMIN_WIN_HEIGHT - 6));
	btn->SetSizing(sizing_HorizSticky | sizing_VertSticky);
	btn->SetTitle("\pSave");
	btn->SetTooltipMsg("\pSave all changes");
	btn->SetID(viewID_AdminSaveUsers);
	btn->SetEnable(false);
	btn->SetDefault(true);
	vc->SetDefaultView(btn);	
	btn->Show();
	mViews.pSaveBtn = btn;

	btn = new CButtonView(vc, SRect(ADMIN_WIN_WIDTH - 230, ADMIN_WIN_HEIGHT - 32, ADMIN_WIN_WIDTH - 140, ADMIN_WIN_HEIGHT - 6));
	btn->SetSizing(sizing_HorizSticky | sizing_VertSticky);
	btn->SetTitle("\pRevert");
	btn->SetTooltipMsg("\pRevert all changes");
	btn->SetID(viewID_AdminRevertUsers);
	btn->SetEnable(false);
	btn->Show();
	mViews.pRevertBtn = btn;

	btn = new CButtonView(vc, SRect(ADMIN_WIN_WIDTH - 120, ADMIN_WIN_HEIGHT - 32, ADMIN_WIN_WIDTH - 30, ADMIN_WIN_HEIGHT - 6));
	btn->SetSizing(sizing_HorizSticky | sizing_VertSticky);
	btn->SetTitle("\pClose");
	btn->SetTooltipMsg("\pClose");
	btn->SetID(viewID_AdminClose);
	vc->SetCancelView(btn);	
	btn->Show();
	mViews.pCloseBtn = btn;

	// make container view
	vc = new CContainerView(mViews.pPaneView, SRect(0, 0, ADMIN_WIN_WIDTH - 320, ADMIN_WIN_HEIGHT - 40));
	vc->SetSizing(sizing_BottomRightSticky);
	mViews.pPaneView->SetFocusView(vc);
	mViews.pPaneView->SetViewLimits(vc, 150, 0);
	vc->Show();
	
	// make delete icon button
	icb = new CIconButtonView(vc, SRect(ADMIN_WIN_WIDTH - 344, 3, ADMIN_WIN_WIDTH - 320, 27));
	
	if (gApp->mIconSet == 2){
		icb->SetIconID(3000);
	}else if (gApp->mIconSet == 1){
		icb->SetIconID(212);
		}
	
	icb->SetID(viewID_AdminDeleteUser);
	icb->SetSizing(sizing_HorizSticky);
	icb->SetTooltipMsg("\pDelete Accounts");
	icb->Show();
	mViews.pDeleteUserBtn = icb;

	// make scroller view
	CScrollerView *scr = new CScrollerView(vc, SRect(-1, 30, ADMIN_WIN_WIDTH - 320, ADMIN_WIN_HEIGHT - 40), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	vc->SetFocusView(scr);

	// make file tree view
	mViews.pUserTreeView = new CMyAdminUserTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), this);
	mViews.pUserTreeView->SetBehaviour(itemBehav_DeselectOthers + itemBehav_ShiftExtend + itemBehav_ControlToggle + itemBehav_DoubleClickAction);
	mViews.pUserTreeView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mViews.pUserTreeView->SetCanFocus(true);
	mViews.pUserTreeView->SetID(viewID_AdminUserTree);
	mViews.pUserTreeView->Show();

	// make access view
	mViews.pUserAccessView = new CMyAdminUserAccessView(mViews.pPaneView, SRect(ADMIN_WIN_WIDTH - 314, 28, ADMIN_WIN_WIDTH + 3, ADMIN_WIN_HEIGHT - 38), this);
	mViews.pUserAccessView->SetOptions(scrollerOption_VertBar + scrollerOption_Border + scrollerOption_NoBkgnd);
	mViews.pUserAccessView->SetSizing(sizing_BottomSticky | sizing_HorizSticky);
	mViews.pUserAccessView->SetCanFocus(true);
	mViews.pPaneView->SetViewLimits(mViews.pUserAccessView, 200, 0);
	mViews.pUserAccessView->Show();
}

CMyAdminWin::~CMyAdminWin()
{
}

void CMyAdminWin::SetBoundsInfo(SMyAdminBoundsInfo& inBoundsInfo)
{
	if (inBoundsInfo.stAdminBounds.IsNotEmpty())
	{
		SetBounds(inBoundsInfo.stAdminBounds);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	mViews.pPaneView->SetPanePercent(inBoundsInfo.nAdminPane);
	mViews.pUserTreeView->SetTabs(inBoundsInfo.nAdminTab1, inBoundsInfo.nAdminTab2);
	
}

void CMyAdminWin::GetBoundsInfo(SMyAdminBoundsInfo& outBoundsInfo)
{
	GetBounds(outBoundsInfo.stAdminBounds);

	outBoundsInfo.nAdminPane = mViews.pPaneView->GetPanePercent();
	mViews.pUserTreeView->GetTabs(outBoundsInfo.nAdminTab1, outBoundsInfo.nAdminTab2);
}

bool CMyAdminWin::AddListFromFields(TFieldData inData)
{
	bool bRet = mViews.pUserTreeView->AddListFromFields(inData);
		
	RefreshGlobalButtons();
	DebugBreak("on est avant itemdraw");
	return bRet;
}

TFieldData CMyAdminWin::GetFieldsFromList()
{

	TFieldData pRet =  mViews.pUserTreeView->GetFieldsFromList();

	RefreshGlobalButtons();
	return pRet;
}

void CMyAdminWin::RevertUserList()
{
	mViews.pUserTreeView->RevertUserList();

	RefreshGlobalButtons();
}

bool CMyAdminWin::GetUserListStatus()
{
	return mViews.pUserTreeView->GetUserListStatus();
}

void CMyAdminWin::SetUsersAccess(const SMyAdminUsersAccess& inUsersAccess)
{
	mViews.pUserAccessView->SetUsersAccess(inUsersAccess);
}

void CMyAdminWin::GetUserAccess(SMyUserAccess& outUserAccess, Uint32 inAccessID)
{
	mViews.pUserAccessView->GetUserAccess(outUserAccess, inAccessID);
}

void CMyAdminWin::SetEnableUserAccess(bool inEnable)
{
	mViews.pUserAccessView->SetEnableUserAccess(inEnable);
}

bool CMyAdminWin::UpdateUserAccess(Uint32 inAccessID)
{
	if (mViews.pUserTreeView->UpdateUserAccess(inAccessID))
	{
		RefreshGlobalButtons();
		return true;
	}
	
	return false;
}

bool CMyAdminWin::RestoreUserAccess(Uint32 inAccessID)
{
	if (mViews.pUserTreeView->RestoreUserAccess(inAccessID))
	{
		RefreshGlobalButtons();
		return true;
	}
	
	return false;
}

bool CMyAdminWin::AddNewUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass)
{
	if (mViews.pUserTreeView->AddNewUser(inName, inLogin, inPass))
	{
		RefreshGlobalButtons();
		return true;
	}
	
	return false;
}

bool CMyAdminWin::GetSelectedUser(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass, bool *outIsNewUser)
{
	return mViews.pUserTreeView->GetSelectedUser(outName, outLogin, outPass, outIsNewUser);
}

bool CMyAdminWin::ModifySelectedUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass)
{
	if (mViews.pUserTreeView->ModifySelectedUser(inName, inLogin, inPass))
	{
		RefreshGlobalButtons();
		return true;
	}
	
	return false;
}

bool CMyAdminWin::DeleteSelectedUsers()
{
	if (mViews.pUserTreeView->DeleteSelectedUsers())
	{
		RefreshGlobalButtons();
		return true;
	}
	
	return false;
}

void CMyAdminWin::EnableUserButtons()
{
	mViews.pOpenUserBtn->Enable();
	mViews.pDeleteUserBtn->Enable();
}

void CMyAdminWin::DisableUserButtons(bool inDisableDelete)
{
	mViews.pOpenUserBtn->Disable();
	
	if (inDisableDelete)
		mViews.pDeleteUserBtn->Disable();
}

void CMyAdminWin::RefreshGlobalButtons()
{

	if (mViews.pUserTreeView->GetUserListStatus())
	{
		mViews.pRevertBtn->Enable();
		mViews.pSaveBtn->Enable();
	}
	else
	{
		mViews.pRevertBtn->Disable();
		mViews.pSaveBtn->Disable();
	}
}


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyAdminEditUserWin::CMyAdminEditUserWin(bool inIsNewUser, bool inDisable)
	: CWindow(SRect(0,0,310,160), windowLayer_Modal)
{
	// setup window
	if (inIsNewUser)
		SetTitle("\pNew Account");
	else
		SetTitle("\pEdit Account");

	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	// make container view for content
	CContainerView *vc = new CContainerView(this, SRect(0,0,310,160));
	vc->Show();
	mViews.pContainerView = vc;

	// make box
	CBoxView *box = new CBoxView(vc, SRect(10,10,300,116));
	box->SetStyle(boxStyle_Sunken);
	box->Show();

	// make labels
	CLabelView *lbl = new CLabelView(vc, SRect(20,25,95,41));
	lbl->SetText("\pName:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,55,95,71));
	lbl->SetText("\pLogin:");
	lbl->Show();
	lbl = new CLabelView(vc, SRect(20,85,95,101));
	lbl->SetText("\pPassword:");
	lbl->Show();
	
	// make text boxes
	CScrollerView *scr = MakeTextBoxView(vc, SRect(110,20,290,46), scrollerOption_Border, &mViews.pNameText);
	mViews.pNameText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	vc->SetFocusView(scr);
	if (inDisable)
		scr->Disable();

	scr = MakeTextBoxView(vc, SRect(110,50,290,76), scrollerOption_Border, &mViews.pLoginText);
	mViews.pLoginText->SetEnterKeyAction(enterKeyAction_None);
	scr->Show();
	if (inDisable)
		scr->Disable();

	// make password box
	if (inIsNewUser)
	{
		scr = MakeTextBoxView(vc, SRect(110,80,290,106), scrollerOption_Border, &mViews.pPassText);
		mViews.pPassText->SetEnterKeyAction(enterKeyAction_None,enterKeyAction_None);
		mViews.pBulletPassText = nil;
	}
	else
	{
		scr = MakePasswordBoxView(vc, SRect(110,80,290,106), scrollerOption_Border, &mViews.pBulletPassText);
		mViews.pPassText = nil;		
	}
	
	scr->Show();
	mViews.pPasswordScr = scr;	
	
	if (inDisable)
		scr->Disable();
	
	// make buttons
	CButtonView *pSaveBtn;
	SButtons btns[] = {{1, "\pSave", btnOpt_Default, &pSaveBtn}, {2, "\pCancel", btnOpt_Cancel, nil}};
	CButtonView::BuildButtons(vc, SRect(120,126,300,152), btns);
	
	if (inDisable)
		pSaveBtn->Disable();
}

void CMyAdminEditUserWin::SetInfo(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass)
{
	if (inName)
		mViews.pNameText->SetText(inName + 1, inName[0]);
	
	if (inLogin)
		mViews.pLoginText->SetText(inLogin + 1, inLogin[0]);

	if (inPass)
	{
		if (mViews.pBulletPassText)
			mViews.pBulletPassText->SetText(inPass + 1, inPass[0]);
		else if(mViews.pPassText)
			mViews.pPassText->SetText(inPass + 1, inPass[0]);
	}
}

bool CMyAdminEditUserWin::GetInfo(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass)
{
	if (outName)
		outName[0] = mViews.pNameText->GetText(outName + 1, 31);
		
	if (outLogin)
		outLogin[0] = mViews.pLoginText->GetText(outLogin + 1, 31);

	if (outPass)
	{
		outPass[0] = 0;
		
		// check password
		if ((mViews.pBulletPassText && !mViews.pBulletPassText->IsDummyPassword() && mViews.pBulletPassText->GetPasswordSize() > 31) || 
		    (mViews.pPassText && mViews.pPassText->GetTextSize() > 31))
		{
			if (mViews.pBulletPassText)
				mViews.pBulletPassText->SetText(nil, 0);
			else if (mViews.pPassText)
				mViews.pPassText->SetText(nil, 0);

			mViews.pContainerView->SetFocusView(mViews.pPasswordScr);
			
			gApp->DisplayStandardMessage("\pPassword too long", "\pPlease select a password that is no longer than 31 characters.", icon_Stop, 1);
			return false;
		}

		if (mViews.pBulletPassText)
			outPass[0] = mViews.pBulletPassText->GetText(outPass + 1, 31);
		else if (mViews.pPassText)
			outPass[0] = mViews.pPassText->GetText(outPass + 1, 31);
	}
	
	return true;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
void CMyAdmInSpectorWin::SetBoundsInfo(SMyAdmInSpectorBoundsInfo& inBoundsInfo)
{
	if (inBoundsInfo.stAdmInSpectorBounds.IsNotEmpty())
	{
		SetBounds(inBoundsInfo.stAdmInSpectorBounds);
		SetAutoBounds(windowPos_Best, windowPosOn_WinScreen);
	}
	else
		SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);
	
	mViews.pPaneView->SetPanePercent(inBoundsInfo.nAdmInSpectorPane);
	mViews.pAdmInSpectorTreeView->SetTabs(inBoundsInfo.nAdmInSpectorTab1, inBoundsInfo.nAdmInSpectorTab2);
}

void CMyAdmInSpectorWin::GetBoundsInfo(SMyAdmInSpectorBoundsInfo& outBoundsInfo)
{
	GetBounds(outBoundsInfo.stAdmInSpectorBounds);

	outBoundsInfo.nAdmInSpectorPane = mViews.pPaneView->GetPanePercent();
	mViews.pAdmInSpectorTreeView->GetTabs(outBoundsInfo.nAdmInSpectorTab1, outBoundsInfo.nAdmInSpectorTab2);
}

CMyAdmInSpectorWin::CMyAdmInSpectorWin(CWindow *inParent)
	: CWindow(SRect(0, 0, ADMINSPECTOR_WIN_WIDTH, ADMINSPECTOR_WIN_HEIGHT), windowLayer_Standard, windowOption_Sizeable + windowOption_CloseBox + windowOption_ZoomBox, 0, inParent)
{
	// setup window
	SetTitle("\pHotline AdmInSpector");
	SetLimits(400, 250);
	SetAutoBounds(windowPos_Center, windowPosOn_WinScreen);

#if WIN32
	_SetWinIcon(*this, 210);
#endif

	// make container view for content
	CContainerView *vc = new CContainerView(this, SRect(0, 0, ADMINSPECTOR_WIN_WIDTH, ADMINSPECTOR_WIN_HEIGHT));
	vc->Show();

	// make background box
	CBoxView *box = new CBoxView(vc, SRect(0, 0, ADMINSPECTOR_WIN_WIDTH, 30));
	box->SetStyle(boxStyle_Raised);
	box->SetSizing(sizing_RightSticky);
	box->Show();

	// make pane view
	mViews.pPaneView = new CPaneView(vc, SRect(0, 0, ADMINSPECTOR_WIN_WIDTH, ADMINSPECTOR_WIN_HEIGHT - 40));
	mViews.pPaneView->SetSizing(sizing_BottomRightSticky);
	vc->SetFocusView(mViews.pPaneView);
	mViews.pPaneView->Show();

	// make icon buttons
	/*
	CIconButtonView *icb = new CIconButtonView(vc, SRect(3,3,27,27));
	icb->SetIconID(234);
	icb->SetID(viewID_AdminNewUser);
	icb->SetTooltipMsg("\pNew Account");
	icb->Show();
	mViews.pNewUserBtn = icb;
	
	icb = new CIconButtonView(vc, SRect(30,3,54,27));
	icb->SetIconID(418);
	icb->SetID(viewID_AdminOpenUser);
	icb->SetTooltipMsg("\pOpen Account");
	icb->Show();
	mViews.pOpenUserBtn = icb;
*/
	// make buttons
	
	CButtonView *btn = new CButtonView(vc, SRect(ADMINSPECTOR_WIN_WIDTH - 340, ADMINSPECTOR_WIN_HEIGHT - 32, ADMINSPECTOR_WIN_WIDTH - 250, ADMINSPECTOR_WIN_HEIGHT - 6));
	/*
	btn->SetSizing(sizing_HorizSticky | sizing_VertSticky);
	btn->SetTitle("\pSave");
	btn->SetTooltipMsg("\pSave all changes");
	btn->SetID(viewID_AdminSaveUsers);
	btn->SetEnable(false);
	btn->SetDefault(true);
	vc->SetDefaultView(btn);	
	btn->Show();
	mViews.pSaveBtn = btn;

	btn = new CButtonView(vc, SRect(ADMINSPECTOR_WIN_WIDTH - 230, ADMINSPECTOR_WIN_HEIGHT - 32, ADMINSPECTOR_WIN_WIDTH - 140, ADMINSPECTOR_WIN_HEIGHT - 6));
	btn->SetSizing(sizing_HorizSticky | sizing_VertSticky);
	btn->SetTitle("\pRevert");
	btn->SetTooltipMsg("\pRevert all changes");
	btn->SetID(viewID_AdminRevertUsers);
	btn->SetEnable(false);
	btn->Show();
	mViews.pRevertBtn = btn;
*/
	btn = new CButtonView(vc, SRect(ADMINSPECTOR_WIN_WIDTH - 120, ADMINSPECTOR_WIN_HEIGHT - 32, ADMINSPECTOR_WIN_WIDTH - 30, ADMINSPECTOR_WIN_HEIGHT - 6));
	btn->SetSizing(sizing_HorizSticky | sizing_VertSticky);
	btn->SetTitle("\pClose");
	btn->SetTooltipMsg("\pClose");
	btn->SetID(viewID_AdmInSpectorClose);
	vc->SetCancelView(btn);	
	btn->Show();
	mViews.pCloseBtn = btn;

	// make container view
	vc = new CContainerView(mViews.pPaneView, SRect(0, 0, ADMINSPECTOR_WIN_WIDTH - 120, ADMINSPECTOR_WIN_HEIGHT - 40));
	vc->SetSizing(sizing_BottomRightSticky);
	mViews.pPaneView->SetFocusView(vc);
	mViews.pPaneView->SetViewLimits(vc, 150, 0);
	vc->Show();
	
	/*// make delete icon button
	icb = new CIconButtonView(vc, SRect(ADMINSPECTOR_WIN_WIDTH - 344, 3, ADMINSPECTOR_WIN_WIDTH - 320, 27));
	icb->SetIconID(212);
	icb->SetID(viewID_AdminDeleteUser);
	icb->SetSizing(sizing_HorizSticky);
	icb->SetTooltipMsg("\pDelete Accounts");
	icb->Show();
	mViews.pDeleteUserBtn = icb;
*/
	// make scroller view
	CScrollerView *scr = new CScrollerView(vc, SRect(-1, 30, ADMINSPECTOR_WIN_WIDTH - 120, ADMINSPECTOR_WIN_HEIGHT - 40), scrollerOption_VertBar + scrollerOption_PlainBorder + scrollerOption_NoFocusBorder + LIST_BACKGROUND_OPTION);
	scr->SetSizing(sizing_BottomRightSticky);
	scr->SetCanFocus(true);
	scr->Show();
	vc->SetFocusView(scr);

	// make file tree view
	
	mViews.pAdmInSpectorTreeView = new CMyAdmInSpectorTreeView(scr, SRect(0, 0, scr->GetVisibleContentWidth(), scr->GetVisibleContentHeight()), this);
	mViews.pAdmInSpectorTreeView->SetBehaviour(itemBehav_DeselectOthers + itemBehav_ShiftExtend + itemBehav_ControlToggle + itemBehav_DoubleClickAction);
	mViews.pAdmInSpectorTreeView->SetSizing(sizing_RightSticky | sizing_FullHeight);
	mViews.pAdmInSpectorTreeView->SetCanFocus(true);
	mViews.pAdmInSpectorTreeView->SetID(viewID_GetAdminSpectorInfo);
	mViews.pAdmInSpectorTreeView->Show();

	/*// make access view
	mViews.pUserAccessView = new CMyAdminUserAccessView(mViews.pPaneView, SRect(ADMINSPECTOR_WIN_WIDTH - 314, 28, ADMINSPECTOR_WIN_WIDTH + 3, ADMINSPECTOR_WIN_HEIGHT - 38), this);
	mViews.pUserAccessView->SetOptions(scrollerOption_VertBar + scrollerOption_Border + scrollerOption_NoBkgnd);
	mViews.pUserAccessView->SetSizing(sizing_BottomSticky | sizing_HorizSticky);
	mViews.pUserAccessView->SetCanFocus(true);
	mViews.pPaneView->SetViewLimits(mViews.pUserAccessView, 200, 0);
	mViews.pUserAccessView->Show();*/
}

CMyAdmInSpectorWin::~CMyAdmInSpectorWin()
{
}
bool CMyAdmInSpectorWin::AddListFromFields(TFieldData inData)
{
	bool bRet = mViews.pAdmInSpectorTreeView->AddListFromFields(inData);
	//RefreshGlobalButtons();
	return bRet;
}

