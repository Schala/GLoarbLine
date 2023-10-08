#if MACINTOSH

/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "USound.h"

#include <OSUtils.h>
#include <Sound.h>

static pascal void _SNCallbackProc(SndChannelPtr inChan, SndCommand *inCmd);
static void _SNProcess();
static void _SNDeinit();
static void _SNPlayHandle(Handle inHdl);

#if TARGET_API_MAC_CARBON
	static SndCallBackUPP _SNCallbackUPP = ::NewSndCallBackUPP(_SNCallbackProc);
#else
	static RoutineDescriptor _SNCallbackRD = BUILD_ROUTINE_DESCRIPTOR(uppSndCallBackProcInfo, _SNCallbackProc);
	#define _SNCallbackUPP	(&_SNCallbackRD)
#endif

static struct {
	SndChannelPtr chan;
	Handle currentSound;
	Int16 soundQueue[16];
	Int16 soundQueueCount;
	Uint16 soundDone	: 1;
} _SNData = {0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},0,0};

extern Uint8 _gDisableSound;
extern void (*_APSoundProcess)();
void _WakeupApp();

bool _HasGestaltAttribute(OSType inSelector, Uint32 inBit);

// mac error handling
void _FailMacError(Int16 inMacError, const Int8 *inFile, Uint32 inLine);
inline void _CheckMacError(Int16 inMacError, const Int8 *inFile, Uint32 inLine) { if (inMacError) _FailMacError(inMacError, inFile, inLine); }
#if DEBUG
	#define FailMacError(id)		_FailMacError(id, __FILE__, __LINE__)
	#define CheckMacError(id)		_CheckMacError(id, __FILE__, __LINE__)
#else
	#define FailMacError(id)		_FailMacError(id, nil, 0)
	#define CheckMacError(id)		_CheckMacError(id, nil, 0)
#endif

/* -------------------------------------------------------------------------- */

void USound::Init()
{
	if (_SNData.chan == nil)
	{	
		CheckMacError(::SndNewChannel(&_SNData.chan, 0, 0, _SNCallbackUPP));
		_APSoundProcess = _SNProcess;
		
		UProgramCleanup::InstallAppl(_SNDeinit);
	}
}

void USound::Beep()
{
	short saveResFile = CurResFile();
	
	UseResFile(LMGetSysMap());
	
	::SysBeep(20);
	
	UseResFile(saveResFile);
}

// if <inCanQueue>, sound will be queued if cannot play right now (eg, b/c another is playing)
void USound::Play(TRez /* inRef */, Int32 inID, Uint32 inCanQueue)
{
	if (_gDisableSound || inID == 0) return;
	
	Init();
	
	if (_SNData.soundDone)
	{
		_SNData.soundDone = false;
		if (_SNData.currentSound)
		{
			::HUnlock(_SNData.currentSound);
			::ReleaseResource(_SNData.currentSound);
			_SNData.currentSound = nil;
		}
	}
	
	// check if already playing a sound
	if (_SNData.currentSound)
	{
		if (inCanQueue && (_SNData.soundQueueCount < (sizeof(_SNData.soundQueue) / sizeof(Int16))))
		{
			_SNData.soundQueue[_SNData.soundQueueCount] = inID;
			_SNData.soundQueueCount++;
		}
		
		return;
	}
	
	_SNPlayHandle(::GetResource('snd ', inID));
}

/* -------------------------------------------------------------------------- */
#pragma mark -

TSoundOutput USound::NewOutput(Uint32 inChanCount, Uint32 inSampleRate, Uint32 inSampleSize, Uint32 inCompType, Uint32 inSpecial, TMessageProc inProc, void *inContext, Uint32 /* inOptions */)
{
	#pragma unused(inChanCount, inSampleRate, inSampleSize, inCompType, inSpecial, inProc, inContext)
	Fail(errorType_Misc, error_Unimplemented);
	return nil;
}

// causes sound playback to be immediately stopped on the specified output, and all msg_ReleaseBuffer messages will be sent before this function returns
void USound::FlushOutput(TSoundOutput inRef)
{
	if (inRef)
	{
		Fail(errorType_Misc, error_Unimplemented);
	}
}

// WARNING! If there are still buffers being played, playback will be stopped immediately, but you will not receive any msg_ReleaseBuffer messages!  Use FlushOutput() if needed.
void USound::DisposeOutput(TSoundOutput inRef)
{
	if (inRef)
	{
		Fail(errorType_Misc, error_Unimplemented);
	}
}

// for volumes, a value of 0xFFFF represents full volume, and a value of 0x0000 is silence
void USound::SetOutputVolume(TSoundOutput inRef, const Uint32 *inChans, Uint32 inChanCount, Uint32 /* inOptions */)
{
	#pragma unused(inChans, inChanCount)
	Require(inRef);
	Fail(errorType_Misc, error_Unimplemented);
}

Uint32 USound::GetOutputVolume(TSoundOutput inRef, Uint32 *outChans, Uint32 inMaxCount, Uint32 /* inOptions */)
{
	#pragma unused(outChans, inMaxCount)
	Require(inRef);
	Fail(errorType_Misc, error_Unimplemented);
	return 0;
}

/*
 * EnqueueOutput() adds a buffer to the output queue for the specified output.  The buffer must remain
 * valid until the sound output driver has finished playing it, at which point it will post a 
 * msg_ReleaseBuffer to the msg proc you specified when you called NewOutput().  The <inContext>
 * parameter for the message will be that which you specified for NewOutput().  The <inObject>
 * parameter will be <inRef>, and the data will consist of the <inData> ptr directly followed by 
 * <inDataSize>.  To avoid breaks in sound output, it is recommended that you enqueue two buffers
 * to begin with, and then with every msg_ReleaseBuffer you get, enqueue another buffer.  Buffers
 * should be at least (sampleRate * sampleSize * channelCount * 2) in size.  Also, you cannot split
 * samples across buffers - <inDataSize> must be a multiple of (sampleSize * channelCount).
 */
void USound::EnqueueOutput(TSoundOutput inRef, const void *inData, Uint32 inDataSize)
{
	Require(inRef && inData && inDataSize);
	Fail(errorType_Misc, error_Unimplemented);
}

// returns ptr to a 32-byte area in <inRef> that you can use for your own purposes
Uint8 *USound::GetOutputScratch(TSoundOutput inRef)
{
	Require(inRef);
	Fail(errorType_Misc, error_Unimplemented);
	return nil;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

static pascal void _SNCallbackProc(SndChannelPtr inChan, SndCommand *inCmd)
{
	#pragma unused(inChan, inCmd)
	
	_SNData.soundDone = true;
	_WakeupApp();
}

static void _SNProcess()
{
	if (_SNData.soundDone)
	{
		_SNData.soundDone = false;
		if (_SNData.currentSound)
		{
			::HUnlock(_SNData.currentSound);
			::ReleaseResource(_SNData.currentSound);
			_SNData.currentSound = nil;
		}
		
		if (_SNData.soundQueueCount)
		{
			USound::Play(nil, _SNData.soundQueue[0], false);
			_SNData.soundQueueCount--;
			BlockMoveData(_SNData.soundQueue+1, _SNData.soundQueue, _SNData.soundQueueCount * sizeof(Int16));
		}
	}
}

// ******** should really kill channel on ExitToShell() too...
static void _SNDeinit()
{
	if (_SNData.chan)
	{
		::SndDisposeChannel(_SNData.chan, true);
		_SNData.chan = nil;
	}
}

void _SNPlayHandle(Handle inHdl)
{
	if (_gDisableSound || _SNData.currentSound) return;
	
	if (inHdl)
	{
		_SNData.currentSound = inHdl;
		::HLockHi(inHdl);
		CheckMacError(::SndPlay(_SNData.chan, (SndListResource **)inHdl, true));
		
		SndCommand endCmd;
		endCmd.cmd = callBackCmd;
		endCmd.param1 = 0;

		CheckMacError(::SndDoCommand(_SNData.chan, &endCmd, true));
	}
}

Handle _SNGetSoundFromFile(TFSRefObj* inRef)
{
	Int16 ref;
	Handle h = nil;
	
	ref = FSpOpenResFile((FSSpec *)inRef, fsRdPerm);
	if (ref != -1)
	{
		UseResFile(ref);
		
		if (Count1Resources('snd ') >= 1)
		{
			h = Get1IndResource('snd ', 1);
			if (h) DetachResource(h);
		}
		
		CloseResFile(ref);
	}
	
	return h;
}

#ifndef FixedToInt
#define FixedToInt(a) ((short) ((Fixed) (a) + ((Fixed)0x00008000) >> 16))
#endif

THdl _MacSoundToPortaSound(void *inHdl)
{
	long offset;
	Uint32 hdlSize, s;
	THdl h;

	Require(inHdl);
	
	// test for Sound Mgr 3.0 (we need it for GetSoundHeaderOffset)
	if (!_HasGestaltAttribute(gestaltSoundAttr, gestaltMultiChannels) || GetSoundHeaderOffset == nil)
		Fail(errorType_Misc, error_FormatUnknown);

	// get ptr to sound header
	hdlSize = ::GetHandleSize((Handle)inHdl);
	CheckMacError(::GetSoundHeaderOffset((SndListHandle)inHdl, &offset));
	char saveState = HGetState((Handle)inHdl);
	HLock((Handle)inHdl);
	SoundHeader *shp = (SoundHeader *)(BPTR(*(SndListHandle)inHdl)+offset);
	
	try
	{
		switch (shp->encode)
		{
			case stdSH:
			{
				s = shp->length;
				
				// check enough data in mac snd
				if ((offset + OFFSET_OF(SoundHeader, sampleArea) + s) > hdlSize)
					Fail(errorType_Misc, error_Corrupt);
				
				// flatten the sound!
				h = USound::FlattenToHandle(1, FixedToInt(shp->sampleRate), 1, s, shp->sampleArea, s);
			}
			break;
			
			case extSH:
			{
				ExtSoundHeader *eshp = (ExtSoundHeader *)shp;
				
				// get sample size in bytes
				Uint32 sampSize = eshp->sampleSize / 8;
				Uint32 sampCount = eshp->numFrames * eshp->numChannels;
				
				// we only support sample sizes that are whole bytes
				if ((sampSize * 8) != eshp->sampleSize)
					Fail(errorType_Misc, error_FormatUnknown);
				
				// determine size in bytes of all the samples
				s = sampCount * sampSize;
				
				// check enough data in mac snd
				if ((offset + OFFSET_OF(SoundHeader, sampleArea) + s) > hdlSize)
					Fail(errorType_Misc, error_Corrupt);
				
				// flatten the sound!
				h = USound::FlattenToHandle(eshp->numChannels, FixedToInt(eshp->sampleRate), sampSize, sampCount, eshp->sampleArea, s);
			}
			break;
			
			default:
				Fail(errorType_Misc, error_FormatUnknown);
				break;
		}
	}
	catch(...)
	{
		HSetState((Handle)inHdl, saveState);
		throw;
	}

	HSetState((Handle)inHdl, saveState);
	
	return h;
}






#endif /* MACINTOSH */
