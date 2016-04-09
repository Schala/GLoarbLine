/**********************************************************************Might be nicer if the error type and error code were combined intothe one Uint32 value.  The high word could contain the type,and the low word the ID.Then it's like:Fail(error_Protocol);instead ofFail(errorType_Misc, error_Protocol);And error_Protocol would be defined like:error_Protocol = 101 | errorTypeMask_Misc	<<<<<< "mask" isn't the right word, it would be (errorType_Misc << 16), which is not a mask but the value for the hi wordAnd it would be easy to check IDs too:catch(SError& err){	if (err == error_Protocol) ...}*******************************************************************//* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory *//*UError includes functions for displaying an error alert to the user, gettingan error message from an error ID, providing file and line number informationabout where the error occured (when compiled for debug).It is based around the SError type, which is thrown as an exception when anerror is detected.-- Misc Errors --1		An unknown error has occured.2		An unknown error has occured (invalid value).3		An unknown error has occured (protocol breach).4		Operation is unimplemented.5		Operation timed out.6		Operation was aborted.7		Data version is not compatible.8		Data format is not compatible.9		Data is corrupt.10		Internal limit cannot be exceeded.11		Value is out of range.12		Specified item does not exist.13		An item with the same identifier already exists.-- Requirement Errors --100		A necessary requirement was not met.101		Multi-threading ability is required but not available.102		Sound ability is required but not available.103		Globally floating windows ability is required but not available.*/#include "UError.h"/* -------------------------------------------------------------------------- */// example:  ID 1/123 [-34] @ test.cp #104Uint32 UError::GetDetailMessage(const SError& inError, void *outText, Uint32 inMaxSize){	if (inError.file == nil || inError.file[0] == 0)	{		if (inError.special == 0)			return UText::Format(outText, inMaxSize, "ID %hi/%hi", inError.type, inError.id);		else			return UText::Format(outText, inMaxSize, "ID %hi/%hi [%li]", inError.type, inError.id, inError.special);	}	else	{		if (inError.special == 0)			return UText::Format(outText, inMaxSize, "ID %hi/%hi @ %s #%lu", inError.type, inError.id, inError.file, inError.line);		else			return UText::Format(outText, inMaxSize, "ID %hi/%hi [%li] @ %s #%lu", inError.type, inError.id, inError.special, inError.file, inError.line);	}}void UError::Log(TFSRefObj* inLogFile, const SError& inError){	Uint8 msg[256];	Uint8 detail[128];	Uint8 date[128];	Uint8 log[256];		try	{		// get info		date[0] = UDateTime::DateToText(date+1, sizeof(date)-1, kShortDateText + kTimeWithSecsText);		msg[0] = GetMessage(inError, msg+1, sizeof(msg)-1);		detail[0] = GetDetailMessage(inError, detail+1, sizeof(detail)-1);		log[0] = UText::Format(log+1, sizeof(log)-1, "%#-22.22s %#s (%#s)\r", date, msg, detail);						// create file if doesn't exist		if (!inLogFile->Exists()) inLogFile->CreateFile('TEXT', 'ttxt');				// write info to end of file		StFileOpener fop(inLogFile);		inLogFile->Write(inLogFile->GetSize(), log+1, log[0]);	}	catch(...)	{		// don't throw	}}void __Fail(Int16 inType, Int16 inID){	throw SError(inType, inID);}void __Fail(Int16 inType, Int16 inID, const Int8 *inFile, Uint32 inLine){	throw SError(inType, inID, inFile, inLine);}void __Fail(Int16 inType, Int16 inID, const Int8 *inFile, Uint32 inLine, bool inFatal, bool inSilent){	throw SError(inType, inID, inFile, inLine, inFatal, inSilent);}/* -------------------------------------------------------------------------- */#pragma mark -SError::SError(Int16 inID){	type = errorType_Misc;	id = inID;	file = "";	line = 0;	fatal = false;	silent = false;	special = 0;}SError::SError(Int16 inType, Int16 inID){	type = inType;	id = inID;	file = "";	line = 0;	fatal = false;	silent = false;	special = 0;}SError::SError(Int16 inID, const Int8 *inFile, Uint32 inLine){	type = errorType_Misc;	id = inID;	file = inFile;	line = inLine;	fatal = false;	silent = false;	special = 0;}SError::SError(Int16 inType, Int16 inID, const Int8 *inFile, Uint32 inLine){	type = inType;	id = inID;	file = inFile;	line = inLine;	fatal = false;	silent = false;	special = 0;}SError::SError(Int16 inID, const Int8 *inFile, Uint32 inLine, bool inFatal, bool inSilent){	type = errorType_Misc;	id = inID;	file = inFile;	line = inLine;	fatal = (inFatal != 0);	silent = (inSilent != 0);	special = 0;}SError::SError(Int16 inType, Int16 inID, const Int8 *inFile, Uint32 inLine, bool inFatal, bool inSilent){	type = inType;	id = inID;	file = inFile;	line = inLine;	fatal = (inFatal != 0);	silent = (inSilent != 0);	special = 0;}