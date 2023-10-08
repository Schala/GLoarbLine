/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "AW.h"
#include "CString.h"
#include "UStringConverter.h"
#include "StString.h"

HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  operator==                                                  [public]
// ---------------------------------------------------------------------
// 
bool CString::operator== (const CString& rhs) const {	
	return mString == rhs.mString;
}

// ---------------------------------------------------------------------
//  operator<                                                   [public]
// ---------------------------------------------------------------------
// 
bool
CString::operator< (const CString& rhs) const
{
	return mString < rhs.mString;
}

// ---------------------------------------------------------------------
//  CString                                                     [public]
// ---------------------------------------------------------------------
// 
CString::CString (const char* inPtr)
{
	Init(inPtr, std::strlen(inPtr));
}

// ---------------------------------------------------------------------
//  CString                                                     [public]
// ---------------------------------------------------------------------
// 
CString::CString (const unsigned char* inPtr)
{
	Init(&inPtr[1], inPtr[0]);
}

// ---------------------------------------------------------------------
//  CString                                                     [public]
// ---------------------------------------------------------------------
// 
CString::CString (const void* inPtr, std::size_t inLength)
{
	Init(inPtr, inLength);
}
	
// ---------------------------------------------------------------------
//  Init                                                       [private]
// ---------------------------------------------------------------------
// 
void
CString::Init (const void* inPtr, std::size_t inLength)
{
	(*this) = UStringConverter::StringDataToUnistring(inPtr, inLength);
}

// ---------------------------------------------------------------------
//  compareNoCase                                               [public]
// ---------------------------------------------------------------------
// 
int
CString::compareNoCase (const CString& inStr) const
{
	StCStyleString myString (*this);
	StCStyleString otherString (*this);
	for (int i=0;i<std::min(inStr.length(), length());++i) {
		int a = std::tolower(static_cast<char*>(myString)[i]);
		int b = std::tolower(static_cast<char*>(otherString)[i]);
		if (a<b) return -1;
		else if (a>b) return 1;
	}
	if (inStr.length() == length()) return 0;
	if (inStr.length() > length()) return 1;
	else return -1;
}

HL_End_Namespace_BigRedH