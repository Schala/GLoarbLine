/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#pragma once
#include "typedefs.h"

enum {
	kDontShowBytes		= 1,	// show "<1K" instead of "300" for example
	kSmallKilobyteFrac	= 2		// show fractions on less than 10K, eg, "1.8K"
};

class UText
{
	public:
		// comparison
		static bool Equal(const void *inTextA, Uint32 inSizeA, const void *inTextB, Uint32 inSizeB, Uint32 inEncoding, Uint32 inCaseSens);
		static Int16 Compare(const void *inTextA, Uint32 inSizeA, const void *inTextB, Uint32 inSizeB, Uint32 inEncoding = 0);
		static Int16 CompareInsensitive(const void *inDataA, const void *inDataB, Uint32 inSize, Uint32 inEncoding = 0);
		static Int16 CompareInsensitive(const void *inDataA, Uint32 inSizeA, const void *inDataB, Uint32 inSizeB, Uint32 inEncoding = 0);
		static Uint8 *SearchInsensitive(const void *inSearchData, Uint32 inSearchSize, const void *inData, Uint32 inDataSize);
		static Uint8 *ReplaceTextLoginMessage(const void *inSearchData, Uint32 inSearchSize, const void *inData, Uint32 inDataSize);
		
		// conversions to and from text
		static Uint32 IntegerToText(void *outText, Uint32 inMaxSize, Int32 inInteger, Uint32 inShowThousands = false);
		static Uint32 SizeToText(Uint32 inByteSize, void *outText, Uint32 inMaxSize, Uint32 inOptions = 0);
		static Uint32 SecondsToText(Uint32 inSeconds, void *outText, Uint32 inMaxSize);
		static Int32 TextToInteger(const void *inText, Uint32 inTextSize, Uint32 inEncoding = 0, Uint32 inBase = 0);
		static Uint32 TextToUInteger(const void *inText, Uint32 inTextSize, Uint32 inEncoding = 0, Uint32 inBase = 0);

		// formatting text
		static Uint32 Format(void *outText, Uint32 inMaxSize, const Int8 inFormat[], ...);
		static Uint32 FormatArg(void *outText, Uint32 inMaxSize, const Int8 inFormat[], void *inArgs);
		
		// operations on text
		static void MakeLowercase(void *ioText, Uint32 inSize, Uint32 inEncoding = 0);
		static void MakeUppercase(void *ioText, Uint32 inSize, Uint32 inEncoding = 0);
		static void ReplaceNonPrinting(void *ioText, Uint32 inSize, Uint32 inEncoding, Uint16 inReplaceWith, Uint32 inOptions = 0);
		
		// misc
		static Uint32 GetVisibleLength(const void *inText, Uint32 inTextSize, Uint32 inEncoding = 0);
		static Uint32 GetCaretTime();
		static bool FindWord(const void *inText, Uint32 inTextSize, Uint32 inEncoding, Uint32 inOffset, Uint32& outWordOffset, Uint32& outWordSize);

		// character types
		static bool IsAlphaNumeric(Uint16 inChar, Uint32 inEncoding = 0);		// a-z, A-Z, 0-9
		static bool IsNumeric(Uint16 inChar, Uint32 inEncoding = 0);			// 0-9
		static bool IsAlphabetic(Uint16 inChar, Uint32 inEncoding = 0);			// a-z, A-Z
		static bool IsControl(Uint16 inChar, Uint32 inEncoding = 0);			// 0x00-0x1F, 0x7F
		static bool IsGraph(Uint16 inChar, Uint32 inEncoding = 0);				// 0x21-0x7E
		static bool IsLower(Uint16 inChar, Uint32 inEncoding = 0);				// a-z
		static bool IsUpper(Uint16 inChar, Uint32 inEncoding = 0);				// A-Z
		static bool IsPrinting(Uint16 inChar, Uint32 inEncoding = 0);			// 0x20-0x7E except 0x7F
		static bool IsPunctuation(Uint16 inChar, Uint32 inEncoding = 0);		// #$%*()"'\`~:;,<>/?\|[]{}-_=+
		static bool IsSpace(Uint16 inChar, Uint32 inEncoding = 0);				// space, return, newline, tab etc
		static bool IsHex(Uint16 inChar, Uint32 inEncoding = 0);				// 0-9, A-F, a-f
		static bool IsNumeric(const Uint8 *inStr);
		static Uint16 ToUpper(Uint16 inChar, Uint32 inEncoding = 0);
		static Uint16 ToLower(Uint16 inChar, Uint32 inEncoding = 0);

		#define __control_char		0x01
		#define __motion_char		0x02
		#define __space_char		0x04
		#define __punctuation		0x08
		#define __digit				0x10
		#define __hex_digit			0x20
		#define __lower_case		0x40
		#define __upper_case		0x80

		#define __letter			(__lower_case | __upper_case)
		#define __alphanumeric		(__letter | __digit)
		#define __graphic			(__alphanumeric | __punctuation)
		#define __printable			(__graphic | __space_char)
		#define __whitespace		(__motion_char | __space_char)
		#define __control			(__motion_char | __control_char)

		static int isalnum(int c)	{	return (__ctype_map[(unsigned char)c] & __alphanumeric) != 0;	}
		static int isalpha(int c)	{	return (__ctype_map[(unsigned char)c] & __letter) != 0;			}
		static int iscntrl(int c)	{	return (__ctype_map[(unsigned char)c] & __control) != 0;		}
		static int isdigit(int c)	{	return (__ctype_map[(unsigned char)c] & __digit) != 0;			}
		static int isgraph(int c)	{	return (__ctype_map[(unsigned char)c] & __graphic) != 0;		}
		static int islower(int c)	{	return (__ctype_map[(unsigned char)c] & __lower_case) != 0;		}
		static int isprint(int c)	{	return (__ctype_map[(unsigned char)c] & __printable) != 0;		}
		static int ispunct(int c)	{	return (__ctype_map[(unsigned char)c] & __punctuation) != 0;	}
		static int isspace(int c)	{	return (__ctype_map[(unsigned char)c] & __whitespace) != 0;		}
		static int isupper(int c)	{	return (__ctype_map[(unsigned char)c] & __upper_case) != 0;		}
		static int isxdigit(int c)	{	return (__ctype_map[(unsigned char)c] & __hex_digit) != 0;		}
		static int tolower(int c)	{	return __lower_map[(unsigned char)c];							}
		static int toupper(int c)	{	return __upper_map[(unsigned char)c];							}
	
		static unsigned char __ctype_map[];
		static unsigned char __lower_map[];
		static unsigned char __upper_map[];

};




