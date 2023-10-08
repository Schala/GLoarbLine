// =====================================================================
//  HL_Handler.h                       (C) Hotline Communications 2000
// =====================================================================
// A set of functions to respond to QuickTime's requests

#ifndef _H_HL_Handler_
#define _H_HL_Handler_


//-----------------------------------------------------------------------
// Includes
#include <Components.h>

typedef void (*HL_ProgressProc)( Str255 inFileName,
						unsigned long inFileSize,
						unsigned long inFileCompleted );
typedef void (*HL_EventProc)();


// Registration Etc. Functions
bool				HL_HandlerRegister();
void				HL_HandlerUnregister();
ComponentInstance	HL_HandlerOpen( Handle inDataRef );
bool				HL_HandlerIsReading( Str255 inTempFileName );
void				HL_HandlerCancelReading( Str255 inTempFileName );
void				HL_HandlerStop( ComponentInstance inInstance );
void				HL_HandlerClose( ComponentInstance inInstance );
void				HL_SetEventProc( HL_EventProc inProc );

void				HL_SetProgressProc( HL_ProgressProc inProc );
void				HL_Progress( Str255 inFileName,
						unsigned long inFileSize,
						unsigned long inFileCompleted );

void				HL_SetEventProc( HL_EventProc inProc );
void				HL_Event();

#endif //_H_HL_Handler_
