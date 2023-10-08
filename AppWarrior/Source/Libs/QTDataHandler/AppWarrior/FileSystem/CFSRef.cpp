/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
#include "AW.h"
#include "CFSRef.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  Notify			                                   		 [protected]
// ---------------------------------------------------------------------
// notify the listener that the previous request was completed
// since it's a helper function this will be transparent to exceptions

void
CFSRef::Notify(UInt32 inTypeID, bool inSuccess)
{
	Message msg(inTypeID, inSuccess, *this);
	BroadcastMessage(msg);
}

HL_End_Namespace_BigRedH
