/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */#pragma once#include "CDecompressImage.h"#include "jpeglib.h"class CDecompressJpeg : public CDecompressImage{	friend class CDecompressPict;		public:		// constructor		CDecompressJpeg(Uint32 inSize, bool inSilentFail = false);		virtual ~CDecompressJpeg();				virtual bool Decompress(const void *inData, Uint32 inDataSize);				// set		void SetScaleDenominator(Uint8 inScaleDenominator); 		// get		J_COLOR_SPACE GetColorSpace();				bool IsProgressiveImage();		protected:		J_COLOR_SPACE mJpegColorSpace;		bool mProgressiveImage;		Uint8 mScaleDenominator;         // = 1, 2, 3 or 4						struct jpeg_decompress_struct mCinfo; 		struct jpeg_error_mgr mJerr;          		Uint8 mStage, mScanStage;			  		JSAMPARRAY mJpegBuffer;					  		bool ReadJpegImage();};