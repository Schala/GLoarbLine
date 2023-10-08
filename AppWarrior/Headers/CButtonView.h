/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"
#include "CView.h"
#include "CContainerView.h"

enum {
	kNotDefaultButton,
	kDefaultButton
};

class CButtonView;

struct SButtons {
	Uint32 id;
	Uint8 *title;
	Uint32 options;
	CButtonView **outBtn;
};

// options
enum {
	btnOpt_CommandID	= 0x01,
	btnOpt_Regular		= 0x02,
	btnOpt_Default		= 0x04,
	btnOpt_Cancel		= 0x08
};


class CButtonView : public CView
{
	public:
		// construction
		CButtonView(CViewHandler *inHandler, const SRect& inBounds);
		CButtonView(CViewHandler *inHandler, const SRect& inBounds, Uint32 inCmdID, const Uint8 *inTitle, Uint32 inDefault = false, TFontDesc inFont = nil);
		virtual ~CButtonView();
		
		// title
		virtual void SetTitle(const void *inText, Uint32 inSize);
		void SetTitle(const Uint8 *inText)						{	SetTitle(inText+1, inText[0]);							}
		Uint32 GetTitle(void *outText, Uint32 inMaxSize) const	{	return UMemory::Read(mTitle, 0, outText, inMaxSize);	}
		virtual void SetFont(TFontDesc inFont);

		// default outline
		virtual void SetDefault(bool inDefault);
		bool IsDefault() const									{	return mIsDefault;										}
		
		// hilite status
		bool IsHilited() const									{	return mIsHilited;										}
		bool IsFlashHilited() const								{	return mIsFlashHilited;									}
		virtual void Flash();
		
		// misc
		virtual void Timer(TTimer inTimer);
		virtual bool ChangeState(Uint16 inState);
		virtual bool SetEnable(bool inEnable);
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);
		virtual void MouseDown(const SMouseMsgData& inInfo);
		virtual void MouseUp(const SMouseMsgData& inInfo);
		virtual void MouseEnter(const SMouseMsgData& inInfo);
		virtual void MouseLeave(const SMouseMsgData& inInfo);
		virtual bool KeyDown(const SKeyMsgData& inInfo);
				
		static bool BuildButton(CContainerView *inHandler, const SRect& inBounds, SButtons *inButton);
		static bool BuildButtons(CContainerView *inHandler, const SRect& inBounds, SButtons *inButtons);
		
	protected:
		THdl mTitle;
		TFontDesc mFont;
		TTimer mFlashTimer;
		Uint8 mIsDefault		: 1;
		Uint8 mIsFocused		: 1;
		Uint8 mIsHilited		: 1;
		Uint8 mIsFlashHilited	: 1;
		Uint8 mIsAltHit			: 1;
		Uint8 mIsDrawDisabled	: 1;
		
		// internal functions
		void UpdateActive();
};

