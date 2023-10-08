/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
 
#include "CDecompressImage.h"


CDecompressImage::CDecompressImage(Uint32 inSize, bool inSilentFail, bool inBottomTopUpdate)
{
	mBuffer = nil;
	mBufferSize = inSize;
	
	mOffset = 0;
	mBytesInBuffer = 0;
		
	ClearStruct(mPixmap);
	mImage = nil;
	
	mReadRows = 0;
	mUpdateRows = 0;
	
	mSilentFail = inSilentFail;
	mBottomTopUpdate = inBottomTopUpdate;
	mFullUpdate = false;
	mIsComplete = false;
}

CDecompressImage::~CDecompressImage()
{
	ClearPixmap();
	
	if(mImage)
		UGraphics::DisposeImage(mImage);
}

bool CDecompressImage::GetSize(Uint32& outWidth, Uint32& outHeight)
{
	if(mPixmap.width && mPixmap.height)
	{
		outWidth = mPixmap.width;
		outHeight = mPixmap.height;
		
		return true;
	}
	
	return false;
}

bool CDecompressImage::IsComplete()			
{	
	return mIsComplete;		
}

void CDecompressImage::Draw(TImage inDest, const SPoint& inDestPt, const SPoint& inSourcePt, Uint32 inWidth, Uint32 inHeight, Uint32 inOptions)
{
	UpdateImage();
	
	if(mImage)
	{
	    StImageLocker lockImage(mImage);
        UGraphics::CopyPixels(inDest, inDestPt, mImage, inSourcePt, inWidth, inHeight, inOptions);
    }
    else
    {
    	inDest->SetInkColor(color_White);
    	inDest->FillRect(SRect(inDestPt.x, inDestPt.y, inWidth, inHeight));
    }
}

TImage CDecompressImage::GetImagePtr()		
{	
	UpdateImage(); 
	
	return mImage;									
}
	
TImage CDecompressImage::DetachImagePtr()	
{	
	UpdateImage(); 
	
	TImage tmpImage = mImage;	
	mImage = nil;	

	return tmpImage;	
}

void CDecompressImage::InitCompatibleImage()
{
	if (mImage || !mPixmap.width || !mPixmap.height)
		return;
			
	mImage = UGraphics::NewCompatibleImage(mPixmap.width, mPixmap.height);
	if (!mImage)
		Fail(errorType_Image, imgError_InsufficientMemory);

	mImage->SetInkColor(color_White);
	
	StImageLocker lockImage(mImage);
	mImage->FillRect(SRect(0, 0, mPixmap.width, mPixmap.height));
}

void CDecompressImage::UpdateImage()
{
	InitCompatibleImage();
	
	if (mImage && mPixmap.data && mReadRows)
	{
		if (mFullUpdate)
		{
			StImageLocker lockImage(mImage);
  		    if (!mBottomTopUpdate)
		        UGraphics::CopyPixels(mImage, SPoint(0,0), mPixmap, SPoint(0,0), mPixmap.width, mReadRows);
			else
		        UGraphics::CopyPixels(mImage, SPoint(0,mPixmap.height-mReadRows), mPixmap, SPoint(0,mPixmap.height-mReadRows), mPixmap.width, mReadRows);

			mFullUpdate = false;
		
			mUpdateRows = mReadRows;
		}
		else if (mReadRows != mUpdateRows)
		{
			StImageLocker lockImage(mImage);
  		    if (!mBottomTopUpdate)
		        UGraphics::CopyPixels(mImage, SPoint(0,mUpdateRows), mPixmap, SPoint(0,mUpdateRows), mPixmap.width, mReadRows-mUpdateRows);
			else
		        UGraphics::CopyPixels(mImage, SPoint(0,mPixmap.height-mReadRows), mPixmap, SPoint(0,mPixmap.height-mReadRows), mPixmap.width, mReadRows-mUpdateRows);

			mUpdateRows = mReadRows;
		}
	}
}

void CDecompressImage::ClearPixmap()
{
	if(mPixmap.colorTab)
		UMemory::Dispose((TPtr)mPixmap.colorTab);

	if(mPixmap.data)
		UMemory::Dispose((TPtr)mPixmap.data);

	ClearStruct(mPixmap);
	
	mReadRows = 0;
	mUpdateRows  = 0;
}

Uint32 CDecompressImage::ReadImage(void *outBuf, Uint32 inBufSize, bool inExpectedEnd)
{
	if(mOffset + inBufSize > mBufferSize)
	{
		if(!inExpectedEnd)
			Fail(errorType_Image, imgError_UnexpectedEnd);
		
		return END_OF_FB;
	}

	if(mOffset + inBufSize > mBytesInBuffer)
		return END_OF_BYTES;

	Uint32 nRet = UMemory::Copy(outBuf, (Uint8*)mBuffer + mOffset, inBufSize);
		
	if(nRet != inBufSize)
		Fail(errorType_Image, imgError_ReadingError);

	mOffset += nRet;
	return nRet;
}
