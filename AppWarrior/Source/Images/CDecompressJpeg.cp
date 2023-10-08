/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CDecompressJpeg.h"


CDecompressJpeg::CDecompressJpeg(Uint32 inSize, bool inSilentFail) 
	: CDecompressImage(inSize, inSilentFail, false)
{
	mJpegColorSpace = JCS_RGB;
	mProgressiveImage = false;
	mScaleDenominator = 1;
	
	mStage = 1;
	mScanStage = 0;

 	try
 	{
	 	mCinfo.err = jpeg_std_error(&mJerr);

		jpeg_create_decompress(&mCinfo);
	  	jpeg_stdio_src(&mCinfo, 2048, inSize);
	  	// library try to read 2048 bytes (if don,t exist in buffer wait) 
	}
	catch(SError& inError)
	{
		jpeg_destroy_decompress(&mCinfo);
		
		if (mSilentFail)
			inError.silent = true;
			
		throw;
	}
}

CDecompressJpeg::~CDecompressJpeg()
{
	if(mStage != 5)
		jpeg_destroy_decompress(&mCinfo);
}
		
void CDecompressJpeg::SetScaleDenominator(Uint8 inScaleDenominator)
{
	mScaleDenominator = inScaleDenominator;
}

J_COLOR_SPACE CDecompressJpeg::GetColorSpace()
{
	return mJpegColorSpace;
}

bool CDecompressJpeg::IsProgressiveImage()
{
	return mProgressiveImage;
}

bool CDecompressJpeg::Decompress(const void *inData, Uint32 inDataSize)
{
	((my_src_ptr) mCinfo.src)->pSourceBuffer = inData;
	((my_src_ptr) mCinfo.src)->nBytesInBuffer = inDataSize;

	try
	{
		return ReadJpegImage();
	}
	catch(SError& inError)
	{
		jpeg_destroy_decompress(&mCinfo);
		mStage = 5;
		
		if (mSilentFail)
			inError.silent = true;
			
		throw; 
	}
	
	return false;
}

bool CDecompressJpeg::ReadJpegImage()
{
	Uint32 nOldReadRows = mReadRows;

startagain:

	switch(mStage)
	{
		case 1:
			 // This IF is for fixing a JPEG Library BUG
			if(((my_src_ptr) mCinfo.src)->nBufferSize < 7168 || 
			   (((my_src_ptr) mCinfo.src)->nBufferSize < 12288 && ((my_src_ptr) mCinfo.src)->nBytesInBuffer >= 7168) || 
			   (((my_src_ptr) mCinfo.src)->nBytesInBuffer >= 12288))
			{
				if(jpeg_read_header(&mCinfo, TRUE))        
				{
					if(jpeg_has_multiple_scans(&mCinfo))
						mCinfo.buffered_image = true;

					mStage = 2;
					goto startagain;
				}
			}
			break;
				
		case 2:					
	  		if(jpeg_start_decompress(&mCinfo))
	  		{
				if(mCinfo.out_color_space != JCS_RGB && mCinfo.out_color_space != JCS_GRAYSCALE)
					Fail(errorType_Image, imgError_UnsupportedFormat);

	  			mPixmap.width = mCinfo.output_width;
		   		mPixmap.height = mCinfo.output_height;
				mPixmap.depth = 8*mCinfo.out_color_components;
				mPixmap.rowBytes = mCinfo.output_width * mCinfo.output_components;
				mPixmap.colorCount = mCinfo.actual_number_of_colors;

				mJpegColorSpace = mCinfo.out_color_space;
				mProgressiveImage = mCinfo.buffered_image;
	
				if(mJpegColorSpace==JCS_GRAYSCALE && mCinfo.out_color_components==1)
				{
					mPixmap.colorCount = 256;
					try
					{
						mPixmap.colorTab = (Uint32*)UMemory::NewClear(1024);
					}
					catch(...)
					{
						Fail(errorType_Image, imgError_InsufficientMemory);
					}
			
					Uint8 *p = (Uint8 *)mPixmap.colorTab;	
					for(Uint16 i=0; i<256; i++)
					{
						*p++ = i; 
						*p++ = i; 
						*p++ = i; 
						*p++ = 0;
					}
				}

				try
				{
					mPixmap.data = UMemory::NewClear(mPixmap.rowBytes*mPixmap.height);
				}
				catch(...)
				{
					Fail(errorType_Image, imgError_InsufficientMemory);
				}
	
				mJpegBuffer = (*mCinfo.mem->alloc_sarray)((j_common_ptr) &mCinfo, JPOOL_IMAGE, mPixmap.rowBytes, 1);
			
				mStage = 3;
			 	mScanStage = 1;
				goto startagain;
			}
			break;
		
		case 3:
		 	if(mCinfo.buffered_image)
		 	{
scanagain:
 				switch(mScanStage)
 				{
 					case 1:
	 					Uint16 nScanRet;
	 					do
	 					{
	 						nScanRet=jpeg_consume_input(&mCinfo);
				 			
			 				if(nScanRet==JPEG_SCAN_COMPLETED)
		 					{
		 						mScanStage = 2;
		 						goto scanagain;;
							}
						}
						while(nScanRet);
 						break;
	 				
 					case 2:
 						if(jpeg_start_output(&mCinfo, mCinfo.input_scan_number))
 						{
							mReadRows = 0;
		 					mScanStage=3;
	 						goto scanagain;
 						}
 						break;
 					
 					case 3:
						while (mReadRows < mCinfo.output_height && jpeg_read_scanlines(&mCinfo, mJpegBuffer, 1))
		 				{ 
							UMemory::Copy((Uint8*)mPixmap.data + mReadRows*mPixmap.rowBytes, mJpegBuffer[0], mPixmap.rowBytes);
							mReadRows++;
						}
					
				  		if(mReadRows == mCinfo.output_height)
				  		{
				  			mScanStage = 4;	
				  			goto scanagain;
				  		}
						break;
							
					case 4:	
		  				if(jpeg_finish_output(&mCinfo))
						{			  							
							mFullUpdate = true;
							
							mScanStage = 1;
		
							if(jpeg_input_complete(&mCinfo))
							{
								mStage = 4;
								goto startagain;
							}
						
							goto scanagain;  
						}
				};
	 		}
		 	else
		 	{
		 		while (mReadRows < mCinfo.output_height && jpeg_read_scanlines(&mCinfo, mJpegBuffer, 1)) 
				{
  					UMemory::Copy((Uint8*)mPixmap.data + mReadRows*mPixmap.rowBytes, mJpegBuffer[0], mPixmap.rowBytes);
					mReadRows++;
				}
	
				if(mReadRows == mCinfo.output_height)
				{
					mStage = 4;
					goto startagain;
				}
			}
			break;

		case 4:
			if(jpeg_finish_decompress(&mCinfo))
			{
				mOffset = ((my_src_ptr) mCinfo.src)->nOffset;
				jpeg_destroy_decompress(&mCinfo);

				mStage = 5;
				goto startagain;
			}
			break;
			
		case 5: 
			mIsComplete = true;
			break;
	};

	if(mStage != 5)
		mOffset = ((my_src_ptr) mCinfo.src)->nOffset;
	
	return (mReadRows > nOldReadRows);
}

