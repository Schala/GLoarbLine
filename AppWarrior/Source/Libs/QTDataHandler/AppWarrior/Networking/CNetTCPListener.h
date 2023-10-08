// =====================================================================
//  CNetTCPListener.h                    (C) Hotline Communications 1999
// =====================================================================
//

#ifndef _H_CNetTCPListener_
#define _H_CNetTCPListener_

#if PRAGMA_ONCE
	#pragma once
#endif

#include "CNetAddress.h"

HL_Begin_Namespace_BigRedH

class CNetTCPConnection;

class CNetTCPListener
{
	public:
									CNetTCPListener( const CNetAddress& inListenAddress );
									// throws CMemoryException
		virtual					~CNetTCPListener();

			// ** Protocol **
		const CNetAddress&	GetRemoteAddress();
									// throws nothing

		void						Listen();
									// throws CNetworkException
		void						Accept( CNetTCPConnection &outConnection );
									// throws CNetworkException
		void						Reject();
									// throws nothing

	private:
		class CNetConnectionPS;
		CNetConnectionPS *mPlatform;

		const CNetAddress	mListenAddress;
		CNetAddress			mRemoteAddress;

								CNetTCPListener();
								CNetTCPListener(const CNetTCPListener&);
		CNetTCPListener&	operator= (const CNetTCPListener&);
};

HL_End_Namespace_BigRedH

#endif // _H_CNetTCPListener_
