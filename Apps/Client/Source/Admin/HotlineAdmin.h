/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once

#include "Hotline.h"


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark ¥¥ Structures ¥¥

// SMyAdminUsersAccess
#pragma mark SMyAdminUsersAccess
#pragma options align=packed
struct SMyAdminUsersAccess {
	Uint8 data[64];
	
	void Clear()										{	UMemory::Clear(data, sizeof(data));			}
	void Fill(Uint8 inValue)							{	UMemory::Fill(data, sizeof(data), inValue);	}
	Uint8 HasPriv(Uint32 inPriv)	const				{	return data[inPriv];						}
	void SetPriv(Uint32 inPriv, Uint8 inValue)			{	data[inPriv] = inValue;						}
};
#pragma options align=reset

// SMyAdminUserInfo
#pragma mark SMyAdminUserInfo
struct SMyAdminUserInfo {
	SMyUserDataFile stUserInfo;
	SMyUserDataFile stSavedUserInfo;
	SMyUserAccess stSavedAccess;
	
	bool IsNewUser();
	bool IsModifiedUser();
	bool IsModifiedLogin();

	void SaveUser();
	void RevertUser();
};

// SMyAdmInSpectorInfo
#pragma mark SMyUserListItem
struct SMyUserListItem {
		TIcon icon;
		Uint16 id;
		Uint16 flags;
		Uint8 name[];
	
};

// SMyAdminBoundsInfo
#pragma mark SMyAdminBoundsInfo
struct SMyAdminBoundsInfo {
	SRect stAdminBounds;
	Uint8 nAdminPane;
	Uint8 nAdminTab1, nAdminTab2;
};
// SMyAdmInSpectorBoundsInfo
#pragma mark SMyAdmInSpectorBoundsInfo
struct SMyAdmInSpectorBoundsInfo {
	SRect stAdmInSpectorBounds;
	Uint8 nAdmInSpectorPane;
	Uint8 nAdmInSpectorTab1, nAdmInSpectorTab2;
};

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
#pragma mark ¥¥ Views ¥¥

#pragma mark CMyAdminUserTreeView
class CMyAdminWin;
class CMyAdminUserTreeView : public CTabbedTreeView<SMyAdminUserInfo>
{
	public:
		CMyAdminUserTreeView(CViewHandler *inHandler, const SRect& inBounds, CMyAdminWin *inAdminWin);
		virtual ~CMyAdminUserTreeView();
		
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);

		bool AddListFromFields(TFieldData inData);
		TFieldData GetFieldsFromList();
		void RevertUserList();
		void ClearUserList();
		bool GetUserListStatus();
	
		bool UpdateUserAccess(Uint32 inAccessID);
		bool RestoreUserAccess(Uint32 inAccessID);
	
		bool AddNewUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool ModifySelectedUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool DeleteSelectedUsers();
		bool GetSelectedUser(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass, bool *outIsNewUser);

		// tree events
		virtual void SelectionChanged(Uint32 inTreeIndex, SMyAdminUserInfo *inTreeItem, bool inIsSelected);
		virtual void DisclosureChanged(Uint32 inTreeIndex, SMyAdminUserInfo *inTreeItem, Uint8 inDisclosure);
		
	protected:
		TIcon mAllUsersIcon;
		TIcon mNoPrivIcon, mAllPrivIcon, mDisconnectPrivIcon, mRegularPrivIcon; 
		TIcon mNewNoPrivIcon, mNewAllPrivIcon, mNewDisconnectPrivIcon, mNewRegularPrivIcon; 

		CMyAdminWin *mAdminWin;
		CPtrList<SMyUserDataFile> mDeleteUserList;

		Uint32 GetUserLoginIndex(const Uint8 *inLogin);
		static Int32 CompareLogins(void *inItemA, void *inItemB, void *inRef);

		void AddDeleteUser(SMyAdminUserInfo *inUserInfo);
		void SaveDeleteUserList(TFieldData& inData);
		void RevertDeleteUserList();
		void ClearDeleteUserList();

		void SetUsersAccess(Uint32 inTreeIndex, bool inIsSelected);
		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyAdminUserInfo *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);
};

#pragma mark CMyAdminAccessCheckBoxView
class CMyAdminUserAccessView;
class CMyAdminAccessCheckBoxView : public CCheckBoxView
{
	public:
		CMyAdminAccessCheckBoxView(CViewHandler *inHandler, const SRect& inBounds, CMyAdminUserAccessView *inUserAccessView);
		
		void SetAccessID(Uint32 inID)			{	mAccessID = inID;		}
		Uint32 GetAccessID()					{	return mAccessID;		}
		void SetInitialMark()					{	mInitialMark = mMark;	}
		
		virtual void MouseUp(const SMouseMsgData& inInfo);
		virtual bool KeyDown(const SKeyMsgData& inInfo);

	protected:
		Uint32 mAccessID;
		Uint16 mInitialMark;

		CMyAdminUserAccessView *mUserAccessView;
		
		void SetNextMarkValue();
};

#pragma mark CMyAdminUserAccessView
class CMyAdminUserAccessView : public CScrollerView
{
	public:
		CMyAdminUserAccessView(CViewHandler *inHandler, const SRect& inBounds, CMyAdminWin *inAdminWin);
		virtual ~CMyAdminUserAccessView();
		
		void SetUsersAccess(const SMyAdminUsersAccess& inUsersAccess);
		void GetUserAccess(SMyUserAccess& outUserAccess, Uint32 inAccessID);
		void SetEnableUserAccess(bool inEnable);
		bool UpdateUserAccess(Uint32 inAccessID);
		bool RestoreUserAccess(Uint32 inAccessID);
	
	protected:
		CMyAdminAccessCheckBoxView *mAccessChecks[51];

		CMyAdminWin *mAdminWin;
		//CMyAdmInSpectorWin *mAdmInSpectorWin;
};
#pragma mark CMyAdmInSpectorTreeView
class CMyAdmInSpectorWin;
class CMyAdmInSpectorTreeView : public CTabbedTreeView<SMyUserListItem>
//class CMyAdmInSpectorTreeView : public CTabbedTreeView<SMyAdminUserInfo>
{
	public:
		CMyAdmInSpectorTreeView(CViewHandler *inHandler, const SRect& inBounds, CMyAdmInSpectorWin *inAdmInSpectorWin);
		virtual ~CMyAdmInSpectorTreeView();
		
		void SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2);
		void GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2);

		virtual Uint32 GetItemCount() const;
		bool AddListFromFields(TFieldData inData);
		TFieldData GetFieldsFromList();
		void RevertUserList();
		void ClearUserList();
		bool GetUserListStatus();
	
		//bool UpdateUserAccess(Uint32 inAccessID);
		//bool RestoreUserAccess(Uint32 inAccessID);
	
		//bool AddNewUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		//bool ModifySelectedUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		//bool DeleteSelectedUsers();
		bool GetSelectedUser(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass, bool *outIsNewUser);

		// tree events
		virtual void SelectionChanged(Uint32 inTreeIndex, SMyUserListItem *inTreeItem, bool inIsSelected);
		virtual void DisclosureChanged(Uint32 inTreeIndex, SMyUserListItem *inTreeItem, Uint8 inDisclosure);
		
	protected:
	/*
	TIcon mAllUsersIcon;
		TIcon mNoPrivIcon, mAllPrivIcon, mDisconnectPrivIcon, mRegularPrivIcon; 
		TIcon mNewNoPrivIcon, mNewAllPrivIcon, mNewDisconnectPrivIcon, mNewRegularPrivIcon; 

		CMyAdminWin *mAdminWin;
		CPtrList<SMyUserDataFile> mDeleteUserList;

		Uint32 GetUserLoginIndex(const Uint8 *inLogin);
		static Int32 CompareLogins(void *inItemA, void *inItemB, void *inRef);

		void AddDeleteUser(SMyAdminUserInfo *inUserInfo);
		void SaveDeleteUserList(TFieldData& inData);
		void RevertDeleteUserList();
		

		void SetUsersAccess(Uint32 inTreeIndex, bool inIsSelected);
		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyAdminUserInfo *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);

	*/
	
		
	
		TIcon mRefMessageIcon, mRefChatIcon; 
		TIcon mNoPrivIcon, mAllPrivIcon, mDisconnectPrivIcon, mRegularPrivIcon; 
		CMyAdmInSpectorWin *mAdmInSpectorWin;
		CPtrList<SMyUserListItem> mAdmInSpectorList;
		static Int32 CompareLogins(void *inItemA, void *inItemB, void *inRef);
		void ClearDeleteUserList();
		virtual void ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyUserListItem *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions = 0);

		
		
		
};

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -
#pragma mark ¥¥ Windows ¥¥

#pragma mark CMyAdminWin
class CMyAdminWin : public CWindow
{
	public:
		CMyAdminWin(CWindow *inParent);
		virtual ~CMyAdminWin();

		void SetBoundsInfo(SMyAdminBoundsInfo& inBoundsInfo);
		void GetBoundsInfo(SMyAdminBoundsInfo& outBoundsInfo);

		bool AddListFromFields(TFieldData inData);
		TFieldData GetFieldsFromList();
		void RevertUserList();
		bool GetUserListStatus();

		void SetUsersAccess(const SMyAdminUsersAccess& inUsersAccess);
		void GetUserAccess(SMyUserAccess& outUserAccess, Uint32 inAccessID);
		void SetEnableUserAccess(bool inEnable);
		bool UpdateUserAccess(Uint32 inAccessID);
		bool RestoreUserAccess(Uint32 inAccessID);
		
		bool AddNewUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool GetSelectedUser(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass, bool *outIsNewUser);
		bool ModifySelectedUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool DeleteSelectedUsers();
	
		void EnableUserButtons();
		void DisableUserButtons(bool inDisableDelete = true);
		void RefreshGlobalButtons();
		
	protected:
		struct {
			CPaneView *pPaneView;
			CIconButtonView *pNewUserBtn, *pOpenUserBtn, *pDeleteUserBtn;
			CButtonView *pSaveBtn, *pRevertBtn, *pCloseBtn;
			CMyAdminUserTreeView *pUserTreeView;
			CMyAdminUserAccessView *pUserAccessView;
		} mViews;
};

#pragma mark CMyAdminEditUserWin
class CMyAdminEditUserWin : public CWindow
{
	public:
		CMyAdminEditUserWin(bool inIsNewUser, bool inDisable = false);
				
		void SetInfo(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool GetInfo(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass);
		
		void SetDummyPassword()		{	if (mViews.pBulletPassText) mViews.pBulletPassText->SetDummyPassword();		}
		bool IsDummyPassword()		{	return mViews.pBulletPassText && mViews.pBulletPassText->IsDummyPassword();	}
		
	protected:
		struct {
			CContainerView *pContainerView;
			CScrollerView *pPasswordScr;
			CTextView *pNameText, *pLoginText, *pPassText;
			CPasswordTextView *pBulletPassText;
		} mViews;		
};
#pragma mark CMyAdmInSpectorWin
class CMyAdmInSpectorWin : public CWindow
{
	public:
		CMyAdmInSpectorWin(CWindow *inParent);
		virtual ~CMyAdmInSpectorWin();

		void SetBoundsInfo(SMyAdmInSpectorBoundsInfo& inBoundsInfo);
		void GetBoundsInfo(SMyAdmInSpectorBoundsInfo& outBoundsInfo);

		//bool AddUserList(TFieldData inData);
		bool AddListFromFields(TFieldData inData);
		TFieldData GetFieldsFromList();
		void RevertUserList();
		bool GetUserListStatus();

		void SetUsersAccess(const SMyAdminUsersAccess& inUsersAccess);
		void GetUserAccess(SMyUserAccess& outUserAccess, Uint32 inAccessID);
		void SetEnableUserAccess(bool inEnable);
		bool UpdateUserAccess(Uint32 inAccessID);
		bool RestoreUserAccess(Uint32 inAccessID);
		
		bool AddNewUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool GetSelectedUser(Uint8 *outName, Uint8 *outLogin, Uint8 *outPass, bool *outIsNewUser);
		bool ModifySelectedUser(const Uint8 *inName, const Uint8 *inLogin, const Uint8 *inPass);
		bool DeleteSelectedUsers();
	
		void EnableUserButtons();
		void DisableUserButtons(bool inDisableDelete = true);
		void RefreshGlobalButtons();
		
	protected:
		struct {
			CPaneView *pPaneView;
			CIconButtonView *pNewUserBtn, *pOpenUserBtn, *pDeleteUserBtn;
			CButtonView *pSaveBtn, *pRevertBtn, *pCloseBtn;
			CMyAdmInSpectorTreeView *pAdmInSpectorTreeView;
			//CMyAdminUserAccessView *pUserAccessView;
		} mViews;
};

