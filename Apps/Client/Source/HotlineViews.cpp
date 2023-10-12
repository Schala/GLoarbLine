/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "Hotline.h"

#if WIN32
void _SetWinIcon(TWindow inRef, Int16 inID);
#endif


/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */

CMyIconPickView::CMyIconPickView(CViewHandler *inHandler, const SRect& inBounds)
	: CView(inHandler, inBounds)
{
	mIcon = nil;
	mIconID = 0;

	mMark = 0;
	mNext = nil;
}

CMyIconPickView::~CMyIconPickView()
{
	UIcon::Release(mIcon);
}

void CMyIconPickView::SetIcon(Int16 inIconID)
{
	if (inIconID != mIconID)
	{
		UIcon::Release(mIcon);
		mIcon = nil;
	
		mIconID = inIconID;
		Refresh();
	}
}

void CMyIconPickView::SetMark(bool inMark)
{
	if ((inMark!=0) != mMark)
	{
		mMark = (inMark!=0);
		Refresh();
	}
}

void CMyIconPickView::Draw(TImage inImage, const SRect& /* inUpdateRect */, Uint32 /* inDepth */)
{
	if (mMark)
	{
		SRect r;
		Int16 th = 2 + 2;
		
		inImage->SetPenSize(2);
		r = mBounds;
		r.Enlarge(-1, -1);
		inImage->SetInkColor(color_GrayD);
		inImage->FrameRect(r);
		
		inImage->SetPenSize(1);
		r = mBounds;

		inImage->SetInkColor(color_White);
		inImage->DrawLine(SLine(r.left, r.top, r.right-1, r.top));
		inImage->DrawLine(SLine(r.left, r.top, r.left, r.bottom-1));
		inImage->DrawLine(SLine(r.left+th, r.bottom-th, r.right-th, r.bottom-th));
		inImage->DrawLine(SLine(r.right-th, r.top+th, r.right-th, r.bottom-th));

		inImage->SetInkColor(color_Gray8);
		inImage->DrawLine(SLine(r.left+th-1, r.top+th-1, r.right-th, r.top+th-1));
		inImage->DrawLine(SLine(r.left+th-1, r.top+th-1, r.left+th-1, r.bottom-th));
		inImage->DrawLine(SLine(r.left+1, r.bottom-1, r.right-1, r.bottom-1));
		inImage->DrawLine(SLine(r.right-1, r.top+1, r.right-1, r.bottom-1));
	}
	
	if (!mIcon)
		mIcon = UIcon::Load(mIconID);

	if (mIcon)
		mIcon->Draw(inImage, mBounds, align_Center, transform_None);
}

void CMyIconPickView::MouseDown(const SMouseMsgData& inInfo)
{
	CView::MouseDown(inInfo);
	
	if (mMark == 0)
	{
		CMyIconPickView *v = mNext;
		
		while (v && v != this)
		{
			v->SetMark(false);
			v = v->mNext;
		}
		
		SetMark(true);
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyOptIconTab::CMyOptIconTab(const SPoint& inPoint)
	: CContainerView(nil, SRect(inPoint.x, inPoint.y, inPoint.x + 320, inPoint.y + 220))
{
	// make label view
	mTextLbl = new CLabelView(this, SRect(0,0,264,34));
	mTextLbl->Show();
	
	// make scroller view
	mIconsScroll = new CScrollerView(this, SRect(0,38,314,214), scrollerOption_VertBar + scrollerOption_Border + scrollerOption_NoBkgnd);
	mIconsScroll->Show();
	
	// make container view
	CContainerView *vc = new CContainerView(mIconsScroll, SRect(0,0,314,10));
	
	// make icon pickers
	Int16 iconIDs[] = { 141,  149,  150,  151,  172,  184,  204, 
						2013, 2036, 2037, 2055, 2400, 2505, 2534, 
						2578, 2592, 4004, 4015, 4022, 4104, 4131, 
						4134, 4136, 4169, 4183, 4197, 4240, 4247, 
						128,  129,  130,  131,  132,  133,  134,
						135,  136,  137,  138,  139,  140,  142, 
						143,  144,  145,  146,  147,  148,  152, 
						153,  154,  155,  156,  157,  158,  159, 
						160,  161,  162,  163,  164,  165,  166, 
						167,  168,  169,  170,  171,  173,  174, 
						175,  176,  177,  178,  179,  180,  181, 
						182,  183,  185,  186,  187,  188,  189, 
						190,  191,  192,  193,  194,  195,  196, 
						197,  198,  199,  200,  201,  202,  203, 
						205,  206,  207,  208,  209,  212,  214, 
						215,  220,  233,  236,  237,  243,  244, 
						277,  410,  414,  500,  666,  1250, 1251, 
						1968, 1969, 2000, 2001, 2002, 2003, 2004, 
						2006, 2007, 2008, 2009, 2010, 2011, 2012, 
						2014, 2015, 2016, 2017, 2018, 2019, 2020, 
						2021, 2022, 2023, 2024, 2025, 2026, 2027, 
						2028, 2029, 2030, 2031, 2032, 2033, 2034, 
						2035, 2038, 2040, 2041, 2042, 2043, 2044, 
						2045, 2046, 2047, 2048, 2049, 2050, 2051,
						2052, 2053, 2054, 2056, 2057, 2058, 2059, 
						2060, 2061, 2062, 2063, 2064, 2065, 2066, 
						2067, 2070, 2071, 2072, 2073, 2075, 2079, 
						2098, 2100, 2101, 2102, 2103, 2104, 2105, 
						2106, 2107, 2108, 2109, 2110, 2112, 2113, 
						2115, 2116, 2117, 2118, 2119, 2120, 2121, 
						2122, 2123, 2124, 2125, 2126, 4150, 2223, 
						2401, 2402, 2403, 2404, 2500, 2501, 2502, 
						2503, 2504, 2506, 2507, 2528, 2529, 2530, 
						2531, 2532, 2533, 2535, 2536, 2537, 2538, 
						2539, 2540, 2541, 2542, 2543, 2544, 2545,
						2546, 2547, 2548, 2549, 2550, 2551, 2552, 
						2553, 2554, 2555, 2556, 2557, 2558, 2559, 
						2560, 2561, 2562, 2563, 2564, 2565, 2566, 
						2567, 2568, 2569, 2570, 2571, 2572, 2573, 
						2574, 2575, 2576, 2577, 2579, 2580, 2581, 
						2582, 2583, 2584, 2585, 2586, 2587, 2588, 
						2589, 2590, 2591, 2593, 2594, 2595, 2596, 
						2597, 2598, 2599, 2600, 3000, 3001, 3002,
						3003, 3004, 3005, 3006, 3007, 3008, 3009,
						3010, 3011, 3012, 3013, 3014, 3015, 3016,
						3017, 3018, 3019, 3020, 3021, 3022, 3023,
						3024, 3025, 3026, 3027, 3028, 3029, 3030,
						3031, 3032, 3033, 3034, 3035, 3036, 3037,
						3038, 3039, 3040, 3041, 3042, 3043, 3044,
						3045, 3046, 3047, 3048, 3049, 3050, 3051,
						3052, 3053, 3054, 3055, 3056, 3057, 3058,
						3059, 3060, 3061, 3062, 3063, 3064,
						 4000, 4001, 4002, 
						4003, 4005, 4006, 4007, 4008, 4009, 4010, 
						4011, 4012, 4013, 4014, 4016, 4017, 4018, 
						4019, 4020, 4021, 4023, 4024, 4025, 4026,
						4027, 4028, 4029, 4030, 4031, 4032, 4033, 
						4034, 4035, 4036, 4037, 4038, 4039, 4040, 
						4041, 4042, 4043, 4044, 4045, 4046, 4047, 
						4048, 4049, 4050, 4051, 4052, 4053, 4054, 
						4055, 4056, 4057, 4058, 4059, 4060, 4061, 
						4062, 4063, 4064, 4065, 4066, 4067, 4068, 
						4069, 4070, 4071, 4072, 4073, 4074, 4075, 
						4076, 4077, 4078, 4079, 4080, 4081, 4082, 
						4083, 4084, 4085, 4086, 4087, 4088, 4089, 
						4090, 4091, 4092, 4093, 4094, 4095, 4096, 
						4097, 4098, 4099, 4100, 4101, 4102, 4103,
						4105, 4106, 4107, 4108, 4109, 4110, 4111, 
						4112, 4113, 4114, 4115, 4116, 4117, 4118, 
						4119, 4120, 4121, 4122, 4123, 4124, 4125, 
						4126, 4127, 4128, 4129, 4130, 4132, 4133, 
						4135, 4137, 4138, 4139, 4140, 4141, 4142, 
						4143, 4144, 4145, 4146, 4147, 4148, 4149, 
						4151, 4152, 4153, 4154, 4155, 4156, 4157, 
						4158, 4159, 4160, 4161, 4162, 4163, 4164, 
						4165, 4166, 4167, 4168, 4170, 4171, 4172, 
						4173, 4174, 4175, 4176, 4177, 4178, 4179, 
						4180, 4181, 4182, 4184, 4185, 4186, 4187,
						4188, 4189, 4190, 4191, 4192, 4193, 4194, 
						4195, 4196, 4198, 4199, 4200, 4201, 4202, 
						4203, 4204, 4205, 4206, 4207, 4208, 4209, 
						4210, 4211, 4212, 4213, 4214, 4215, 4216, 
						4217, 4218, 4219, 4220, 4221, 4222, 4223, 
						4224, 4225, 4226, 4227, 4228, 4229, 4230, 
						4231, 4232, 4233, 4234, 4235, 4236, 4238, 
						4241, 4242, 4243, 4244, 4245, 4246, 4248, 
						4249, 4250, 4251, 4252, 4253, 4254, 31337,
						6001, 6002, 6003, 6004, 6005, 6008, 6009, 
						6010, 6011, 6012, 6013, 6014, 6015, 6016, 
						6017, 6018, 6023, 6025, 6026, 6027, 6028,
						6029, 6030, 6031, 6032, 6033, 6034, 6035
	};
		
	Uint32 nHorizDepl = kIconHoriz, nVertDepl = kIconVert;
	
	Uint32 i;
	for (i=0; i<kIconCount; i++)
	{
		CMyIconPickView *icp = new CMyIconPickView(vc, SRect(nHorizDepl, nVertDepl, nHorizDepl + kIconSize, nVertDepl + kIconSize));
		icp->SetIcon(iconIDs[i]);
		icp->Show();
		
		mIcons[i] = icp;
		
		if ((i + 1)%kIconColNum)
			nHorizDepl += kIconSize + kIconSpace;
		else
		{
			nHorizDepl = kIconHoriz;
			nVertDepl += kIconSize + kIconSpace;
		}
	}

	for (i=0; i<kIconCount-1; i++)
		mIcons[i]->SetNext(mIcons[i+1]);
	
	mIcons[i]->SetNext(mIcons[0]);
	
	vc->SetBounds(SRect(0, 0, 314, nVertDepl + kIconVert));
	vc->Show();	
}

void CMyOptIconTab::SetText(const Uint8 *inText)
{
	mTextLbl->SetText(inText);
}

void CMyOptIconTab::SetIconID(Int16 inID)
{
	for (Uint32 i=0; i<kIconCount; i++)
	{
		if (mIcons[i]->GetIcon() == inID)
		{
			mIcons[i]->SetMark(true);
			
			Uint32 nVertDepl = (i/kIconColNum)*(kIconSize + kIconSpace);
			mIconsScroll->SetScroll(0, nVertDepl);			
			break;
		}
	}
}

Int16 CMyOptIconTab::GetIconID()
{
	for (Uint32 i=0; i<kIconCount; i++)
	{
		if (mIcons[i]->GetMark())
			return mIcons[i]->GetIcon();
	}
	
	return 0;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyAccessCheckBoxView::CMyAccessCheckBoxView(CViewHandler *inHandler, const SRect& inBounds)
	: CCheckBoxView(inHandler, inBounds)
{

}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyListStatusView::CMyListStatusView(CViewHandler *inHandler, const SRect& inBounds)
	: CTabbedListView(inHandler, inBounds)
{
#if USE_LARGE_FONT
	mTabHeight = 23;
	mCellHeight = 27;
#elif WIN32
	mTabHeight = 18;
#endif

#if DISABLE_TABS
	mTabHeight = 0;
#endif

	mStatus = 0;
	mCustStatMsg = nil;
		
	mRefreshSelectedOnSetActive = true;
}

CMyListStatusView::~CMyListStatusView()
{
	if (mCustStatMsg)
	{
		delete mCustStatMsg;
		mCustStatMsg = nil;
	}
}

void CMyListStatusView::SetStatus(Uint32 inStatus)
{
	if (inStatus == mStatus)
		return;
	
	Uint32 nOldStatus = mStatus;
	mStatus = inStatus;

	if (!GetItemCount() && nOldStatus == listStat_Hide)
		SetFullHeight();

	Refresh();
}

void CMyListStatusView::SetStatus(const Uint8 inMsg[])
{
	Uint32 nOldStatus = mStatus;
	mStatus = listStat_Custom;
	
	if (mCustStatMsg)
	{
		delete mCustStatMsg;
		mCustStatMsg = nil;
	}
	mCustStatMsg = UMemory::New(inMsg, inMsg[0] + 1);

	if (!GetItemCount() && nOldStatus == listStat_Hide)
		SetFullHeight();
	
	Refresh();
}

Uint32 CMyListStatusView::GetFullHeight() const
{
	if (mHandler && !GetItemCount())
	{
		CScrollerView *pHandler = dynamic_cast<CScrollerView *>(mHandler);
		if (pHandler)
		{
			SRect stBounds;
			pHandler->GetBounds(stBounds);
			
			return stBounds.GetHeight() - 2;
		}		
	}
	
	return CTabbedListView::GetFullHeight();
}

void CMyListStatusView::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth)
{
#if USE_LARGE_FONT
	UGraphics::SetFont(inImage, kDefaultFont, nil, 16);
#else
	UGraphics::SetFont(inImage, kDefaultFont, nil, 9);
#endif

#if MACINTOSH
	inImage->SetInkMode(mode_Or);
#endif
	
	// draw the status text
	if (!GetItemCount() && mStatus != listStat_Hide)
	{
		const Uint8 *statTxt;
		
		switch (mStatus)
		{
			case listStat_Loading:
				statTxt = "\pLoading...";
				break;
			
			case listStat_0Items:
				statTxt = "\p0 items in list";
				break;
			
			case listStat_Custom:
				statTxt = mCustStatMsg ? BPTR(mCustStatMsg) : "\p";
				break;
			
			default:	// listStat_Hide
				statTxt = "\p";
				break;
		}
		
		// set color
		inImage->SetInkColor(color_Gray);
		
		SRect stRect = mBounds;
		stRect.Enlarge(-4);
		stRect.top += mTabHeight;
		if (stRect.bottom < stRect.top)
			stRect.bottom = stRect.top;

		inImage->DrawTextBox(stRect, inUpdateRect, statTxt + 1, statTxt[0], 0, textAlign_Left);
	}

	CTabbedListView::Draw(inImage, inUpdateRect, inDepth);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

Uint16 CMyFileListView::mIconIndent = 10;
Uint16 CMyFileListView::mNameIndent = CMyFileListView::mIconIndent + 26;

CMyFileListView::CMyFileListView(CViewHandler *inHandler, const SRect& inBounds, bool inListExpl)
	: CMyListStatusView(inHandler, inBounds)
{
	mListExpl = inListExpl;
	
	// set headers
	AddTab("\pName", inBounds.GetWidth() - 40, 60);
	AddTab("\pSize", 1, 1, align_CenterHoriz);
}

CMyFileListView::~CMyFileListView()
{
	DeleteAll();
	ClearUploadList();	
}

void CMyFileListView:: SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
	if (!inTabPercent1 && !inTabPercent2)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyFileListView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}

Uint32 CMyFileListView::GetItemCount() const
{
	return mFileList.GetItemCount();
}

bool CMyFileListView::AddListFromFields(TFieldData inData)
{
#pragma options align=packed
	struct {
		SMyFileInfo info;
		Uint8 data[256];
	} infoBuf;
#pragma options align=reset
	SMyFileInfo& info = infoBuf.info;

	SMyFileItem *item;
	Uint32 startCount = mFileList.GetItemCount();
	Uint32 addCount = 0;
	Uint16 nCount = inData->GetFieldCount();
	//gloglo
	
	for (Uint16 i=1; i <= nCount; i++)
	{
	//MsgBox(142, i);
		if (inData->GetFieldID(i) == myField_FileNameWithInfo)
		{
			inData->GetFieldByIndex(i, &info, sizeof(infoBuf));
			
			Uint32 nSize = FB(info.nameSize);
			if (nSize > 255) nSize = 255;
			item = (SMyFileItem *)UMemory::New(sizeof(SMyFileItem) + nSize + 1);
			
			try
			{
				if (info.type == TB((Uint32)'fldr'))
				{	
				
					
								
					if (UText::SearchInsensitive("upload", 6, info.nameData, nSize)){ 
						
						if (gApp->mIconSet == 2){
							item->icon = UIcon::Load(3001);
						}else if (gApp->mIconSet == 1){
							item->icon = UIcon::Load(421);
						}
								
					
					}else if  (UText::SearchInsensitive("drop box", 8, info.nameData, nSize)){
					
					if (gApp->mIconSet == 2){
							item->icon = UIcon::Load(3002);
						}else if (gApp->mIconSet == 1){
							item->icon = UIcon::Load(196);
						}
						
						
					}else{
					
					if (gApp->mIconSet == 2){
							item->icon = UIcon::Load(3003);
						}else if (gApp->mIconSet == 1){
							item->icon = UIcon::Load(401);
						}
						
					}
					
					
					
					
				}
				else
					item->icon = UIcon::Load(HotlineFileTypeToIcon(info.type));
				
				item->type = info.type;
				item->creator = info.creator;
				item->fileSize = FB(info.fileSize);
				item->nameSize = 0;				
				item->name[0] = nSize;
				UMemory::Copy(item->name+1, info.nameData, nSize);
				
				mFileList.AddItem(item);
			}
			catch(...)
			{
				UMemory::Dispose((TPtr)item);
				throw;
			}
			
			addCount++;
		}
	}
	
	mFileList.Sort(CompareNames);
	ItemsInserted(startCount+1, addCount);
	
	return addCount != 0;
}

Uint32 CMyFileListView::SelectNames(const Uint8 *inStr)
{
	Uint32 i = 0;
	SMyFileItem *pFileItem;

	while (mFileList.GetNext(pFileItem, i))
	{
		if (!UText::CompareInsensitive(inStr + 1, inStr[0], pFileItem->name + 1, pFileItem->name[0]))
		{
			DeselectAll();
			SelectItem(i);
			MakeItemVisible(i);	

			return i;
		}
	}
	
	return 0;
}

Uint32 CMyFileListView::SearchNames(const Uint8 *inStr)
{
	Uint8 searchStr[256];
	Uint8 str[256];
	
	UMemory::Copy(searchStr, inStr, inStr[0] + 1);
	UText::MakeLowercase(searchStr + 1, searchStr[0]);
	
	Uint32 i = 0;
	SMyFileItem *pFileItem;

	while (mFileList.GetNext(pFileItem, i))
	{
		UMemory::Copy(str, pFileItem->name, pFileItem->name[0] + 1);
		UText::MakeLowercase(str + 1, str[0]);
		
		if (UMemory::Search(searchStr + 1, searchStr[0], str + 1, str[0]))
			return i;
	}
	
	return 0;
}

// returns true if folder
bool CMyFileListView::GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode, Uint32 *outCreatorCode, const SMouseMsgData *inInfo)
{
	outItemName[0] = 0;
	if (outTypeCode) *outTypeCode = 0;
	if (outCreatorCode) *outCreatorCode = 0;

	Uint32 nSelected = GetFirstSelectedItem();
	if (!nSelected)
		return false;
		
	SMyFileItem *pFileItem = mFileList.GetItem(nSelected);
	if (!pFileItem)
		return false;

	if (inInfo)
	{		
		SRect stBounds;
		GetItemRect(nSelected, stBounds);
				
		Uint32 nItemLeft = stBounds.left + mIconIndent;
		Uint32 nItemRight = stBounds.left + mNameIndent + pFileItem->nameSize;

		if (inInfo->loc.x < nItemLeft || inInfo->loc.x > nItemRight)
			return false;
	}
	
	UMemory::Copy(outItemName, pFileItem->name, pFileItem->name[0] + 1);
	if (outTypeCode) *outTypeCode = pFileItem->type;
	if (outCreatorCode) *outCreatorCode = pFileItem->creator;

	return (pFileItem->type == TB((Uint32)'fldr') && pFileItem->creator == 0);
}

void CMyFileListView::DeleteAll()
{
	Uint32 i = 0;
	SMyFileItem *pFileItem;
		
	while (mFileList.GetNext(pFileItem, i))
	{
		UIcon::Release(pFileItem->icon);
		UMemory::Dispose((TPtr)pFileItem);
	}
	
	Uint32 nCount = mFileList.GetItemCount();
	mFileList.Clear();
	ItemsRemoved(1, nCount);
}

bool CMyFileListView::HasPartialFile(const Uint8 *inFileName, bool inIsFolder)
{
	Uint32 i = 0;
	SMyFileItem *pFileItem;

	Uint32 nSearchNameSize = inFileName[0];

	while (mFileList.GetNext(pFileItem, i))
	{
		if (inIsFolder)
		{
			if (pFileItem->type == TB((Uint32)'fldr') && pFileItem->creator == 0)
			{
				if (pFileItem->name[0] == nSearchNameSize && UMemory::Equal(pFileItem->name + 1, inFileName + 1, nSearchNameSize))
					return true;
			}
		}
		else if (pFileItem->type == TB((Uint32)'HTft') && pFileItem->creator == TB((Uint32)'HTLC'))
		{
			Uint8 psValidatedName[256];
			UMemory::Copy(psValidatedName, pFileItem->name, pFileItem->name[0] + 1);

			// take the .hpf out of the name
			if (psValidatedName[0] >= 4 && !UMemory::Compare(psValidatedName + psValidatedName[0] - 3, ".hpf", 4))
				psValidatedName[0] -= 4;
	
			if (psValidatedName[0] == nSearchNameSize && UMemory::Equal(psValidatedName + 1, inFileName + 1, nSearchNameSize))
				return true;
		}
	}
	
	return false;
}

TFSRefObj* CMyFileListView::GetItemToUpload()
{
	TFSRefObj* fp = mUploadList.GetItem(1);
	if (fp) mUploadList.RemoveItem(1);
	return fp;
}

void CMyFileListView::ClearUploadList()		
{	
	Uint32 i = 0;
	TFSRefObj* fp;
	
	while (mUploadList.GetNext(fp ,i))
		delete fp;
	
	mUploadList.Clear();	
}

void CMyFileListView::DragEnter(const SDragMsgData& inInfo)
{
	CMyListStatusView::DragEnter(inInfo);
	
	if (mDragAndDropHandler)
		mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);
}

void CMyFileListView::DragMove(const SDragMsgData& inInfo)
{
	CMyListStatusView::DragMove(inInfo);
		
	if (mDragAndDropHandler)
		mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);
}

bool CMyFileListView::Drop(const SDragMsgData& inInfo)
{
	Uint16 i, c;
	Uint32 item, sa, sb;
	TDrag drag = inInfo.drag;
	TFSRefObj* fp;
	Uint8 buf[2048];
	bool accepted = false;
	bool uploadHit = false;
	
	if (!mDragAndDropHandler)
		return false;
	
	mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);
	Uint16 action = inInfo.drag->GetDragAction();
	
	if (action == dragAction_None)
		return false;
	
	c = drag->GetItemCount();
	for (i=1; i<=c; i++)
	{
		item = drag->GetItem(i);
		
		fp = drag->GetFileSysFlavor(item);
		if (fp)
		{
			try
			{
				if (fp->Exists())
				{
					accepted = true;
					
					if (!uploadHit)
					{
						Hit(hitType_Drop, 1, flavor_File);
						uploadHit = true;
					}
					
					mUploadList.AddItem(fp);
				}
				else
					delete fp;
			}
			catch(...)
			{
				delete fp;
				throw;
			}
		}
		else
		{
			sa = drag->GetFlavorData(item, 'HLFN', buf+1, 63);
			if (sa)
			{
				buf[0] = sa;
				sb = drag->GetFlavorData(item, 'HLFP', buf+sa+3, sizeof(buf)-sa-3);
				*(Uint16 *)(buf+sa+1) = sb;
				accepted = true;
				Hit(hitType_Drop, inInfo.mods, 'HLFN', buf, sa + sb + 3);
			}
		}
	}

	return accepted;
}

void CMyFileListView::SetItemSelect(Uint32 inItem, bool inSelect)
{
	CMyListStatusView::SetItemSelect(inItem, inSelect);
	
	if (mListExpl)
		((CMyFileListWin*)GetDragAndDropHandler())->SetAccess();
	else
		((CMyFileExplWin*)GetDragAndDropHandler())->SetAccess();
}

void CMyFileListView::ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inOptions)
	
	// get file item
	SMyFileItem *pFileItem = mFileList.GetItem(inListIndex);
	if (!pFileItem)
		return;
	
	SRect stRect;
	SColor stHiliteCol, stTextCol;
	Uint8 psText[256];
	
	// get info
	bool bIsSelected = mSelectData.GetItem(inListIndex);
	bool bIsActive = IsFocus() && mIsEnabled;
	bool bIsFolder = (pFileItem->type == TB((Uint32)'fldr') && pFileItem->creator == 0);
	bool bIsBadAlias = (pFileItem->type == TB((Uint32)'alis'));
	
	// set font if drawing for drag
	if (inOptions == kDrawItemForDrag)
	{
		UGraphics::SetFont(inImage, kDefaultFont, nil, 9);
	#if MACINTOSH
		inImage->SetInkMode(mode_Or);
	#endif
	}
	
	// draw selection
	if (bIsSelected)
	{
		// calc background rect
		if (inOptions == kDrawItemForDrag)
			stRect = inBounds;
		else
			stRect.Set(inBounds.left + 1, inBounds.top + 1, inBounds.right - 1, inBounds.bottom);

		UUserInterface::GetHighlightColor(&stHiliteCol);
		inImage->SetInkColor(stHiliteCol);

		if (bIsActive)
			inImage->FillRect(stRect);
		else
			inImage->FrameRect(stRect);
	}
	
	// draw light lines around this item
	if (inOptions != kDrawItemForDrag)
	{
		UUserInterface::GetSysColor(sysColor_Light, stHiliteCol);
		inImage->SetInkColor(stHiliteCol);
		stRect = inBounds; stRect.bottom++;
		inImage->FrameRect(stRect);
	}
	
	// determine color to draw name in
	if (bIsBadAlias)
		stTextCol = color_Gray7;
	else
		UUserInterface::GetSysColor(bIsSelected && bIsActive ? sysColor_InverseHighlight : sysColor_Label, stTextCol);

	// set color
	inImage->SetInkColor(stTextCol);

	// set font
	inImage->SetFont(kDefaultFont, nil, 9);

	// draw item icon and name
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{	
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += mIconIndent;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
			pFileItem->icon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
		
		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += mNameIndent;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
	
		// draw name
		inImage->SetFontEffect(fontEffect_Bold);
		inImage->DrawTruncText(stRect, pFileItem->name + 1, pFileItem->name[0], 0, align_Left + align_CenterVert);

		if (!pFileItem->nameSize)
			pFileItem->nameSize = inImage->GetTextWidth(pFileItem->name + 1, pFileItem->name[0]);
	}
	
	// draw size/count
	pBounds = inTabRectList.GetItem(2);
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
			
		if (bIsBadAlias)
		{
			psText[0] = 1;
			psText[1] = '-';
		}
		else if (bIsFolder)
		{    
			psText[0] = UText::IntegerToText(psText + 2, sizeof(psText) - 3, pFileItem->fileSize) + 2;
			psText[1] = '(';
			psText[psText[0]] = ')';
		}
		else
			psText[0] = UText::SizeToText(pFileItem->fileSize, psText + 1, sizeof(psText) - 1, kDontShowBytes);
	
		inImage->SetFontEffect(fontEffect_Plain);
		inImage->DrawTruncText(stRect, psText + 1, psText[0], 0, align_Right | align_CenterVert);
	}
}

// compare file names for sorting by the list because pc servers don't
Int32 CMyFileListView::CompareNames(void *inPtrA, void *inPtrB, void *inRef)
{
	#pragma unused(inRef)
	Uint8 *nameA = ((SMyFileItem *)inPtrA)->name;
	Uint8 *nameB = ((SMyFileItem *)inPtrB)->name;

	Int32 outVal = UText::CompareInsensitive(nameA + 1, nameB + 1, min(nameA[0], nameB[0]));
	if (outVal == 0 && nameA[0] != nameB[0])
		outVal = nameA[0] > nameB[0] ? 1 : -1;
	
	return outVal;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

Uint16 CMyFileTreeView::mIconIndent = 2;
Uint16 CMyFileTreeView::mNameIndent = CMyFileTreeView::mIconIndent + 26;

CMyFileTreeView::CMyFileTreeView(CViewHandler *inHandler, const SRect& inBounds, TPtr *inPathData, Uint32 *inPathSize, bool inTreeExpl)
	: CMyTreeStatusView(inHandler, inBounds)
{	
	mPathData = inPathData;
	mPathSize = inPathSize;
	
	mTreeExpl = inTreeExpl;
	mKeyDownTime = 0;

	mRefreshFolderIndex = 0;
	mDropFolderIndex = 0;
	mSavedFolderIndex = 0;	
	
	// set headers
	AddTab("\pName", inBounds.GetWidth() - 40, 80);
	AddTab("\pSize", 1, 1, align_CenterHoriz);
}

CMyFileTreeView::~CMyFileTreeView()
{
	DeleteAll();
	ClearUploadList();	
}

void CMyFileTreeView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
	if (!inTabPercent1 && !inTabPercent2)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyFileTreeView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}

bool CMyFileTreeView::AddListFromFields(const void *inPathData, Uint32 inPathSize, TFieldData inData)
{
#pragma options align=packed
	struct {
		SMyFileInfo info;
		Uint8 data[256];
	} infoBuf;
#pragma options align=reset
	SMyFileInfo& info = infoBuf.info;

	Uint32 nFolderIndex;
	if (!GetFolderIndex(inPathData, inPathSize, nFolderIndex))
		return false;

	SMyFileItem *item;
	Uint32 addCount = 0;
	Uint16 nCount = inData->GetFieldCount();
		
	for (Uint16 i=1; i <= nCount; i++)
	{
		if (inData->GetFieldID(i) == myField_FileNameWithInfo)
		{
			inData->GetFieldByIndex(i, &info, sizeof(infoBuf));
			
			bool bIsFolder = info.type == TB('fldr');
			if (!bIsFolder && !mTreeExpl)
				continue;
			
			Uint32 nSize = FB(info.nameSize);
			if (nSize > 255) nSize = 255;
			item = (SMyFileItem *)UMemory::New(sizeof(SMyFileItem) + nSize + 1);
			
			try
			{
				if (bIsFolder)
				{					
					if (UText::SearchInsensitive("upload", 6, info.nameData, nSize)){
					
					if (gApp->mIconSet == 2){
							item->icon = UIcon::Load(3001);
						}else if (gApp->mIconSet == 1){
							item->icon = UIcon::Load(421);
						}
						
						
					}else if (UText::SearchInsensitive("drop box", 8, info.nameData, nSize)){
						
						if (gApp->mIconSet == 2){
							item->icon = UIcon::Load(3002);
						}else if (gApp->mIconSet == 1){
							item->icon = UIcon::Load(196);
						}
		
					}else{
					
					if (gApp->mIconSet == 2){
							item->icon = UIcon::Load(3003);
						}else if (gApp->mIconSet == 1){
							item->icon = UIcon::Load(401);
						}
						
					}
				}
				else
					item->icon = UIcon::Load(HotlineFileTypeToIcon(info.type));
				
				item->type = info.type;
				item->creator = info.creator;
				item->fileSize = FB(info.fileSize);
				item->nameSize = 0;
				item->name[0] = nSize;
				UMemory::Copy(item->name + 1, info.nameData, nSize);
				
				AddTreeItem(nFolderIndex, item, bIsFolder);
			}
			catch(...)
			{
				UMemory::Dispose((TPtr)item);
				throw;
			}

			addCount++;
		}
	}
	
	// sort
	Sort(nFolderIndex, CompareNames);

	// update folder count
	if (nFolderIndex)
	{
		SMyFileItem *pFolder = GetTreeItem(nFolderIndex);		
		if (pFolder && (gApp->HasFolderPriv(myAcc_ViewDropBoxes) || !UText::SearchInsensitive("drop box", 8, pFolder->name + 1, pFolder->name[0])))
		{
			if (mTreeExpl)
				nCount = GetChildTreeCount(nFolderIndex);
			
			if (pFolder->fileSize != nCount)
				pFolder->fileSize = nCount;
		}
	}
	
	return (addCount != 0);
}

Uint32 CMyFileTreeView::SelectNames(const Uint8 *inStr)
{
	Uint32 i = 0;
	SMyFileItem *pFileItem;

	while (GetNextVisibleTreeItem(pFileItem, i))
	{		
		if (!UText::CompareInsensitive(inStr + 1, inStr[0], pFileItem->name + 1, pFileItem->name[0]))
		{
			DeselectAllTreeItems();
			SelectTreeItem(i);
			MakeTreeItemVisibleInList(i);	

			return i;
		}
	}
	
	return 0;
}

Uint32 CMyFileTreeView::SelectChildNames(const Uint8 *inStr)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SMyFileItem *pFileItem = GetTreeItem(nSelected);
	if (!pFileItem || pFileItem->type != TB('fldr'))
		return 0;
	
	Disclose(nSelected);

	if (!GetChildTreeCount(nSelected))
		return 0;

	Uint32 i = nSelected + 1;
	pFileItem = GetTreeItem(i);
	if (!pFileItem)
		return 0;

	do
	{
		if (!UText::CompareInsensitive(inStr + 1, inStr[0], pFileItem->name + 1, pFileItem->name[0]))
		{
			DeselectAllTreeItems();
			SelectTreeItem(i);
			MakeTreeItemVisibleInList(i);	

			return i;
		}
		
	} while (GetNextTreeItem(pFileItem, i, true));

	return 0;
}

Uint32 CMyFileTreeView::SearchNames(const Uint8 *inStr)
{
	Uint8 searchStr[256];
	Uint8 str[256];
	
	UMemory::Copy(searchStr, inStr, inStr[0] + 1);
	UText::MakeLowercase(searchStr + 1, searchStr[0]);
	
	Uint32 i = 0;
	SMyFileItem *pFileItem;

	while (GetNextVisibleTreeItem(pFileItem, i))
	{
		UMemory::Copy(str, pFileItem->name, pFileItem->name[0] + 1);
		UText::MakeLowercase(str + 1, str[0]);
		
		if (UMemory::Search(searchStr + 1, searchStr[0], str + 1, str[0]))
			return i;
	}
	
	return 0;
}

// if a file is selected return parent path
void *CMyFileTreeView::RefreshSelectedFolderPath(Uint32& outPathSize)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	void *pPathData = GetFolderPath(nSelected, outPathSize);
	DeletePath(pPathData, outPathSize);

	if (nSelected)
	{
		SelectTreeItem(nSelected);
		
		if (GetDisclosure(nSelected) != optTree_Disclosed)
		{
			mRefreshFolderIndex = nSelected;
			SetDisclosure(nSelected, optTree_Disclosed);
		}
	}
		
	return pPathData;
}

// if a file is selected return parent folder path
void *CMyFileTreeView::GetSelectedFolderPath(Uint32& outPathSize)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	return GetFolderPath(nSelected, outPathSize);
}

void *CMyFileTreeView::GetSelectedParentFolderPath(Uint32& outPathSize)
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SMyFileItem *pFileItem = GetTreeItem(nSelected);
	if (pFileItem && pFileItem->type == TB('fldr'))
		nSelected = GetParentTreeIndex(nSelected);
	
	return GetFolderPath(nSelected, outPathSize);
}

bool CMyFileTreeView::GetDropFolderPath(void **outPathData, Uint32& outPathSize)
{
	*outPathData = nil;
	outPathSize = 0;
	
	if (mSavedFolderIndex)
	{
		*outPathData =  GetFolderPath(mSavedFolderIndex, outPathSize);
		mSavedFolderIndex = 0;
	}
	else if (*mPathData && *mPathSize)
	{
		outPathSize = *mPathSize;
		*outPathData = UMemory::New(outPathSize);
		UMemory::Copy(*outPathData, *mPathData, outPathSize);
	}
	
	return true;
}

// returns true if folder
bool CMyFileTreeView::GetSelectedItemName(Uint8 *outItemName, Uint32 *outTypeCode, Uint32 *outCreatorCode, const SMouseMsgData *inInfo)
{
	outItemName[0] = 0;
	if (outTypeCode) *outTypeCode = 0;
	if (outCreatorCode) *outCreatorCode = 0;

	Uint32 nSelected = GetFirstSelectedTreeItem();
	if (!nSelected)
		return false;
		
	SMyFileItem *pFileItem = GetTreeItem(nSelected);
	if (!pFileItem)
		return false;
	
	if (inInfo)
	{		
		SRect stBounds;
		if (!GetTreeItemRect(nSelected, stBounds))
			return false;
		
		Uint32 nItemLeft = stBounds.left + mIconIndent;
		Uint32 nItemRight = stBounds.left + mNameIndent + pFileItem->nameSize;

		if (inInfo->loc.x < nItemLeft || inInfo->loc.x > nItemRight)
			return false;
	}
	
	UMemory::Copy(outItemName, pFileItem->name, pFileItem->name[0] + 1);
	if (outTypeCode) *outTypeCode = pFileItem->type;
	if (outCreatorCode) *outCreatorCode = pFileItem->creator;
	
	return (pFileItem->type == TB((Uint32)'fldr') && pFileItem->creator == 0);
}

bool CMyFileTreeView::CollapseSelectedFolder()
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SMyFileItem *pFileItem = GetTreeItem(nSelected);
	if (!pFileItem || pFileItem->type != TB('fldr'))
		return false;
	
	return Collapse(nSelected);
}

bool CMyFileTreeView::DiscloseSelectedFolder()
{
	Uint32 nSelected = GetFirstSelectedTreeItem();
	
	SMyFileItem *pFileItem = GetTreeItem(nSelected);
	if (!pFileItem || pFileItem->type != TB('fldr'))
		return false;
	
	return Disclose(nSelected);
}

void CMyFileTreeView::DeletePath(const void *inPathData, Uint32 inPathSize)
{
	Uint32 nFolderIndex;
	if (!GetFolderIndex(inPathData, inPathSize, nFolderIndex))
		return;

	if (!nFolderIndex)
	{
		DeleteAll();
		return;
	}
	
	Uint32 i = nFolderIndex;
	SMyFileItem *pFileItem;
	Uint32 nFolderLevel = GetTreeItemLevel(nFolderIndex);
	
	while (GetNextTreeItem(pFileItem, i))
	{
		if (nFolderLevel >= GetTreeItemLevel(i))
			break;
		
		UIcon::Release(pFileItem->icon);
		UMemory::Dispose((TPtr)pFileItem);
	}

	RemoveChildTree(nFolderIndex);
}

void CMyFileTreeView::DeleteAll()
{
	Uint32 i = 0;
	SMyFileItem *pFileItem;
	
	while (GetNextTreeItem(pFileItem, i))
	{
		UIcon::Release(pFileItem->icon);
		UMemory::Dispose((TPtr)pFileItem);
	}
	
	ClearTree();
}

bool CMyFileTreeView::HasPartialFile(const Uint8 *inFileName, const void *inPathData, Uint32 inPathSize, bool inIsFolder)
{
	Uint32 nIndex;
	if (!GetFolderIndex(inPathData, inPathSize, nIndex))
		return false;

	if (!GetChildTreeCount(nIndex))
		return false;
	
	SMyFileItem *pFileItem = GetTreeItem(++nIndex);
	if (!pFileItem)
		return false;

	Uint32 nSearchNameSize = inFileName[0];
	
	do
	{
		if (inIsFolder)
		{
			if (pFileItem->type == TB((Uint32)'fldr') && pFileItem->creator == 0)
			{
				if (pFileItem->name[0] == nSearchNameSize && UMemory::Equal(pFileItem->name + 1, inFileName + 1, nSearchNameSize))
					return true;
			}
		}
		else if (pFileItem->type == TB((Uint32)'HTft') && pFileItem->creator == TB((Uint32)'HTLC'))
		{
			Uint8 psValidatedName[256];
			UMemory::Copy(psValidatedName, pFileItem->name, pFileItem->name[0] + 1);

			// take the .hpf out of the name
			if (psValidatedName[0] >= 4 && !UMemory::Compare(psValidatedName + psValidatedName[0] - 3, ".hpf", 4))
				psValidatedName[0] -= 4;
	
			if (psValidatedName[0] == nSearchNameSize && UMemory::Equal(psValidatedName + 1, inFileName + 1, nSearchNameSize))
				return true;
		}
	
	} while (GetNextTreeItem(pFileItem, nIndex, true));
	
	return false;
}

TFSRefObj* CMyFileTreeView::GetItemToUpload()
{
	TFSRefObj* fp = mUploadList.GetItem(1);
	if (fp) mUploadList.RemoveItem(1);
	return fp;
}

void CMyFileTreeView::ClearUploadList()		
{	
	Uint32 i = 0;
	TFSRefObj* fp;
	
	while (mUploadList.GetNext(fp ,i))
		delete fp;
	
	mUploadList.Clear();	
}

void CMyFileTreeView::DragEnter(const SDragMsgData& inInfo)
{
	CMyTreeStatusView::DragEnter(inInfo);
	
	mSavedFolderIndex = 0;	
	
	if (mDragAndDropHandler)
	{
		mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);	

		if (!IsMouseOnTab(inInfo.loc) && inInfo.drag->GetDragAction() != dragAction_None)
			SetDropFolderIndex(inInfo);
	}
}

void CMyFileTreeView::DragMove(const SDragMsgData& inInfo)
{
	CMyTreeStatusView::DragMove(inInfo);
	
	if (mDragAndDropHandler)
	{
		mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);		

		if (!IsMouseOnTab(inInfo.loc) && inInfo.drag->GetDragAction() != dragAction_None)
			SetDropFolderIndex(inInfo);
	}
}

void CMyFileTreeView::DragLeave(const SDragMsgData& inInfo)
{
	CMyTreeStatusView::DragLeave(inInfo);

	if (mDropFolderIndex)
	{
		mSavedFolderIndex = mDropFolderIndex;
		mDropFolderIndex = 0;
		
		RefreshTreeItem(mSavedFolderIndex);
	}
}

bool CMyFileTreeView::Drop(const SDragMsgData& inInfo)
{
	Uint16 i, c;
	Uint32 item, sa, sb;
	TDrag drag = inInfo.drag;
	TFSRefObj* fp;
	Uint8 buf[2048];
	bool accepted = false;
	bool uploadHit = false;
	
	if (!mDragAndDropHandler)
		return false;
	
	mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);
	Uint16 action = inInfo.drag->GetDragAction();
	
	if (action == dragAction_None)
		return false;
	
	c = drag->GetItemCount();
	for (i=1; i<=c; i++)
	{
		item = drag->GetItem(i);
		
		fp = drag->GetFileSysFlavor(item);
		if (fp)
		{
			try
			{
				if (fp->Exists())
				{
					accepted = true;
					
					if (!uploadHit)
					{
						Hit(hitType_Drop, 1, flavor_File);
						uploadHit = true;
					}
					
					mUploadList.AddItem(fp);
				}
				else
					delete fp;
			}
			catch(...)
			{
				delete fp;
				throw;
			}
		}
		else
		{
			sa = drag->GetFlavorData(item, 'HLFN', buf+1, 63);
			if (sa)
			{
				buf[0] = sa;
				sb = drag->GetFlavorData(item, 'HLFP', buf+sa+3, sizeof(buf)-sa-3);
				*(Uint16 *)(buf+sa+1) = sb;
				accepted = true;
				Hit(hitType_Drop, inInfo.mods, 'HLFN', buf, sa + sb + 3);
			}
		}
	}

	return accepted;
}

bool CMyFileTreeView::KeyDown(const SKeyMsgData& inInfo)
{
	if (!mTreeExpl && (inInfo.keyCode == key_Left || inInfo.keyCode == key_Right || inInfo.keyCode == key_Up || inInfo.keyCode == key_Down))
		mKeyDownTime = UDateTime::GetMilliseconds();
		
	return CMyTreeStatusView::KeyDown(inInfo);
}

void CMyFileTreeView::SelectionChanged(Uint32 inTreeIndex, SMyFileItem *inTreeItem, bool inIsSelected)
{
	#pragma unused(inTreeIndex, inTreeItem, inIsSelected)

	if (mTreeExpl)
		((CMyFileTreeWin*)GetDragAndDropHandler())->SetAccess();
	else
	{
		((CMyFileExplWin*)GetDragAndDropHandler())->SetAccess();
		
		if (UDateTime::GetMilliseconds() - mKeyDownTime > 500)
			((CMyFileExplWin*)GetDragAndDropHandler())->GetContentFileList();
		else
			((CMyFileExplWin*)GetDragAndDropHandler())->GetContentFileListTimer();
	}
}

void CMyFileTreeView::DisclosureChanged(Uint32 inTreeIndex, SMyFileItem *inTreeItem, Uint8 inDisclosure)
{
	#pragma unused(inTreeItem)
	
	if (inTreeIndex != mRefreshFolderIndex && inDisclosure == optTree_Disclosed && !GetChildTreeCount(inTreeIndex))
	{
		Uint32 nPathSize;
		void *pPathData = GetFolderPath(inTreeIndex, nPathSize);
		if (!pPathData)
			return;
		
		DeletePath(pPathData, nPathSize);
		
		if (mTreeExpl)
			((CMyFileTreeWin*)GetDragAndDropHandler())->GetFileContent(pPathData, nPathSize);
		else
			((CMyFileExplWin*)GetDragAndDropHandler())->GetFileContent(pPathData, nPathSize);

		UMemory::Dispose((TPtr)pPathData);
	}
	
	if (mRefreshFolderIndex)
		mRefreshFolderIndex = 0;
}

bool CMyFileTreeView::GetFolderIndex(const void *inPathData, Uint32 inPathSize, Uint32& outFolderIndex)
{
	outFolderIndex = 0;
	
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
		SMyFileItem *pFileItem = GetTreeItem(nIndex);
		if (!pFileItem)
			return false;
	
		bool bFound = false;
		do
		{
			if (pFileItem->type == TB('fldr') && pFileItem->name[0] == nSize && !UMemory::Compare(pFileItem->name + 1, p, nSize))
			{
				bFound = true;
				break;
			}
			
		} while (GetNextTreeItem(pFileItem, nIndex, true));
		
		if (!bFound)
			return false;
			
		p += nSize;
	}
	
	outFolderIndex = nIndex;
	return true;
}

// if inFolderIndex is a file index return parent folder path
void *CMyFileTreeView::GetFolderPath(Uint32 &ioFolderIndex, Uint32& outPathSize)
{
	outPathSize = 0;
	
	if (!ioFolderIndex)
	{
		if (*mPathData && *mPathSize)
		{
			outPathSize = *mPathSize;
			return UMemory::New(*mPathData, *mPathSize);
		}
		
		return nil;
	}
	
	SMyFileItem *pFileItem = GetTreeItem(ioFolderIndex);
	if (!pFileItem)
		return nil;
	
	if (pFileItem->type != TB('fldr'))
	{
		ioFolderIndex = GetParentTreeIndex(ioFolderIndex);
		if (!ioFolderIndex)
		{
			if (*mPathData && *mPathSize)
			{
				outPathSize = *mPathSize;
				return UMemory::New(*mPathData, *mPathSize);
			}
			
			return nil;
		}
	
		pFileItem = GetTreeItem(ioFolderIndex);
		if (!pFileItem)
			return nil;
	}
	
	Uint32 nIndex = ioFolderIndex;
	Uint32 nPathSize = 2;
	
	if (*mPathData && *mPathSize)
		nPathSize = *mPathSize;
	
	do
	{
		nPathSize += pFileItem->name[0] + 3;
		
	} while ((pFileItem = GetParentTreeItem(nIndex, &nIndex)) != nil);
	
	if (nPathSize < 6)
		return nil;
	
	void *pPathData = UMemory::NewClear(nPathSize);
	if (!pPathData)
		return nil;
	
	if (*mPathData && *mPathSize)
		UMemory::Copy(pPathData, *mPathData, *mPathSize);
	
	Uint8 *pEndPath = (Uint8 *)pPathData + nPathSize;

	nIndex = ioFolderIndex;
	pFileItem = GetTreeItem(nIndex);
	
	do
	{
		if (pEndPath - pFileItem->name[0] - 3 < (Uint8 *)pPathData + (*mPathSize ? *mPathSize : 2))
		{
			UMemory::Dispose((TPtr)pPathData);
			return nil;
		}
		
		pEndPath -= pFileItem->name[0] + 1;
		UMemory::Copy(pEndPath, pFileItem->name, pFileItem->name[0] + 1);
		pEndPath -= 2;
		
#if MACINTOSH
		(*(Uint16 *)pPathData)++;
#else
		*(Uint16 *)pPathData = TB( (Uint16)(FB(*(Uint16 *)pPathData) + 1));
#endif
	
	} while ((pFileItem = GetParentTreeItem(nIndex, &nIndex)) != nil);

	outPathSize = nPathSize;	
	return pPathData;
}

void CMyFileTreeView::SetDropFolderIndex(const SDragMsgData& inInfo)
{
	Uint32 nFolderIndex = mDropFolderIndex;
	mDropFolderIndex = PointToTreeItem(inInfo.loc);
			
	if (mDropFolderIndex)
	{
		SRect stBounds;
		SMyFileItem *pFileItem = GetTreeItem(mDropFolderIndex);
		
		if (pFileItem && GetTreeItemRect(mDropFolderIndex, stBounds))
		{	
			Uint32 nItemLeft = stBounds.left + mIconIndent;
			Uint32 nItemRight = stBounds.left + mNameIndent + pFileItem->nameSize;

			if (inInfo.loc.x >= nItemLeft && inInfo.loc.x <= nItemRight)
			{
				if (pFileItem->type != TB('fldr'))
					mDropFolderIndex = GetParentTreeIndex(mDropFolderIndex);
			}
			else
				mDropFolderIndex = 0;
		}
		else
			mDropFolderIndex = 0;
	}	
	
	if (mDropFolderIndex != nFolderIndex)
	{
		if (nFolderIndex)
			RefreshTreeItem(nFolderIndex);
		
		if (mDropFolderIndex)
			RefreshTreeItem(mDropFolderIndex);
	}
}

void CMyFileTreeView::ItemDraw(Uint32 inTreeIndex, Uint32 inTreeLevel, SMyFileItem *inTreeItem, STreeViewItem *inTreeViewItem, TImage inImage, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	#pragma unused(inTreeIndex, inTreeLevel, inOptions)
	
	SRect stRect;
	SColor stHiliteCol, stTextCol;
	Uint8 psText[256];
	
	// get info
	bool bIsActive = IsFocus() && mIsEnabled && inTreeViewItem->bIsSelected;
	bool bIsFolder = inTreeItem->type == TB((Uint32)'fldr') && inTreeItem->creator == 0;
	bool bIsBadAlias = inTreeItem->type == TB((Uint32)'alis');
	
	// set font
	inImage->SetFont(kDefaultFont, nil, 9);
	inImage->SetFontEffect(fontEffect_Bold);

	if (!inTreeItem->nameSize)
		inTreeItem->nameSize = inImage->GetTextWidth(inTreeItem->name + 1, inTreeItem->name[0]);
	
	// draw item icon and name
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{
		if (inTreeIndex == mDropFolderIndex)
		{
			UUserInterface::GetHighlightColor(&stHiliteCol);
			inImage->SetInkColor(stHiliteCol);

			// set rect
			stRect = *pBounds;
			stRect.top += 1;
			stRect.left += mNameIndent;
			stRect.right = stRect.left + inTreeItem->nameSize;
			if (stRect.right > pBounds->right)
				stRect.right = pBounds->right;

			// fill rect	
			if (stRect.right > stRect.left)
				inImage->FillRect(stRect);
		}
	
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += mIconIndent;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
			inTreeItem->icon->Draw(inImage, stRect, align_Center, (bIsActive || inTreeIndex == mDropFolderIndex) ? transform_Dark : transform_None);
	
		// determine color to draw name in
		if (bIsBadAlias)
			stTextCol = color_Gray7;
		else
			UUserInterface::GetSysColor((bIsActive || inTreeIndex == mDropFolderIndex) ? sysColor_InverseHighlight : sysColor_Label, stTextCol);

		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += mNameIndent;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
	
		// draw name
		inImage->SetInkColor(stTextCol);
		inImage->SetFontEffect(fontEffect_Bold);
		inImage->DrawTruncText(stRect, inTreeItem->name + 1, inTreeItem->name[0], 0, align_Left + align_CenterVert);
	}
		
	// draw size/count
	pBounds = inTabRectList.GetItem(2);
	if (pBounds && pBounds->GetWidth())
	{
		// determine color to draw size/count in
		if (bIsBadAlias)
			stTextCol = color_Gray7;
		else
			UUserInterface::GetSysColor(bIsActive ? sysColor_InverseHighlight : sysColor_Label, stTextCol);

		// set rect
		stRect = *pBounds;
		stRect.top += 3;
		stRect.bottom -= 2;
		stRect.left += 1;
		stRect.right -= 2;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
			
		if (bIsBadAlias)
		{
			psText[0] = 1;
			psText[1] = '-';
		}
		else if (bIsFolder)
		{    
			psText[0] = UText::IntegerToText(psText + 2, sizeof(psText) - 3, inTreeItem->fileSize) + 2;
			psText[1] = '(';
			psText[psText[0]] = ')';
		}
		else
			psText[0] = UText::SizeToText(inTreeItem->fileSize, psText + 1, sizeof(psText) - 1, kDontShowBytes);
	
		inImage->SetInkColor(stTextCol);
		inImage->SetFontEffect(fontEffect_Plain);
		inImage->DrawTruncText(stRect, psText + 1, psText[0], 0, align_Right | align_CenterVert);
	}
}

Int32 CMyFileTreeView::CompareNames(void *inItemA, void *inItemB, void *inRef)
{
	#pragma unused(inRef)
	
	if (!inItemA || !inItemB)
		return 0;
	
	Uint8 *pNameA = ((SMyFileItem *)inItemA)->name;
	Uint8 *pNameB = ((SMyFileItem *)inItemB)->name;

	Int32 nOutVal = UText::CompareInsensitive(pNameA + 1, pNameB + 1, min(pNameA[0], pNameB[0]));
	if (nOutVal == 0 && pNameA[0] != pNameB[0])
		nOutVal = pNameA[0] > pNameB[0] ? 1 : -1;
	
	return nOutVal;
}

// inType always uses mac byte ordering
Int16 HotlineFileTypeToIcon(Uint32 inType)
{
	Int16 id;
	
	switch (FB(inType))
	{
		case 'URL ':
		if (gApp->mIconSet == 2){
			id = 3020;
		}else if (gApp->mIconSet == 1){
			id = 233;
		}
			
			break;
		case 'fldr':
		if (gApp->mIconSet == 2){
			id = 3003;
		}else if (gApp->mIconSet == 1){
			id = 401;
		}
			
			break;
		case 'HTft':
		if (gApp->mIconSet == 2){
			id = 3046;
		}else if (gApp->mIconSet == 1){
			id = 402;
		}
			
			break;
		case 'SITD':
		case 'SIT!':
		case 'SIT5':
		//sit
		if (gApp->mIconSet == 2){
			id = 403;
		}else if (gApp->mIconSet == 1){
			id = 403;
		}
			
			break;
		case 'TEXT':
		if (gApp->mIconSet == 2){
			id = 3048;
		}else if (gApp->mIconSet == 1){
			id = 404;
		}
			
			break;
		case 'ttro':
		if (gApp->mIconSet == 2){
			id = 3045;
		}else if (gApp->mIconSet == 1){
			id = 405;
		}
			
			break;
		case 'PICT':
		case 'JPEG':
		case 'PNGf':
		
		case 'GIFf':
		case '8BPS':
		case 'BMP ':
		if (gApp->mIconSet == 2){
			id = 3054;
		}else if (gApp->mIconSet == 1){
			id = 406;
		}
			
			break;
		case 'DEXE':
		case 'APPL':
		if (gApp->mIconSet == 2){
			id = 3061;
		}else if (gApp->mIconSet == 1){
			id = 407;
		}
			
			break;
		case 'HTbm':
		case 'HTtb':
		case 'HTss':
		if (gApp->mIconSet == 2){
			id = 408;
		}else if (gApp->mIconSet == 1){
			id = 408;
		}
			
			break;
		case 'Seg1':
		case 'Seg2':
		case 'Seg3':
		case 'SegN':
		//broken sit
		if (gApp->mIconSet == 2){
			id = 409;
		}else if (gApp->mIconSet == 1){
			id = 409;
		}
			
			break;
		case 'alis':
		//point interrogation
		if (gApp->mIconSet == 2){
			id = 422;
		}else if (gApp->mIconSet == 1){
			id = 422;
		}
			
			break;
		
		case 'dImg':
		case 'rohd':
		//img disque
		if (gApp->mIconSet == 2){
			id = 423;
		}else if (gApp->mIconSet == 1){
			id = 423;
		}
			
			break;
		case 'Mp3 ':
		case 'MP3 ':
		case 'mp3!':
		case 'SwaT':
		case 'RAE ':
		case 'AIFF':
		case 'WAVE':
		case 'Sd2f':
		case 'DRM!':
		case 'ULAW':
		case 'MiDi':
		case 'YAMA':
		if (gApp->mIconSet == 2){
			id = 3056;
		}else if (gApp->mIconSet == 1){
			id = 424;
		}
			
			break;
		case 'MooV':
		case 'MPEG':
		case 'AVI ':
		case 'VfW ':
		if (gApp->mIconSet == 2){
			id = 3058;
		}else if (gApp->mIconSet == 1){
			id = 425;
		}
			
			break;
		case 'ZIP ':
		case 'RAR ':
		case 'ACE ':
		if (gApp->mIconSet == 2){
			id = 3053;
		}else if (gApp->mIconSet == 1){
			id = 426;
		}
			
			break;
		case 'SWFL':
		if (gApp->mIconSet == 2){
			id = 3050;
		}else if (gApp->mIconSet == 1){
			id = 427;
		}
			
			break;
		case 'JAR ':
		//.jar
		if (gApp->mIconSet == 2){
			id = 2061;
		}else if (gApp->mIconSet == 1){
			id = 2061;
		}
			
			break;
		case 'DLL ':
		if (gApp->mIconSet == 2){
			id = 4046;
		}else if (gApp->mIconSet == 1){
			id = 4046;
		}
			
			break;
		default:
		if (gApp->mIconSet == 2){
			id = 3059;
		}else if (gApp->mIconSet == 1){
			id = 400;
		}
			
			break;
	}
	
	return id;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyTaskListView::CMyTaskListView(CViewHandler *inHandler, const SRect& inBounds)
	: CListView(inHandler, inBounds)
{
	mCellHeight = 39;
	mRefreshSelectedOnSetActive = true;
}

CMyTaskListView::~CMyTaskListView()
{
	SMyTaskNameInfo *p = nil;
	Uint32 i = 0;
	
	while (mTaskList.GetNext(p, i))
		UMemory::Dispose((TPtr)p);
}

Uint32 CMyTaskListView::GetItemCount() const
{
	return mTaskList.GetItemCount();
}

void CMyTaskListView::AddTask(CMyTask *inTask, const Uint8 inText[])
{
	if (!inText) 
		return;
	
	SMyTaskNameInfo *info = nil;
	Uint8 *text = nil;
	
	try
	{
		info = (SMyTaskNameInfo *)UMemory::NewClear(sizeof(SMyTaskNameInfo));
		info->task = inTask;
		
		if (inText && inText[0])
		{
			info->textSize = inText[0];
			info->textData = text = (Uint8 *)UMemory::New(inText+1, inText[0]);
		}
		
		mTaskList.AddItem(info);
	
		ItemsInserted(mTaskList.GetItemCount(), 1);
	}
	catch (...)
	{
		mTaskList.RemoveItem(info);
		UMemory::Dispose((TPtr)info);
		UMemory::Dispose((TPtr)text);
		throw;
	}
}

void CMyTaskListView::RemoveTask(CMyTask *inTask)
{
	Uint32 i;
	SMyTaskNameInfo *info = TaskToInfo(inTask, &i);
	
	if (info)
	{
		UMemory::Dispose((TPtr)info->textData);
		UMemory::Dispose((TPtr)info);
		
		mTaskList.RemoveItem(i);
		
		ItemsRemoved(i, 1);
	}
}

void CMyTaskListView::SetTaskProgress(CMyTask *inTask, Uint32 inVal, Uint32 inMax, const Uint8 inDesc[])
{
	Uint32 i;
	SMyTaskNameInfo *info = TaskToInfo(inTask, &i);
	bool refresh = false;
	
	if (info)
	{
		if (info->progVal != inVal || info->progMax != inMax)
		{
			info->progVal = inVal;
			info->progMax = inMax;
			refresh = true;
		}
		
		if (inDesc)
		{
			UMemory::Dispose((TPtr)info->textData);
			info->textSize = 0;
			info->textData = nil;
			
			if (inDesc[0])
			{
				info->textData = (Uint8 *)UMemory::New(inDesc+1, inDesc[0]);
				info->textSize = inDesc[0];
			}
		
			refresh = true;
		}
		
		if (refresh) 
			RefreshItem(i);
	}
}

void CMyTaskListView::SetItemSelect(Uint32 inItem, bool inSelect)
{
	CListView::SetItemSelect(inItem, inSelect);
	
	gApp->mTasksWin->UpdateButtons();
}

void CMyTaskListView::ItemsRemoved(Uint32 inAtItem, Uint32 inCount)
{
	CListView::ItemsRemoved(inAtItem, inCount);
	
	gApp->mTasksWin->UpdateButtons();
}

void CMyTaskListView::ShowFinishedBar(CMyTask *inTask)
{
	Uint32 i;
	SMyTaskNameInfo *info = TaskToInfo(inTask, &i);
	
	if (info)
	{
		if ((info->progVal < info->progMax) || (info->progMax == 0))
		{
			info->progVal = 100;
			info->progMax = 100;
			RefreshItem(i);
		}
	}
}

SMyTaskNameInfo *CMyTaskListView::TaskToInfo(CMyTask *inTask, Uint32 *outIndex)
{
	Uint32 i = 0;
	SMyTaskNameInfo *info;
	
	while (mTaskList.GetNext(info, i))
	{
		if (info->task == inTask)
		{
			if (outIndex) *outIndex = i;
			return info;
		}
	}
	
	if (outIndex) *outIndex = 0;
	return nil;
}

CMyTask *CMyTaskListView::GetSelectedTask()
{
	Uint32 i = GetFirstSelectedItem();
	
	if (i)
	{
		SMyTaskNameInfo *info = mTaskList.GetItem(i);
		if (info) return info->task;
	}
	
	return nil;
}

Uint32 CMyTaskListView::GetFullHeight() const
{
	if (mHandler && !mTaskList.GetItemCount())
	{
		CScrollerView *pHandler = dynamic_cast<CScrollerView *>(mHandler);
		if (pHandler)
		{
			SRect stBounds;
			pHandler->GetBounds(stBounds);
			
			return stBounds.GetHeight() - 2;
		}		
	}

	return CListView::GetFullHeight();
}

void CMyTaskListView::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth)
{
	UGraphics::SetFont(inImage, kDefaultFont, nil, 9);
#if MACINTOSH
	inImage->SetInkMode(mode_Or);
#endif
	
	CListView::Draw(inImage, inUpdateRect, inDepth);
}

void CMyTaskListView::ItemDraw(Uint32 inItem, TImage inImage, const SRect& inBounds, const SRect& /* inUpdateRect */, Uint32 /* inOptions */)
{
	enum {
		borderSpace		= 4,
		innerSpace		= 4,
		textHeight		= 10,
		barHeight		= 13
	};
	
	bool isSelected = mSelectData.GetItem(inItem);
	SRect bounds, r;
	SColor hiliteCol;
	SMyTaskNameInfo *info;
	SProgressBarInfo prog;
	
	bool isActive = IsFocus() && mIsEnabled;

	// get info
	info = mTaskList.GetItem(inItem);
	
	// calc background rect
	bounds.top = inBounds.top + 1;
	bounds.bottom = inBounds.bottom - 2;
	bounds.left = inBounds.left + 1;
	bounds.right = inBounds.right - 1;
	
	// draw selection
	if (isSelected)
	{
		UUserInterface::GetHighlightColor(&hiliteCol);
		inImage->SetInkColor(hiliteCol);
		
		if (isActive)
			inImage->FillRect(bounds);
		else
			inImage->FrameRect(bounds);
	}
	
	// draw divider line between items
	r.top = bounds.bottom + 1;
	r.bottom = r.top + 1;
	r.left = inBounds.left;
	r.right = inBounds.right;
	inImage->SetInkColor(color_Black);
	inImage->FillRect(r);
	
	// draw light frame around this item
	r = inBounds;
	r.bottom--;
	UUserInterface::GetSysColor(sysColor_Light, hiliteCol);
	inImage->SetInkColor(hiliteCol);
	inImage->FrameRect(r);
	
	// draw name
	if (info->textData && info->textSize)
	{
		// determine color to draw text in
		UUserInterface::GetSysColor(isSelected && isActive ? sysColor_InverseHighlight : sysColor_Label, hiliteCol);

		r.top = bounds.top + borderSpace;
		r.bottom = r.top + textHeight;
		r.left = bounds.left + borderSpace + 1;
		r.right = bounds.right;
		inImage->SetInkColor(hiliteCol);
		UGraphics::DrawText(inImage, r, info->textData, info->textSize, 0, align_Left + align_CenterVert);
	}
	
	// draw progress bar
	r.top = bounds.top + borderSpace + textHeight + innerSpace;
	r.bottom = r.top + barHeight;
	r.left = bounds.left + borderSpace;
	r.right = bounds.right - borderSpace;
	prog.val = info->progVal;
	prog.max = info->progMax;
	prog.options = 0;
	UProgressBar::Draw(inImage, r, prog);
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyUserListView::CMyUserListView(CViewHandler *inHandler, const SRect& inBounds)
	: CMyListStatusView(inHandler, inBounds)
{
#if DRAW_USERLIST_ICONS
	mRefMessageIcon = UIcon::Load(226);
	mRefChatIcon = UIcon::Load(227);
	
	mIsDownloading = UIcon::Load(194);
	
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
		
	
	
		
	// set headers
	AddTab("\pName", inBounds.GetWidth() - 40, 60);
	AddTab("\pStatus");
#else
	AddTab("\pName");
#endif

	SetStatus(statTxt_UserlistNotOnline);
}

CMyUserListView::~CMyUserListView()
{
	Uint32 i = 0;
	SMyUserListItem *pUserItem;
		
	while (mUserList.GetNext(pUserItem, i))
	{
		UIcon::Release(pUserItem->icon);
		
		UMemory::Dispose((TPtr)pUserItem);
	}
	
	mUserList.Clear();
	
#if DRAW_USERLIST_ICONS
	UIcon::Release(mRefMessageIcon);
	UIcon::Release(mRefChatIcon);
	
	UIcon::Release(mIsDownloading);
	UIcon::Release(mNoPrivIcon);
	UIcon::Release(mAllPrivIcon);
	UIcon::Release(mDisconnectPrivIcon);
	UIcon::Release(mRegularPrivIcon);
#endif
}

void CMyUserListView::SetTabs(Uint8 inTabPercent1, Uint8 inTabPercent2)
{
	if (!inTabPercent1 && !inTabPercent2)
		return;
	
	CPtrList<Uint8> lPercentList;
	lPercentList.AddItem(&inTabPercent1);
	lPercentList.AddItem(&inTabPercent2);
	
	SetTabPercent(lPercentList);
	lPercentList.Clear();
}

void CMyUserListView::GetTabs(Uint8& outTabPercent1, Uint8& outTabPercent2)
{
	outTabPercent1 = GetTabPercent(1);
	outTabPercent2 = GetTabPercent(2);
}

Uint32 CMyUserListView::GetItemCount() const
{
	return mUserList.GetItemCount();
}

void CMyUserListView::AddListFromFields(TFieldData inData)
{
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

	startCount = mUserList.GetItemCount();
	//DebugBreak("%i",startCount);
	addCount = 0;
	n = inData->GetFieldCount();
	
	for (i=1; i<=n; i++)
	{
		if (inData->GetFieldID(i) == myField_UserNameWithInfo)
		{
			inData->GetFieldByIndex(i, &info, sizeof(infoBuf));
			
			s = FB(info.nameSize);
			if (s > 63) s = 63;
			
			item = (SMyUserListItem *)UMemory::New(sizeof(SMyUserListItem) + s + 1);
			item->icon = nil;
			try
			{
			
				item->icon = UIcon::Load(FB((Uint16)info.iconID) == 0 ? 414 : FB((Uint16)info.iconID));
				//MsgBox(142, FB((Uint16)info.iconID));
				item->id = FB(info.id);
				item->flags = FB(info.flags);
				item->name[0] = UMemory::Copy(item->name + 1, info.nameData, s);
				
				mUserList.AddItem(item);
				//DebugBreak("%s",item->name);
			}
			catch(...)
			{
				UIcon::Release(item->icon);
				UMemory::Dispose((TPtr)item);
				throw;
			}
			
			addCount++;
			
		}
	}

	ItemsInserted(startCount + 1, addCount);
	
}

// returns true if the user existed (returns false and does nothing if user not found)
bool CMyUserListView::UpdateUser(Uint16 inID, Int16 inIconID, Uint16 inFlags, const void *inNameData, Uint32 inNameSize, Uint8 *outOldName)
{
	SMyUserListItem **userList;
	SMyUserListItem *p;
	Uint32 i, n;
	SMyUserListItem *np;
	
	userList = mUserList.GetArrayPtr();
	n = mUserList.GetItemCount();
	
	for (i=0; i!=n; i++)
	{
		if (userList[i]->id == inID)	// if found existing user
		{
			p = userList[i];
			
			// copy old name out if necessary
			if (outOldName)
				outOldName[0] = UMemory::Copy(outOldName + 1, p->name + 1, p->name[0] > 31 ? 31 : p->name[0]);
			
			// create new info
			if (inNameSize > 63) inNameSize = 63;
			np = (SMyUserListItem *)UMemory::New(sizeof(SMyUserListItem) + inNameSize + 1);
			np->icon = UIcon::Load(inIconID);
			np->id = inID;
			np->flags = inFlags;
			np->name[0] = inNameSize;
			UMemory::Copy(np->name+1, inNameData, inNameSize);
			
			// store new info in same position
			mUserList.SetItem(i+1, np);
			
			// dispose old info
			UIcon::Release(p->icon);
			UMemory::Dispose((TPtr)p);
			
			// refresh item and return true because user already existed
			RefreshItem(i+1);
			return true;
		}
	}
	
	return false;
}

void CMyUserListView::AddUser(Uint16 inID, Int16 inIconID, Uint16 inFlags, const void *inNameData, Uint32 inNameSize)
{
	SMyUserListItem *np;
	
	if (inNameSize > 63) inNameSize = 63;
	
	np = (SMyUserListItem *)UMemory::New(sizeof(SMyUserListItem) + inNameSize + 1);
	np->icon = UIcon::Load(inIconID);
	np->id = inID;
	np->flags = inFlags;
	np->name[0] = inNameSize;
	UMemory::Copy(np->name+1, inNameData, inNameSize);
	
	mUserList.AddItem(np);
	ItemsInserted(mUserList.GetItemCount(), 1);
}

// returns whether or not the user existed (and has now been deleted)
bool CMyUserListView::DeleteUserByID(Uint16 inID, Uint8 *outName)
{
	SMyUserListItem **userList;
	SMyUserListItem *p;
	Uint32 i, n;
	
	userList = mUserList.GetArrayPtr();
	n = mUserList.GetItemCount();
	
	for (i=0; i!=n; i++)
	{
		if (userList[i]->id == inID)
		{
			p = userList[i];
			
			if (outName)
				outName[0] = UMemory::Copy(outName + 1, p->name + 1, p->name[0] > 31 ? 31 : p->name[0]);
			
			UIcon::Release(p->icon);
			UMemory::Dispose((TPtr)p);
			mUserList.RemoveItem(i+1);
			ItemsRemoved(i+1, 1);
			return true;
		}
	}
	
	return false;
}

bool CMyUserListView::GetUserByID(Uint16 inID, Uint8 *outName)
{
	SMyUserListItem **userList;
	SMyUserListItem *p;
	Uint32 i, n;
	
	userList = mUserList.GetArrayPtr();
	n = mUserList.GetItemCount();
	
	for (i=0; i!=n; i++)
	{
		if (userList[i]->id == inID)
		{
			p = userList[i];
			
			if (outName)
				outName[0] = UMemory::Copy(outName + 1, p->name + 1, p->name[0] > 31 ? 31 : p->name[0]);
			
			return true;
		}
	}
	
	return false;
}

void CMyUserListView::DeleteAll()
{
	Uint32 i = 0;
	SMyUserListItem *pUserItem;
		
	while (mUserList.GetNext(pUserItem, i))
	{
		UIcon::Release(pUserItem->icon);
		UMemory::Dispose((TPtr)pUserItem);
	}
	
	Uint32 nCount = mUserList.GetItemCount();
	
	mUserList.Clear();
	ItemsRemoved(1, nCount);
	
	SetStatus(statTxt_UserlistNotOnline);
}

Uint16 CMyUserListView::GetSelectedUserID(Uint8 *outName)
{
	Uint32 nSelected = GetFirstSelectedItem();
	
	if (nSelected)
	{
		SMyUserListItem *pUserItem = mUserList.GetItem(nSelected);
		
		if (pUserItem)
		{
			if (outName)
				outName[0] = UMemory::Copy(outName + 1, pUserItem->name + 1, pUserItem->name[0] > 31 ? 31 : pUserItem->name[0]);
			
			return pUserItem->id;
		}
	}
	
	return 0;
}

void CMyUserListView::GetRandomUserName(Uint8 *outName)
{
	SMyUserListItem *pUserItem = mUserList.GetItem(UMath::GetRandom(1, mUserList.GetItemCount()));
	
	if (pUserItem)
		outName[0] = UMemory::Copy(outName + 1, pUserItem->name + 1, pUserItem->name[0] > 31 ? 31 : pUserItem->name[0]);
	else
		outName[0] = 0;
}

void CMyUserListView::DragEnter(const SDragMsgData& inInfo)
{
	CView::DragEnter(inInfo);
	
	if (mDragAndDropHandler)
		mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);
}

void CMyUserListView::DragMove(const SDragMsgData& inInfo)
{
	CView::DragMove(inInfo);
	
	if (mDragAndDropHandler)
		mDragAndDropHandler->HandleSetDragAction((CSelectableItemsView *)this, inInfo);
}

void CMyUserListView::ItemDraw(Uint32 inListIndex, TImage inImage, const SRect& inBounds, const CPtrList<SRect>& inTabRectList, Uint32 inOptions)
{
	enum {
		nIconIndent = 10,
		nIconSpace = 4,
		nNameIndent = nIconIndent + 16 + 10
	};

	// get item
	SMyUserListItem *pUserListItem = mUserList.GetItem(inListIndex);
	if (!pUserListItem)
		return;
	
	SRect stRect;
	SColor stHiliteCol, stNameCol; 

	// get info
	bool bIsSelected = mSelectData.GetItem(inListIndex);
	bool bIsActive = IsFocus() && mIsEnabled;
	Uint16 nFlags = pUserListItem->flags;
	
	// set font if drawing for drag
	if (inOptions == kDrawItemForDrag)
	{
		UGraphics::SetFont(inImage, kDefaultFont, nil, 9);
#if MACINTOSH
		inImage->SetInkMode(mode_Or);
#endif
	}

	// draw selection
	if (bIsSelected)
	{
		// calc background rect
		if (inOptions == kDrawItemForDrag)
			stRect = inBounds;
		else
			stRect.Set(inBounds.left + 1, inBounds.top + 1, inBounds.right - 1, inBounds.bottom);

		UUserInterface::GetHighlightColor(&stHiliteCol);
		inImage->SetInkColor(stHiliteCol);

		if (bIsActive)
			inImage->FillRect(stRect);
		else
			inImage->FrameRect(stRect);
	}
	
	// draw light lines around this item
	if (inOptions != kDrawItemForDrag)
	{
		UUserInterface::GetSysColor(sysColor_Light, stHiliteCol);
		inImage->SetInkColor(stHiliteCol);
		stRect = inBounds; stRect.bottom++;
		inImage->FrameRect(stRect);
	}
	
	// draw item icon and name
	const SRect* pBounds = inTabRectList.GetItem(1);
	if (pBounds && pBounds->GetWidth())
	{	
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += nIconIndent;
		stRect.right = stRect.left + 16;
	
		// draw icon
		if (stRect.right < pBounds->right)
			pUserListItem->icon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
			
		// determine name color
		
		/*
		if ((nFlags & 0x01) && (nFlags & 0x02))	// if away admin
		{
			stNameCol.red = 0xEEEE;
			stNameCol.green = stNameCol.blue = 0x8888;
		}
		
		if ((nFlags & 0x01) && (nFlags & 0x40))	// if Invisible away
		{
		//MsgBox(142, nFlags);
			stNameCol.red = 0xEEE;
			stNameCol.green = stNameCol.blue = 0x8888;
			
		}
		
		
		
		else if (nFlags & 0x01)					// if away
		{
			stNameCol = color_Gray7;
		}
		*/
		/////////////////////////////
		if (nFlags & 0x01)					// if away
		{
			if (nFlags & 0x02){
				if(nFlags & 0x40){
				
					stNameCol.red = 0xEEE;
					stNameCol.green = stNameCol.blue = 0x8888;
					
				}else{
					stNameCol.red = 0xEEEE;
					stNameCol.green = stNameCol.blue = 0x8888;
				}
			
			}else if (nFlags & 0x40){
			
			stNameCol.red = 0xEEE;
			stNameCol.green = stNameCol.blue = 0x8888;
			
			}else{
			stNameCol = color_Gray7;
			}
		}
		
		///////////////////////////
		
		
		else if ((nFlags & 0x02))				// if admin
		{
			if(nFlags & 0x40){
				
					stNameCol.red = 0;
					stNameCol.green = stNameCol.blue = 0xDDDD;
					
				}else{
					stNameCol.red = 0xDDDD;
					stNameCol.green = stNameCol.blue = 0;
				}
			
		}
		
		
		else if ((nFlags & 0x40))				// if Invisible
		{
		//MsgBox(142, nFlags);
			stNameCol.red = 0;
			stNameCol.green = stNameCol.blue = 0xDDDD;
			
		}
		
		
		else
			UUserInterface::GetSysColor(bIsSelected && bIsActive ? sysColor_InverseHighlight : sysColor_Label, stNameCol);
	
		// set rect
		stRect = *pBounds;
		stRect.top += 1;
		stRect.left += nNameIndent;
		if (stRect.right < stRect.left)
			stRect.right = stRect.left;
	
		// draw name
		inImage->SetInkColor(stNameCol);
		inImage->SetFontEffect(fontEffect_Bold);
		inImage->DrawTruncText(stRect, pUserListItem->name + 1, pUserListItem->name[0], 0, align_Left + align_CenterVert);
	}

#if DRAW_USERLIST_ICONS
	// draw icons
	pBounds = inTabRectList.GetItem(2);
	if (pBounds && pBounds->GetWidth())
	{
		// set rect
		stRect = *pBounds;
		stRect.top += 2;
		stRect.bottom = stRect.top + 16;
		stRect.left += 2;
		stRect.right = stRect.left + 16;
		if (stRect.right > pBounds->right)
			return;

		if (nFlags & 0x08)		// if refuse private chat
			mRefChatIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);

		// set rect
		stRect.left = stRect.right + nIconSpace;
		stRect.right = stRect.left + 16;
		if (stRect.right > pBounds->right)
			return;

		if (nFlags & 0x04)		// if refuse private messages
			mRefMessageIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);

		// set rect
		stRect.left = stRect.right + nIconSpace;
		stRect.right = stRect.left + 16;
		if (stRect.right > pBounds->right)
			return;

		if (gApp->GetServerVersion() >= 184)	// only 1.8.4 server or better send additional user information
		{
			if (nFlags & 0x10)			// all privs
				mAllPrivIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
			else if (nFlags & 0x02)		// disconnect user priv
				mDisconnectPrivIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
			else if (nFlags & 0x20)		// no privs
				mNoPrivIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
			//else if (nFlags & 0x009)		// Download
				//mIsDownloading->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
			
			else						// regular privs
				mRegularPrivIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
		}
		else if (nFlags & 0x02)			// disconnect user priv
			mDisconnectPrivIcon->Draw(inImage, stRect, align_Center, bIsActive && bIsSelected ? transform_Dark : transform_None);
	}
#endif
}

Uint32 CMyUserListView::SearchNames(const Uint8 inStr[])
{
	Uint8 searchStr[256];
	Uint8 str[256];
	SMyUserListItem *p = nil;
	Uint32 i = 0;
	Uint32 s;
	
	UMemory::Copy(searchStr, inStr, inStr[0]+1);
	UText::MakeLowercase(searchStr+1, searchStr[0]);
	
	while (mUserList.GetNext(p, i))
	{
		s = p->name[0];
		if (s > sizeof(str)-1) s = sizeof(str)-1;
		UMemory::Copy(str+1, p->name+1, s);
		str[0] = s;
		
		UText::MakeLowercase(str+1, str[0]);
		
		if (UMemory::Search(searchStr+1, searchStr[0], str+1, str[0]))
			return i;
	}
	
	return 0;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMySubjectView::CMySubjectView(CViewHandler *inHandler, const SRect& inBounds)
	: CView(inHandler, inBounds)
{
	mText = nil;
	mIsHilited = false;
}

CMySubjectView::~CMySubjectView()
{
	UMemory::Dispose(mText);
}

void CMySubjectView::SetText(const void *inText, Uint32 inSize)
{
	if (mText == nil)
		mText = UMemory::NewHandle(inText, inSize);
	else
		UMemory::Set(mText, inText, inSize);
	
	Refresh();
}

void CMySubjectView::Draw(TImage inImage, const SRect& /* inUpdateRect */, Uint32 /* inDepth */)
{
	SColor col;
	SRect r;
	
	inImage->Reset();

	r = mBounds;
	UUserInterface::GetSysColor(sysColor_Frame, col);
	inImage->SetInkColor(col);
	inImage->FrameRect(r);
	
	r.Enlarge(-1);
	if (mIsHilited)
	{
	#if WIN32
		UUserInterface::GetSysColor(sysColor_Dark, col);
		inImage->SetInkColor(col);
		inImage->FillRect(r);
		//UUserInterface::GetSysColor(sysColor_Label, col);
		//inImage->SetInkColor(col);
		inImage->SetInkColor(color_White);
	#else
		inImage->SetInkColor(color_Gray5);
		inImage->FillRect(r);
		inImage->SetInkColor(color_White);
	#endif
	}
	else
	{
		UUserInterface::DrawRaisedBox(inImage, r);
		UUserInterface::GetSysColor(sysColor_Label, col);
		inImage->SetInkColor(col);
	}
	
	r.left += 10;
	r.top--;
	r.right--;
	inImage->SetFont(kDefaultFont, nil, 9, fontEffect_Bold);
	inImage->DrawText(r, "Subject:", 8, 0, align_Left + align_CenterVert);
	
	if (mText)
	{
		r.left += 60;
		void *p;
		StHandleLocker locker(mText, p);
		inImage->DrawTruncText(r, p, UMemory::GetSize(mText), 0, align_Left + align_CenterVert);
	}
}

void CMySubjectView::MouseDown(const SMouseMsgData& inInfo)
{
	CView::MouseDown(inInfo);
	
	if (IsEnabled() && IsActive())
	{
		mIsHilited = true;
		Refresh();
	}
}

void CMySubjectView::MouseUp(const SMouseMsgData& inInfo)
{
	CView::MouseUp(inInfo);
	
	if (mIsHilited)
	{
		mIsHilited = false;
		Refresh();
		
		if (IsMouseWithin())
			Hit((inInfo.mods & modKey_Option) ? hitType_Alternate : hitType_Standard);
	}
}

void CMySubjectView::MouseEnter(const SMouseMsgData& inInfo)
{
	CView::MouseEnter(inInfo);
	
	if (!mIsHilited && IsAnyMouseBtnDown() && IsEnabled() && IsActive())
	{
		mIsHilited = true;
		Refresh();
	}
}

void CMySubjectView::MouseLeave(const SMouseMsgData& inInfo)
{
	CView::MouseLeave(inInfo);
	
	if (mIsHilited)
	{
		mIsHilited = false;
		Refresh();
	}
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyClickPicView::CMyClickPicView(CViewHandler *inHandler, const SRect& inBounds, Int32 inPictID)
	: CPictureView(inHandler, inBounds, inPictID)
{

}

void CMyClickPicView::MouseUp(const SMouseMsgData& inInfo)
{
	#pragma unused(inInfo)
	Hit();
}

CWindow *MakeClickPicWin(const SRect& inBounds, Int32 inID, const Uint8 *inTxt)
{
	CWindow *win = new CWindow(inBounds, windowLayer_Modal, 0, 1);
	
	if (inTxt)
	{
		SRect r(0, 0, inBounds.GetWidth(), inBounds.GetHeight());
		CContainerView *vc = new CContainerView(win, r);
		vc->Show();
		CMyClickPicView *cpv = new CMyClickPicView(vc, r, inID);
		cpv->Show();
		
		r.left += 66;
		r.top += 174;
		r.bottom = r.top + 16;
		
		CLabelView *lbl = new CLabelView(vc, r, inTxt);
		//TFontDesc fd = fd_Default9->Clone();
		//TFontDesc fd = fd_Default9Bold->Clone();
		TFontDesc fd = fd_Default10->Clone();
		
		fd->SetLock(false);
		//fd->SetColor(color_White);
		fd->SetColor(14144727);
		lbl->SetFont(fd);
		lbl->Show();
	}
	else
	{
		CMyClickPicView *cpv = new CMyClickPicView(win, SRect(0, 0, inBounds.GetWidth(), inBounds.GetHeight()), inID);
		cpv->Show();
	}

	return win;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyConnectedInfoView::CMyConnectedInfoView(CViewHandler *inHandler, const SRect& inBounds)
	: CView(inHandler, inBounds)
{
	mText[0] = 0;
	mAltText[0] = 0;
	
	mTimer = nil;
	mFont = nil;
}

CMyConnectedInfoView::CMyConnectedInfoView(CViewHandler *inHandler, const SRect& inBounds, const Uint8 *inText, TFontDesc inFont)
	: CView(inHandler, inBounds)
{
	mText[0] = 0;
	mAltText[0] = 0;
	
	mTimer = nil;
	mFont = inFont;
	
	if (inText && inText[0])
	{
		UMemory::Copy(mText, inText, inText[0] + 1);
		SetTooltipMsg(mText);
	}
}

CMyConnectedInfoView::~CMyConnectedInfoView()
{
	if (mFont)
		delete mFont;
		
	if (mTimer)
		delete mTimer;
}

void CMyConnectedInfoView::SetText(const Uint8 *inText)
{
	if (inText && inText[0])
		UMemory::Copy(mText, inText, inText[0] + 1);
	else
		mText[0] = 0;
		
	SetTooltipMsg(mText);
	
	// if we aren't waiting for timer to expire, set the visible text to this right away
	if (!mTimer)
		Refresh();
}

Uint32 CMyConnectedInfoView::GetText(void *outText, Uint32 inMaxSize)
{
	if (!outText || !inMaxSize)
		return 0;
		
	return UMemory::Copy(outText, mText + 1, mText[0] > inMaxSize ? inMaxSize : mText[0]);
}

void CMyConnectedInfoView::SetAltText(const Uint8 *inText)
{
	if (inText && inText[0])
		UMemory::Copy(mAltText, inText, inText[0] + 1);
	else
		mAltText[0] = 0;
}

Uint32 CMyConnectedInfoView::GetAltText(void *outText, Uint32 inMaxSize)
{
	if (!outText || !inMaxSize)
		return 0;
		
	return UMemory::Copy(outText, mAltText + 1, mAltText[0] > inMaxSize ? inMaxSize : mAltText[0]);
}

void CMyConnectedInfoView::ShowAltText()
{
	if (!mAltText[0])
		return;
	
	if (mTimer)
	{
		mTimer->Simulate();
		return;
	}
	
	mTimer = StartNewTimer(2000);
	Refresh();
}

void CMyConnectedInfoView::SetFont(TFontDesc inFont)
{
	if (mFont)
		delete mFont;
	
	mFont = inFont;
}

void CMyConnectedInfoView::Timer(TTimer inTimer)
{
	if (!inTimer)	// shouldn't need this, but check in case mTimer == nil and inTimer == nil
		return;
		
	if (inTimer == mTimer)
	{
		delete mTimer;
		mTimer = nil;
		
		Refresh();
	}
}

void CMyConnectedInfoView::MouseDown(const SMouseMsgData& inInfo)
{
	CView::MouseDown(inInfo);
	
	Hit();
}

void CMyConnectedInfoView::Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth)
{
	CView::Draw(inImage, inUpdateRect, inDepth);
	UUserInterface::DrawSunkenBox(inImage, mBounds);
	
	inImage->SetFont(mFont);
	
	void *pText;
	Uint32 nSize;
	
	if (mTimer)
	{
		pText = mAltText + 1;
		nSize = mAltText[0];
	}
	else
	{
		pText = mText + 1;
		nSize = mText[0];
	}
	
	inImage->DrawTextBox(SRect(mBounds.left + 3, mBounds.top + 1, mBounds.right - 3, mBounds.bottom - 1), inUpdateRect, pText, nSize, 0, mFont->GetAlign());
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyImageView::CMyImageView(CViewHandler *inHandler, const SRect& inBounds, CDecompressImage *inDecompressImage)
	: CImageView(inHandler, inBounds, inDecompressImage)
{
}

bool CMyImageView::LaunchURL()
{
	if (!mURL)
		return false;

	new CMyLaunchUrlTask(mURL);	
	return true;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyAnimatedGifView::CMyAnimatedGifView(CViewHandler *inHandler, const SRect& inBounds, CDecompressGif *inDecompressGif)
	: CAnimatedGifView(inHandler, inBounds, inDecompressGif)
{
}

bool CMyAnimatedGifView::LaunchURL()
{
	if (!mURL)
		return false;
		
	new CMyLaunchUrlTask(mURL);
	return true;
}

/* ÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑÑ */
#pragma mark -

CMyQuickTimeView::CMyQuickTimeView(CViewHandler *inHandler, const SRect& inBounds, Uint16 inResizeOptions, Uint16 inOptions)
	: CQuickTimeView(inHandler, inBounds, inResizeOptions, inOptions)
{
	mSaveFileWin = nil;
}

void CMyQuickTimeView::SetSaveFileWin(CMySaveFileWin *inSaveFileWin)
{
	mSaveFileWin = inSaveFileWin;
}

bool CMyQuickTimeView::SaveMovieAs()
{
	if (mSaveFileWin)
		return mSaveFileWin->SaveFileAs();
		
	return false;
}

bool CMyQuickTimeView::LaunchURL()
{
	if (!mURL)
		return false;
		
	new CMyLaunchUrlTask(mURL);
	return true;
}

