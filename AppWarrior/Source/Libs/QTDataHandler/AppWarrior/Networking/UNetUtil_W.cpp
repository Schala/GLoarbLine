/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "UNetUtil.h"


HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  StartNetworking                                     [public][static]
// ---------------------------------------------------------------------
// Start up Winsock

void
UNetUtil::StartNetworking()
{
	const UInt16 _winsock_ver_ = 0x0002;
	//
	WSADATA data;
	WSAStartup(_winsock_ver_, &data);
}


// ---------------------------------------------------------------------
//  StopNetworking                                      [public][static]
// ---------------------------------------------------------------------
// Close Winsock

void
UNetUtil::StopNetworking()
{
	WSACleanup();
}

HL_End_Namespace_BigRedH
