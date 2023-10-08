/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "UText.h"
#include "stdarg.h"

static Uint32 _FormatTextArg(Int8 *sbuffer, Uint32 inMaxSize, const Int8 *fmt, va_list arg);

/* -------------------------------------------------------------------------- */

/*
#define __CNTL			1
#define __WHIT			2
#define __SPAC			4
#define __PUNC			8
#define __DIGT			16
#define __XDIG			32
#define __UPPR			64
#define __LOWR			128
#define __UPPX			(__XDIG|__UPPR)
#define __LOWX			(__XDIG|__LOWR)

static const Uint8 __ctypedata[256] = {
	// standard ASCII characters (0-127)
	__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,
	__CNTL,__WHIT,__WHIT,__WHIT,__WHIT,__WHIT,__CNTL,__CNTL,
	__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,
	__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,__CNTL,
	__SPAC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__DIGT,__DIGT,__DIGT,__DIGT,__DIGT,__DIGT,__DIGT,__DIGT,
	__DIGT,__DIGT,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__PUNC,__UPPX,__UPPX,__UPPX,__UPPX,__UPPX,__UPPX,__UPPR,
	__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,
	__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,
	__UPPR,__UPPR,__UPPR,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__PUNC,__LOWX,__LOWX,__LOWX,__LOWX,__LOWX,__LOWX,__LOWR,
	__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,
	__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,
	__LOWR,__LOWR,__LOWR,__PUNC,__PUNC,__PUNC,__PUNC,__CNTL,
	
#if MACINTOSH
	// mac extended characters (127-255)
	__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__LOWR,
	__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,
	__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,
	__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,__LOWR,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__LOWR,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__UPPR,__UPPR,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__LOWR,__LOWR,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__PUNC,__PUNC,__PUNC,__UPPR,__UPPR,__UPPR,__UPPR,__LOWR,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,
	__LOWR,__UPPR,__PUNC,__PUNC,__PUNC,__PUNC,__LOWR,__LOWR,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__UPPR,__UPPR,__UPPR,
	__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,__UPPR,
	__PUNC,__UPPR,__UPPR,__UPPR,__UPPR,__PUNC,__PUNC,__PUNC,
	__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__PUNC,__CNTL
#endif
};
*/

//#define __ctype_map[(unsigned char)c]		__ctypedata[(Uint8)(c)]
//#define __isspace(c)	(__ctype_map[(unsigned char)c] & (__WHIT|__SPAC))

//#define __ctype_map[(unsigned char)c]		__ctype_map[(unsigned char)c]
//#define __isspace(c)	(__ctype_map[(unsigned char)c] & __whitespace)

/*
static const unsigned char ___lower_map[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '[',  '\\', ']',  '^',  '_',
	'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '{',  '|',  '}',  '~',  0x7F,
#if MACINTOSH
	'Š',  'Œ',  '',  '',  '–',  'š',  'Ÿ',  '‡',  'ˆ',  '‰',  'Š',  '‹',  'Œ',  '',  '',  '',
	'',  '‘',  '’',  '“',  '”',  '•',  '–',  '—',  '˜',  '™',  'š',  '›',  'œ',  '',  '',  'Ÿ',
	' ',  '¡',  '¢',  '£',  '¤',  '¥',  '¦',  '§',  '¨',  '©',  'ª',  '«',  '¬',  '­',  '¾',  '¿',
	'°',  '±',  '²',  '³',  '´',  'µ',  '¶',  '·',  '¸',  '¹',  'º',  '»',  '¼',  '½',  '¾',  '¿',
	'À',  'Á',  'Â',  'Ã',  'Ä',  'Å',  'Æ',  'Ç',  'È',  'É',  'Ê',  'ˆ',  '‹',  '›',  'Ï',  'Ï',
	'Ğ',  '-',  'Ò',  'Ó',  'Ô',  'Õ',  'Ö',  '×',  'Ø',  'Ø',  'Ú',  'Û',  'Ü',  'İ',  'Ş',  'ß',
	'à',  'á',  'â',  'ã',  'ä',  '‰',  '',  '‡',  '‘',  '',  '’',  '”',  '•',  '“',  '—',  '™',
	'ğ',  '˜',  'œ',  '',  '',  'õ',  'ö',  '÷',  'ø',  'ù',  'ú',  'û',  'ü',  'ı',  'ş',  0xFF
#else
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
#endif
};

static const unsigned char ___upper_map[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[',  '\\', ']',  '^',  '_',
	'`',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '{',  '|',  '}',  '~',  0x7F,
#if MACINTOSH
	'€',  '',  '‚',  'ƒ',  '„',  '…',  '†',  'ç',  'Ë',  'å',  '€',  'Ì',  '',  '‚',  'ƒ',  'é',
	'æ',  'è',  'ê',  'í',  'ë',  'ì',  '„',  'î',  'ñ',  'ï',  '…',  '›',  'ò',  'ô',  'ó',  '†',
	' ',  '¡',  '¢',  '£',  '¤',  '¥',  '¦',  '§',  '¨',  '©',  'ª',  '«',  '¬',  '­',  '®',  '¯',
	'°',  '±',  '²',  '³',  '´',  'µ',  '¶',  '·',  '¸',  '¹',  'º',  '»',  '¼',  '½',  '®',  '¯',
	'À',  'Á',  'Â',  'Ã',  'Ä',  'Å',  'Æ',  'Ç',  'È',  'É',  'Ê',  'Ë',  'Ì',  'Í',  'Î',  'Î',
	'Ğ',  'Ñ',  'Ò',  'Ó',  'Ô',  'Õ',  'Ö',  '×',  'Ù',  'Ù',  'Ú',  'Û',  'Ü',  'İ',  'Ş',  'ß',
	'à',  'á',  'â',  'ã',  'ä',  'å',  'æ',  'ç',  'è',  'é',  'ê',  'ë',  'ì',  'í',  'î',  'ï',
	'ğ',  'ñ',  'ò',  'ó',  'ô',  'õ',  'ö',  '÷',  'ø',  'ù',  'ú',  'û',  'ü',  'ı',  'ş',  0xFF
#else
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
#endif
};
*/

/* -------------------------------------------------------------------------- */

bool UText::Equal(const void *inTextA, Uint32 inSizeA, const void *inTextB, Uint32 inSizeB, Uint32 /* inEncoding */, Uint32 inCaseSens)
{
	if (inSizeA != inSizeB) return false;

	Uint8 *pa = (Uint8 *)inTextA;
	Uint8 *pb = (Uint8 *)inTextB;
	Uint32 i;
	
	if (inCaseSens)
	{
		for (i = 0; i != inSizeA; i++)
			if (pa[i] != pb[i]) return false;
	}
	else
	{
		for (i = 0; i != inSizeA; i++)
			if (__lower_map[pa[i]] != __lower_map[pb[i]]) return false;
	}
	
	return true;
}

Int16 UText::Compare(const void *inTextA, Uint32 inSizeA, const void *inTextB, Uint32 inSizeB, Uint32 /* inEncoding */)
{
	if (inSizeA < inSizeB)
		return -1;
	else if (inSizeA > inSizeB)
		return 1;
	else
		return UMemory::Compare(inTextA, inTextB, inSizeA);
}

Int16 UText::CompareInsensitive(const void *inTextA, Uint32 inSizeA, const void *inTextB, Uint32 inSizeB, Uint32 /* inEncoding */)
{
	if (inSizeA < inSizeB)
		return -1;
	else if (inSizeA > inSizeB)
		return 1;
	else
		return UText::CompareInsensitive(inTextA, inTextB, inSizeA);
}

Int16 UText::CompareInsensitive(const void *inDataA, const void *inDataB, Uint32 n, Uint32 inEncoding)
{
	#pragma unused(inEncoding)

	const unsigned char *p1;
	const unsigned char *p2;


#if USE_PRE_INCREMENT
	for(p1 = (const unsigned char *)inDataA - 1, p2 = (const unsigned char *)inDataB - 1, n++; --n;)
	{
		if(tolower(*++p1) != tolower(*++p2))
			return (tolower(*p1) < tolower(*p2) ? -1 : +1);
	}

#else // USE_PRE_INCREMENT

	for(p1 = (const unsigned char *)inDataA, p2 = (const unsigned char *)inDataB, n++; --n;)
	{
		if(tolower(*p1++) != tolower(*p2++))
			return ((tolower(*--p1) < tolower(*--p2)) ? -1 : +1);
	}
#endif // USE_PRE_INCREMENT


#if 0
#if USE_PRE_INCREMENT
	
	for (p1 = (const unsigned char *)inDataA - 1, p2 = (const unsigned char *)inDataB - 1, n++; --n;)
	{
		// there is a bug here!  if the first condition is not met, the second is not tested and p2 does not get incremented!
		if (isalpha(*++p1) && isalpha(*++p2))
		{
			if (tolower(*p1) != tolower(*p2))
				return ((*p1 < *p2) ? -1 : +1);
		}
		else if (*p1 != *p2)
			return ((*p1 < *p2) ? -1 : +1);
	}
	
#else // USE_PRE_INCREMENT
	
	for (p1 = (const unsigned char *)inDataA, p2 = (const unsigned char *)inDataB, n++; --n;)
		if (isalpha(*p1) && isalpha(*p2))
		{
			if (tolower(*p1++) != tolower(*p2++))
				return ((*--p1 < *--p2) ? -1 : +1);
		}
		else if (*p1++ != *p2++)
			return ((*--p1 < *--p2) ? -1 : +1);

#endif // USE_PRE_INCREMENT
#endif // 0

	return 0;
}

// returns ptr to first occurance of <inSearchData> (of <inSearchSize> bytes in length) within <inData>
Uint8 *UText::SearchInsensitive(const void *inSearchData, Uint32 inSearchSize, const void *inData, Uint32 inDataSize)
{
	Uint8 *p, *tp, *sp;
	Uint32 sn;
	Uint8 firstc;
		
	if (inSearchSize == 1)
	{
		firstc = tolower(*(Uint8 *)inSearchData);
		p = (Uint8 *)inData;
		
		while (inDataSize--)
		{
			if (tolower(*p++) == firstc)
				return p-1;
		}
	}
	else if (inSearchSize)
	{
		firstc = tolower(*(Uint8 *)inSearchData);
		p = (Uint8 *)inData;
		
		while (inDataSize--)
		{
			if (tolower(*p) == firstc)
			{
				sp = ((Uint8 *)inSearchData) + 1;
				sn = inSearchSize - 1;
				tp = p + 1;
				if (sn > inDataSize) return nil;
				while (sn--)
				{
					if (tolower(*sp++) != tolower(*tp++)) goto keepgoing;
				}
				
				return p;
			}
			
			keepgoing:
			p++;
		}
	}
	
	// not found
	return nil;
}

/* -------------------------------------------------------------------------- */
#pragma mark -

Uint32 UText::IntegerToText(void *outText, Uint32 inMaxSize, Int32 inInteger, Uint32 inShowThousands)
{
	if (inMaxSize == 0) return 0;
	if (inInteger == 0)
	{
		*(Uint8 *)outText = '0';
		return 1;
	}
	
	Uint8 buf[32];
	Uint8 *p = buf + sizeof(buf);
	Uint8 *op = (Uint8 *)outText;
	Uint32 n = inInteger < 0 ? -inInteger : inInteger;
	Uint32 c = 0;
	
	if (inShowThousands)
	{
		for (; n; n /= 10)
		{
			*--p = n % 10 + '0';
			c++;
			if (c == 3)
			{
				*--p = ',';
				c = 0;
			}
		}
		if (*p == ',') p++;
	}
	else
	{
		for (; n; n /= 10)
			*--p = n % 10 + '0';
	}
	if (inInteger < 0) *--p = '-';
	
	n = (buf + sizeof(buf)) - p;
	if (n > inMaxSize) n = inMaxSize;
	inMaxSize = n;
	
	while (n--) *op++ = *p++;
		
	return inMaxSize;
}

Uint32 UText::SizeToText(Uint32 s, void *outText, Uint32 inMaxSize, Uint32 inOptions)
{
	Uint8 buf[64];
	Uint8 *op = (Uint8 *)outText;
	Uint8 *ap, *bp, *ep;
	Uint32 n, r;
	
	// error checking
#if DEBUG
	if (inMaxSize < 16)
		DebugBreak("UText::SizeToText - buffer is too small");
#else
	#pragma unused(inMaxSize)
#endif

	if (s == 0)
	{
		// zero bytes
		op[0] = '0';
		op[1] = 'K';
		return 2;
	}
	else if (s < 1024)
	{
		if (inOptions & kDontShowBytes)
		{
			// show less than one kilobyte
			op[0] = '<';
			op[1] = '1';
			op[2] = 'K';
			return 3;
		}
		else
		{
			// just show bytes
			ap = ep = &buf[sizeof(buf)];
			for (; s; s /= 10) *--ap = s % 10 + '0';
			for (bp = op; ap < ep;) *bp++ = *ap++;
			return bp - op;
		}
	}
	else if (s < 1048576)
	{
		// show kilobytes
		n = s >> 10;		// s / 1024
		r = (s - (n << 10)) / 103;			// 103 is from 1024/10, but rounded up to avoid r=10
		ap = ep = &buf[sizeof(buf)];
		if (r && (inOptions & kSmallKilobyteFrac) && n < 10)
		{
			for (; r; r /= 10) *--ap = r % 10 + '0';
			*--ap = '.';
		}
		for (; n; n /= 10) *--ap = n % 10 + '0';
		for (bp = op; ap < ep;) *bp++ = *ap++;
		*bp++ = 'K';
		return bp - op;
	}
	else
	{
		// show megabytes
		n = s >> 20;		// s / 1048576
		r = (s - (n << 20)) / 104858;		// 104858 is from 1048576/10, but rounded up to avoid r=10
		ap = ep = &buf[sizeof(buf)];
		if (r)
		{
			for (; r; r /= 10) *--ap = r % 10 + '0';
			*--ap = '.';
		}
		for (; n; n /= 10) *--ap = n % 10 + '0';
		for (bp = op; ap < ep;) *bp++ = *ap++;
		*bp++ = 'M';
		return bp - op;
	}
}

Uint32 UText::SecondsToText(Uint32 inSeconds, void *outText, Uint32 inMaxSize)
{
	Uint32 etrMin = inSeconds / 60;
	Uint32 etrHour = etrMin / 60;
	Uint32 etrDay = etrHour / 24;
	Uint32 etrSec = inSeconds - (etrMin * 60);
	etrMin -= (etrHour * 60);
	etrHour -= (etrDay * 24);
	
	if (etrDay)
		return UText::Format(outText, inMaxSize, "%lu:%.2lu:%.2lu:%.2lu", etrDay, etrHour, etrMin, etrSec);
	else if (etrHour)
		return UText::Format(outText, inMaxSize, "%lu:%.2lu:%.2lu", etrHour, etrMin, etrSec);
	else
		return UText::Format(outText, inMaxSize, "%lu:%.2lu", etrMin, etrSec);
}

Int32 UText::TextToInteger(const void *inText, Uint32 inTextSize, Uint32 /* inEncoding */, Uint32 inBase)
{
	if (!inText || !inTextSize)
		return 0;
	
	const Int8 *pText = (Int8 *)inText;
	Uint32 nTextSize = inTextSize;
	
	while  (nTextSize && (*pText == ' ' || *pText == '0'))
	{
		pText++;
		nTextSize--;
	}
	
	if (!nTextSize)
		return 0;
	
	Int8 str[64];
	if (nTextSize > sizeof(str) - 1) 
		nTextSize = sizeof(str) - 1;
	
	for (Uint32 i = 0; i != nTextSize; i++)
		str[i] = pText[i];
	
	str[nTextSize] = 0;
	
	return strtol(str, nil, inBase);
}

Uint32 UText::TextToUInteger(const void *inText, Uint32 inTextSize, Uint32 /* inEncoding */, Uint32 inBase)
{
	if (!inText || !inTextSize)
		return 0;

	const Int8 *pText = (Int8 *)inText;
	Uint32 nTextSize = inTextSize;
	
	while  (nTextSize && (*pText == ' ' || *pText == '0'))
	{
		pText++;
		nTextSize--;
	}
	
	if (!nTextSize)
		return 0;

	Int8 str[64];
	if (nTextSize > sizeof(str) - 1) 
		nTextSize = sizeof(str) - 1;
	
	for (Uint32 i = 0; i != nTextSize; i++)
		str[i] = pText[i];
	
	str[nTextSize] = 0;
	
	return strtoul(str, nil, inBase);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

/*
-- Formatting Numbers --

			Dec		Hex

Int32		%li		%lX, %lx
Uint32		%lu		%lX, %lx
Int16		%hi		%hX, %hx
Uint16		%hu		%hX, %hx
Uint8		%d		%X , %x
*/

Uint32 UText::Format(void *outText, Uint32 inMaxSize, const Int8 inFormat[], ...)
{
	Uint32 s;
	va_list args;
	
	va_start(args, inFormat);
	s = _FormatTextArg((Int8 *)outText, inMaxSize, inFormat, args);
	va_end(args);
	
	return s;
}

Uint32 UText::FormatArg(void *outText, Uint32 inMaxSize, const Int8 inFormat[], void *inArgs)
{
	return _FormatTextArg((Int8 *)outText, inMaxSize, inFormat, (va_list)inArgs);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

void UText::MakeLowercase(void *ioText, Uint32 inSize, Uint32 /* inEncoding */)
{
	Uint8 *p = (Uint8 *)ioText;
	
	while (inSize--)
	{
		*p = __lower_map[*p];
		p++;
	}
}

void UText::MakeUppercase(void *ioText, Uint32 inSize, Uint32 /* inEncoding */)
{
	Uint8 *p = (Uint8 *)ioText;
	
	while (inSize--)
	{
		*p = __upper_map[*p];
		p++;
	}
}

void UText::ReplaceNonPrinting(void *ioText, Uint32 inSize, Uint32 /* inEncoding */, Uint16 inReplaceWith, Uint32 inOptions)
{
	Uint8 *p = (Uint8 *)ioText;
	Uint8 c = inReplaceWith;
	
	if (inOptions & 1)	// if replace CRs and tabs as well
	{
		while (inSize--)
		{
			//if ((__ctype(*p) & (__UPPR|__LOWR|__DIGT|__PUNC|__SPAC)) == 0)
			if ((__ctype_map[*p] & (__upper_case|__lower_case|__digit|__punctuation|__space_char)) == 0)
				*p = c;
			p++;
		}
	}
	else
	{
		while (inSize--)
		{
			//if ((__ctype(*p) & (__UPPR|__LOWR|__DIGT|__PUNC|__SPAC|__WHIT)) == 0)
			if ((__ctype_map[*p] & (__upper_case|__lower_case|__digit|__punctuation|__space_char|__motion_char)) == 0)
				*p = c;
			p++;
		}
	}
}

// returns ptr to first occurance of <inSearchData> (of <inSearchSize> bytes in length) within <inData>
Uint8 *UText::ReplaceTextLoginMessage(const void *inSearchData, Uint32 inSearchSize, const void *inData, Uint32 inDataSize){


	Uint8 *p, *tp, *sp;
	Uint32 sn;
	Uint8 firstc;
		
	if (inSearchSize == 1)
	{
		firstc = tolower(*(Uint8 *)inSearchData);
		p = (Uint8 *)inData;
		
		while (inDataSize--)
		{
			if (tolower(*p++) == firstc)
				return p-1;
		}
	}
	else if (inSearchSize)
	{
		firstc = tolower(*(Uint8 *)inSearchData);
		p = (Uint8 *)inData;
		
		while (inDataSize--)
		{
			if (tolower(*p) == firstc)
			{
				sp = ((Uint8 *)inSearchData) + 1;
				sn = inSearchSize - 1;
				tp = p + 1;
				if (sn > inDataSize) return nil;
				while (sn--)
				{
					if (tolower(*sp++) != tolower(*tp++)) goto keepgoing;
				}
				
				return p;
			}
			
			keepgoing:
			p++;
		}
	}
	
	// not found
	return nil;
}
/* -------------------------------------------------------------------------- */
#pragma mark -

#if !MACINTOSH
Uint32 UText::GetVisibleLength(const void *inText, Uint32 inTextSize, Uint32 /* inEncoding */)
{
	Uint8 *p = (Uint8 *)inText + inTextSize;
	
	while (inTextSize-- && (isspace(*--p))) {}
	
	if (inTextSize == max_Uint32) return 0;
	
	return (p + 1) - (Uint8 *)inText;
}
#endif

#if !MACINTOSH
bool UText::FindWord(const void *inText, Uint32 inTextSize, Uint32 /* inEncoding */, Uint32 inOffset, Uint32& outWordOffset, Uint32& outWordSize)
{
	Uint8 *p;
	Uint32 n, startOffset, endOffset;
	
	if (inTextSize == 0)
	{
		outWordOffset = outWordSize = 0;
		return false;
	}
	if (inOffset >= inTextSize)
	{
		outWordOffset = inTextSize;
		outWordSize = 0;
		return false;
	}
		
	p = (Uint8 *)inText + inOffset;
	n = inOffset;
	
	if (UText::IsSpace(*p))
	{
		// look for word consisting of spaces
		
		while (n-- && isspace(*--p)) {}
	
		if (n == -1)
			startOffset = 0;
		else
			startOffset = (p + 1) - (Uint8 *)inText;
		
		p = (Uint8 *)inText + inOffset;
		n = inTextSize - inOffset;
		
		while (n-- && isspace(*p++)) {}
		
		if (n == -1)
			endOffset = inTextSize;
		else
			endOffset = p - (Uint8 *)inText - 1;
	}
	else
	{
		// look for word consisting of anything but spaces
		
		while (n-- && !isspace(*--p)) {}
	
		if (n == -1)
			startOffset = 0;
		else
			startOffset = (p + 1) - (Uint8 *)inText;
		
		p = (Uint8 *)inText + inOffset;
		n = inTextSize - inOffset;
		
		while (n-- && !isspace(*p++)) {}
		
		if (n == -1)
			endOffset = inTextSize;
		else
			endOffset = p - (Uint8 *)inText - 1;
	}
		
	outWordOffset = startOffset;
	outWordSize = endOffset - startOffset;

	return true;
}
#endif

/* -------------------------------------------------------------------------- */
#pragma mark -

#define ctrl	__control_char
#define motn	__motion_char
#define spac	__space_char
#define punc	__punctuation
#define digi	__digit
#define hexd	__hex_digit
#define lowc	__lower_case
#define uppc	__upper_case
#define dhex	(hexd | digi)
#define uhex	(hexd | uppc)
#define lhex	(hexd | lowc)

unsigned char UText::__ctype_map[256] = {
	// standard ASCII characters (0-127)
	
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, motn, motn, motn, motn, motn, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	spac, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	dhex, dhex, dhex, dhex, dhex, dhex, dhex, dhex, dhex, dhex, punc, punc, punc, punc, punc, punc,
	punc, uhex, uhex, uhex, uhex, uhex, uhex, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc,
	uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, punc, punc, punc, punc, punc,
	punc, lhex, lhex, lhex, lhex, lhex, lhex, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, punc, punc, punc, punc, ctrl,

	
#if MACINTOSH
	// mac extended characters (127-255)
	uppc, uppc, uppc, uppc, uppc, uppc, uppc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	punc, punc, punc, punc, punc, punc, punc, lowc, punc, punc, punc, punc, punc, punc, uppc, uppc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, lowc, lowc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, uppc, uppc, uppc, uppc, lowc,
	punc, punc, punc, punc, punc, punc, punc, punc, lowc, uppc, punc, punc, punc, punc, lowc, lowc,
	punc, punc, punc, punc, punc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc,
	punc, uppc, uppc, uppc, uppc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, 000,
//#elif BEOS
#else
	/*// BeOS extended characters (127-255)
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, spac, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,


	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
/*	
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
	
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn, motn,
	*/
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,

#endif
};

unsigned char UText::__lower_map[256] = {

	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '[',  '\\', ']',  '^',  '_',
	'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '{',  '|',  '}',  '~',  0x7F,


#if MACINTOSH
	'Š',  'Œ',  '',  '',  '–',  'š',  'Ÿ',  '‡',  'ˆ',  '‰',  'Š',  '‹',  'Œ',  '',  '',  '',
	'',  '‘',  '’',  '“',  '”',  '•',  '–',  '—',  '˜',  '™',  'š',  '›',  'œ',  '',  '',  'Ÿ',
	' ',  '¡',  '¢',  '£',  '¤',  '¥',  '¦',  '§',  '¨',  '©',  'ª',  '«',  '¬',  '­',  '¾',  '¿',
	'°',  '±',  '²',  '³',  '´',  'µ',  '¶',  '·',  '¸',  '¹',  'º',  '»',  '¼',  '½',  '¾',  '¿',
	'À',  'Á',  'Â',  'Ã',  'Ä',  'Å',  'Æ',  'Ç',  'È',  'É',  'Ê',  'ˆ',  '‹',  '›',  'Ï',  'Ï',
	'Ğ',  'Ñ',  'Ò',  'Ó',  'Ô',  'Õ',  'Ö',  '×',  'Ø',  'Ø',  'Ú',  'Û',  'Ü',  'İ',  'Ş',  'ß',
	'à',  'á',  'â',  'ã',  'ä',  '‰',  '',  '‡',  '‘',  '',  '’',  '”',  '•',  '“',  '—',  '™',
	'ğ',  '˜',  'œ',  '',  '',  'õ',  'ö',  '÷',  'ø',  'ù',  'ú',  'û',  'ü',  'ı',  'ş',  0xFF
#else
/*
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
*/
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  0xFF
#endif
};

unsigned char UText::__upper_map[256] = {

	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[',  '\\', ']',  '^',  '_',
	'`',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '{',  '|',  '}',  '~',  0x7F,

#if MACINTOSH
	'€',  '',  '‚',  'ƒ',  '„',  '…',  '†',  'ç',  'Ë',  'å',  '€',  'Ì',  '',  '‚',  'ƒ',  'é',
	'æ',  'è',  'ê',  'í',  'ë',  'ì',  '„',  'î',  'ñ',  'ï',  '…',  'Í',  'ò',  'ô',  'ó',  '†',
	' ',  '¡',  '¢',  '£',  '¤',  '¥',  '¦',  '§',  '¨',  '©',  'ª',  '«',  '¬',  '­',  '®',  '¯',
	'°',  '±',  '²',  '³',  '´',  'µ',  '¶',  '·',  '¸',  '¹',  'º',  '»',  '¼',  '½',  '®',  '¯',
	'À',  'Á',  'Â',  'Ã',  'Ä',  'Å',  'Æ',  'Ç',  'È',  'É',  'Ê',  'Ë',  'Ì',  'Í',  'Î',  'Î',
	'Ğ',  'Ñ',  'Ò',  'Ó',  'Ô',  'Õ',  'Ö',  '×',  'Ù',  'Ù',  'Ú',  'Û',  'Ü',  'İ',  'Ş',  'ß',
	'à',  'á',  'â',  'ã',  'ä',  'å',  'æ',  'ç',  'è',  'é',  'ê',  'ë',  'ì',  'í',  'î',  'ï',
	'ğ',  'ñ',  'ò',  'ó',  'ô',  'õ',  'ö',  '÷',  'ø',  'ù',  'ú',  'û',  'ü',  'ı',  'ş',  0xFF
#else
/*
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
	0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
	0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
	0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
	0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
	0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
	0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
*/
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  
	'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  'Ä',  0xFF
#endif
};



#pragma mark -

bool UText::IsAlphaNumeric(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__UPPR|__LOWR|__DIGT));
	return isalnum(c);
}

bool UText::IsNumeric(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__DIGT));
	return isdigit(c);
}

bool UText::IsAlphabetic(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__UPPR|__LOWR));
	return isalpha(c);
}

bool UText::IsControl(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__CNTL|__WHIT));
	return iscntrl(c);
}

bool UText::IsNumeric(const Uint8 *str)
{
	Int16 i, n;
	if (str==nil) return false;
	
	n = *str;
	for(i=1; i<=n; i++)
		//if (!(__ctype(str[i]) & (__DIGT))) return false;
		if (!(__ctype_map[str[i]] & (__digit))) return false;
	return true;
}

bool UText::IsGraph(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__UPPR|__LOWR|__DIGT|__PUNC));
	return isgraph(c);
}

bool UText::IsLower(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__LOWR));
	return islower(c);
}

bool UText::IsPrinting(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__UPPR|__LOWR|__DIGT|__PUNC|__SPAC));
	return isprint(c);
}

bool UText::IsPunctuation(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__PUNC));
	return ispunct(c);
}

bool UText::IsSpace(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__WHIT|__SPAC));
	return isspace(c);
}

bool UText::IsUpper(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__UPPR));
	return isupper(c);
}

bool UText::IsHex(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__DIGT|__XDIG));
	return isxdigit(c);
}

Uint16 UText::ToUpper(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__LOWR)) ? (c ^ 0x20) : c;
	return toupper(c);
}

Uint16 UText::ToLower(Uint16 c, Uint32 /* inEncoding */)
{
	//return (__ctype_map[(unsigned char)c] & (__UPPR)) ? (c ^ 0x20) : c;
	return tolower(c);
}

/* -------------------------------------------------------------------------- */
#pragma mark -

// sbuffer isn't a c-string, ie, it's not null terminated by this func
static Uint32 _FormatTextArg(Int8 *sbuffer, Uint32 inMaxSize, const char *fmt, va_list arg)
{
	struct format
	{
		unsigned short	leftJustify		: 1;
		unsigned short	forceSign		: 1;
		unsigned short	altForm			: 1;
		unsigned short	zeroPad			: 1;
		unsigned short	havePrecision	: 1;
		unsigned short	hSize			: 1;
		unsigned short 	lSize			: 1;
		unsigned short 	LSize			: 1;
		char			sign;
		char			exponent;
		int				fieldWidth;
		int				precision;
	} default_format = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	register long c, i, j, nwritten = 0;
	register unsigned long n;
	register char *s;
	char buf[512], *digits, *st;
	struct format F;

	for (c = *fmt; c; c = *++fmt)
	{
		if (c != '%') goto copy1;
		F = default_format;

		//  decode flags
		for (;;)
		{
			c = *++fmt;
			if      (c == '-')	F.leftJustify = true;
			else if (c == '+')	F.forceSign = true;
			else if (c == ' ')	F.sign = ' ';
			else if (c == '#')	F.altForm = true;
			else if (c == '0')	F.zeroPad = true;
			else break;
		}

		//  decode field width
		if (c == '*')
		{
			if ((F.fieldWidth = va_arg(arg, int)) < 0)
			{
				F.leftJustify = TRUE;
				F.fieldWidth = -F.fieldWidth;
			}
			c = *++fmt;
		}
		else
		{
			for (; c >= '0' && c <= '9'; c = *++fmt)
				F.fieldWidth = (10 * F.fieldWidth) + (c - '0');
		}

		//  decode precision
		if (c == '.')
		{
			if ((c = *++fmt) == '*')
			{
				F.precision = va_arg(arg, int);
				c = *++fmt;
			}
			else
			{
				for (; c >= '0' && c <= '9'; c = *++fmt)
					F.precision = (10 * F.precision) + (c - '0');
			}
			if (F.precision >= 0) F.havePrecision = true;
		}

		//  perform appropriate conversion

		s = &buf[sizeof(buf)];
		if (F.leftJustify) F.zeroPad = false;

conv:	switch (c)
		{
			case 'h':
				F.hSize = true;
				c = *++fmt;
				goto conv;
				
			case 'l' :
				F.lSize = true;
				c = *++fmt;
				goto conv;
				
			case 'L':
				F.LSize = true;
				c = *++fmt;
				goto conv;
				
			case 'd':
			case 'i':
				if (F.lSize) n = va_arg(arg, long);
				else n = va_arg(arg, int);
				if (F.hSize) n = (short) n;
				if ((long) n < 0) { n = -n; F.sign = '-'; }
				else if (F.forceSign) F.sign = '+';
				goto decimal;
				
			case 'u':
				if (F.lSize) n = va_arg(arg, unsigned long);
				else n = va_arg(arg, unsigned int);
				if (F.hSize) n = (unsigned short) n;
				F.sign = 0;
				goto decimal;
				
			decimal:
				if (!F.havePrecision)
				{
					if (F.zeroPad)
					{
						F.precision = F.fieldWidth;
						if (F.sign) --F.precision;
					}
					if (F.precision < 1) F.precision = 1;
				}
				for (i = 0; n; n /= 10, i++) *--s = n % 10 + '0';
				for (; i < F.precision; i++) *--s = '0';
				if (F.sign) { *--s = F.sign; i++; }
				break;

			case 'o':
				if (F.lSize) n = va_arg(arg, unsigned long);
				else n = va_arg(arg, unsigned int);
				if (F.hSize) n = (unsigned short) n;
				if (!F.havePrecision)
				{
					if (F.zeroPad) F.precision = F.fieldWidth;
					if (F.precision < 1) F.precision = 1;
				}
				for (i = 0; n; n /= 8, i++) *--s = n % 8 + '0';
				if (F.altForm && i && *s != '0') { *--s = '0'; i++; }
				for (; i < F.precision; i++) *--s = '0';
				break;

			case 'p':
				F.havePrecision = F.lSize = true;
				F.precision = 8;
				
			case 'X':
				digits = "0123456789ABCDEF";
				goto hexadecimal;
						
			case 'x':
				digits = "0123456789abcdef";

			hexadecimal:
				if (F.lSize) n = va_arg(arg, unsigned long);
				else n = va_arg(arg, unsigned int);
				if (F.hSize) n = (unsigned short) n;
				if (!F.havePrecision)
				{
					if (F.zeroPad)
					{
						F.precision = F.fieldWidth;
						if (F.altForm) F.precision -= 2;
					}
					if (F.precision < 1) F.precision = 1;
				}
				for (i = 0; n; n /= 16, i++) *--s = digits[n % 16];
				for (; i < F.precision; i++) *--s = '0';
				if (F.altForm) { *--s = c; *--s = '0'; i += 2; }
				break;

			case 'c':
				*--s = va_arg(arg, int);
				i = 1;
				break;

			case 's':
				s = va_arg(arg, char *);
				if (F.altForm)
				{
					i = (unsigned char) *s++;
					if (F.havePrecision && i > F.precision) i = F.precision;
				}
				else if (F.havePrecision)
				{
					i = 0;
					if (s)
					{
						st = s;
						while (*st++)
						{
							if (i == F.precision) break;
							++i;
						}
					}
				}
				else
				{
					i = 0;
					if (s)
					{
						st = s;
						while (*st++) ++i;
					}
				}
				break;

			case 'n':
				s = (char *)va_arg(arg, void *);
				if      (F.hSize) * (short *) s = nwritten;
				else if (F.lSize) * (long  *) s = nwritten;
				else              * (int   *) s = nwritten;
				continue;

			// unknown conversion, abort
			default:
				goto done;

			case '%':
			copy1:
				*sbuffer++ = c;
				++nwritten;
				if (nwritten == inMaxSize) goto done;
				continue;
		}

		//  pad on the left
		if (i < F.fieldWidth && !F.leftJustify)
		{
			do {
				*sbuffer++ = ' ';
				++nwritten;
				if (nwritten == inMaxSize) goto done;
			} while (i < --F.fieldWidth);
		}
		
		//  write the converted result
		for (j=0; j<i; j++)
		{
			*sbuffer++ = *s++;
			++nwritten;
			if (nwritten == inMaxSize) goto done;
		}

		//  pad on the right
		for (; i < F.fieldWidth; i++)
		{
			*sbuffer++ = ' ';
			++nwritten;
			if (nwritten == inMaxSize) goto done;
		}
	}

done:
	return nwritten;
}

Uint32 UMemory::HexToData(const void *inHex, Uint32 inHexSize, void *outData, Uint32 inMaxSize)
{
	Uint8 *sp, *ep, *op, *fp;
	Uint8 ca, cb, tca, tcb;
	
	sp = (Uint8 *)inHex;
	ep = sp + inHexSize;
	op = (Uint8 *)outData;
	fp = op + inMaxSize;
	
	for(;;)
	{
		if (sp == ep || op == fp) break;
		
		ca = *sp++;
		tca = UText::__ctype_map[ca];
		
		if (tca & __hex_digit)
		{
			if (tca & __upper_case)
				ca -= 'A' - 10;
			else if (tca & __lower_case)
				ca -= 'a' - 10;
			else
				ca -= '0';

			if (sp != ep)	// if have another character (not end)
			{
				cb = *sp++;
				tcb = UText::__ctype_map[cb];
				
				if (tcb & __hex_digit)
				{
					if (tcb & __upper_case)
						cb -= 'A' - 10;
					else if (tca & __lower_case)
						cb -= 'a' - 10;
					else
						cb -= '0';
					
					*op++ = (ca << 4) | cb;		// (ca * 16) + cb
				}
				else
				{
					*op++ = ca;
				}
			}
			else
			{
				*op++ = ca;
			}
		}
	}
	
	return op - (Uint8 *)outData;
}
