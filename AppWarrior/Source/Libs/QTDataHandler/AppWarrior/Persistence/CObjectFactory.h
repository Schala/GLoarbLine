// =====================================================================//  CObjectFactory.h                     (C) Hotline Communications 1999// =====================================================================//	Associates class ID's with a function with returns an object of that //  class//	Used for creating classes from data in a Stream#ifndef _H_CObjectFactory_#define _H_CObjectFactory_#include "CPersistenceException.h"#if PRAGMA_ONCE	#pragma once#endif				 HL_Begin_Namespace_BigRedHclass	CObjectFactory {	public:			// ** Types **		typedef FourCharCode	ClassID;		typedef void*			(*ClassCreatorFunc)();			// ** Construction **		static	CObjectFactory& Instance();									// throws PersistenceException								~CObjectFactory();									// throws nothing			// ** Operations **		void					RegisterClass( ClassID inClassID											 , ClassCreatorFunc inCreatorFunc );									// throws PersistenceException		void					UnregisterClass( ClassID inClassID );									// throws PersistenceException		void*					CreateObject( ClassID inClassID );									// throws PersistenceException			private:								CObjectFactory();									// throws nothing											static std::auto_ptr<CObjectFactory>	sInstance; // Singleton		std::map<ClassID,ClassCreatorFunc>		mCreators;};template <class T>class	CObjectFactoryOf{};#if 0	#pragma mark TObjectFactory#endif// =====================================================================//  TObjectFactory// =====================================================================// TObjectFactory will automatically declare a "create from stream"// function for a class and register that function with the// CObjectFactory. You do NOT have to declare a "create from stream"// function within the class itself.//// Examples://		Name of the class you want to register is "MyClass"//// If you define a class_ID within MyClass:////		TObjectFactory <MyClass>::Register();//// If you don't define a class_ID or want to use a different class_ID:////		TObjectFactory <MyClass>::RegisterID('ABCD');//// where 'ABCD' is a four-character class ID. This form can be used// to associate multiple class IDs with the same class.template <class T, class B>class TObjectFactory {	public:		static T*		Create()	                    	{ 		                    	return dynamic_cast<T*>(CObjectFactory		                    	::Instance().CreateObject( T::eClassID )); 	                    	}		static void		Register()							{	                        CObjectFactory::Instance().	                        		RegisterClass( T::eClassID,									(CObjectFactory::ClassCreatorFunc)CreateFunc );	                    	}		static void		RegisterID(CObjectFactory::ClassID inClassID)	                    	{	                        CObjectFactory::Instance().	                        		RegisterClass( inClassID,									(CObjectFactory::ClassCreatorFunc)CreateFunc );	                    	}	private:		friend class CObjectFactory;				static B*		CreateFunc()							{ return new T; }};	// Macros for those who don't like to type much.#define	REGISTER_(ClassName)				TObjectFactory<ClassName, >::Register()	// Registers a class for factory creation from an ID, and takes a base class name	// for the pointer type it returns.#define REGISTER_ID_(Class, Base, ClassID)	TObjectFactory< Class, Base >::RegisterID(ClassID)#define REGISTER_VIEW_(Class)				TObjectFactory< Class, CView >::Register()#define	CREATE_(Class)						TObjectFactory< Class >::Create()HL_End_Namespace_BigRedH#endif // _H_CObjectFactory_