/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"

class CPaneView : public CContainerView
{
	public:
		CPaneView(CViewHandler *inHandler, const SRect& inBounds);

		void SetViewLimits(CView *inView, Int32 inMinWidth, Int32 inMinHeight);
		
//		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);
		virtual bool SetBounds(const SRect& inBounds);

		void SetPanePercent(Uint8 inPanePercent);
		Uint8 GetPanePercent();

		virtual void MouseDown(const SMouseMsgData& inInfo);
		virtual void MouseUp(const SMouseMsgData& inInfo);
		virtual void MouseMove(const SMouseMsgData& inInfo);
		virtual void MouseLeave(const SMouseMsgData& inInfo);

	protected:
		
		bool mIgnoreSetBounds;
		bool mMoving;
		SRect mSliderRect;
		SRect mOldSliderRect;
		SRect mOldBounds[2];
		SPoint mDownPt;
		Uint32 mOrientation;
		
		struct
		{
			Int32 minWidth;
			Int32 minHeight;
		} mLimit[2];
				
		void RecalcSlider();
		
		virtual void HandleInstall(CView *inView);
		virtual void HandleSetBounds(CView *inView, const SRect& inBounds);
};


