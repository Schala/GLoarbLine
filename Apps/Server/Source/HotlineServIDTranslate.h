/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

/*
	Routines for translating between a 32-bit integer and strings
	composed of chars that are valid in the Engage AdManager Software.
	Why?  Hotline supports only 32-bit community IDs, but a more intuitive value
	is desired than simply an integer.
*/
 
// the maximum length of the character ID
extern const int kHLCommID_MaxIDLen;

/*
	Translate from an unsigned long to a string
	outCharID is a NULL-terminated string size kHLCommID_MaxIDLen (+ 1 for NULL)
	returns 0 if success, -1 if failed (id is invalid)
*/

int HLCommID_IntToChar(unsigned long inIntID, char *outCharID);

/*
	Translate from a string to an unsigned long
	returns 0 if ID is invalid or empty

	inID is a NULL-terminated string
	(and only the first kHLCommID_MaxIDLen valid characters will be processed)
	
	returns the integer id
*/

unsigned long HLCommID_CharToInt(const char *inID);


// translate from unsigned integer to base 36
Uint32 TranslateToBase36(Uint32 inData, Uint8 *outData, Uint32 inMaxSize);
Uint32 TranslateToBase36(Uint32 inData, Uint8 *outData, Uint32 inMaxSize, Uint32 inMinSize);

// translate from base 36 to unsigned integer
Uint32 TranslateFromBase36(const Uint8 *inData, Uint32 inSize);

