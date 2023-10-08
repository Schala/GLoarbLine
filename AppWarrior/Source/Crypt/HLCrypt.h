#ifndef _HLCRYPT_H
#define _HLCRYPT_H

typedef struct
{
UInt8 *data;
UInt32 len;
}DataBuffer;


/* Abstract Base Classes */

class HLCipher
{
public:
HLCipher() {}
~HLCipher() {}

virtual void Init() {}
virtual void SetEncodeKey(DataBuffer /* buffer */) {}
virtual void SetDecodeKey(DataBuffer /* buffer */) {}
virtual void Encode(DataBuffer /* buffer */) {}
virtual void Decode(DataBuffer /* buffer */) {}
virtual bool SelfTest() { return true; }
};

class HLHash
{
public:
HLHash() {}
~HLHash() {}

virtual UInt32	GetMacLen() {return 0;}
virtual void 	HMAC_XXX(UInt8* /* output */, DataBuffer /* key */ , DataBuffer /* text */) {}
};



class HLCrypt
{
public:
HLCrypt() { mCipher=nil; mHash=nil; mSessionKey.data=nil; mEncodeKey.data=nil; mDecodeKey.data=nil; }
~HLCrypt();

void Init(HLCipher *cipher, HLHash *hash, DataBuffer sessionKey, DataBuffer password, bool isClient);
void Encode(DataBuffer buffer) { mCipher->Encode(buffer); }
void Decode(DataBuffer buffer) { mCipher->Decode(buffer); }
void PermEncodeKey(UInt32 n);
void PermDecodeKey(UInt32 n);

HLHash		*mHash;

protected:
HLCipher	*mCipher;
UInt32		mMacLen;
DataBuffer	mSessionKey;
DataBuffer	mEncodeKey;
DataBuffer	mDecodeKey;
};

#endif /* _HLCRYPT_H */