#include "HLRand.h"
#include "ANSI.h"
#include "UFileSys.h"


//static storage space:
HLRAND_HASH HLRand::sTheHash;
UInt8 		HLRand::sRandPool [HLRAND_HASHLEN];
UInt32		HLRand::sRandCounter;

#ifdef HLRAND_DEBUG
TFSRefObj *HLRand::sDebug_OF;
UInt32 HLRand::sDebugOffset;
#endif



void
HLRand::Init()
{
ReadSeed();

#ifdef HLRAND_DEBUG
sDebug_OF=UFS::New(kProgramFolder, nil, "\pHLRand.out");
if(!sDebug_OF->Exists())
	sDebug_OF->CreateFile('????', '????');	
sDebug_OF->Open(perm_ReadWrite);
sDebugOffset=sDebug_OF->GetSize();
#endif
}

void
HLRand::Cleanup()
{
HLRand::WriteSeed();
#ifdef HLRAND_DEBUG
HLRand::sDebug_OF->Close();
#endif
}

void
HLRand::ReadSeed()
{
TFSRefObj *dataRef, *seedRef;
DataBuffer seed=(DataBuffer){0,0};

dataRef=UFS::New(kProgramFolder, nil, "\pData");
scopekill(TFSRefObj, dataRef);
if(!dataRef->Exists())
		dataRef->CreateFolder();

seedRef=UFS::New(dataRef, nil, "\prand_seed");
scopekill(TFSRefObj, seedRef);

if (!seedRef->Exists())
	{
	seed.len=HLRAND_SEED_LEN;
	seed.data=new UInt8 [seed.len]; //misc. uninitialized data
	}
else
	{	
	seedRef->Open(perm_Read);

	seed.len=seedRef->GetSize();
	seed.data=new UInt8 [seed.len];

	seedRef->Read(0,seed.data,seed.len);
	seedRef->Close();
	}

sTheHash.HMAC_XXX(sRandPool,seed,seed);

delete [] seed.data;
return;
}

void
HLRand::WriteSeed()
{
DataBuffer seed=(DataBuffer){0,0};
TFSRefObj *dataRef, *seedRef;

dataRef=UFS::New(kProgramFolder, nil, "\pData");
scopekill(TFSRefObj, dataRef);
if(!dataRef->Exists())
		dataRef->CreateFolder();

seedRef=UFS::New(dataRef, nil, "\prand_seed");
scopekill(TFSRefObj, seedRef);

if (!seedRef->Exists())
	seedRef->CreateFile('????', '????');
	
seedRef->Open(perm_ReadWrite);

seed.len=HLRAND_SEED_LEN;
seed.data=new UInt8 [seed.len];

GetBytes(seed);

seedRef->Write(0,seed.data, seed.len);

seedRef->Close();

delete [] seed.data;
}

void
HLRand::GetBytes(DataBuffer outBytes)
{
UInt8 temp [HLRAND_HASHLEN];
UInt32 n;
#ifdef HLRAND_DEBUG
DataBuffer debugBytes=outBytes;
#endif

while(outBytes.len)
	{
	sTheHash.HMAC_XXX(temp, (DataBuffer){(UInt8*)&sRandCounter, sizeof(sRandCounter)}, (DataBuffer){sRandPool,HLRAND_HASHLEN});
	sRandCounter++;
		
	n=(outBytes.len<HLRAND_HASHLEN)?outBytes.len:HLRAND_HASHLEN;
	memcpy(outBytes.data, temp, n);
	outBytes.len-=n;
	outBytes.data+=n;
	}
	
#ifdef HLRAND_DEBUG
sDebugOffset+=sDebug_OF->Write(sDebugOffset, debugBytes.data, debugBytes.len);
#endif	
}

void
HLRand::Churn(DataBuffer data)
{
sTheHash.HMAC_XXX(sRandPool,data, (DataBuffer){sRandPool,HLRAND_HASHLEN});
}
