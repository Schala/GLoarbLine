// =====================================================================
//  CException.h                         (C) Hotline Communications 1999
// =====================================================================
// Base class for all exceptions

#ifndef _H_CException_
#define _H_CException_

#if PRAGMA_ONCE
	#pragma once
#endif				    

#ifndef _H_UAWError_
#include "UAWError.h"
#endif

#if (defined(COMPILER_CW) && defined(_MSL_NO_EXCEPTIONS)) ||\
	(defined(COMPILER_MSVC) && !defined(_CPPUNWIND))
	#pragma message("Exceptions are disabled, please set them to enabled!")
#endif

HL_Begin_Namespace_BigRedH
class CException
{
	public:
		
			// ** construction & destruction **
						 	CException( AW_ErrorCode inErr
						 			  #if DEBUG
						 			  , const char inFname[], int inLine
						 			  #endif
						 			  , OS_ErrorCode inCode = kNoOSError
						 			  );
						 	CException( const CException & inExcept );
		virtual				~CException( );
		
			// ** message reporting **
							//??(will change as we get CString)
		virtual CString	GetMessage() const;
	
			// ** AW_ErrorCode **
		AW_ErrorCode		GetErrorCode() const
									{ return mErrorCode; }
	
			// ** DEBUG mode management **
		#if DEBUG			// since __FILE__ is not unicode we have to 
							// use   char[]  as the arguments type
		void				SetSource( const char inFname[], int inLine );
		#endif
									
	private:
		AW_ErrorCode		mErrorCode;		// cross platform error code
		OS_ErrorCode		mErrorCodeOS;	// OS specific error code
		#if DEBUG
			CString			mSourceFile;
			UInt32			mSourceLine;
		#endif	
};	

#if DEBUG
	#define	_SOURCE_		,__FILE__,__LINE__
	#define	_SOURCEARG_		, const char inFname[], int inLine
	#define	_SOURCECOPY_	, inFname, inLine
#else
	#define	_SOURCE_
	#define	_SOURCEARG_
	#define	_SOURCECOPY_
#endif

#define THROW_( awErr, osErr )\
	throw CException( (UAWError::awErr) _SOURCE_, (osErr) )
#define THROW_UNKNOWN_()\
	THROW_( eUnknownExceptionError, kNoOSError )
#define THROW_OS_( osErr )	{ OS_ErrorCode err = (osErr); \
	if(err != kNoOSError) THROW_(eTranslate,err);}

HL_End_Namespace_BigRedH
#endif	// _H_CException_
