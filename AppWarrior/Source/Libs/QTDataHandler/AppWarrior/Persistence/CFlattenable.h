// =====================================================================
//	CFlattenable.h                       (C) Hotline Communications 2000
// =====================================================================
// Base class for CImage CSound and any other flattenable object.
// It's a listener since it must listen to incoming data notifications 
// from the stream.

#ifndef _H_CFlattenable_
#define _H_CFlattenable_

#include "CInStream.h"
#include "CListener.h"

#if PRAGMA_ONCE
	#pragma once
#endif
			   
HL_Begin_Namespace_BigRedH

class CFlattenable : public CListener
{
	public:
									CFlattenable();
										// throws nothing
		virtual						~CFlattenable();
										// throws nothing

				// create object from mime type and input stream
		static std::auto_ptr<CFlattenable> 
									GetObjectFromType(const CString& inType);
										// throws nothing

				// override this function to read the initial data
		virtual void 				AttachInStream(std::auto_ptr<CInStream>& inInStream) = 0;
										// throws CMessageException

	protected:
		void						ListenToMessage( const CMessage &inMessage )=0;
										// throws CMessageException
		CInStream&					GetInStream();
										// throws CStreamException
	private:
		std::auto_ptr<CInStream> 	mInStream;
};

HL_End_Namespace_BigRedH
#endif // _H_CFlattenable_
