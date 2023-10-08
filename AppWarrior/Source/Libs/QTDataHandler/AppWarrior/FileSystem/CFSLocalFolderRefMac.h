/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// A reference to a local file system folder

#ifndef _H_CFSLocalFolderRefMac_
#define _H_CFSLocalFolderRefMac_

#include "CFSLocalFolderRef.h"
#include <Files.h>

#if PRAGMA_ONCE
	#pragma once
#endif
			   
HL_Begin_Namespace_BigRedH

class CFSLocalFolderRefMac : public CFSLocalFolderRef
{
	public:
									CFSLocalFolderRefMac
										(const FSSpec& inFSpec);
										// throws ???
									CFSLocalFolderRefMac
										(const CString& inFolderName);
										// throws ???
									CFSLocalFolderRefMac
										(const CFSLocalFolderRefMac& inOther);
										// throws ???

		virtual	std::auto_ptr<CFSRef> Clone() const;
										// throws CFSException

		virtual const CString& 	GetName() const;
										// throws nothing

		virtual const FSSpec&		GetFSpec() const;
										// throws CFSException

		virtual bool				IsEqual(const CFSRef& inOther) const;
										// throws nothing
		virtual bool				IsLessThan(const CFSRef& inOther) const;
										// throws nothing

		virtual void				LoadRights();
										// throws CFSException
		virtual CFileRights&		GetRights();
										// throws CFSException
//		virtual void				SaveRights();
										// throws CFSException

		virtual void 				Create();
										// throws CFSException
		virtual void 				Delete();
										// throws CFSException
		
		virtual void 				LoadContent();
										// throws CFSException
		virtual const Content&		GetContent();
										// throws CFSException

	private:
		Content						mContent;
		mutable FSSpec				mFSpec;
		mutable bool				mSpecSetup;
		CString					mFolderName;
		CFileRights					mRights;

		bool						mContentLoaded;
		bool						mRightsLoaded;
		
		void						SetupSpec( bool inMustExist = true ) const;
										// throws CException
}; // class CFSLocalFolderRefMac 

HL_End_Namespace_BigRedH
#endif // _H_CFSLocalFolderRefMac_