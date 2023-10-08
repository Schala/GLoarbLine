/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
//
//  Mac-specific implementation of QuickTime component's dispatcher

//-----------------------------------------------------------------------
// includes

#include "AW.h"
//#undef PRAGMA_ONCE // to avoid macro redefinition warning
#include <Components.h> // Mac's components 
#include <QuicktimeComponents.h> // Mac's components 
#include "QTDispatcher.h" 
#include "CDataHandlerComponent.h"
#include "ComponentMacros.h"
//#include "NewHandler.h"


HL_Using_Namespace_BigRedH

//pascal ComponentResult  MyOpen( Handle storage, ComponentInstance self );
//pascal ComponentResult  MyClose( Handle storage, ComponentInstance self );

//extern pascal OSErr __initialize(const CFragInitBlock *theInitBlock);
//extern pascal void __terminate(void);

extern "C" {

//
//------------------------------------------------------------------------------------------
//
// component routines
//
//

// main dispatch function, out of HL namespace, to be exported
extern "C" pascal ComponentResult HL_Handler( ComponentParameters *params,
												Handle storage );

//#pragma options align=mac68K

enum {
	// Main
	FAT_VALUE_2(fat_pi_Dispatcher_ProcInfo,				kPascalStackBased, ComponentResult, ComponentParameters *, Handle),

	// Data Handler
	FAT_VALUE_3(fat_pi_canusedataref_ProcInfo,			kPascalStackBased, ComponentResult, Handle, Handle, long *),
	FAT_VALUE_2(fat_pi_setdataref_ProcInfo,				kPascalStackBased, ComponentResult, Handle, Handle),
	FAT_VALUE_2(fat_pi_getfilesize_ProcInfo, 			kPascalStackBased, ComponentResult, Handle, long *),
	FAT_VALUE_2(fat_pi_getavailablefilesize_ProcInfo, 	kPascalStackBased, ComponentResult, Handle, long *),
	FAT_VALUE_2(fat_pi_getfilename_ProcInfo,			kPascalStackBased, ComponentResult, Handle, StringPtr),
	FAT_VALUE_2(fat_pi_getmimetype_ProcInfo,			kPascalStackBased, ComponentResult, Handle, StringPtr),
	FAT_VALUE_2(fat_pi_getfiletype_ProcInfo,			kPascalStackBased, ComponentResult, Handle, OSType*),
	FAT_VALUE_1(fat_pi_openforread_ProcInfo,			kPascalStackBased, ComponentResult, Handle),
	FAT_VALUE_1(fat_pi_closeforread_ProcInfo,			kPascalStackBased, ComponentResult, Handle),
	FAT_VALUE_2(fat_pi_getscheduleaheadtime_ProcInfo,	kPascalStackBased, ComponentResult, Handle, long *),
	FAT_VALUE_3(fat_pi_comparedataref_ProcInfo,			kPascalStackBased, ComponentResult, Handle, Handle, Boolean *),
	FAT_VALUE_1(fat_pi_datatask_ProcInfo,				kPascalStackBased, ComponentResult, Handle),
	FAT_VALUE_7(fat_pi_scheduledata_ProcInfo,			kPascalStackBased, ComponentResult, Handle, Ptr, long, long, long, DataHSchedulePtr,  DataHCompletionUPP),
	FAT_VALUE_3(fat_pi_finishdata_ProcInfo,				kPascalStackBased, ComponentResult, Handle, Ptr, Boolean)
};

// Setup UPPs
//Main
//FAT_RD_ALLOC(dispatcher_rd, fat_pi_Dispatcher_ProcInfo, HL_Handler);

//#pragma options align=reset


// Main dispatcher

pascal ComponentResult
HL_Handler( ComponentParameters *params, Handle storage )
{
	ProcPtr theProc = nil;
	ProcInfoType theInfo;
	ComponentResult err = noErr;

	if( params->what < 0 ){
		switch( params->what ){
			// common to all components selectors
			case kComponentOpenSelect:
				theProc = (ProcPtr)DataHandlerOpen;
				theInfo = uppCallComponentOpenProcInfo;
				break;
			case kComponentCloseSelect:
				theProc = (ProcPtr)DataHandlerClose;
				theInfo = uppCallComponentCloseProcInfo;
				break;
			case kComponentCanDoSelect:
				theProc = (ProcPtr)DataHandlerCanDo;
				theInfo = uppCallComponentCanDoProcInfo;
				break;
			case kComponentVersionSelect:
				theProc = (ProcPtr)DataHandlerVersion;
				theInfo = uppCallComponentOpenProcInfo;
				break;
		}
	} else {
		switch( params->what ){
			// insert datahandling-specific dispatch here
			case kDataHCanUseDataRefSelect:
				theProc = (ProcPtr)DataHandlerCanUseDataRef;
				theInfo = fat_pi_canusedataref_ProcInfo;
				break;
			case kDataHSetDataRefSelect:
				theProc = (ProcPtr)DataHandlerSetDataRef;
				theInfo = fat_pi_setdataref_ProcInfo;
				break;
			case kDataHGetFileSizeSelect:
				theProc = (ProcPtr)DataHandlerGetFileSize;
				theInfo = fat_pi_getfilesize_ProcInfo;
				break;
			case kDataHGetAvailableFileSizeSelect:
				theProc = (ProcPtr)DataHandlerGetAvailableFileSize;
				theInfo = fat_pi_getavailablefilesize_ProcInfo;
				break;
			case kDataHGetFileNameSelect:
				theProc = (ProcPtr)DataHandlerGetFileName;
				theInfo = fat_pi_getfilename_ProcInfo;
				break;
			case kDataHGetMIMETypeSelect:
				theProc = (ProcPtr)DataHandlerGetMimeType;
				theInfo = fat_pi_getmimetype_ProcInfo;
				break;
			case kDataHGetMacOSFileTypeSelect:
				theProc = (ProcPtr)DataHandlerGetFileType;
				theInfo = fat_pi_getfiletype_ProcInfo;
				break;
			case kDataHOpenForReadSelect:
				theProc = (ProcPtr)DataHandlerOpenForRead;
				theInfo = fat_pi_openforread_ProcInfo;
				break;
			case kDataHCloseForReadSelect:
				theProc = (ProcPtr)DataHandlerCloseForRead;
				theInfo = fat_pi_closeforread_ProcInfo;
				break;
			case kDataHGetScheduleAheadTimeSelect:
				theProc = (ProcPtr)DataHandlerGetScheduleAheadTime;
				theInfo = fat_pi_getscheduleaheadtime_ProcInfo;
				break;
			case kDataHCompareDataRefSelect:
				theProc = (ProcPtr)DataHandlerCompareDataRef;
				theInfo = fat_pi_comparedataref_ProcInfo;
				break;
			case kDataHTaskSelect:
				theProc = (ProcPtr)DataHandlerDataTask;
				theInfo = fat_pi_datatask_ProcInfo;
				break;
			case kDataHScheduleDataSelect:
				theProc = (ProcPtr)DataHandlerScheduleData;
				theInfo = fat_pi_scheduledata_ProcInfo;
				break;
			case kDataHFinishDataSelect:
				theProc = (ProcPtr)DataHandlerFinishData;
				theInfo = fat_pi_finishdata_ProcInfo;
				break;

			case kDataHFlushDataSelect:
				theProc = (ProcPtr)DataHandlerFlushData;
				theInfo = fat_pi_finishdata_ProcInfo;
				break;


//			case kCloseDataRefSelect:
//				theProc = (ProcPtr)DataHandlerCloseDataRef;
//				theInfo = fat_pi_finishdata_ProcInfo;
//				break;

			default: 
				err = badComponentSelector; // not implemented
				break;
		}
	}

	if( theProc != nil ){
		err = ::CallComponentFunctionWithStorageProcInfo( storage, params, theProc, theInfo );
	}
//	if( hlProc ){
//		err = CallComponentFunctionWithStorage( (Handle)storage, params, hlProc );
//	}

#if DEBUG
	UDebug::Message( "    main(%x,%x) : %s - %x", (UInt32)params, (UInt32)storage,
			CDataHandlerComponent::Instance().GetSelectorName(params->what),
			params->what );
#endif

	return err;
}


/*
// ---------------------------------------------------------------------
//  MyOpen
// ---------------------------------------------------------------------
// Handles setting up C++ and then calling the real function Open

pascal ComponentResult MyOpen( Handle storage, ComponentInstance self )
{
	if( ::GetComponentRefcon( (Component)self ) == 0L ){
//		OSStatus resultCode = ::InitOpenTransport();
//		NH_SetupNewHandler();
		::SetComponentRefcon( (Component)self, 1L );
	}

	return DataHandlerOpen( (CDataHandlerConnection*)storage, self );
}


// ---------------------------------------------------------------------
//  MyClose
// ---------------------------------------------------------------------
// Handles calling the real function and then tearing down C++

pascal ComponentResult MyClose( Handle storage, ComponentInstance self )
{
	ComponentResult result = DataHandlerClose( (CDataHandlerConnection*)storage, self );

	if( (::GetComponentRefcon( (Component)self ) == 1L) &&
			(::CountComponentInstances( (Component)self ) == 1L) ){ // Last 1
//		NH_ResetNewHandler();
		::SetComponentRefcon( (Component)self, 0L );
	}
	return result;
}
*/

} // Extern "C"
