/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// IP address

#include "AW.h"
#include "UNetUtilMac.h"
#include "CNetTCPConnection.h"
#include "CNetworkException.h"
#include "UAWError.h"

HL_Begin_Namespace_BigRedH

class CNetTCPConnection::CNetConnectionPS
{
	public:
		TEndpoint*	mSocket;

		CNetConnectionPS() : mSocket( kOTInvalidEndpointRef ) {}
};

static OSStatus HandleLookEvent( TEndpoint *inSocket, OSStatus inResult );

// ---------------------------------------------------------------------
//  CNetTCPConnection                                           [public]
// ---------------------------------------------------------------------
// Constructor

CNetTCPConnection::CNetTCPConnection()
	: CNetConnection(), mPlatform( new CNetTCPConnection::CNetConnectionPS )
{
}


// ---------------------------------------------------------------------
//  CNetTCPConnection                                           [public]
// ---------------------------------------------------------------------
// Constructor

CNetTCPConnection::CNetTCPConnection( const CNetAddress& inLocalAddress )
	: CNetConnection( inLocalAddress ), mPlatform( new CNetTCPConnection::CNetConnectionPS )
{
}


// ---------------------------------------------------------------------
//  ~CNetTCPConnection                                          [public]
// ---------------------------------------------------------------------
// Destructor

CNetTCPConnection::~CNetTCPConnection()
{
	try {
		Disconnect();
		delete mPlatform;
	} catch( ... ){
	}
}


// ---------------------------------------------------------------------
//  Connect                                                     [public]
// ---------------------------------------------------------------------
//

bool
CNetTCPConnection::Connect( const CNetAddress& inRemoteAddress )
{
	bool connected = false;

	try {
		if( mPlatform->mSocket == kOTInvalidEndpointRef ){ // Not connected
			OSStatus otStatus;
			TEndpoint *theSocket;
			SetRemoteAddress( inRemoteAddress );
			OTConfiguration *theConfig = ::OTCreateConfiguration( kTCPName );
		#if TARGET_API_MAC_CARBON
			theSocket = ::OTOpenEndpointInContext( theConfig, 0, nil, &otStatus, NULL);
		#else
			theSocket = ::OTOpenEndpoint( theConfig, 0, nil, &otStatus);
		#endif
			if( (theSocket != kOTInvalidEndpointRef) &&
						(otStatus == kOTNoError) ){
				// Get the endpoint info
				TEndpointInfo theInfo;
				otStatus = theSocket->GetEndpointInfo( &theInfo );
				THROW_OS_( otStatus );

				// Setup the options
				otStatus = theSocket->SetSynchronous();
				THROW_OS_( otStatus );
				otStatus = theSocket->SetBlocking();
				THROW_OS_( otStatus );
				otStatus = theSocket->UseSyncIdleEvents();
				THROW_OS_( otStatus );
			#if TARGET_API_MAC_CARBON
				otStatus = theSocket->InstallNotifier(UNetUtilMac::sYieldingNotifierUPP, nil );
			#else
				otStatus = theSocket->InstallNotifier(UNetUtilMac::YieldingNotifier, nil );
			#endif
				THROW_OS_( otStatus );

				// Setup the binding
				TBind reqAddr, retAddr;
				reqAddr.addr.len = reqAddr.addr.maxlen = theInfo.addr;
				reqAddr.addr.buf = (unsigned char*)UNetUtilMac::NetAddressToOT( GetLocalAddress() );
				retAddr.addr.len = retAddr.addr.maxlen = theInfo.addr;
				retAddr.addr.buf = (unsigned char*)::NewPtrClear(retAddr.addr.maxlen);
				otStatus = theSocket->Bind( &reqAddr, &retAddr );
				THROW_OS_( otStatus );
				SetLocalAddress( UNetUtilMac::OTToNetAddress( (InetAddress*)retAddr.addr.buf ) );
				::DisposePtr( (Ptr)reqAddr.addr.buf );
				::DisposePtr( (Ptr)retAddr.addr.buf );
			
				// Connect
				InetAddress remAddr;
				OTInitInetAddress( &remAddr, GetRemoteAddress().GetPort(),
									GetRemoteAddress().GetIP() );
				TCall sndCall;
				sndCall.addr.buf 	= (UInt8 *)&remAddr;
				sndCall.addr.len 	= sizeof(remAddr);
				sndCall.opt.buf 	= nil;		// no connection options
				sndCall.opt.len 	= 0;
				sndCall.udata.buf 	= nil;		// no connection data
				sndCall.udata.len 	= 0;
				sndCall.sequence 	= 0;		// ignored by OTConnect
				otStatus = theSocket->Connect( &sndCall, nil );
				THROW_OS_( otStatus );
				mPlatform->mSocket = theSocket;
				connected = true;
			} else {
				THROW_( eNetworkCannotCreate, otStatus );
			}
		}
	} catch (...) {
		RETHROW_NET_( eConnecting, inRemoteAddress );
	}
	return connected;
}


// ---------------------------------------------------------------------
//  IsConnected                                                  [public]
// ---------------------------------------------------------------------
//

bool
CNetTCPConnection::IsConnected()
{
	return mPlatform->mSocket != kOTInvalidEndpointRef;
}


// ---------------------------------------------------------------------
//  Disconnect                                                  [public]
// ---------------------------------------------------------------------
//

void
CNetTCPConnection::Disconnect()
{
	try {
		if( mPlatform->mSocket != kOTInvalidEndpointRef ){ // Connected
			OSStatus otStatus;
			otStatus = mPlatform->mSocket->SndDisconnect( nil );
			try {
				HandleLookEvent( mPlatform->mSocket, otStatus );
			} catch( ... ){
			}
			otStatus = mPlatform->mSocket->Unbind();
			THROW_OS_( otStatus );
			mPlatform->mSocket->RemoveNotifier();
			otStatus = mPlatform->mSocket->Close();
			THROW_OS_( otStatus );
			mPlatform->mSocket = kOTInvalidEndpointRef;
		}
	} catch( ... ){
		RETHROW_NET_( eDisconnecting, GetRemoteAddress() );
	}
}


// ---------------------------------------------------------------------
//  GetAvailable                                                [public]
// ---------------------------------------------------------------------
//

UInt32
CNetTCPConnection::GetAvailable()
{
	UInt32 bytesAvail = 0;
	
	try {
		if( mPlatform->mSocket != kOTInvalidEndpointRef ){ // Connected
			// Find out how many are available
			size_t theSize;
			OTResult theResult;
			theResult = mPlatform->mSocket->CountDataBytes( &theSize );
			if( theResult == kOTNoDataErr ){
				bytesAvail = 0;
			} else if( theResult < 0 ){
				THROW_OS_( HandleLookEvent( mPlatform->mSocket, theResult ) );
			} else {
				bytesAvail = theSize;
			}
		}
	} catch( ... ){
		RETHROW_NET_( eGetAvailable, GetRemoteAddress() );
	}

	return bytesAvail;
}


// ---------------------------------------------------------------------
//  Read                                                        [public]
// ---------------------------------------------------------------------
//

UInt32
CNetTCPConnection::Read( UInt8 *inBuffer, UInt32 inMaxSize,
					UInt16 inTimeoutSec )
{
	SInt32 bytesRead = 0;
	UInt32 maxSize = inMaxSize;
	
	try {
		if( mPlatform->mSocket != kOTInvalidEndpointRef ){ // Connected
			OTFlags junkFlags;
			OTResult theResult;
			size_t theSize;
			// Find out how many are available
			theResult = mPlatform->mSocket->CountDataBytes( &theSize );
			THROW_OS_( HandleLookEvent( mPlatform->mSocket, theResult ) );
			if( theSize > 0 ){
				if( theSize > maxSize ){
					theSize = maxSize;
				}
				bytesRead = mPlatform->mSocket->Rcv( inBuffer, theSize, &junkFlags );
				if( bytesRead < 0 ){
					THROW_OS_( bytesRead );
				}
			} else {
				theSize = 1;
				if( inTimeoutSec == 0 ){
					bytesRead = mPlatform->mSocket->Rcv( inBuffer, theSize, &junkFlags );
				} else {
					CNetTimeoutMac theTimer( mPlatform->mSocket, inTimeoutSec * 1000 );
					bytesRead = mPlatform->mSocket->Rcv( inBuffer, theSize, &junkFlags );
					if( theTimer.HasFired() ){
						THROW_( eNetworkConnectionTimedOut, kETIMEDOUTErr );
					}
				}
				if( bytesRead < 0 ){
					THROW_OS_( bytesRead );
				}
				maxSize--;
				theResult = mPlatform->mSocket->CountDataBytes( &theSize );
				if( theSize > 0 ){
					if( theSize > maxSize ){
						theSize = maxSize;
					}
					bytesRead = mPlatform->mSocket->Rcv( inBuffer+1, theSize, &junkFlags );
					if( bytesRead < 0 ){
						THROW_OS_( bytesRead );
					}
					bytesRead++; // Handle the first 1
				}
			}
		}
	} catch( ... ){
		RETHROW_NET_( eReading, GetRemoteAddress() );
	}

	return bytesRead;
}


// ---------------------------------------------------------------------
//  Read                                                        [public]
// ---------------------------------------------------------------------
//

UInt32
CNetTCPConnection::Write( UInt8 *inBuffer, UInt32 inMaxSize,
							UInt16 inTimeoutSec )
{
	UInt32 bytesWritten = 0;
	
	try {
		if( mPlatform->mSocket != kOTInvalidEndpointRef ){ // Connected
			if( inTimeoutSec > 0 ){
				CNetTimeoutMac theTimer( mPlatform->mSocket, inTimeoutSec * 1000 );
				bytesWritten = mPlatform->mSocket->Snd( inBuffer, inMaxSize, nil );
				if( theTimer.HasFired() ){
					THROW_( eNetworkConnectionTimedOut, kETIMEDOUTErr );
				}
			} else {
				bytesWritten = mPlatform->mSocket->Snd( inBuffer, inMaxSize, nil );
			}
			
			if( bytesWritten < 0 ){
				THROW_OS_( HandleLookEvent( mPlatform->mSocket, bytesWritten ) );
			}
		}
	} catch( ... ){
		RETHROW_NET_( eWriting, GetRemoteAddress() );
	}
	return bytesWritten;
}


// ---------------------------------------------------------------------
//  HandleLookEvent                                             [static]
// ---------------------------------------------------------------------
//

OSStatus
HandleLookEvent( TEndpoint *inSocket, OSStatus inResult )
{
	OSStatus otStatus = kOTNoError;
	
	if( inResult == kOTLookErr ){
		otStatus = inSocket->Look();
		switch( otStatus ){
			case T_DISCONNECT:
				otStatus = inSocket->RcvDisconnect( nil );
				if( otStatus == kOTNoError ){
					THROW_( eNetworkConnectionAborted, kNoOSError );
				} else {
					THROW_OS_( otStatus );
				}
				break;
			case T_ORDREL:
				otStatus = inSocket->RcvOrderlyDisconnect();
				if( otStatus == kOTNoError ){
					otStatus = inSocket->SndOrderlyDisconnect();
				} else {
					THROW_OS_( otStatus );
				}
				break;
			case T_DATA:
			case T_GODATA:
				otStatus = inSocket->Look();
				break;
		}
	}
	
	return otStatus;
}

HL_End_Namespace_BigRedH

