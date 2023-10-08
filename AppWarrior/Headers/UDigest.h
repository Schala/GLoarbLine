/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"

class UDigest
{
	public:
		// Encoder/Decoder in Base64 format
		static void *Base64_Encode(const void *inData, Uint32 inDataSize, Uint32& outDataSize);
		static void *Base64_Decode(const void *inData, Uint32 inDataSize, Uint32& outDataSize);

		// Encoder/Decoder in UU format
		static void *UU_Encode(const void *inData, Uint32 inDataSize, Uint32& outDataSize);
		static void *UU_Decode(const void *inData, Uint32 inDataSize, Uint32& outDataSize);
		
		// Encoder in MD5 format (outDataSize = 16 bytes)
		static void *MD5_Encode(const void *inData, Uint32 inDataSize, Uint32& outDataSize);
		// no decoder here
};


#define _MD5_DIGEST_LENGTH 16

class _MD5Digest;     
class _MD5 
{
	public:
		_MD5();        		     	     				    // automatically calls _MD5::Init()
		virtual ~_MD5(); 		          				    // automatically calls _MD5::Clear()

	  	void Update(const Uint8 *inData, Uint32 inDataSize);// process data.
   		void Report(_MD5Digest &outDigest);   				// used to generate the digest
 
	   	void Init(); 	                					// used for reinitialization
		void Clear();    	             					// used to wipe clean interal data

	protected:
	   Uint32 mState[4];         							// state (ABCD)
	   Uint32 mCount[2];         							// number of bits, modulo 2^64 (lsb first)
	   Uint8  mBuffer[64];       							// input buffer
 
	   void Transform(const Uint8 inBlock[64]);
	   void Encode(Uint8  *outData, const Uint32 *inData, Uint32 inDataSize);
	   void Encode(_MD5Digest &outDigest);
	   void Decode(Uint32 *outData, const Uint8 *inData, Uint32 inDataSize);
};

class _MD5Digest 
{
	public:
   		_MD5Digest(); 						             // automatically calls _MD5Digest::Clear()
   		~_MD5Digest(); 						             // automatically calls _MD5Digest::Clear()
   
   		Uint8 &operator[](Uint32 inIndex);
	   	Uint8 operator[](Uint32 inIndex) const;
	
	   	void Clear();

	private:
   		Uint8 mData[_MD5_DIGEST_LENGTH];

	   	friend bool operator==(const _MD5Digest &inLhs, const _MD5Digest &inRhs);
	   	friend bool operator!=(const _MD5Digest &inLhs, const _MD5Digest &inRhs);
	   	friend void _MD5::Encode(_MD5Digest &outDigest);
};
