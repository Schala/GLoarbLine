/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "CApplication.h"
#include "UError.h"
#include "UWindow.h"
#include "CWindow.h"
#include "UApplication.h"

void _DisposeAllWindows();


CApplication *gApplication = nil;

/* -------------------------------------------------------------------------- */

CApplication::CApplication()
{
	// init managers
	UOperatingSystem::Init();
	UApplication::Init();
	UTimer::Init();
	UWindow::Init();
	UGraphics::Init();
	URegion::Init();
	HLRand::Init();
	
	// init optional managers
	if (UDragAndDrop::IsAvailable())
		UDragAndDrop::Init();
	
	// init tooltips
	UTooltip::Init();
	
	// init data members
	gApplication = this;
	
	// install message handler
	UApplication::SetMessageHandler(MessageHandler, this);
	
	// install command-Q key command
	const SKeyCmd cmdQ = { 10000, 'q', modKey_Command };
	UWindow::InstallKeyCommands(&cmdQ, 1, MessageHandler, this);
}

CApplication::~CApplication()
{
	if (gApplication == this)
	{
		gApplication = nil;
		UApplication::SetMessageHandler(nil, 0);
		
		gWindowList.ClearWindowList();
		
		HLRand::Cleanup();
	}
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CApplication::Run()
{
	UApplication::Run();
}

// called when the user wants to quit, don't HAVE to quit (eg, user can click on cancel button)
void CApplication::UserQuit()
{
	Quit();
}

void CApplication::Quit()
{
	UApplication::Quit();
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void CApplication::KeyCommand(Uint32 inCmd, const SKeyMsgData& /* inInfo */)
{
	if (inCmd == 10000) UserQuit();
}

void CApplication::Error(const SError& inError)
{
	if (!inError.silent)
		UError::Alert(inError);
}

void CApplication::Timer(TTimer /* inTimer */)
{
	// do nothing by default
}

void CApplication::OpenDocuments()
{
	TFSRefObj* ref;
	
	for(;;)
	{
		ref = UApplication::GetDocumentToOpen();
		if (ref == nil) break;
		scopekill(TFSRefObj, ref);
		
		OpenDocument(ref);
	}
}

// does NOT take ownership of inRef
void CApplication::OpenDocument(TFSRefObj* /* inRef */)
{
	// do nothing by default
}

void CApplication::ShowPreferences()
{
	// do nothing by default
}

/* -------------------------------------------------------------------------- */
#pragma mark -

// this only gets called for windows where ProcessModal() has not been called
void CApplication::WindowHit(CWindow */* inWindow */, const SHitMsgData& /* inInfo */)
{
	// do nothing by default
}

void CApplication::WindowHitHandler(void *inContext, void */* inObject */, Uint32 /* inMsg */, const void *inData, Uint32 /* inDataSize */)
{
	gApplication->WindowHit((CWindow *)inContext, *(SHitMsgData *)inData);
}

void CApplication::HandleMessage(void *inObject, Uint32 inMsg, const void *inData, Uint32 /* inDataSize */)
{
	switch (inMsg)
	{
		case msg_KeyCommand:
			KeyCommand(*(Uint32 *)inData, *(SKeyMsgData *)(BPTR(inData)+sizeof(Uint32)));
			break;
		case msg_Quit:
			UserQuit();
			break;
		case msg_Error:
			Error(*(SError *)inData);
			break;
		case msg_Timer:
			Timer((TTimer)inObject);
			break;
		case msg_OpenDocuments:
			OpenDocuments();
			break;
		case msg_ShowPreferences:
			ShowPreferences();
			break;
/*
	#if WIN32
		case msg_ServiceStop:
		case msg_ServiceShutdown:
			UserQuit();
			break;
	#endif
*/
	}
}

void CApplication::MessageHandler(void *inContext, void *inObject, Uint32 inMsg, const void *inData, Uint32 inDataSize)
{
	((CApplication *)inContext)->HandleMessage(inObject, inMsg, inData, inDataSize);
}



