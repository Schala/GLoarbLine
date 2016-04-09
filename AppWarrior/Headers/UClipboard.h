/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */#pragma once#include "typedefs.h"#include "UGraphics.h"class UClipboard{	public:		static THdl GetData(const Int8 *inType);		static void SetData(const Int8 *inType, const void *inData, Uint32 inDataSize);		static void BeginSet();		static void EndSet();		static void SetImageData(TImage inImage, const SRect *inRect = nil, Uint32 inOptions = 0);		static TImage GetImageData(SRect *outRect = nil, Uint32 inOptions = 0);		static void SetSoundData(THdl inHdl, Uint32 inOptions = 0);		static THdl GetSoundData(Uint32 inOptions = 0);};