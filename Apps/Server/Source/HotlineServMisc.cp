/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "HotlineServ.h"


/* ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ */

#if WIN32
bool WinIsNewsCat(const Uint8 *inName, Uint32 s)
{
	return (s > 3 && inName[s - 4] == '.' &&
		   (inName[s - 3] == 'h' || inName[s - 3] == 'H') &&
		   (inName[s - 2] == 'n' || inName[s - 2] == 'N') &&
		   (inName[s - 1] == 'z' || inName[s - 1] == 'Z'));
}
#endif


