// =====================================================================
//  TObjectCreator.h                     (C) Hotline Communications 1999
// =====================================================================
//	Associates class ID's with a function with returns an object of that 
//  class
//	Used for creating classes from data in a Stream

#ifndef _H_TObjectCreator_
#define _H_TObjectCreator_

#if PRAGMA_ONCE
	#pragma once
#endif				 

#include "AWTypes.h"

HL_Begin_Namespace_BigRedH

template < class T > 
class TObjectFactory
{
	public:
			// ** Types **
		typedef FourCharCode	ClassID;
		typedef T*				(*ClassCreatorFunc)();

			// ** Operations **
		void					RegisterCreator( ClassID inClassID,
									ClassCreatorFunc inCreatorFunc );
									// throws ???
		void					UnregisterCreator( ClassID inClassID );
									// throws ???

		T*						CreateObject( ClassID inClassID );
									// throws CMemoryException, ???

	private:
		std::map<ClassID, ClassCreatorFunc> mCreators;

}; // class TObjectFactory

// ---------------------------------------------------------------------
//  RegisterCreator                                             [public]
// ---------------------------------------------------------------------
// Register a ClassID and associated ClassCreatorFunc
//
// If ClassID is already registered, the existing ClassCreatorFunc is
// replaced.

template <class T> void
TObjectFactory<T>::RegisterCreator( ClassID inClassID,
								  ClassCreatorFunc inCreatorFunc )
{
	mCreators[inClassID] = inCreatorFunc;
}


// ---------------------------------------------------------------------
//  UnregisterCreator                                           [public]
// ---------------------------------------------------------------------
// Unregister a class by removing its ClassID from class table

template<class T> void
TObjectFactory<T>::UnregisterCreator( ClassID inClassID )
{
	mCreators.erase( inClassID );
}


// ---------------------------------------------------------------------
//  CreateObject                                                [public]
// ---------------------------------------------------------------------
// Return a newly created object of the specified class using data from
// a Stream???

template<class T> T*
TObjectFactory<T>::CreateObject( ClassID inClassID )
{
	// this will fail if the ID is not found
	ASSERT( mCreators.find( inClassID ) != mCreators.end() );

	T *object = ((*(mCreators[inClassID]))());
	return object;
}


//template <class Class, class Base> Base*	
//TObjectCreatorFunctor()
//{
//	return new Class;
//}

template <class Class, class Base>
class TObjectFunctor
{
public:
	static Base* 		Create() 
							// throws CMemoryException, ???
							{ return new Class; }
};

/* example =============================================================
	
	TObjectFactory < CView > factory;
	
	// registration example
	factory.RegisterCreator(CEdit::eClassID, &TObjectFunctor<CEdit,CView>::Create);

	// object creation example
	CVierw* view = factory.CreateObject( id );
					// where id is taken from a stream or initialized as:
					// TObjectFactory < CView > :: ClassID id  = CEdit::eClassID;

============================================================= example */


HL_End_Namespace_BigRedH
#endif // _H_TObjectCreator_
