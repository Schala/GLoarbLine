// =====================================================================
//  UDebug.h                             (C) Hotline Communications 1999
// =====================================================================
//

#ifndef _H_UDebug_
#define _H_UDebug_

#if PRAGMA_ONCE
	#pragma once
#endif


HL_Begin_Namespace_BigRedH

class UDebug
{
	public:
								// generate a debug breakpoint
		#if DEBUG		
				static void		Breakpoint();
									// throws nothing
		#else	
				static void		Breakpoint() {}
									// throws nothing
		#endif	

								// generate a debug message
								//?? maybe we should put a CString here
		#if DEBUG		
				static void		Message(const char inMsg[] , ...);
									// throws nothing
		#else
				static void		Message(const char inMsg[] , ...){}
									// throws nothing
		#endif						
};

HL_End_Namespace_BigRedH

#if DEBUG		
#if TARGET_OS_WIN32
	#include "UDebug_W.h"
#elif TARGET_OS_MAC
	#include "UDebug_M.h"
#elif TARGET_OS_UNIX
	#include "UDebug_U.h"
#endif
#endif

#endif //_H_UDebug_
