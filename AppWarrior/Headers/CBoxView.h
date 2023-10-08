/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"
#include "CView.h"

enum {
	boxStyle_Etched			= 0,
	boxStyle_Sunken			= 1,
	boxStyle_Raised			= 2,
	boxStyle_Bar			= 3,
	boxStyle_Pattern		= 4
};

class CBoxView : public CView
{	
	public:
		// construction
		CBoxView(CViewHandler *inHandler, const SRect& inBounds);
		CBoxView(CViewHandler *inHandler, const SRect& inBounds, Uint16 inStyle, const Uint8 *inTitle = nil, TFontDesc inFont = nil);
		virtual ~CBoxView();
		
		// title
		virtual void SetTitle(const void *inText, Uint32 inSize);
		void SetTitle(const Uint8 *inText)						{	SetTitle(inText+1, inText[0]);							}
		Uint32 GetTitle(void *outText, Uint32 inMaxSize) const	{	return UMemory::Read(mTitle, 0, outText, inMaxSize);	}
		virtual void SetFont(TFontDesc inFont);
		
		// properties
		virtual void SetStyle(Uint16 inStyle);
		Uint16 GetStyle() const									{	return mStyle;											}
		virtual void SetPattern(Int16 inID);
	
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);

	protected:
		THdl mTitle;
		TFontDesc mFont;
		TPattern mPattern;
		Uint16 mStyle;
};



