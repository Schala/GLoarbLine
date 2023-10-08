#ifndef _HLMD5_H
#define _HLMD5_H

#include "HLCrypt.h"




class HLMD5 : public HLHash
{
public:
HLMD5() {}
~HLMD5() {}

static UInt8 *GetName() { return "\pHMAC-MD5"; }

virtual UInt32	GetMacLen() {return _MD5_DIGEST_LENGTH;}
virtual void 	HMAC_XXX(UInt8* output, DataBuffer key, DataBuffer text);

protected:
_MD5 mMD5;
};

#endif /* _HLMD5_H */