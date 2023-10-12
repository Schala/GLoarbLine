 #include "HLMD5.h"
 
 void
 HLMD5::HMAC_XXX(UInt8* md, DataBuffer key, DataBuffer text)
 {
	UInt8 mdkey[_MD5_DIGEST_LENGTH];
	UInt8 k_ipad[64], k_opad[64];
	_MD5Digest tempDigest;
	unsigned int i;

	if (key.len > 64)
		{
			{
			mMD5.Init();
			mMD5.Update(key.data, key.len);
			mMD5.Report(tempDigest);
			for(i=0;i<_MD5_DIGEST_LENGTH;i++) mdkey[i]=tempDigest[i];			
			key.len = _MD5_DIGEST_LENGTH;
			}
		key.data = mdkey;
		}

	memcpy(k_ipad, key.data, key.len);
	memcpy(k_opad, key.data, key.len);
	memset(k_ipad+key.len, 0, 64 - key.len);
	memset(k_opad+key.len, 0, 64 - key.len);

	for (i = 0; i < 64; i++)
		{
		k_ipad[i] ^= 0x36;
		k_opad[i] ^= 0x5c;
		}

		mMD5.Init();
		mMD5.Update(k_ipad, 64);
		mMD5.Update(text.data, text.len);
		mMD5.Report(tempDigest);
		for(i=0;i<_MD5_DIGEST_LENGTH;i++) md[i]=tempDigest[i];			

		mMD5.Init();
		mMD5.Update(k_opad, 64);
		mMD5.Update(md, _MD5_DIGEST_LENGTH);
		mMD5.Report(tempDigest);
		for(i=0;i<_MD5_DIGEST_LENGTH;i++) md[i]=tempDigest[i];					
 }