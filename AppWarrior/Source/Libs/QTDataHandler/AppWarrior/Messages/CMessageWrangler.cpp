/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CMessageWrangler.h"
#include "CListener.h"
#include "USyncroHelpers.h"
#include "CMessageException.h"

using namespace std;


HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//	CMessageWrangler                                            [public]
// ---------------------------------------------------------------------
// Constructor

CMessageWrangler::CMessageWrangler()
{
}


// ---------------------------------------------------------------------
//  CMessageWrangler                                   [public][virtial]
// ---------------------------------------------------------------------
// Destructor

CMessageWrangler::~CMessageWrangler()
{
	try {
		StMutex lLock(mLLocker);
		list<CListener*>::iterator it;
		for( it = mListeners.begin(); it != mListeners.end(); it++ ){
			(*it)->SetWrangler( nil );
		}
	} catch( ... ){
	}
}


// ---------------------------------------------------------------------
//  RegisterListener                                           [private]
// ---------------------------------------------------------------------
// Register a Listener to a Wrangler
//

void
CMessageWrangler::RegisterListener( CListener &inListener )
{
	try {
		StMutex lLock(mLLocker);
		list<CListener*>::iterator found;
		found = std::find( mListeners.begin(), mListeners.end(), &inListener );
		
		if( found == mListeners.end() ){ // Add if not already a Listener
			mListeners.push_back( &inListener );
		}
	} catch( ... ){
		RETHROW_MESSAGE_( eAddListener );
	}
}


// ---------------------------------------------------------------------
//  UnregisterListener                                             [private]
// ---------------------------------------------------------------------
// Unregister a Listener from a Broadcaster

void
CMessageWrangler::UnregisterListener( CListener &inListener )
{
	try {
		{
			StMutex lLock(mLLocker);
			mListeners.remove( &inListener );
		}
		StMutex qLock(mQLocker);
		// Remove all messages in the Q for this listener
		struct FindListener hasListener(&inListener);
		//?? remove_if is not working on MSVC so we copy the full code in here
		// mQueue.remove_if( hasListener );
		{ // remove_if
			std::list<ListenerMessagePair>::iterator e = mQueue.end();
			for (std::list<ListenerMessagePair>::iterator i = mQueue.begin()
				; i != e
				; ++i)
			{
				if (hasListener(*i))
				{
					std::list<ListenerMessagePair>::iterator j = i;
					for (++j; j != e; ++j)
						if (!hasListener(*j))
							break;
					i = mQueue.erase(i, j);
					if (i == e)
						break;
				}
			}
		} // remove_if
	} catch( ... ){
		RETHROW_MESSAGE_( eRemoveListener );
	}
}


// ---------------------------------------------------------------------
//	QueueMessage                                               [private]
// ---------------------------------------------------------------------
// Add a message and the listener to the Q

void
CMessageWrangler::QueueMessage( CListener &inListener,
								CMessage *inMessage )
{
	try {
		StMutex qLock(mQLocker);
		ListenerMessagePair theBronco( &inListener, inMessage );
		mQueue.push_back( theBronco );
	} catch( ... ){
		RETHROW_MESSAGE_( eQueueMessage );
	}
}


// ---------------------------------------------------------------------
//	HandleNextMessage                                           [public]
// ---------------------------------------------------------------------
// Send a message to the associated Listener

bool
CMessageWrangler::HandleNextMessage()
{
	CListener *theListener = nil;
	CMessage *theMessage = nil;
	try {
		{
			StMutex qLock(mQLocker);
			if( !mQueue.empty() ) {
				ListenerMessagePair theBronco( mQueue.front() );
				mQueue.pop_front();
				theListener = theBronco.first;
				theMessage = theBronco.second;
			}
		}
		if( (theListener != nil) && (theMessage != nil) ){
			theListener->ListenToMessage( *theMessage );
			delete theMessage;
			return true;
		} else {
			if( theMessage != nil ){
				delete theMessage;
			}
			return false;
		}	
	} catch( ... ){
		if( theMessage != nil ){
			delete theMessage;
		}
		RETHROW_MESSAGE_( eBroadcastMessage );
	}
	return false;
}


// ---------------------------------------------------------------------
//	HandleAllMessages                                           [public]
// ---------------------------------------------------------------------
// Send all messages to the associated Listeners

void
CMessageWrangler::HandleAllMessages()
{
	bool queueEmpty = false;

	try {
		while( true ){
			{
				StMutex qLock(mQLocker);
				queueEmpty = mQueue.empty();
			}
			if ( queueEmpty == true ) break;
			HandleNextMessage();
		}
	} catch( ... ){
		RETHROW_MESSAGE_( eBroadcastMessage );
	}
}

HL_End_Namespace_BigRedH
