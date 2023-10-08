/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */
// A utility class for getting temporary folder name

#include "aw.h"
#include "UFileUtils.h"
#include "CFSException.h"


HL_Begin_Namespace_BigRedH

// ---------------------------------------------------------------------
//  GetTemporaryFolder                                  [public][static]
// ---------------------------------------------------------------------
// Returns the pathname to the temporary folder on the current platform.

CString
UFileUtils::GetTemporaryFolder()
{
	char buffer[_MAX_PATH];
	try 
	{
		if (::GetTempPath(_MAX_PATH, buffer) == 0)
			THROW_OS_( GetLastError() );
	}
	catch( ... )
	{
		RETHROW_FS_( eGetTempFolder, L"" );
	}
	return CString(buffer);
}

HL_End_Namespace_BigRedH
