/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// Implements TCP connection operations for Windows & Linux
// UNIX will be tested soon :^))

#include "AW.h"
#if TARGET_OS_UNIX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
	#include <sys/time.h>
	#include <errno.h>
	#define INVALID_SOCKET (-1)
	#define SOCKET_ERROR (-1)
	#define closesocket close
	#define ioctlsocket ioctl
	#define WSAGetLastError() errno
	#define SOCKET int
#endif
#include "CNetTCPConnection.h"
#include "CNetTCPConnection_UW.h"
#include "CNetworkException.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
// CNetTCPConnection                                            [public]
// ---------------------------------------------------------------------
// Creates TCP connection class with default local address

CNetTCPConnection::CNetTCPConnection()
	: mPlatform( new CNetTCPConnection::CNetConnectionPS )
{
}

// ---------------------------------------------------------------------
// CNetTCPConnection                                            [public]
// ---------------------------------------------------------------------
// Creates TCP connection class and sets local address

CNetTCPConnection::CNetTCPConnection( const CNetAddress& inLocalAddress )
	: CNetConnection( inLocalAddress ), mPlatform( new CNetTCPConnection::CNetConnectionPS )
{
}

// ---------------------------------------------------------------------
// ~CNetTCPConnection                                  [virtual][public]
// ---------------------------------------------------------------------
// Destructor disconnects if connected

CNetTCPConnection::~CNetTCPConnection()
{
	Disconnect();
	delete mPlatform;
}

// ---------------------------------------------------------------------
// IsConnected                                         [virtual][public]
// ---------------------------------------------------------------------
// Is network connection active

bool
CNetTCPConnection::IsConnected()
{
	return bool(mPlatform->mSocket != INVALID_SOCKET);
}


// ---------------------------------------------------------------------
// Connect                                             [virtual][public]
// ---------------------------------------------------------------------
// Connects to remote host

bool
CNetTCPConnection::Connect( const CNetAddress& inRemoteAddress )
{
	try
	{
		if ( mPlatform->mSocket != INVALID_SOCKET )
			if ( inRemoteAddress == GetRemoteAddress() )
				return true;
			else
			{
				Disconnect();		//?? Is this a good idea?
				return false;		//?? Or just do this?
			}

		mPlatform->mSocket = socket( PF_INET, SOCK_STREAM, 0 );
		if ( mPlatform->mSocket == INVALID_SOCKET )
			THROW_NET_( eConnecting, eNetworkCannotCreate, inRemoteAddress, OS_ErrorCode(::WSAGetLastError()) );
		SetRemoteAddress( inRemoteAddress );

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = GetLocalAddress().GetPort();
		address.sin_addr.s_addr = GetLocalAddress().GetIP();
		if ( bind( mPlatform->mSocket, (sockaddr *)&address, sizeof( struct sockaddr ) ) != 0 )
		{
			Disconnect();
			THROW_NET_( eConnecting, eNetworkCannotBind, inRemoteAddress, OS_ErrorCode(::WSAGetLastError()) );
		}

		address.sin_port = inRemoteAddress.GetPort();
		address.sin_addr.s_addr = inRemoteAddress.GetIP();
		if ( connect( mPlatform->mSocket, (sockaddr *)&address, sizeof( struct sockaddr ) ) != 0 )
		{
			Disconnect();
			return false;
		}
	}
	catch (...)
	{
		RETHROW_NET_( eConnecting, inRemoteAddress );
	}
	SetRemoteAddress( inRemoteAddress );
	return true;
}


// ---------------------------------------------------------------------
// Disconnect                                          [virtual][public]
// ---------------------------------------------------------------------
// Disconnect if connected

void
CNetTCPConnection::Disconnect()
{
	if ( mPlatform->mSocket == INVALID_SOCKET )
		return;
//??	shutdown( mPlatform->mSocket, 2 );
	closesocket( mPlatform->mSocket );
	mPlatform->mSocket = INVALID_SOCKET;
}


// ---------------------------------------------------------------------
// GetAvailable                                        [virtual][public]
// ---------------------------------------------------------------------
// Get number of bytes available for reading

UInt32
CNetTCPConnection::GetAvailable()
{
	UInt32 received;
	if ( ioctlsocket( mPlatform->mSocket, FIONREAD, &received ) != 0 )
	{
		CNetAddress address;
		THROW_NET_( eReading, eNetworkCannotGetAvailable, address, OS_ErrorCode(::WSAGetLastError()) );
	}
	return received;
}

// ---------------------------------------------------------------------
// Read                                                [virtual][public]
// ---------------------------------------------------------------------
// Blocking read operation

UInt32
CNetTCPConnection::Read( UInt8 *inBuffer, UInt32 inMaxSize,
				UInt16 inTimeoutSec )
{
	if ( inTimeoutSec > 0 )
	{
		fd_set set;
		FD_ZERO( &set );
		FD_SET( mPlatform->mSocket, &set );
		timeval timeout;
		timeout.tv_sec = inTimeoutSec;
		timeout.tv_usec = 0;
		int rc = select( 1, &set, NULL, NULL, &timeout );
		if ( rc == 0 )
			return 0;
		else if ( rc < 0 )
		{
			CNetAddress address;
			THROW_NET_( eReading, eNetworkUnknown, address, OS_ErrorCode(::WSAGetLastError()) );
		}
	}
	int received = recv( mPlatform->mSocket, (char*)inBuffer, (size_t)inMaxSize, 0 );
	if ( received == SOCKET_ERROR )
	{
		CNetAddress address;
		THROW_NET_( eReading, eNetworkConnectionAborted, address, OS_ErrorCode(::WSAGetLastError()) );
	}
	return static_cast<UInt32>(received);
}


// ---------------------------------------------------------------------
// Write                                               [virtual][public]
// ---------------------------------------------------------------------
// Blocking write operation

UInt32
CNetTCPConnection::Write( UInt8 *inBuffer, UInt32 inMaxSize,
					UInt16 inTimeoutSec )
{
	if ( inTimeoutSec > 0 )
	{
		fd_set set;
		FD_ZERO( &set );
		FD_SET( mPlatform->mSocket, &set );
		timeval timeout;
		timeout.tv_sec = inTimeoutSec;
		timeout.tv_usec = 0;
		int rc = select( 1, NULL, &set, NULL, &timeout );
		if ( rc == 0 )
			return 0;
		else if ( rc < 0 )
		{
			CNetAddress address;
			THROW_NET_( eWriting, eNetworkUnknown, address, OS_ErrorCode(::WSAGetLastError()) );
		}
	}
	int sent = send( mPlatform->mSocket, (char*)inBuffer, (size_t)inMaxSize, 0 );
	if ( sent == SOCKET_ERROR )
	{
		CNetAddress address;
		THROW_NET_( eWriting, eNetworkConnectionAborted, address, OS_ErrorCode(::WSAGetLastError()) );
	}
	return static_cast<UInt32>(sent);
}

HL_End_Namespace_BigRedH