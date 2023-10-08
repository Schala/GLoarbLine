/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "CImageView.h"
#include "CDecompressGif.h"


class CAnimatedGifView : public CImageView
{
	public:
		// construction
		CAnimatedGifView(CViewHandler *inHandler, const SRect& inBounds, CDecompressGif *inDecompressGif);
		virtual ~CAnimatedGifView();
		
		// drawing
		virtual void Draw(TImage inImage, const SRect& inUpdateRect, Uint32 inDepth);

		// mouse events
		virtual void MouseUp(const SMouseMsgData& inInfo);

		// key events
		virtual void KeyUp(const SKeyMsgData& inInfo);

		virtual void PlayImage();
		virtual void StopImage();

		// set
		virtual bool SetImage(Uint32 inWidth=0, Uint32 inHeight=0);
		virtual bool FinishImage();

	protected:
		GifInfo mGifInfo;
		CPtrList<SImageInfo> mImageInfoList;
		Uint16 mImageCount;
		bool mAnimatedGif;

		TImage mCurrentBackImage;
		TImage mNewBackImage;
		TImage mPrevBackImage;
		bool mBackImage;

		bool mBackColor;
		TTimer mImageTimer;
		bool mStartTimer;
		
		static void ImageTimer(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize);
		void CheckTimer();
		void SaveBackground();
		void RestoreBackground();

		bool DrawImage(TImage outImage, const SRect& inUpdateRect, const SRect& inViewRect, Uint16 inImageCount, TImage inBackImage);
		bool DrawBackImage(TImage outImage, const SRect& inUpdateRect, const SRect& inViewRect, TImage inBackImage);
		bool DrawForeImage(TImage outImage, const SRect& inUpdateRect, const SRect& inViewRect, Uint16 inImageCount);
		
		void InitAnimatedGif();
		void PutImageInList();
		bool InitCompatibleImage(TImage &outImage);
};
