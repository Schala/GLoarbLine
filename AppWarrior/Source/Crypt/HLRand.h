#ifndef _HLRAND_H
#define _HLRAND_H

#if DEBUG
#define HLRAND_DEBUG
#include "UFileSys.h"
#endif

#include "HLSha1.h"

#define HLRAND_SEED_LEN 256

//SHA1
#define HLRAND_HASHLEN SHA_DIGESTSIZE
#define HLRAND_HASH HLSha1


class HLRand
{
public:
static void Init();
static void Cleanup();
static void GetBytes(DataBuffer outBytes);
static void Churn(DataBuffer);

protected:
static void ReadSeed();
static void WriteSeed();

static HLRAND_HASH sTheHash;
static UInt8 	sRandPool [HLRAND_HASHLEN];
static UInt32	sRandCounter;

#ifdef HLRAND_DEBUG
static TFSRefObj *sDebug_OF;
static UInt32 sDebugOffset;
#endif

};


#endif /* _HLRAND_H */