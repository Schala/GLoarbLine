/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "TrackerServ.h"


/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */

CMyLoginListView::CMyLoginListView(CViewHandler *inHandler, const SRect &inBounds, CTextView *inPassText)
	: CGeneralListView(inHandler, inBounds)
{
	mPassText = inPassText;
	mLoginListIndex = 0;
}

Uint32 CMyLoginListView::AddLogin()
{
	Uint8 bufPass[32];
	bufPass[0] = mPassText->GetText(bufPass + 1, sizeof(bufPass) - 1);
	if (!bufPass[0])
		return 0;
		
	SMyLoginInfo *pLoginInfo = (SMyLoginInfo *)UMemory::NewClear(sizeof(SMyLoginInfo));
	if (!pLoginInfo)
		return 0;
		
	UMemory::Copy(pLoginInfo->psPassword, bufPass, bufPass[0] + 1);
	mLoginListIndex = 0;
	
	return AddItem(pLoginInfo);
}

bool CMyLoginListView::ModifyLogin(Uint32 inIndex, Uint8 *inPass)
{
	if (inIndex > GetItemCount())
		return false;

	SMyLoginInfo *pLoginInfo = GetItem(inIndex);
	if (!pLoginInfo)
		return false;
		
	if (inPass && UMemory::Compare(pLoginInfo->psPassword + 1, pLoginInfo->psPassword[0], inPass + 1, inPass[0]))
		UMemory::Copy(pLoginInfo->psPassword, inPass, inPass[0] + 1);

	RefreshItem(inIndex);
	return true;
}

bool CMyLoginListView::ModifySelectedLogin()
{
	if (!mLoginListIndex)
		return false;
	
	Uint8 bufPass[32];
	bufPass[0] = mPassText->GetText(bufPass + 1, sizeof(bufPass) - 1);
	mPassText->SetText(nil, 0);

	Uint32 nIndex = mLoginListIndex;
	mLoginListIndex = 0;
		
	return ModifyLogin(nIndex, bufPass);
}

bool CMyLoginListView::DeleteSelectedLogin()
{
	mLoginListIndex = 0;
	mPassText->SetText(nil, 0);

	return DeleteSelectedItem();
}

void CMyLoginListView::SetItemSelect(Uint32 inItem, bool inSelect)
{
	ModifySelectedLogin();

	CGeneralListView::SetItemSelect(inItem, inSelect);

	SMyLoginInfo *pLoginInfo = nil;
	mLoginListIndex = GetSelectedItem(&pLoginInfo);

	if (mLoginListIndex && pLoginInfo)
		mPassText->SetText(pLoginInfo->psPassword + 1, pLoginInfo->psPassword[0]);
}

void CMyLoginListView::ItemDraw(Uint32 inItem, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions)
{
	if (inItem > GetItemCount())
		return;
	
	CGeneralListView::ItemDraw(inItem, inImage, inBounds, inUpdateRect, inOptions);
	
	SMyLoginInfo *pLoginInfo = GetItem(inItem);
	if (!pLoginInfo)
		return;
	
	SColor stTextCol;
	if (IsFocus() && mIsEnabled && mSelectData.GetItem(inItem))
		UUserInterface::GetSysColor(sysColor_InverseHighlight, stTextCol);
	else
		UUserInterface::GetSysColor(sysColor_Label, stTextCol);

	inImage->SetInkColor(stTextCol);
	
	// draw password
	SRect rBounds = inBounds;
	rBounds.top += 3;
	rBounds.bottom -= 2;
	rBounds.left += 2;

	inImage->DrawTruncText(rBounds, pLoginInfo->psPassword + 1, pLoginInfo->psPassword[0], nil, textAlign_Left);
}


/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */
#pragma mark -

CMyPermBanListView::CMyPermBanListView(CViewHandler *inHandler, const SRect &inBounds, CTextView *inBanUrlText)
	: CGeneralListView(inHandler, inBounds)
{
	mBanUrlText = inBanUrlText;
	mPermBanListIndex = 0;
}

Uint32 CMyPermBanListView::AddPermBan()
{
	Uint8 bufURL[16];
	bufURL[0] = mBanUrlText->GetText(bufURL + 1, sizeof(bufURL) - 1);

	SIPAddress stIPAddress;
	UTransport::TextToIPAddress(bufURL, stIPAddress);
	if (!stIPAddress.un_IP.stDW_IP.nDW_IP)
		return 0;
		
	SMyPermBanInfo *pPermBanInfo = (SMyPermBanInfo *)UMemory::NewClear(sizeof(SMyPermBanInfo));
	if (!pPermBanInfo)
		return 0;
		
	pPermBanInfo->nAddr = stIPAddress.un_IP.stDW_IP.nDW_IP;
	mPermBanListIndex = 0;
	
	return AddItem(pPermBanInfo);
}

bool CMyPermBanListView::ModifyPermBan(Uint32 inIndex, Uint8 *inURL)
{
	if (inIndex > GetItemCount())
		return false;

	SMyPermBanInfo *pPermBanInfo = GetItem(inIndex);
	if (!pPermBanInfo)
		return false;
		
	SIPAddress stIPAddress;
	if(inURL && UTransport::TextToIPAddress(inURL, stIPAddress) && pPermBanInfo->nAddr != stIPAddress.un_IP.stDW_IP.nDW_IP)
		pPermBanInfo->nAddr = stIPAddress.un_IP.stDW_IP.nDW_IP;
	
	RefreshItem(inIndex);
	return true;
}

bool CMyPermBanListView::ModifySelectedPermBan()
{
	if (!mPermBanListIndex)
		return false;
	
	Uint8 bufURL[16];
	bufURL[0] = mBanUrlText->GetText(bufURL + 1, sizeof(bufURL) - 1);
	mBanUrlText->SetText(nil, 0);

	Uint32 nIndex = mPermBanListIndex;
	mPermBanListIndex = 0;
		
	return ModifyPermBan(nIndex, bufURL);
}

bool CMyPermBanListView::DeleteSelectedPermBan()
{
	mPermBanListIndex = 0;
	mBanUrlText->SetText(nil, 0);

	return DeleteSelectedItem();
}

void CMyPermBanListView::SetItemSelect(Uint32 inItem, bool inSelect)
{
	ModifySelectedPermBan();

	CGeneralListView::SetItemSelect(inItem, inSelect);

	SMyPermBanInfo *pPermBanInfo = nil;
	mPermBanListIndex = GetSelectedItem(&pPermBanInfo);

	if (mPermBanListIndex && pPermBanInfo)
	{
		SIPAddress stIPAddress;
		stIPAddress.un_IP.stDW_IP.nDW_IP = pPermBanInfo->nAddr;
		
		Uint8 bufURL[16];
		bufURL[0] = UTransport::IPAddressToText(stIPAddress, bufURL + 1, sizeof(bufURL) - 1);
	
		mBanUrlText->SetText(bufURL + 1, bufURL[0]);
	}
}

void CMyPermBanListView::ItemDraw(Uint32 inItem, TImage inImage, const SRect& inBounds, const SRect& inUpdateRect, Uint32 inOptions)
{
	if (inItem > GetItemCount())
		return;
	
	CGeneralListView::ItemDraw(inItem, inImage, inBounds, inUpdateRect, inOptions);
	
	SMyPermBanInfo *pPermBanInfo = GetItem(inItem);
	if (!pPermBanInfo)
		return;
	
	SColor stTextCol;
	if (IsFocus() && mIsEnabled && mSelectData.GetItem(inItem))
		UUserInterface::GetSysColor(sysColor_InverseHighlight, stTextCol);
	else
		UUserInterface::GetSysColor(sysColor_Label, stTextCol);

	inImage->SetInkColor(stTextCol);
	
	// draw addr
	SRect rBounds = inBounds;
	rBounds.top += 3;
	rBounds.bottom -= 2;
	rBounds.left += 2;
	
	SIPAddress stIPAddress;
	stIPAddress.un_IP.stDW_IP.nDW_IP = pPermBanInfo->nAddr;
		
	Uint8 bufURL[16];
	bufURL[0] = UTransport::IPAddressToText(stIPAddress, bufURL + 1, sizeof(bufURL) - 1);
	
	inImage->DrawText(rBounds, bufURL + 1, bufURL[0], nil, textAlign_Left);	
}

