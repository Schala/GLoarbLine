// =====================================================================
//  CNetTCPConnection_UW.h               (C) Hotline Communications 2000
// =====================================================================
// Platform specific part of CNetTCPConnection class

HL_Begin_Namespace_BigRedH

class CNetTCPConnection::CNetConnectionPS
{
	public:
		SOCKET	mSocket;

		CNetConnectionPS() : mSocket( INVALID_SOCKET ) {}
};

HL_End_Namespace_BigRedH