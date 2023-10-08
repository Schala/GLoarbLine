/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"

typedef class TExternalAppObj *TExternalApp;


// app module
class UExternalApp
{
	public:
		// new, dispose
		static TExternalApp New(const void *inAppPath, Uint32 inPathSize, const void *inParam = nil, Uint32 inParamSize = 0);
		static void Dispose(TExternalApp inApp);
		
		// activate, close, suspend, resume
		static bool TryToActivate(TExternalApp inApp);
		static bool TryToClose(TExternalApp inApp);
		static bool Suspend(TExternalApp inApp);
		static bool Resume(TExternalApp inApp);

		// launch
		static bool IsRegisteredAssociation(const Uint8 *inAssociation);
		static bool RegisterAssociation(const Uint8 *inAssociation, const Uint8 *inTitle = nil);
		static Uint32 SearchAssociation(const Uint8 *inFileName, Uint32 inTypeCode, void *outAppPath, Uint32 inMaxPathSize);
		static TExternalApp LaunchAssociation(const Uint8 *inFileName, Uint32 inTypeCode, const void *inParam = nil, Uint32 inParamSize = 0);
		static TExternalApp LaunchApplication(TFSRefObj* inApp, const void *inParam = nil, Uint32 inParamSize = 0);

		// search
		static bool SearchRunningApp(const Int8 *inAppPath, Uint32 inMsg = 0, const Int8 *inMessage = nil);		
		
		// registry settings
		static bool ReadSystemRegistry(const char* key, const char* value, char* buffer, Uint32& bufferSize);
};


// object interface
class TExternalAppObj
{
	public:
		// activate, close, suspend, resume
		bool TryToActivate()								{	return UExternalApp::TryToActivate(this);	}
		bool TryToClose()									{	return UExternalApp::TryToClose(this);		}
		bool Suspend()										{	return UExternalApp::Suspend(this);			}
		bool Resume()										{	return UExternalApp::Resume(this);			}

		void operator delete(void *p)						{	UExternalApp::Dispose((TExternalApp)p);		}
	protected:
		TExternalAppObj() {}				// force creation via UExternalApp
};

