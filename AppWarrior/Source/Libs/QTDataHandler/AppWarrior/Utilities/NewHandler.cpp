/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "aw.h"

#if defined(COMPILER_MSVC)
#	include <new.h>
#else
#	include <new>
#endif

#include "NewHandler.h"
#include "CMemoryException.h"

using namespace std;

HL_Begin_Namespace_BigRedH

new_handler gOldHandler;
// ---------------------------------------------------------------------
//  NH_SetupNewHandler                                          [public]		
// ---------------------------------------------------------------------
// Setup routine. Register NH_NewHandler with system

void
NH_SetupNewHandler()
{
#if defined(COMPILER_MSVC)
	gOldHandler = (new_handler)_set_new_handler( NH_NewHandler );
#else
	gOldHandler = set_new_handler( NH_NewHandler );
#endif
}


// ---------------------------------------------------------------------
//  NH_SetupNewHandler                                          [public]		
// ---------------------------------------------------------------------
// Setup routine. Register NH_NewHandler with system

void
NH_ResetNewHandler()
{
#if defined(COMPILER_MSVC)
	_set_new_handler( (_PNH)gOldHandler );
#else
	set_new_handler( gOldHandler );
#endif
}


// ---------------------------------------------------------------------
//  NH_NewHandler                                               [public]		
// ---------------------------------------------------------------------
// Called when new fails. Throws CMemoryException

#if defined(COMPILER_MSVC)
int
NH_NewHandler(std::size_t)
#else
void
NH_NewHandler()
#endif
{
	THROW_MEMORY_( eAllocatingMemory, eMemoryNotEnough, kNoOSError );
}

HL_End_Namespace_BigRedH
