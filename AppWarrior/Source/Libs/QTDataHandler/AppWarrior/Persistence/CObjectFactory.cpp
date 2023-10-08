/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
//
// Maintains a table of (Class ID, Creator Function Pointer) pairs that
// is used for dynamically creating objects from external data at
// runtime

#include "AW.h"
#include "CObjectFactory.h"


HL_Begin_Namespace_BigRedH

std::auto_ptr<CObjectFactory> CObjectFactory::sInstance( nil );

// ---------------------------------------------------------------------
//	Instance                                            [static][public]
// ---------------------------------------------------------------------
// Get the singleton instance of the Object Factory

CObjectFactory&
CObjectFactory::Instance()
{
	try
	{
		if( sInstance.get() == 0 )
			sInstance = std::auto_ptr<CObjectFactory> ( new CObjectFactory );
	}
	catch (...) { RETHROW_PERSISTENCE_(eCreateFactory); }		
	return *sInstance;
}


// ---------------------------------------------------------------------
//	CObjectFactory                                             [private]
// ---------------------------------------------------------------------
// Constructor

CObjectFactory::CObjectFactory()
{
}


// ---------------------------------------------------------------------
//	~CObjectFactory                                             [public]
// ---------------------------------------------------------------------
// Destructor

CObjectFactory::~CObjectFactory()
{
}


// ---------------------------------------------------------------------
//  RegisterClass                                               [public]
// ---------------------------------------------------------------------
// Register a ClassID and associated ClassCreatorFunc
//
// If ClassID is already registered, the existing ClassCreatorFunc is
// replaced.

void
CObjectFactory::RegisterClass( ClassID inClassID,
		ClassCreatorFunc inCreatorFunc )
{
	try
	{
		mCreators[inClassID] = inCreatorFunc;
	}
	catch (...) { RETHROW_PERSISTENCE_(eRegister); }
}


// ---------------------------------------------------------------------
//  UnregisterClass                                             [public]
// ---------------------------------------------------------------------
// Unregister a class by removing its ClassID from class table

void
CObjectFactory::UnregisterClass( ClassID inClassID )
{
	try
	{
		mCreators.erase( inClassID );
	}
	catch (...) { RETHROW_PERSISTENCE_(eUnregister); }
}


// ---------------------------------------------------------------------
//  CreateObject                                                [public]
// ---------------------------------------------------------------------
// Return a newly created object of the specified class using data from
// a Stream???

void*
CObjectFactory::CreateObject( ClassID inClassID )
{
	void *object = nil;
	try
	{
		// this will fail if the ID is not found
		ASSERT( mCreators.find( inClassID ) != mCreators.end() );
		object = ((*(mCreators[inClassID]))());
	}
	catch (...) { RETHROW_PERSISTENCE_(eCreateObject); }
	return object;
}

HL_End_Namespace_BigRedH
