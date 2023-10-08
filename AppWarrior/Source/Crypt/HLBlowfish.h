#ifndef _HLBLOWFISH_H
#define _HLBLOWFISH_H

#include "HLCrypt.h"


typedef struct
{
UInt32 P[18];
UInt32 S[4][256];
}bf_key;


class HLBlowfish : public HLCipher
{
public:
HLBlowfish() {}
~HLBlowfish() {}
void Init();
void SetEncodeKey(DataBuffer buffer) { ExpandKey(buffer, &mEncodeKey); }
void SetDecodeKey(DataBuffer buffer) { ExpandKey(buffer, &mDecodeKey); }
void Encode(DataBuffer buffer) { OFB64(buffer, &mEncodeKey, (UInt32*)mEncodeIvec, &mEncodeIvecOffset); }
void Decode(DataBuffer buffer) { OFB64(buffer, &mDecodeKey, (UInt32*)mDecodeIvec, &mDecodeIvecOffset); }
bool SelfTest();
static UInt8 *GetName() { return "\pBLOWFISH"; }


protected:
bf_key mEncodeKey;
bf_key mDecodeKey;
UInt8  mEncodeIvec [8];
UInt8  mDecodeIvec [8];
Int32  mEncodeIvecOffset;	
Int32  mDecodeIvecOffset;

inline UInt32 F(UInt32 in, bf_key *key);
inline void Encrypt(UInt32 *L, UInt32 *R,  bf_key *key);
inline void Decrypt(UInt32 *L, UInt32 *R, bf_key *key);
void ExpandKey(DataBuffer inKey, bf_key *outKey);
void OFB64(DataBuffer buffer, bf_key *key, UInt32 *ivec, Int32 *n);
};


#endif /* _HLBLOWFISH_H */