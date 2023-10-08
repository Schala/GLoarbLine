/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// IP address

#include "AW.h"
#include "CNetAddress.h"
#include "UNetUtilMac.h"
#include <OpenTransport.h>
#include <OpenTptInternet.h>
#include "CNetworkException.h"
#include "StString.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  CNetAddress                                                 [public]
// ---------------------------------------------------------------------
// Default Constructor

CNetAddress::CNetAddress()
		: mIP(0), mPort(0)
{
}

// ---------------------------------------------------------------------
//  CNetAddress                                                 [public]
// ---------------------------------------------------------------------
// Constructor

CNetAddress::CNetAddress( const CString& inName, UInt16 inPort,
						bool inInit )
		: mIP(0), mPort(inPort), mName(inName)
{
	if( inInit ){
		GetIP();
	}

}


// ---------------------------------------------------------------------
//  CNetAddress                                                 [public]
// ---------------------------------------------------------------------
// Constructor

CNetAddress::CNetAddress( UInt32 inIP, UInt16 inPort )
		: mIP(inIP), mPort(inPort)
{
}


// ---------------------------------------------------------------------
//  CNetAddress                                                 [public]
// ---------------------------------------------------------------------
// Copy Constructor

CNetAddress::CNetAddress( const CNetAddress& inOther)
{
	operator = (inOther);
}


// ---------------------------------------------------------------------
//  ~CNetAddress                                                [public]
// ---------------------------------------------------------------------
// Destructor

CNetAddress::~CNetAddress()
{
}


// ---------------------------------------------------------------------
//  operator =                                                  [public]
// ---------------------------------------------------------------------
//

CNetAddress&
CNetAddress::operator = ( const CNetAddress& inOther )
{
	if(this != &inOther)
	{
		mIP   = inOther.mIP;
		mPort = inOther.mPort;
		mName = inOther.mName;
	}
	return *this;
}


// ---------------------------------------------------------------------
//  GetIP                                                       [public]
// ---------------------------------------------------------------------
// Gets the IP for the address.
// Lookup based off of the name if required (could be in dotted format)

UInt32
CNetAddress::GetIP() const
{
	try {
		if( (mIP == 0) && !mName.empty() ){
			StCStyleString s(mName);
			InetHostInfo theHost;
			OSStatus otStatus;
			
		#if TARGET_API_MAC_CARBON
			TInternetServices *theLooker = ::OTOpenInternetServicesInContext(kDefaultInternetServicesPath, 0, &otStatus, NULL);
		#else
			TInternetServices *theLooker = ::OTOpenInternetServices(kDefaultInternetServicesPath, 0, &otStatus);
		#endif
			
			if( (theLooker != nil) && (otStatus == kOTNoError) ){
				theLooker->SetSynchronous();
				theLooker->SetBlocking();
				theLooker->UseSyncIdleEvents();
			#if TARGET_API_MAC_CARBON
				theLooker->InstallNotifier(UNetUtilMac::sYieldingNotifierUPP, nil );
			#else
				theLooker->InstallNotifier(UNetUtilMac::YieldingNotifier, nil );
			#endif
				otStatus = theLooker->StringToAddress( s, &theHost );
				if( otStatus == kOTNoError ){
					mIP = theHost.addrs[0];
				} else {
					THROW_NET_( eResolvingAddress, eNetworkCannotResolve, *this, otStatus );
				}
				theLooker->RemoveNotifier();
				theLooker->Close();
			} else {
				THROW_NET_( eResolvingAddress, eNetworkCannotResolve, *this, otStatus );
			}
		}
	} catch (...) {
		RETHROW_NET_(eResolvingAddress,*this);
	}
	return mIP;
}


// ---------------------------------------------------------------------
//  GetName                                                     [public]
// ---------------------------------------------------------------------
// Gets the name for the address. Lookup from IP if needed.

const CString&
CNetAddress::GetName() const
{
	try {
		if( (mIP != 0) && mName.empty() ){
			InetDomainName theName;
			OSStatus otStatus;

		#if TARGET_API_MAC_CARBON
			TInternetServices *theLooker = ::OTOpenInternetServicesInContext(kDefaultInternetServicesPath, 0, &otStatus, NULL);
		#else
			TInternetServices *theLooker = ::OTOpenInternetServices(kDefaultInternetServicesPath, 0, &otStatus);
		#endif
		
			if( (theLooker != nil) && (otStatus == kOTNoError) ){
				theLooker->SetSynchronous();
				theLooker->SetBlocking();
				theLooker->UseSyncIdleEvents();
			#if TARGET_API_MAC_CARBON
				theLooker->InstallNotifier(UNetUtilMac::sYieldingNotifierUPP, nil );
			#else
				theLooker->InstallNotifier(UNetUtilMac::YieldingNotifier, nil );
			#endif
				otStatus = theLooker->AddressToName( mIP, theName );
				if( otStatus == kOTNoError ){
					mName = CString( theName );
				} else {
					THROW_NET_( eResolvingAddress, eNetworkCannotResolve, *this, otStatus );
				}
				theLooker->RemoveNotifier();
				theLooker->Close();
			} else {
				THROW_NET_( eResolvingAddress, eNetworkCannotResolve, *this, otStatus );
			}
		}
	} catch (...) {
		RETHROW_NET_(eResolvingAddress,*this);
	}
	return mName;
}


HL_End_Namespace_BigRedH

