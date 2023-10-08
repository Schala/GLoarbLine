/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "MsgBox.h"
#include "USound.h"
#include "CWindow.h"
#include "CLabelView.h"
#include "CIconView.h"
#include "CButtonView.h"
#include "CPictureView.h"
#include "CContainerView.h"

#include "stdarg.h"

/* -------------------------------------------------------------------------- */

/*
 * MsgBox() creates and displays a modal message box window according to the
 * specified 'MSGB' resource, and returns the number of the button that the user
 * hit to dismiss the window.  Note that MsgBox() does not release the 'MSGB'
 * resource, so it should be marked purgeable.
 */
Uint16 MsgBox(Int32 inID, ...)
{
	CWindow *win;
	Int32 sound;
	Uint32 hitID;
	
	va_list args;
	va_start(args, inID);
	
	win = MakeMsgBoxWin(inID, &sound, args);
	
	try
	{
		// play sound
		if (sound)
		{
			if (sound == 1)
				USound::Beep();
			else
				USound::Play(nil, sound, false);
		}
		
		hitID = TrackMsgBoxWin(win);
	}
	catch(...)
	{
		delete win;
		throw;
	}
	
	delete win;

	return hitID;
}

Uint16 MsgBox(const SMsgBox& inInfo)
{
	CWindow *win;
	Uint16 btn;
	
	win = MakeMsgBoxWin(inInfo);
	
	try
	{
		// play sound
		if (inInfo.sound)
		{
			if (inInfo.sound == 1)
				USound::Beep();
			else
				USound::Play(nil, inInfo.sound, false);
		}

		btn = TrackMsgBoxWin(win);
	}
	catch(...)
	{
		delete win;
		throw;
	}
	
	delete win;
	
	return btn;
}

CWindow *MakeMsgBox(const SMsgBox& inInfo)
{
	CWindow *win = MakeMsgBoxWin(inInfo);
	
	try
	{
		// play sound
		if (inInfo.sound)
		{
			if (inInfo.sound == 1)
				USound::Beep();
			else
				USound::Play(nil, inInfo.sound, false);
		}
	}
	catch(...)
	{
		// don't throw
	}
	
	return win;
}

CWindow *MakeMsgBox(const Uint8 *inTitle, const Uint8 *inMsg, Int16 inIcon, Int16 inSound, bool inShowTextView)
{
	SMsgBox stInfo;
	ClearStruct(stInfo);
	
	stInfo.icon = inIcon;
	stInfo.sound = inSound;
	stInfo.title = inTitle;
	stInfo.messageSize = inMsg[0];
	stInfo.messageData = inMsg + 1;
	stInfo.showTextView = inShowTextView;

	return MakeMsgBox(stInfo);
}

CWindow *MakeMsgBox(const Uint8 *inTitle, const void *inMsg, Uint16 inMsgSize, Uint32 inFontSize, Int16 inIcon, Int16 inSound, bool inShowTextView)
{
	SMsgBox stInfo;
	ClearStruct(stInfo);
	
	stInfo.icon = inIcon;
	stInfo.sound = inSound;
	stInfo.title = inTitle;
	stInfo.messageData = inMsg;
	stInfo.messageSize = inMsgSize;
	stInfo.fontSize = inFontSize;
	stInfo.showTextView = inShowTextView;

	return MakeMsgBox(stInfo);
}

Uint16 TrackMsgBoxWin(CWindow *inWin)
{
	Uint32 hitID;
	
	Require(inWin);
	
	inWin->Show();
	
	for(;;)
	{
		inWin->ProcessModal();
		
		hitID = inWin->GetHitID();
		
		if (hitID == 1 || hitID == 2 || hitID == 3)
			break;
	}
	
	return hitID;
}

#pragma mark -

char **_GetMacRes(Uint32 inType, Int16 inID);
void *_LoadMacRes(Uint32 inType, Int16 inID, Uint32 *outStateData, Uint32 *outSize = nil);
void _ReleaseMacRes(Uint32 *inStateData);

CWindow *MakeMsgBoxWin(Int32 inID, Int32 *outSound, void *inArgs)
{
#if MACINTOSH

	CWindow *win;	
	Uint8 buf[256];
	
	SMsgBox info;
	ClearStruct(info);
	
	Uint32 resState[4];
	Uint8 *p = (Uint8 *)_LoadMacRes('MSGB', inID, resState);

	try
	{
		UMemory::Copy(&info, p, OFFSET_OF(SMsgBox, title));
		info.title = (Uint8 *)(p + OFFSET_OF(SMsgBox, title));

		const Uint8 *message = info.title + info.title[0] + 1;
		info.messageSize = message[0];
		info.messageData = message + 1;

		info.button1 = message + message[0] + 1;
		info.button2 = info.button1 + info.button1[0] + 1;
		info.button3 = info.button2 + info.button2[0] + 1;
		
		if (outSound)
			*outSound = info.sound;
		
		if (inArgs)
		{
			Int8 fmt[256];
			UMemory::Copy(fmt, info.messageData, info.messageSize);
			fmt[info.messageSize] = 0;
			
			buf[0] = UText::FormatArg(buf+1, sizeof(buf)-1, fmt, inArgs);
			info.messageSize = buf[0];
			info.messageData = buf + 1;
		}
		
		win = MakeMsgBoxWin(info);
	}
	catch(...)
	{
		_ReleaseMacRes(resState);
		throw;
	}
	_ReleaseMacRes(resState);
	
	return win;

#else
	
	TRez rz = URez::SearchChain('MSGB', inID);
	if (rz == nil) Fail(errorType_Rez, rezError_NoSuchResource);
	
	THdl h;
	StRezLoader rzLoad(rz, 'MSGB', inID, h);
	
	CWindow *win;
	Uint8 buf[256];

	SMsgBox info;
	ClearStruct(info);
	
	{
		void *p;
		StHandleLocker lock(h, p);
		CUnflatten unflat(p, h->GetSize());
	
		if (unflat.NotEnufData(12)) Fail(errorType_Misc, error_Corrupt);
	
		info.icon = unflat.ReadWord();
		info.picture = unflat.ReadWord();
		info.sound = unflat.ReadWord();
		info.textStyle = unflat.ReadWord();
		info.btnTextStyle = unflat.ReadWord();
		info.script = unflat.ReadWord();
		info.title = unflat.ReadPString();
		
		const Uint8 *message = unflat.ReadPString();
		info.messageSize = message[0];
		info.messageData = message + 1;
		
		info.button1 = unflat.ReadPString();
		info.button2 = unflat.ReadPString();
		info.button3 = unflat.ReadPString();
		
		if (outSound)
			*outSound = info.sound;
		
		if (inArgs)
		{
			Int8 fmt[256];
			UMemory::Copy(fmt, info.messageData, info.messageSize);
			fmt[info.messageSize] = 0;
			
			buf[0] = UText::FormatArg(buf+1, sizeof(buf)-1, fmt, inArgs);
			info.messageSize = buf[0];
			info.messageData = buf + 1;
		}
	
		win = MakeMsgBoxWin(info);
	}
	
	return win;
	
#endif
}

CWindow *MakeMsgBoxWin(const SMsgBox& inInfo)
{
	const Uint8 kDefaultButtonTitle[] = "\pOK";
	const Uint16 kButtonWidth					= 84;
	const Uint16 kButtonHeight					= 20;
	const Uint16 kDefButtonExtraSize			= 3;
	const Uint16 kTextWidth						= 275;
	const Uint16 kMinTextHeight					= 80;
	const Uint16 kMaxTextHeight					= 150;
	const Uint16 kSpaceBetweenBtns				= 16;
	const Uint16 kSpaceBetweenIconAndContent	= 18;
	const Uint16 kSpaceBetweenContentAndBtns	= 10;
	const Uint16 kWindowBorderSpace				= 10;
	const Uint16 kMinContentHeight				= 50;
	
	CWindow *win;
	CButtonView *vBtn1, *vBtn2, *vBtn3;
	CIconView *vIcon;
	CLabelView *vLabel;
	CScrollerView *vScroll;
	CTextView *vText;
	CPictureView *vPict;
	CContainerView *vCon;
	SRect rWin, rContent, rIcon, rBtn1, rBtn2, rBtn3;
	TImage img;
	Int32 btnTop, btnBottom;
	const Uint8 *btn1Title, *btn2Title, *btn3Title;

	// calc button titles
	btn1Title = inInfo.button1 && inInfo.button1[0] ? inInfo.button1 : kDefaultButtonTitle;
	btn2Title = inInfo.button2 && inInfo.button2[0] ? inInfo.button2 : nil;
	btn3Title = inInfo.button3 && inInfo.button3[0] ? inInfo.button3 : nil;
	if (btn2Title == nil && btn3Title)
		btn3Title = nil;
	
	// calc icon rect
	if (inInfo.icon)
	{
		rIcon.top = kWindowBorderSpace;
		rIcon.left = kWindowBorderSpace;
		rIcon.bottom = rIcon.top + 32;
		rIcon.right = rIcon.left + 32;
	}
	
	// calc content rect
	Uint32 nBoxHeight = 0;
	rContent.top = kWindowBorderSpace;
	rContent.left = inInfo.icon ? rIcon.right + kSpaceBetweenIconAndContent : kWindowBorderSpace;
	if (inInfo.messageData && inInfo.messageSize)
	{
		// text content
		rContent.right = rContent.left + kTextWidth;
		rContent.bottom = 10000;
		img = UGraphics::GetDummyImage();
		img->SetFont((TFontDesc)nil);

		nBoxHeight = img->GetTextBoxHeight(rContent, inInfo.messageData, inInfo.messageSize, 0, 0) + 6;
		
		if (nBoxHeight <= kMaxTextHeight)
		{
			if (inInfo.showTextView)
			{
				if (nBoxHeight <= kMinTextHeight)
					rContent.bottom = rContent.top + kMinTextHeight;
				else
					rContent.bottom = rContent.top + kMaxTextHeight;
			}
			else
				rContent.bottom = rContent.top + nBoxHeight;
		}
		else
			rContent.bottom = rContent.top + kMaxTextHeight;
	}
	else if (inInfo.picture)
	{
		// picture content
#if MACINTOSH
		TPicture pic = (TPicture)_GetMacRes('PICT', inInfo.picture);
		rContent.bottom = rContent.top + UGraphics::GetPictureHeight(pic);
		rContent.right = rContent.left + UGraphics::GetPictureWidth(pic);
#else
		rContent.bottom = rContent.top + 100;
		rContent.right = rContent.left + 100;
#endif
	}
	else
		Fail(errorType_Misc, error_Param);
	
	// calc button top and bottom
	if (rContent.GetHeight() < kMinContentHeight)
		btnTop = kWindowBorderSpace + kMinContentHeight + kSpaceBetweenContentAndBtns;
	else
		btnTop = rContent.bottom + kSpaceBetweenContentAndBtns;
		
	btnBottom = btnTop + kButtonHeight;

	// calc button 1 rect (default)
	rBtn1.top = btnTop - kDefButtonExtraSize;
	rBtn1.bottom = btnBottom + kDefButtonExtraSize;
	rBtn1.right = rContent.right;
	rBtn1.left = rBtn1.right - kDefButtonExtraSize - kButtonWidth - kDefButtonExtraSize;
	if (inInfo.icon == 0 && btn2Title == nil)						// if no icon and one button
		rBtn1.CenterHoriz(rContent);
	
	// calc button 2 rect
	if (btn2Title)
	{
		rBtn2.top = btnTop;
		rBtn2.bottom = btnBottom;
		rBtn2.right = rBtn1.left - kSpaceBetweenBtns;
		rBtn2.left = rBtn2.right - kButtonWidth;
	}
	
	// calc button 3 rect
	if (btn3Title)
	{
		rBtn3.top = btnTop;
		rBtn3.bottom = btnBottom;
		rBtn3.right = rBtn2.left - kSpaceBetweenBtns;
		rBtn3.left = rBtn3.right - kButtonWidth;
	}
	
	// calc window rect
	rWin.top = rWin.left = 0;
	rWin.bottom = btnBottom + kWindowBorderSpace;
	rWin.right = rContent.right + kWindowBorderSpace;
	
	// create window
	win = new CWindow(rWin, windowLayer_Modal);
	
	try
	{
		// setup window
		win->SetAutoBounds(windowPos_Alert, windowPosOn_WinScreen);
		if (inInfo.title) win->SetTitle(inInfo.title);
		
		// create view container
		vCon = new CContainerView(win, rWin);
		vCon->Show();
		
		// create icon
		if (inInfo.icon)
		{
			vIcon = new CIconView(vCon, rIcon);
			vIcon->SetIconID(inInfo.icon);
			vIcon->Show();
		}
		
		// create content
		if (inInfo.messageData && inInfo.messageSize)
		{
			// text content
			if (!inInfo.showTextView && nBoxHeight <= kMaxTextHeight)	// display the message in a label view
			{
				vLabel = new CLabelView(vCon, rContent);
				//vLabel->SetTextStyle(inInfo.textStyle);
				vLabel->SetText(inInfo.messageData, inInfo.messageSize);
				vLabel->SetID(4);
				vLabel->Show();
			}
			else													// display the message in a text view
			{
				vScroll = MakeTextBoxView(vCon, rContent, scrollerOption_Border + scrollerOption_VertBar, &vText);

				vText->SetFont(kFixedFont, nil, inInfo.fontSize);
				vText->SetEditable(false);
				vText->SetEnterKeyAction(enterKeyAction_None, enterKeyAction_None);
				vText->SetTabSelectText(false);
				vText->SetText(inInfo.messageData, inInfo.messageSize);
				vText->SetID(4);
			
				vScroll->Show();
			}
		}
		else
		{
			// picture content
			vPict = new CPictureView(vCon, rContent);
			vPict->SetPicture(inInfo.picture);
			vPict->Show();
		}
		
		// create button 1
		vBtn1 = new CButtonView(vCon, rBtn1);
		vBtn1->SetDefault(true);
		//vBtn1->SetTitleStyle(inInfo.btnTextStyle);
		vBtn1->SetTitle(btn1Title);
		vBtn1->Show();
		vBtn1->SetID(1);
		
		// create button 2
		if (btn2Title)
		{
			vBtn2 = new CButtonView(vCon, rBtn2);
			//vBtn2->SetTitleStyle(inInfo.btnTextStyle);
			vBtn2->SetTitle(btn2Title);
			vBtn2->Show();
			vBtn2->SetID(2);
		}
		else
			vBtn2 = nil;
		
		// create button 3
		if (btn3Title)
		{
			vBtn3 = new CButtonView(vCon, rBtn3);
			//vBtn3->SetTitleStyle(inInfo.btnTextStyle);
			vBtn3->SetTitle(btn3Title);
			vBtn3->Show();
			vBtn3->SetID(3);
		}
		else
			vBtn3 = nil;
		
		// determine default and cancel buttons
		vCon->SetDefaultView(vBtn1);
		if (vBtn3)
			vCon->SetCancelView(vBtn3);
		else if (vBtn2)
			vCon->SetCancelView(vBtn2);
	}
	catch(...)
	{
		// clean up
		delete win;
		throw;
	}

	// return cool message box window!
	return win;
}

#pragma mark -

void DisplayMessage(const Uint8 *inTitle, const Uint8 *inMsg, Int16 inIcon, Int16 inSound, bool inShowTextView)
{
	SMsgBox stInfo;
	ClearStruct(stInfo);
	
	stInfo.icon = inIcon;
	stInfo.sound = inSound;
	stInfo.title = inTitle;
	stInfo.messageSize = inMsg[0];
	stInfo.messageData = inMsg + 1;
	stInfo.showTextView = inShowTextView;
	
	MsgBox(stInfo);
}

void DisplayMessage(const Uint8 *inTitle, const void *inMsg, Uint16 inMsgSize, Uint32 inFontSize, Int16 inIcon, Int16 inSound, bool inShowTextView)
{
	SMsgBox stInfo;
	ClearStruct(stInfo);
	
	stInfo.icon = inIcon;
	stInfo.sound = inSound;
	stInfo.title = inTitle;
	stInfo.messageData = inMsg;
	stInfo.messageSize = inMsgSize;
	stInfo.fontSize = inFontSize;
	stInfo.showTextView = inShowTextView;
	
	MsgBox(stInfo);
}

