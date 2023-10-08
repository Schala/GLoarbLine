// =====================================================================
//  UDebug_W.h                           (C) Hotline Communications 1999
// =====================================================================
//

#ifndef _H_UDebug_W_
#define _H_UDebug_W_

#if PRAGMA_ONCE
	#pragma once
#endif

#include <winbase.h>

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  Breakpoint                                  [public][static][inline]
// ---------------------------------------------------------------------
inline void UDebug::Breakpoint()
{
	#if _WIN32_WINDOWS==0x0410 && WINVER==0x0400
		if (0 != ::IsDebuggerPresent())
	#endif
			::DebugBreak();
}

// ---------------------------------------------------------------------
//  Message                                     [public][static][inline]
// ---------------------------------------------------------------------
inline void UDebug::Message(const char inMsg[] , ...)
{
	#if _WIN32_WINDOWS==0x0410 && WINVER==0x0400
		if (0 != ::IsDebuggerPresent())
	#endif
		{
			va_list arguments;				// declare
			va_start( arguments, inMsg );   // init
			char buffer[1024];				//??

			//DWORD len = ::FormatMessage( FORMAT_MESSAGE_FROM_STRING
			//						   , inMsg, 0, 0
			//						   , buffer, sizeof(buffer)
			//						   , arguments );
			::wvsprintf(buffer, inMsg, arguments ); //??
			::OutputDebugStringA(buffer); // .c_str());
		}	
}

HL_End_Namespace_BigRedH
#endif //_H_UDebug_W_
