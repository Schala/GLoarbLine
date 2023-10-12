/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "HotlineAdmin.h"


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

CMyAdminUserTreeView::CMyAdminUserTreeView(CViewHandler *inHandler, const SRect& inBounds, CMyAdminWin *inAdminWin)
	: CTabbedTreeView(inHandler, inBounds, 240, 241)
{
DebugBreak("treeview Admin");
	mAdminWin = inAdminWin;

if (gApp->mIconSet == 2){
		mAllUsersIcon = UIcon::Load(3018);
	}else if (gApp->mIconSet == 1){
		mAllUsersIcon = UIcon::Load(200);
		}
	
if (gApp->mIconSet == 2){
		mNoPrivIcon = UIcon::Load(3011);
	}else if (gApp->mIconSet == 1){
		mNoPrivIcon = UIcon::Load(190);
		}
	
	if (gApp->mIconSet == 2){
		mAllPrivIcon = UIcon::Load(3012);
	}else if (gApp->mIconSet == 1){
		mAllPrivIcon = UIcon::Load(191);
		}
	
	if (gApp->mIconSet == 2){
		mDisconnectPrivIcon = UIcon::Load(3009);
	}else if (gApp->mIconSet == 1){
		mDisconnectPrivIcon = UIcon::Load(192);
		}
	
	if (gApp->mIconSet == 2){
		mRegularPrivIcon = UIcon::Load(3010);
	}else if (gApp->mIconSet == 1){
		mRegularPrivIcon = UIcon::Load(193);
		}
	
if (gApp->mIconSet == 2){
		mNewNoPrivIcon = UIcon::Load(3036);
	}else if (gApp->mIconSet == 1){
		mNewNoPrivIcon = UIcon::Load(250);
		}
	
	if (gApp->mIconSet == 2){
		mNewAllPrivIcon = UIcon::Load(3035);
	}else if (gApp->mIconSet == 1){
		mNewAllPrivIcon = UIcon::Load(251);
		}
	
	if (gApp->mIconSet == 2){
		mNewDisconnectPrivIcon = UIcon::Load(3037);
	}else if (gApp->mIconSet == 1){
		mNewDisconnectPrivIcon = UIcon::Load(252);
		}
		
	if (gApp->mIconSet == 2){
		mNewRegularPrivIcon = UIcon::Load(3038);
	}else if (gApp->mIconSet == 1){
		mNewRegularPrivIcon = UIcon::Load(253);
		}
	
	

#if WIN32
	mTabHeight = 18;
#endif

	// set headers
	AddTab("\pLogin", 100, 100);
	AddTab("\pName");
	SetTabs(60, 40);	
}

CMyAdminUserTreeView::~CMyAdminUserTreeView()
{
	ClearUserList();
		
	UIcon::Release(mAllUsersIcon);

	UIcon::Release(mNoPrivIcon);
	UIcon::Release(mAllPrivIcon);
	UIcon::Release(mDisconnectPrivIcon);
	UIcon::Release(mRegularPrivIcon);

	UIcon::Release(mNewNoPrivIcon);
	UIcon::Release(mNewAllPrivIcon);
	UIcon::Release(mNewDisconnectPrivIcon);
	UIcon::Release(mNewRegularPrivIcon);
}
		
void CMyAdminUserTreeView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
	if (!inTabPercent1 && !inTabPercent2)
		return;
	DebugBreak("SetTabs");
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyAdminUserTreeView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
	DebugBreak("GetTabs");
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}

bool CMyAdminUserTreeView::AddListFromFields(TFieldData inData)
{

	ClearUserList();
	
	SMyAdminUserInfo *pUserInfo = (SMyAdminUserInfo *)UMemory::NewClear(sizeof(SMyAdminUserInfo));
	if (inData)
		pUserInfo->stUserInfo.loginSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.loginData, "All Accounts", 12));
	else
		pUserInfo->stUserInfo.loginSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.loginData, "All New Accounts", 16));

	if (AddTreeItem(0, pUserInfo) != 1)
	{
		ClearUserList();
		return false;
	}
	
	if (inData)
	{			
		Uint16 nFieldCount = inData->GetFieldCount();
		if (!nFieldCount)
			return false;
	
		Uint16 nCount = 0;
		Uint8 bufUserData[512];
		
		while (nCount < nFieldCount)
		{
			Uint32 nDataSize = inData->GetFieldByIndex(++nCount, bufUserData, sizeof(bufUserData));
			if (!nDataSize)
				continue;
		
			THdl hUserData = nil;
			try
			{
				hUserData = UMemory::NewHandle(bufUserData, nDataSize);
			}
			catch(...)
			{
				continue;
				// don't throw
			}
		
			StFieldData stUserData;
			stUserData->SetDataHandle(hUserData);
				
			pUserInfo = (SMyAdminUserInfo *)UMemory::NewClear(sizeof(SMyAdminUserInfo));

			pUserInfo->stUserInfo.nameSize = TB((Uint16)stUserData->GetField(myField_UserName, pUserInfo->stUserInfo.nameData, sizeof(pUserInfo->stUserInfo.nameData)));
			pUserInfo->stUserInfo.loginSize = TB((Uint16)stUserData->GetField(myField_UserLogin, pUserInfo->stUserInfo.loginData, sizeof(pUserInfo->stUserInfo.loginData)));
			pUserInfo->stUserInfo.passwordSize = TB((Uint16)stUserData->GetField(myField_UserPassword, pUserInfo->stUserInfo.passwordData, sizeof(pUserInfo->stUserInfo.passwordData)));
			stUserData->GetField(myField_UserAccess, &pUserInfo->stUserInfo.access, sizeof(pUserInfo->stUserInfo.access));
		
			// unscramble login
			Uint32 nSize = TB(pUserInfo->stUserInfo.loginSize);
			Uint8 *pLogin = pUserInfo->stUserInfo.loginData;
			while (nSize--) { *pLogin = ~(*pLogin); pLogin++; }
		
			// set "no change" marker for password
			if (pUserInfo->stUserInfo.passwordSize)
			{
				pUserInfo->stUserInfo.passwordSize = TB((Uint16)1);
				pUserInfo->stUserInfo.passwordData[0] = 0;
			}
		
			// save user
			pUserInfo->SaveUser();
			DebugBreak("Addtreeitem %s", pUserInfo->stUserInfo.loginData);
			AddTreeItem(1, pUserInfo, false);
		}
		
		// sort the list
		Sort(1, CompareLogins);
	}
	
	// disclose and select first item
	Disclose(1);
	DeselectAllTreeItems();
	SelectTreeItem(1);
	MakeTreeItemVisibleInList(1);

	return true;
	
}

TFieldData CMyAdminUserTreeView::GetFieldsFromList()
{
DebugBreak("GetFieldsFromList");
	Uint8 str[256];
	Uint8 *p;
	Uint32 s;

	TFieldData pData = UFieldData::New();
	SaveDeleteUserList(pData);
	
	Uint32 i = 1;	// skip first item
	SMyAdminUserInfo *pUserInfo;
	
	// send modified users
	while (GetNextTreeItem(pUserInfo, i))
	{
		// check if modified
		if (!pUserInfo->IsModifiedUser())
			continue;
				
		// store user data
		StFieldData stUserData;
		
		// check if new user
		if (!pUserInfo->IsNewUser())
		{
			// scramble and store saved login
			str[0] = UMemory::Copy(str + 1, pUserInfo->stSavedUserInfo.loginData, TB(pUserInfo->stSavedUserInfo.loginSize) > sizeof(str) - 1 ? sizeof(str) - 1 : TB(pUserInfo->stSavedUserInfo.loginSize));
			s = str[0];
			p = str + 1;
			while (s--) { *p = ~(*p); p++; }
			stUserData->AddField(myField_Data, str + 1, str[0]);
		}
		
		// scramble and store login
		str[0] = UMemory::Copy(str + 1, pUserInfo->stUserInfo.loginData, TB(pUserInfo->stUserInfo.loginSize) > sizeof(str) - 1 ? sizeof(str) - 1 : TB(pUserInfo->stUserInfo.loginSize));
		s = str[0];
		p = str + 1;
		while (s--) { *p = ~(*p); p++; }
		stUserData->AddField(myField_UserLogin, str + 1, str[0]);
		
		// the password is already scrambled
		stUserData->AddField(myField_UserPassword, pUserInfo->stUserInfo.passwordData, TB(pUserInfo->stUserInfo.passwordSize) > sizeof(str) - 1 ? sizeof(str) - 1 : TB(pUserInfo->stUserInfo.passwordSize));

		stUserData->AddField(myField_UserName, pUserInfo->stUserInfo.nameData, FB(pUserInfo->stUserInfo.nameSize));
		stUserData->AddField(myField_UserAccess, &pUserInfo->stUserInfo.access, sizeof(pUserInfo->stUserInfo.access));
	
		THdl hUserData = stUserData->GetDataHandle();
		if (!hUserData)
			continue;
				
		Uint32 nDataSize = UMemory::GetSize(hUserData);
		Uint8 *pUserData = UMemory::Lock(hUserData);
		pData->AddField(myField_Data, pUserData, nDataSize);
		UMemory::Unlock(hUserData);
		
		pUserInfo->SaveUser();
	}

	// sort the list
	Sort(1, CompareLogins);

	// disclose and select first item
	Disclose(1);
	DeselectAllTreeItems();
	SelectTreeItem(1);
	MakeTreeItemVisibleInList(1);	
	
	return pData;
}

void CMyAdminUserTreeView::RevertUserList()
{
DebugBreak("RevertUserList");
	Uint32 i = 1;	// skip first item
	SMyAdminUserInfo *pUserInfo;
	
	while (GetNextTreeItem(pUserInfo, i))
	{
		if (pUserInfo->IsNewUser())
		{
			// remove from tree and dispose
			RemoveTreeItem(pUserInfo);
			UMemory::Dispose((TPtr)pUserInfo);	
			
			i--;
		}
		else
			pUserInfo->RevertUser();
	}

	RevertDeleteUserList();

	// sort the list
	Sort(1, CompareLogins);

	// disclose and select first item
	Disclose(1);
	DeselectAllTreeItems();
	SelectTreeItem(1);
	MakeTreeItemVisibleInList(1);
}

void CMyAdminUserTreeView::ClearUserList()
{
DebugBreak("ClearUserList");
	ClearDeleteUserList();

	Uint32 i = 0;
	SMyAdminUserInfo *pUserInfo;
	
	while (GetNextTreeItem(pUserInfo, i))
		UMemory::Dispose((TPtr)pUserInfo);
	
	ClearTree();	
}

bool CMyAdminUserTreeView::GetUserListStatus()		
{	
DebugBreak("GetUserListStatus");
	if (mDeleteUserList.GetItemCount())
		return true;

	Uint32 i = 1;	// skip first item
	SMyAdminUserInfo *pUserInfo;
	
	while (GetNextTreeItem(pUserInfo, i))
	{
		// check if modified
		if (pUserInfo->IsModifiedUser())
			return true;
	}
	
	return false;
}

bool CMyAdminUserTreeView::UpdateUserAccess(Uint32 inAccessID)
{
DebugBreak("UpdateUserAccess");
	if (!mAdminWin)
		return false;

	SMyAdminUserInfo *pUserInfo;
	Uint32 nFirstSelected = GetFirstSelectedTreeItem(&pUserInfo);
	if (!nFirstSelected || !pUserInfo)
		return false;

	if (nFirstSelected == 1)					// first item selected
	{
		// check access
		if (!gApp->HasGeneralPriv(myAcc_ModifyUser))
		{
			Uint32 i = 1;	// skip first item
			SMyAdminUserInfo *pCheckUserInfo;
			
			while (GetNextTreeItem(pCheckUserInfo, i))
			{
				// check if new user				
				if (!pCheckUserInfo->IsNewUser())
				{
					// restore access
					SetUsersAccess(nFirstSelected, true);
					
					gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to modify account information.", icon_Caution, 1);
					return false;
				}
			}
		}
		
		// set access
		mAdminWin->GetUserAccess(pUserInfo->stUserInfo.access, inAccessID);
		bool bPriv = pUserInfo->stUserInfo.access.HasPriv(inAccessID);

		// set the priv for all users
		Uint32 i = 1;	// skip first item
		while (GetNextTreeItem(pUserInfo, i))
		{
			pUserInfo->stUserInfo.access.SetPriv(inAccessID, bPriv);
			RefreshTreeItem(i);
		}
	}
	else										// one or more items selected
	{
		Uint32 nLastSelected = GetLastSelectedTreeItem();

		// check access
		if (!gApp->HasGeneralPriv(myAcc_ModifyUser))
		{
			// check if new user				
			if (!pUserInfo->IsNewUser())
			{
				// restore access
				SetUsersAccess(nFirstSelected, true);

				gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to modify account information.", icon_Caution, 1);
				return false;
			}
			else if (nFirstSelected != nLastSelected)	// two or more items selected
			{
				for (Uint32 i = nFirstSelected + 1; i <= nLastSelected; i++)
				{
					if (IsSelected(i))
					{
						SMyAdminUserInfo *pCheckUserInfo = GetTreeItem(i);
						
						// check if new user				
						if (pCheckUserInfo && !pCheckUserInfo->IsNewUser())						
						{
							// restore access
							SetUsersAccess(nFirstSelected, true);

							gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to modify account information.", icon_Caution, 1);
							return false;
						}
					}
				}
			}
		}

		// set access
		mAdminWin->GetUserAccess(pUserInfo->stUserInfo.access, inAccessID);
		bool bPriv = pUserInfo->stUserInfo.access.HasPriv(inAccessID);

		RefreshTreeItem(nFirstSelected);

		if (nFirstSelected != nLastSelected)	// two or more items selected
		{
			for (Uint32 i = nFirstSelected + 1; i <= nLastSelected; i++)
			{
				if (IsSelected(i))
				{
					pUserInfo = GetTreeItem(i);
					if (pUserInfo)
					{
						pUserInfo->stUserInfo.access.SetPriv(inAccessID, bPriv);
						RefreshTreeItem(i);
					}
				}
			}
		}
	}
	
	return true;
}

bool CMyAdminUserTreeView::RestoreUserAccess(Uint32 inAccessID)
{
DebugBreak("RestoreUserAccess");
	SMyAdminUserInfo *pUserInfo;
	Uint32 nFirstSelected = GetFirstSelectedTreeItem(&pUserInfo);
	if (!nFirstSelected || !pUserInfo)
		return false;

	if (nFirstSelected == 1)					// first item selected
	{
		// restore the priv for all users
		Uint32 i = 1;	// skip first item
		while (GetNextTreeItem(pUserInfo, i))
		{
			pUserInfo->stUserInfo.access.SetPriv(inAccessID, pUserInfo->stSavedAccess.HasPriv(inAccessID));
			RefreshTreeItem(i);
		}
	}
	else										// one or more items selected
	{
		pUserInfo->stUserInfo.access.SetPriv(inAccessID, pUserInfo->stSavedAccess.HasPriv(inAccessID));
		RefreshTreeItem(nFirstSelected);

		Uint32 nLastSelected = GetLastSelectedTreeItem();
		if (nFirstSelected != nLastSelected)	// two or more items selected
		{
			for (Uint32 i = nFirstSelected + 1; i <= nLastSelected; i++)
			{
				if (IsSelected(i))
				{
					pUserInfo = GetTreeItem(i);
					if (pUserInfo)
					{
						pUserInfo->stUserInfo.access.SetPriv(inAccessID, pUserInfo->stSavedAccess.HasPriv(inAccessID));
						RefreshTreeItem(i);
					}
				}
			}
		}
	}
	
	return true;
}

bool CMyAdminUserTreeView::AddNewUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass)
{
DebugBreak("AddNewUser");
	if (!inName || !inName[0] || !inLogin || !inLogin[0])
		return false;

	// check access
	if (!gApp->HasGeneralPriv(myAcc_CreateUser))
	{
		gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to create new accounts.", icon_Caution, 1);
		return false;
	}

	Uint32 nIndex = GetUserLoginIndex(inLogin);
	if (nIndex)
	{
		DeselectAllTreeItems();
		SelectTreeItem(nIndex);
		MakeTreeItemVisibleInList(nIndex);
		
		Uint8 psMessage[256];
		psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "Cannot create account Ò%#sÓ because there is already an account with that login.", inLogin);

		gApp->DisplayStandardMessage("\pNew Account", psMessage, icon_Stop, 1);
		return false;
	}

	SMyAdminUserInfo *pUserInfo = (SMyAdminUserInfo *)UMemory::NewClear(sizeof(SMyAdminUserInfo));

	// set version
	pUserInfo->stUserInfo.version = TB((Uint16)kMyUserDataFileVersion);
	
	// set name and login
	pUserInfo->stUserInfo.nameSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.nameData, inName + 1, inName[0] > sizeof(pUserInfo->stUserInfo.nameData) ? sizeof(pUserInfo->stUserInfo.nameData) : inName[0]));
	pUserInfo->stUserInfo.loginSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.loginData, inLogin + 1, inLogin[0] > sizeof(pUserInfo->stUserInfo.loginData) ? sizeof(pUserInfo->stUserInfo.loginData) : inLogin[0]));
	
	// set password
	if (inPass && inPass[0])
	{
		pUserInfo->stUserInfo.passwordSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.passwordData, inPass + 1, inPass[0] > sizeof(pUserInfo->stUserInfo.passwordData) ? sizeof(pUserInfo->stUserInfo.passwordData) : inPass[0]));

		// scramble password
		Uint32 nSize = FB(pUserInfo->stUserInfo.passwordSize);
		Uint8 *pPass = pUserInfo->stUserInfo.passwordData;
		while (nSize--) { *pPass = ~(*pPass); pPass++; }
	}
	
	// set default access
	if (gApp->HasGeneralPriv(myAcc_DownloadFile))		pUserInfo->stUserInfo.access.SetPriv(myAcc_DownloadFile);
	//if (gApp->HasGeneralPriv(myAcc_DownloadFolder))		pUserInfo->stUserInfo.access.SetPriv(myAcc_DownloadFolder);
	if (gApp->HasGeneralPriv(myAcc_UploadFile))			pUserInfo->stUserInfo.access.SetPriv(myAcc_UploadFile);
	if (gApp->HasGeneralPriv(myAcc_UploadFolder))		pUserInfo->stUserInfo.access.SetPriv(myAcc_UploadFolder);
	if (gApp->HasGeneralPriv(myAcc_SendMessage))		pUserInfo->stUserInfo.access.SetPriv(myAcc_SendMessage);
	if (gApp->HasGeneralPriv(myAcc_CreateChat))			pUserInfo->stUserInfo.access.SetPriv(myAcc_CreateChat);
	if (gApp->HasGeneralPriv(myAcc_ReadChat))			pUserInfo->stUserInfo.access.SetPriv(myAcc_ReadChat);
	if (gApp->HasGeneralPriv(myAcc_SendChat))			pUserInfo->stUserInfo.access.SetPriv(myAcc_SendChat);
	if (gApp->HasGeneralPriv(myAcc_NewsReadArt))		pUserInfo->stUserInfo.access.SetPriv(myAcc_NewsReadArt);
	if (gApp->HasGeneralPriv(myAcc_NewsPostArt))		pUserInfo->stUserInfo.access.SetPriv(myAcc_NewsPostArt);
	if (gApp->HasGeneralPriv(myAcc_AnyName))			pUserInfo->stUserInfo.access.SetPriv(myAcc_AnyName);

	nIndex = AddTreeItem(1, pUserInfo, false);

	DeselectAllTreeItems();
	SelectTreeItem(nIndex);
	MakeTreeItemVisibleInList(nIndex);
	
	return true;
}

bool CMyAdminUserTreeView::ModifySelectedUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass)
{
DebugBreak("ModifySelectedUser");
	if (!inName || !inName[0] || !inLogin || !inLogin[0])
		return false;

	SMyAdminUserInfo *pUserInfo;
	Uint32 nFirstSelected = GetFirstSelectedTreeItem(&pUserInfo);
	if (nFirstSelected <= 1 || !pUserInfo)
		return false;

	// check access
	if (!pUserInfo->IsNewUser() && !gApp->HasGeneralPriv(myAcc_ModifyUser))
	{
		gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to modify account information.", icon_Caution, 1);
		return false;
	}

	Uint32 nIndex = GetUserLoginIndex(inLogin);
	if (nIndex && nIndex != nFirstSelected)
	{
		Uint8 psMessage[256];
		psMessage[0] = UText::Format(psMessage + 1, sizeof(psMessage) - 1, "Cannot rename account Ò%#sÓ because there is already an account with that login.", inLogin);

		gApp->DisplayStandardMessage("\pEdit Account", psMessage, icon_Stop, 1);
		return false;
	}

	Uint32 nLastSelected = GetLastSelectedTreeItem();
	if (nFirstSelected != nLastSelected)
		return false;
	
	// set name
	UMemory::Clear(pUserInfo->stUserInfo.nameData, sizeof(pUserInfo->stUserInfo.nameData));
	pUserInfo->stUserInfo.nameSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.nameData, inName + 1, inName[0] > sizeof(pUserInfo->stUserInfo.nameData) ? sizeof(pUserInfo->stUserInfo.nameData) : inName[0]));
		
	// set login
	UMemory::Clear(pUserInfo->stUserInfo.loginData, sizeof(pUserInfo->stUserInfo.loginData));
	pUserInfo->stUserInfo.loginSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.loginData, inLogin + 1, inLogin[0] > sizeof(pUserInfo->stUserInfo.loginData) ? sizeof(pUserInfo->stUserInfo.loginData) : inLogin[0]));
	
	// set password
	if (inPass && inPass[0])
	{
		if (inPass[0] != 1 || inPass[1] != 0)	// if the password has changed
		{
			UMemory::Clear(pUserInfo->stUserInfo.passwordData, sizeof(pUserInfo->stUserInfo.passwordData));
			pUserInfo->stUserInfo.passwordSize = TB((Uint16)UMemory::Copy(pUserInfo->stUserInfo.passwordData, inPass + 1, inPass[0] > sizeof(pUserInfo->stUserInfo.passwordData) ? sizeof(pUserInfo->stUserInfo.passwordData) : inPass[0]));

			// scramble password
			Uint32 nSize = TB(pUserInfo->stUserInfo.passwordSize);
			Uint8 *pPass = pUserInfo->stUserInfo.passwordData;
			while (nSize--) { *pPass = ~(*pPass); pPass++; }
		}
	}
	else
	{
		// clear password
		pUserInfo->stUserInfo.passwordSize = 0;
		UMemory::Clear(pUserInfo->stUserInfo.passwordData, sizeof(pUserInfo->stUserInfo.passwordData));
	}

	RefreshTreeItem(nFirstSelected);

	return true;
}

bool CMyAdminUserTreeView::DeleteSelectedUsers()
{
DebugBreak("DeleteSelectedUsers");
	// get selected user
	SMyAdminUserInfo *pUserInfo;
	Uint32 nFirstSelected = GetFirstSelectedTreeItem(&pUserInfo);
	if (nFirstSelected <= 1 || !pUserInfo)
		return false;

	Uint32 nLastSelected = GetLastSelectedTreeItem();

	// check access
	if (!gApp->HasGeneralPriv(myAcc_DeleteUser))
	{
		// check if new user				
		if (!pUserInfo->IsNewUser())
		{
			gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to delete accounts.", icon_Caution, 1);
			return false;
		}
		else if (nFirstSelected != nLastSelected)	// two or more items selected
		{
			for (Uint32 i = nFirstSelected + 1; i <= nLastSelected; i++)
			{
				if (IsSelected(i))
				{
					SMyAdminUserInfo *pCheckUserInfo = GetTreeItem(i);

					// check if new user				
					if (pCheckUserInfo && !pCheckUserInfo->IsNewUser())						
					{
						gApp->DisplayStandardMessage("\pServer Message", "\pYou are not allowed to delete accounts.", icon_Caution, 1);
						return false;
					}
				}
			}
		}
	}
	
	if (nFirstSelected == nLastSelected)	// one item selected
	{
		// grab a copy of the login
		Uint8 psLogin[32];
		psLogin[0] = UMemory::Copy(psLogin + 1, pUserInfo->stUserInfo.loginData, TB(pUserInfo->stUserInfo.loginSize) > sizeof(psLogin) - 1 ? sizeof(psLogin) - 1 : TB(pUserInfo->stUserInfo.loginSize));

		// ask for confirmation
		if (MsgBox(144, psLogin) != 1)
			return false;	
	}
	else									// two or more items selected
	{
		SMsgBox mb;
		ClearStruct(mb);

		Uint8 *psMessage = "\pDelete selected accounts and all associated files?";

		mb.icon = icon_Caution;
		mb.sound = 1;
		mb.title = "\pDelete Accounts";	
		mb.messageSize = psMessage[0];
		mb.messageData = psMessage + 1;
		mb.button1 = "\pDelete";
		mb.button2 = "\pCancel";
			
		if (MsgBox(mb) != 1)
			return false;
	}
	
	Uint32 nSavedSelected;
	do
	{
		// mark user as deleted
		AddDeleteUser(pUserInfo);
	
		// remove from tree and dispose
		RemoveTreeItem(nFirstSelected);
		UMemory::Dispose((TPtr)pUserInfo);	

		// get next selected user
		nSavedSelected = nFirstSelected;
		nFirstSelected = GetFirstSelectedTreeItem(&pUserInfo);
		
	} while (nFirstSelected && pUserInfo);
	
	// select next user
	Uint32 nTreeCount = GetTreeCount();
	if (nSavedSelected > nTreeCount)
		nSavedSelected = nTreeCount;
	
	DeselectAllTreeItems();
	SelectTreeItem(nSavedSelected);
	MakeTreeItemVisibleInList(nSavedSelected);

	return true;
}

bool CMyAdminUserTreeView::GetSelectedUser(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass, bool *outIsNewUser)
{
DebugBreak("GetSelectedUser");
	SMyAdminUserInfo *pUserInfo;
	Uint32 nFirstSelected = GetFirstSelectedTreeItem(&pUserInfo);
	if (nFirstSelected <= 1 || !pUserInfo)
		return false;

	Uint32 nLastSelected = GetLastSelectedTreeItem();
	if (nFirstSelected != nLastSelected)
		return false;

	if (outName)
		outName[0] = UMemory::Copy(outName + 1, pUserInfo->stUserInfo.nameData, TB(pUserInfo->stUserInfo.nameSize) > 31 ? 31 : TB(pUserInfo->stUserInfo.nameSize));

	if (outLogin)
		outLogin[0] = UMemory::Copy(outLogin + 1, pUserInfo->stUserInfo.loginData, TB(pUserInfo->stUserInfo.loginSize) > 31 ? 31 : TB(pUserInfo->stUserInfo.loginSize));

	if (outPass)
		outPass[0] = UMemory::Copy(outPass + 1, pUserInfo->stUserInfo.passwordData, TB(pUserInfo->stUserInfo.passwordSize) > 31 ? 31 : TB(pUserInfo->stUserInfo.passwordSize));
	
	if (outIsNewUser)
		*outIsNewUser = pUserInfo->IsNewUser();
	
	return true;
}

Uint32 CMyAdminUserTreeView::GetUserLoginIndex(const Uint8 *inLogin)
{
DebugBreak("GetUserLoginIndex");
	if (!inLogin || !inLogin[0])
		return false;

	Uint32 i = 1;	// skip first item
	SMyAdminUserInfo *pUserInfo;
	
	while (GetNextTreeItem(pUserInfo, i))
	{
		if (!UText::CompareInsensitive(inLogin + 1, inLogin[0], pUserInfo->stUserInfo.loginData, TB(pUserInfo->stUserInfo.loginSize)))
			return i;
	}
	
	return 0;
}

Int32 CMyAdminUserTreeView::CompareLogins(void *inItemA, void *inItemB, void *inRef)
{
DebugBreak("CompareLogins");
	#pragma unused(inRef)
	
	if (!inItemA || !inItemB)
		return 0;
	
	Uint8 *pLoginA = ((SMyAdminUserInfo *)inItemA)->stUserInfo.loginData;
	Uint16 nSizeA = ((SMyAdminUserInfo *)inItemA)->stUserInfo.loginSize;

	Uint8 *pLoginB = ((SMyAdminUserInfo *)inItemB)->stUserInfo.loginData;
	Uint16 nSizeB = ((SMyAdminUserInfo *)inItemB)->stUserInfo.loginSize;

	Int32 nOutVal = UText::CompareInsensitive(pLoginA, pLoginB, min(nSizeA, nSizeB));
	if (nOutVal == 0 && nSizeA != nSizeB)
		nOutVal = nSizeA > nSizeB ? 1 : -1;
	
	return nOutVal;
}

void CMyAdminUserTreeView::AddDeleteUser(SMyAdminUserInfo *inUserInfo)
{
DebugBreak("AddDeleteUser");
	if (!inUserInfo)
		return;

	// check if new user
	if (!inUserInfo->IsNewUser())
	{
		SMyUserDataFile *pUserInfo = (SMyUserDataFile *)UMemory::New(sizeof(SMyUserDataFile));
		UMemory::Copy(pUserInfo, &inUserInfo->stSavedUserInfo, sizeof(SMyUserDataFile));
		
		mDeleteUserList.AddItem(pUserInfo);
	}
}

void CMyAdminUserTreeView::SaveDeleteUserList(TFieldData& inData)
{
DebugBreak("SaveDeleteUserList");
	Uint8 str[256];
	Uint8 *p;
	Uint32 s;

	Uint32 i = 0;
	SMyUserDataFile *pUserInfo;
	
	// send deleted user
	while (mDeleteUserList.GetNext(pUserInfo, i))
	{
		// store user data
		StFieldData stUserData;

		// scramble and store saved login
		str[0] = UMemory::Copy(str + 1, pUserInfo->loginData, TB(pUserInfo->loginSize) > sizeof(str) - 1 ? sizeof(str) - 1 : TB(pUserInfo->loginSize));
		s = str[0];
		p = str + 1;
		while (s--) { *p = ~(*p); p++; }
		stUserData->AddField(myField_Data, str + 1, str[0]);

		THdl hUserData = stUserData->GetDataHandle();
		if (!hUserData)
			continue;
				
		Uint32 nDataSize = UMemory::GetSize(hUserData);
		Uint8 *pUserData = UMemory::Lock(hUserData);
		inData->AddField(myField_Data, pUserData, nDataSize);
		UMemory::Unlock(hUserData);
	}

	ClearDeleteUserList();
}

void CMyAdminUserTreeView::RevertDeleteUserList()
{
DebugBreak("RevertDeleteUserList");
	Uint32 i = 0;
	SMyUserDataFile *pUserInfo;
	
	while (mDeleteUserList.GetNext(pUserInfo, i))
	{
		SMyAdminUserInfo *pAdminUserInfo = (SMyAdminUserInfo *)UMemory::NewClear(sizeof(SMyAdminUserInfo));
		
		UMemory::Copy(&pAdminUserInfo->stUserInfo, pUserInfo, sizeof(SMyUserDataFile));
		pAdminUserInfo->SaveUser();
		
		AddTreeItem(1, pAdminUserInfo, false);
	}

	ClearDeleteUserList();
}

void CMyAdminUserTreeView::ClearDeleteUserList()
{
DebugBreak("ClearDeleteUserList");

	Uint32 i = 0;
	SMyUserDataFile *pUserInfo;
	
	while (mDeleteUserList.GetNext(pUserInfo, i))
		UMemory::Dispose((TPtr)pUserInfo);
	
	mDeleteUserList.Clear();
}

void CMyAdminUserTreeView::SetUsersAccess(Uint32 inTreeIndex, bool inIsSelected)
{
DebugBreak("SetUsersAccess");

	if (!mAdminWin)
		return;

	bool bEnableUserAccess = true;
	SMyAdminUsersAccess stUsersAccess;
	stUsersAccess.Clear();

	if (inIsSelected && inTreeIndex == 1)		// first item selected
	{
		if (GetChildTreeCount(1))
		{
			Uint32 i = 1;	// skip first item
			SMyAdminUserInfo *pUserInfo;
	
			while (GetNextTreeItem(pUserInfo, i))
			{
				// check access
				if (!gApp->HasGeneralPriv(myAcc_ModifyUser))
				{
					if (!pUserInfo->IsNewUser())
						bEnableUserAccess = false;
				}
			
				pUserInfo->stSavedAccess = pUserInfo->stUserInfo.access;
				for (Uint32 j = 0; j < sizeof(stUsersAccess.data); j++)
				{		
					if (i == 2)
						stUsersAccess.SetPriv(j, pUserInfo->stUserInfo.access.HasPriv(j));
					else
					{
						Uint8 nPriv = stUsersAccess.HasPriv(j);
						if (nPriv != 2)
						{	
							if (pUserInfo->stUserInfo.access.HasPriv(j))
							{	 
								if (nPriv != 1) 
									stUsersAccess.SetPriv(j, 2);	
							}
							else if (nPriv != 0) 
								stUsersAccess.SetPriv(j, 2);
						}
					}				
				}
			}
		}
		else
			bEnableUserAccess = false;

		mAdminWin->DisableUserButtons();
		mAdminWin->SetUsersAccess(stUsersAccess);
	}
	else										// one or more items selected
	{
		Uint32 nFirstSelected = GetFirstSelectedTreeItem();
		if (nFirstSelected <= 1)
			return;

		SMyAdminUserInfo *pUserInfo = GetTreeItem(nFirstSelected);
		if (!pUserInfo)
			return;
	
		// check access
		if (!gApp->HasGeneralPriv(myAcc_ModifyUser))
		{
			if (!pUserInfo->IsNewUser())
				bEnableUserAccess = false;
		}
	
		pUserInfo->stSavedAccess = pUserInfo->stUserInfo.access;
		for (Uint32 j = 0; j < sizeof(stUsersAccess.data); j++)
			stUsersAccess.SetPriv(j, pUserInfo->stUserInfo.access.HasPriv(j));

		Uint32 nLastSelected = GetLastSelectedTreeItem();

		if (nFirstSelected == nLastSelected)	// one item selected
		{		
			mAdminWin->EnableUserButtons();
			mAdminWin->SetUsersAccess(stUsersAccess);
		}
		else									// two or more items selected
		{				
			for (Uint32 i = nFirstSelected + 1; i <= nLastSelected; i++)
			{
				if (IsSelected(i))
				{
					pUserInfo = GetTreeItem(i);
					if (pUserInfo)
					{
						// check access
						if (!gApp->HasGeneralPriv(myAcc_ModifyUser))
						{
							if (!pUserInfo->IsNewUser())
								bEnableUserAccess = false;
						}
	
						pUserInfo->stSavedAccess = pUserInfo->stUserInfo.access;
						for (Uint32 j = 0; j < sizeof(stUsersAccess.data); j++)
						{
							Uint8 nPriv = stUsersAccess.HasPriv(j);
							if (nPriv != 2)
							{	
								if (pUserInfo->stUserInfo.access.HasPriv(j))
								{	 
									if (nPriv != 1) 
										stUsersAccess.SetPriv(j, 2);	
								}
								else if (nPriv != 0) 
									stUsersAccess.SetPriv(j, 2);
							}
						}
					}
				}
			}
					
			mAdminWin->DisableUserButtons(false);
			mAdminWin->SetUsersAccess(stUsersAccess);
		}		
	}
	
	mAdminWin->SetEnableUserAccess(bEnableUserAccess);
}

void CMyAdminUserTreeView::SelectionChanged(Uint32 inTreeIndex, SMyAdminUserInfo *inTreeItem, bool inIsSelected)
{
	DebugBreak("SelectionChanged");
	if (!inTreeItem)
		return;
	
	Uint32 nLastSelected = GetLastSelectedTreeItem();
	if (inIsSelected && IsSelected(1) && nLastSelected != 1)	// deselect first item
	{
		SelectTreeItem(1, false);
		return;
	}

	SetUsersAccess(inTreeIndex, inIsSelected);
}

void CMyAdminUserTreeView::DisclosureChanged(Uint32 inTreeIndex, SMyAdminUserInfo *inTreeItem, Uint8 inDisclosure)
{
DebugBreak("DisclosureChanged");

	#pragma unused(inTreeItem, inDisclosure)
	
	if (inTreeIndex == 1)
	{
		DeselectAllTreeItems();
		SelectTreeItem(inTreeIndex);
		MakeTreeItemVisibleInList(inTreeIndex);
	}
}

void CMyAdminUserTreeView::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyAdminUserInfo *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
DebugBreak("ItemDraw");
	#pragma unused(inTreeIndex, inOptions)
	
	SRect stRect;
	SColor stTextCol;
	
	// get info
	bool bIsActive = IsFocus() && mIsEnabled && inTreeViewItem->bIsSelected;
	
	// set font
	inImage->SetFont(kDefaultFont, nil, 9);
	inImage->SetFontEffect(fontEffect_Bold);
	
	// set color
	UUserInterface::GetSysColor(bIsActive ? sysColor_InverseHighlight : sysColor_Label, stTextCol);
	inImage->SetInkColor(stTextCol);

	// draw icon and login
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += 10;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
		{
			if (inTreeLevel == 1)			
				mAllUsersIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			else if (inTreeItem->stUserInfo.access.HasAllPriv())
			{
				if (inTreeItem->IsNewUser())
					mNewAllPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mAllPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			else if (inTreeItem->stUserInfo.access.HasPriv(myAcc_DisconUser))
			{
				if (inTreeItem->IsNewUser())
					mNewDisconnectPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mDisconnectPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			else if (inTreeItem->stUserInfo.access.HasNoPriv())
			{
				if (inTreeItem->IsNewUser())
					mNewNoPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mNoPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			else
			{
				if (inTreeItem->IsNewUser())
					mNewRegularPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mRegularPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
		}
		
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 36;

		if (inTreeLevel == 1)
		{
			pBounds = inTabRectList.GetItem(2);
			if (pBounds)
				stRect.right = pBounds->right;
		}

		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
	
		// draw login
		inImage->DrawTruncText(stRect, inTreeItem->stUserInfo.loginData, TB(inTreeItem->stUserInfo.loginSize), 0, align_Left + align_CenterVert);
	}
		
	// draw name
	if (inTreeLevel != 1)
	{
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
			
			inImage->DrawTruncText(stRect, inTreeItem->stUserInfo.nameData, TB(inTreeItem->stUserInfo.nameSize), 0, align_Left + align_CenterVert);
		}
	}
}


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyAdminAccessCheckBoxView::CMyAdminAccessCheckBoxView(CViewHandler *inHandler, const SRect& inBounds, CMyAdminUserAccessView *inUserAccessView)
	: CCheckBoxView(inHandler, inBounds)
{
	mAccessID = 0;
	mInitialMark = 0;

	mUserAccessView = inUserAccessView;
}

void CMyAdminAccessCheckBoxView::MouseUp(const SMouseMsgData& inInfo)
{
	CCheckBoxView::MouseUp(inInfo);
	
	if (IsMouseWithin())
		SetNextMarkValue();
}

bool CMyAdminAccessCheckBoxView::KeyDown(const SKeyMsgData& inInfo)
{
	if (CCheckBoxView::KeyDown(inInfo))
	{
		SetNextMarkValue();
		return true;
	}
	
	return false;
}

void CMyAdminAccessCheckBoxView::SetNextMarkValue()
{
	if (mMark >= 2)
		SetMark(0);
	else if (mMark == 1)
	{
		if (mInitialMark >= 2)
			SetMark(2);
		else
			SetMark(0);
	}
	else
		SetMark(1);
	
	if (mUserAccessView)
	{
		if (mMark >= 2)
			mUserAccessView->RestoreUserAccess(mAccessID);
		else
			mUserAccessView->UpdateUserAccess(mAccessID);
	}
	
	Hit(hitType_Change);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyAdminUserAccessView::CMyAdminUserAccessView(CViewHandler *inHandler, const SRect& inBounds, CMyAdminWin *inAdminWin)
	: CScrollerView(inHandler, inBounds)
{
	mAdminWin = inAdminWin;

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
		{ 2, myAcc_ViewDropBoxes, "\pCan View Drop Boxes" },		// 14
		{ 2, myAcc_ViewOwnDropBox, "\pCan View Own Drop Box" },		// 14
		{ 2, myAcc_MakeAlias, "\pCan Make Aliases" },				// 15

		{ 1, nil, "\pUser Maintenance" },
		{ 2, myAcc_CreateUser, "\pCan Create Accounts" },			// 16
		{ 2, myAcc_DeleteUser, "\pCan Delete Accounts" },			// 17
		{ 2, myAcc_OpenUser, "\pCan Read Accounts" },				// 18
		{ 2, myAcc_ModifyUser, "\pCan Modify Accounts" },
		{ 2, myAcc_AdmInSpector, "\pCan Use AdmInSpector" },			// 19
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
		{ 2, myAcc_ChangeIcon, "\pCan Change User Icon" },
		{ 2, myAcc_BlockDownload, "\pCan Block an User Download" },
		{ 2, myAcc_Visible, "\pCan Set User Invisible" }		// 43
		
		// remember to update size of mAccessChecks if we add checks to this
	};
	
	Uint32 nWidth = this->GetVisibleContentWidth();
	CContainerView *vc = new CContainerView(this, SRect(0,0,nWidth,500));
	vc->SetSizing(sizing_RightSticky);
	vc->Show();
	
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
					r.right = nWidth - 6;
					CBoxView *box = new CBoxView(vc, r);
					box->SetStyle(boxStyle_Sunken);
					box->SetSizing(sizing_RightSticky);
					box->Show();

					r.top = top + 1;
					r.left = 8;
					r.bottom = r.top + 16;
					r.right = nWidth - 10;
					CLabelView *lbl = new CLabelView(vc, r);
					lbl->SetText(p->name);
					lbl->SetSizing(sizing_RightSticky);
					lbl->Show();
					
					top += 20;
					break;
					
				case 2:
					r.top = top;
					r.left = 16;
					r.bottom = r.top + 18;
					r.right = nWidth - 10;
					CMyAdminAccessCheckBoxView *chk = new CMyAdminAccessCheckBoxView(vc, r, this);
					chk->SetTitle(p->name);
					chk->SetAccessID(p->accessID);
					chk->SetAutoMark(false);
					chk->SetStyle(kTickBox);
					chk->SetSizing(sizing_RightSticky);

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
		
		r.top = r.left = 0;
		r.bottom = top + 6;
		r.right = nWidth;
		vc->SetBounds(r);
	}
}

CMyAdminUserAccessView::~CMyAdminUserAccessView()
{
}

void CMyAdminUserAccessView::SetUsersAccess(const SMyAdminUsersAccess& inUsersAccess)
{
	Uint32 nSize = sizeof(mAccessChecks) / sizeof(CMyAdminAccessCheckBoxView*);
	
	while (nSize--)
	{
		mAccessChecks[nSize]->SetMark(inUsersAccess.HasPriv(mAccessChecks[nSize]->GetAccessID()));
		mAccessChecks[nSize]->SetInitialMark();
	}
}

void CMyAdminUserAccessView::GetUserAccess(SMyUserAccess& outUserAccess, Uint32 inAccessID)
{
	Uint32 nSize = sizeof(mAccessChecks) / sizeof(CMyAdminAccessCheckBoxView*);
	
	while (nSize--)
	{
		if (inAccessID == mAccessChecks[nSize]->GetAccessID())
		{
			Uint16 nMark = mAccessChecks[nSize]->GetMark();
			if (nMark < 2)
				outUserAccess.SetPriv(inAccessID, nMark);
			
			break;
		}
	}
}

void CMyAdminUserAccessView::SetEnableUserAccess(bool inEnable)
{
	Uint32 nSize = sizeof(mAccessChecks) / sizeof(CMyAdminAccessCheckBoxView*);
	
	while (nSize--)
	{
		if (inEnable)
		{
			Uint32 nAccessID = mAccessChecks[nSize]->GetAccessID();
			if (gApp->HasGeneralPriv(nAccessID) || nAccessID == myAcc_NoAgreement)
				mAccessChecks[nSize]->Enable();
		}
		else
			mAccessChecks[nSize]->Disable();
	}
}

bool CMyAdminUserAccessView::UpdateUserAccess(Uint32 inAccessID)
{
	if (!mAdminWin)
		return false;
	
	return mAdminWin->UpdateUserAccess(inAccessID);		
}

bool CMyAdminUserAccessView::RestoreUserAccess(Uint32 inAccessID)
{
	if (!mAdminWin)
		return false;
		
	return mAdminWin->RestoreUserAccess(inAccessID);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
	
bool SMyAdminUserInfo::IsNewUser()
{
	if (stSavedUserInfo.loginData && stSavedUserInfo.loginSize)
		return false;
		
	return true;
}
	
bool SMyAdminUserInfo::IsModifiedUser()
{
	return UMemory::Compare(&stUserInfo, &stSavedUserInfo, sizeof(SMyUserDataFile));
}

bool SMyAdminUserInfo::IsModifiedLogin()
{
	if (IsNewUser())
		return false;

	return UMemory::Compare(stUserInfo.loginData, TB(stUserInfo.loginSize), stSavedUserInfo.loginData, TB(stSavedUserInfo.loginSize));
}

void SMyAdminUserInfo::SaveUser()
{
	if (IsModifiedUser())
		UMemory::Copy(&stSavedUserInfo, &stUserInfo, sizeof(SMyUserDataFile));
}

void SMyAdminUserInfo::RevertUser()
{
	if (IsModifiedUser())
		UMemory::Copy(&stUserInfo, &stSavedUserInfo, sizeof(SMyUserDataFile));
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
CMyAdmInSpectorTreeView::CMyAdmInSpectorTreeView(CViewHandler *inHandler, const SRect& inBounds, CMyAdmInSpectorWin *inAdmInSpectorWin)
	: CTabbedTreeView(inHandler, inBounds, 240, 241)
{
	DebugBreak("treeview adminspector");
	mAdmInSpectorWin = inAdmInSpectorWin;
/*
	mAllUsersIcon = UIcon::Load(200);

	mNoPrivIcon = UIcon::Load(190);
	mAllPrivIcon = UIcon::Load(191);
	mDisconnectPrivIcon = UIcon::Load(192);
	mRegularPrivIcon = UIcon::Load(193);

	mNewNoPrivIcon = UIcon::Load(250);
	mNewAllPrivIcon = UIcon::Load(251);
	mNewDisconnectPrivIcon = UIcon::Load(252);
	mNewRegularPrivIcon = UIcon::Load(253);
	*/

#if WIN32
	mTabHeight = 18;
#endif

	// set headers
	AddTab("\pLogin", 100, 100);
	AddTab("\pName");
	SetTabs(60, 40);	
}
CMyAdmInSpectorTreeView::~CMyAdmInSpectorTreeView()
{

	ClearUserList();
	/*	
	UIcon::Release(mAllUsersIcon);

	UIcon::Release(mNoPrivIcon);
	UIcon::Release(mAllPrivIcon);
	UIcon::Release(mDisconnectPrivIcon);
	UIcon::Release(mRegularPrivIcon);

	UIcon::Release(mNewNoPrivIcon);
	UIcon::Release(mNewAllPrivIcon);
	UIcon::Release(mNewDisconnectPrivIcon);
	UIcon::Release(mNewRegularPrivIcon);
	*/
}

void CMyAdmInSpectorTreeView::ClearUserList()
{
DebugBreak("ClearUserList");
	ClearDeleteUserList();

	Uint32 i = 0;
	SMyUserListItem *pUserInfo;
	
	while (GetNextTreeItem(pUserInfo, i))
		UMemory::Dispose((TPtr)pUserInfo);
	
	ClearTree();	
}

void CMyAdmInSpectorTreeView::ClearDeleteUserList()
{
DebugBreak("ClearDeleteUserList");

	Uint32 i = 0;
	SMyUserListItem *pUserInfo;
	
	while (mAdmInSpectorList.GetNext(pUserInfo, i))
		UMemory::Dispose((TPtr)pUserInfo);
	
	mAdmInSpectorList.Clear();
}


Uint32 CMyAdmInSpectorTreeView::GetItemCount() const
{
	//return mAdmInSpectorList.GetItemCount();
}



void CMyAdmInSpectorTreeView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
DebugBreak("SetTabs");
	if (!inTabPercent1 && !inTabPercent2)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyAdmInSpectorTreeView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
DebugBreak("GetTabs");
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}


bool CMyAdmInSpectorTreeView::AddListFromFields(TFieldData inData)
{
ClearUserList();
#pragma options align=packed
	struct {
		SMyUserInfo info;
		Uint8 data[256];
	} infoBuf;
#pragma options align=reset
	SMyUserInfo& info = infoBuf.info;
	SMyUserListItem *item;
	Uint16 i, n;
	Uint32 s, startCount, addCount;

	//startCount = mUserList.GetItemCount(); // ca donne 0 on va le fixer
	startCount = 0;
	
	addCount = 0;
	n = inData->GetFieldCount();
	
	
	
	for (i=1; i<=n; i++)
	{
	
		if (inData->GetFieldID(i) == myField_UserNameWithInfo) // la on recoit du serveur la liste
		{
		
			inData->GetFieldByIndex(i, &info, sizeof(infoBuf));
			
			s = FB(info.nameSize);
			if (s > 63) s = 63;
			
			/////////////////
			
			
			///////////////////
			item = (SMyUserListItem *)UMemory::New(sizeof(SMyUserListItem) + s + 1);
			item->icon = nil;
			
			try
			{
			
				//item->icon = UIcon::Load(FB((Uint16)info.iconID) == 0 ? 414 : FB((Uint16)info.iconID));
				
				item->id = FB(info.id);
				item->flags = FB(info.flags);
				item->name[0] = UMemory::Copy(item->name + 1, info.nameData, s);
				//DebugBreak("ok");
				DebugBreak("on addtreeitem %s",item->name);
				DebugBreak("on addtreeitem %i",item->id);
				//DebugBreak("%i",item->icon);
				//AddTreeItem(1, item, false);
				//mAdmInSpectorList.AddItem(item);
				//SMyUserListItem *item2;
				//mAdmInSpectorList.AddItem(item);
				AddTreeItem(1, item, false);
				
				
			}
			catch(...)
			{
			
				UIcon::Release(item->icon);
				UMemory::Dispose((TPtr)item);
				throw;
			}
			
			addCount++;
			
		}
		// sort the list
		//Sort(1, CompareLogins);
	}

	DebugBreak("la on va essayer de Disclose");
	Disclose(1);
	DeselectAllTreeItems();
	SelectTreeItem(1);
	MakeTreeItemVisibleInList(1);

	
	return true;
	
}
void CMyAdmInSpectorTreeView::DisclosureChanged(Uint32 inTreeIndex, SMyUserListItem *inTreeItem, Uint8 inDisclosure)
{
DebugBreak("DisclosureChanged");

	#pragma unused(inTreeItem, inDisclosure)
	
	if (inTreeIndex == 1)
	{
		DeselectAllTreeItems();
		SelectTreeItem(inTreeIndex);
		MakeTreeItemVisibleInList(inTreeIndex);
	}
}

Int32 CMyAdmInSpectorTreeView::CompareLogins(void *inItemA, void *inItemB, void *inRef)
{
DebugBreak("CompareLogins");
	#pragma unused(inRef)
	
	if (!inItemA || !inItemB)
		return 0;
	
	Uint8 *pLoginA = ((SMyAdminUserInfo *)inItemA)->stUserInfo.loginData;
	Uint16 nSizeA = ((SMyAdminUserInfo *)inItemA)->stUserInfo.loginSize;

	Uint8 *pLoginB = ((SMyAdminUserInfo *)inItemB)->stUserInfo.loginData;
	Uint16 nSizeB = ((SMyAdminUserInfo *)inItemB)->stUserInfo.loginSize;

	Int32 nOutVal = UText::CompareInsensitive(pLoginA, pLoginB, min(nSizeA, nSizeB));
	if (nOutVal == 0 && nSizeA != nSizeB)
		nOutVal = nSizeA > nSizeB ? 1 : -1;
	
	return nOutVal;
}

void CMyAdmInSpectorTreeView::SelectionChanged(Uint32 inTreeIndex, SMyUserListItem *inTreeItem, bool inIsSelected)
{
	DebugBreak("SelectionChanged");
	if (!inTreeItem)
		return;
	
	Uint32 nLastSelected = GetLastSelectedTreeItem();
	if (inIsSelected && IsSelected(1) && nLastSelected != 1)	// deselect first item
	{
		SelectTreeItem(1, false);
		return;
	}

	//SetUsersAccess(inTreeIndex, inIsSelected);
	
}
void CMyAdmInSpectorTreeView::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyUserListItem *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inTreeIndex, inOptions)
	DebugBreak("Draw");
	SRect stRect;
	SColor stTextCol;
	
	// get info
	bool bIsActive = IsFocus() && mIsEnabled && inTreeViewItem->bIsSelected;
	
	// set font
	inImage->SetFont(kDefaultFont, nil, 9);
	inImage->SetFontEffect(fontEffect_Bold);
	
	// set color
	UUserInterface::GetSysColor(bIsActive ? sysColor_InverseHighlight : sysColor_Label, stTextCol);
	inImage->SetInkColor(stTextCol);

	// draw icon and login
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += 10;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
		{
		/*
			if (inTreeLevel == 1)			
				mAllUsersIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			else if (inTreeItem->stUserInfo.access.HasAllPriv())
			{
				if (inTreeItem->IsNewUser())
					mNewAllPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mAllPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			else if (inTreeItem->stUserInfo.access.HasPriv(myAcc_DisconUser))
			{
				if (inTreeItem->IsNewUser())
					mNewDisconnectPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mDisconnectPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			else if (inTreeItem->stUserInfo.access.HasNoPriv())
			{
				if (inTreeItem->IsNewUser())
					mNewNoPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mNoPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			else
			{
				if (inTreeItem->IsNewUser())
					mNewRegularPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
				else
					mRegularPrivIcon->Draw(inImage, stRect, align_Center, bIsActive ? transform_Dark : transform_None);
			}
			*/
		}
		
		
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 36;

		if (inTreeLevel == 1)
		{
			pBounds = inTabRectList.GetItem(2);
			if (pBounds)
				stRect.right = pBounds->right;
		}

		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
	
		// draw login
		/*
		item->id = FB(info.id);
				item->flags = FB(info.flags);
				item->name[0] = UMemory::Copy(item->name + 1, info.nameData, s);
				*/
				DebugBreak("draw: %s", inTreeItem->name);
				
		inImage->DrawTruncText(stRect, inTreeItem->name, TB(inTreeItem->name[0]), 0, align_Left + align_CenterVert);
	}
		
	// draw name
	/*
	if (inTreeLevel != 1)
	{
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
			
			inImage->DrawTruncText(stRect, inTreeItem->stUserInfo.nameData, TB(inTreeItem->stUserInfo.nameSize), 0, align_Left + align_CenterVert);
		}
	}
	*/
}

