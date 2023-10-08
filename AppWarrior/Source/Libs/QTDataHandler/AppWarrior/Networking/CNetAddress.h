// =====================================================================
//  CNetAddress.h                        (C) Hotline Communications 1999
// =====================================================================
// IP address

#ifndef _H_CNetAddress_
#define _H_CNetAddress_

#if PRAGMA_ONCE
	#pragma once
#endif

HL_Begin_Namespace_BigRedH

class CNetAddress
{
	friend class CNetworkException;
	
	public:
			// ** Construction **
							CNetAddress();
							CNetAddress( const CString& inName,
										 UInt16 inPort,
										 bool inInit = false );
		explicit			CNetAddress( UInt32 inIP, UInt16 inPort = 0 );
							CNetAddress( const CNetAddress &inOther );
							~CNetAddress();

		bool				operator == ( const CNetAddress &inOther ) const
								{ return (mPort == inOther.mPort) &&
										(GetIP() == inOther.GetIP()); }
							// throws CNetworkException
		bool				operator != ( const CNetAddress &inOther ) const
								{ return (mPort != inOther.mPort) ||
										(GetIP() != inOther.GetIP()); }
							// throws CNetworkException

		CNetAddress&		operator = ( const CNetAddress &inOther );

			// ** Protocol **
		UInt32				GetIP()				const;
								// throws CNetworkException
		UInt16				GetPort()			const
								{ return mPort; }
		const CString&		GetName()			const;
								// throws CNetworkException

		void				GetHostAddress()	const;
								// throws CNetworkException
	private:
		mutable UInt32		mIP;
		UInt16				mPort;
		mutable CString		mName;
};

HL_End_Namespace_BigRedH

#endif // _H_CNetAddress_
