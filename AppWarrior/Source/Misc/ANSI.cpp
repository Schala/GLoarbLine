/* (c)2003 Hotsprings Inc. Licensed under GPL - see LICENSE in HotlineSources diresctory */

#include "ANSI.h"
#include "UMemory.h"

void __copy_mem(void * dst, const void * src, unsigned long n);
void __move_mem(void * dst, const void * src, unsigned long n);

#pragma warn_possunwant off

#define _begin_extern_c		extern "C" {
#define _end_extern_c		}

#if MACINTOSH
#pragma overload int errno;
#endif
int errno = 0;

#define MALLOC_WIN32_NO_UMEM	0

/* -------------------------------------------------------------------------- */

_begin_extern_c

/*
 * The malloc() function allocates a block of contiguous heap memory <size>
 * bytes large.  Returns a pointer to the first byte of the allocated block
 * if it is successful, or a null pointer if it fails.
 */
void *malloc(size_t size)
{
#if WIN32 && MALLOC_WIN32_NO_UMEM
	
	// DON'T use UMemory::New() here because windoze calls malloc() before WinMain() and it stuffs things up (causes crashes etc)
	return ::GlobalAlloc(GMEM_FIXED, size);
	
#else

	void *p;
	
	try
	{
		p = UMemory::New(size);
	}
	catch(...)
	{
		p = NULL;
	}
	
	return p;

#endif
}

/*
 * The calloc() function allocates contiguous space for <nmemb> elements of
 * size <size>. The space is initialized with zeroes.  Returns a pointer to
 * the first byte of the memory area allocated, or a null pointer if it
 * fails.
 */
void *calloc(size_t nmemb, size_t size)
{
#if WIN32 && MALLOC_WIN32_NO_UMEM
	
	// DON'T use UMemory::New() here because windoze calls malloc() before WinMain() and it stuffs things up (causes crashes etc)
	return ::GlobalAlloc(GMEM_FIXED, nmemb * size);
	
#else

	void *p;
	
	try
	{
		p = UMemory::NewClear(nmemb * size);
	}
	catch(...)
	{
		p = NULL;
	}
	
	return p;

#endif
}

/*
 * The realloc() function changes the size of the memory block pointed to by
 * <ptr> to <size> bytes.  The <size> argument can have a value smaller or
 * larger than the current size of the block <ptr> points to.  The <ptr>
 * argument must be a value from malloc() or calloc().  If <size> is 0, the 
 * memory block pointed to by <ptr> is released.  If <ptr> is a null pointer,
 * realloc() allocates <size> bytes. The old contents of the memory block are
 * preserved in the new block if the new block is larger than the old. If the
 * new block is smaller, the extra bytes are cut from the end of the old block.
 * Returns a pointer to the new block if it is successful and <size> is greater
 * than 0.  Returns a null pointer if not successful or <size> is 0.
 */
void *realloc(void *ptr, size_t size)
{
#if WIN32 && MALLOC_WIN32_NO_UMEM
	
	// see comments in malloc()
	if (ptr == NULL)
	{
		if (size != 0)
			ptr = ::GlobalAlloc(GMEM_FIXED, size);
	}
	else if (size == 0)
	{
		::GlobalFree((HGLOBAL)ptr);
		ptr = nil;
	}
	else
		ptr = ::GlobalReAlloc((HGLOBAL)ptr, size, GMEM_MOVEABLE);
	
	return ptr;
	
#else

	void *p;
	
	try
	{
		p = UMemory::Reallocate((TPtr)ptr, size);
	}
	catch(...)
	{
		p = NULL;
	}
	
	return p;

#endif
}

/*
 * The free() function releases the allocated block pointed to by <ptr>, which
 * must be a value from malloc(), calloc(), or realloc().  Once the memory block
 * <ptr> points to has been released, it is no longer valid. The <ptr> variable
 * must not be used to reference memory again until it is assigned a value from
 * the memory allocation functions.
 */
void free(void *ptr)
{
#if WIN32 && MALLOC_WIN32_NO_UMEM
	
	// see comments in malloc()
	::GlobalFree((HGLOBAL)ptr);
	
#else

	UMemory::Dispose((TPtr)ptr);

#endif
}

/*
 * The memcpy() function copies the first <len> characters from the memory
 * pointed to by <src> to the memory pointed to by <dst>.  The behavior of
 * memcpy() is undefined if the source and destination areas overlap.  Use
 * memmove() to reliably copies overlapping memory blocks.  Returns <dst>.
 */
void *memcpy(void *dst, const void *src, size_t len)
{
#if MACINTOSH
	::BlockMoveData(src, dst, len);
#else
	__copy_mem(dst, src, len);
#endif
	
	return dst;
}

/*
 * The memmove() function copies the first <len> characters from the memory
 * pointed to by <src> to the memory pointed to by <dst>.  Unlike memcpy(), the
 * memmove() function safely copies overlapping memory blocks.  Returns <dst>.
 */
void *memmove(void *dst, const void *src, size_t len)
{
#if MACINTOSH
	::BlockMoveData(src, dst, len);
#else
	__move_mem(dst, src, len);
#endif
	
	return dst;
}

/*
 * The memset() function assigns <val> to the first <n> bytes of the memory pointed
 * to by <dst>.  Returns <dst>.
 */
void *memset(void *dst, int val, size_t n)
{
	UMemory::Fill(dst, n, val);
	return dst;
}

/*
 * The memchr() function looks for the first occurrence of <val> in the first <n>
 * bytes of the memory pointed to by <src>.  Returns a pointer to the found byte, or
 * a null pointer (NULL) if not found.
 */
void *memchr(const void *src, int val, size_t n)
{
	const unsigned char *p;
	
	ASSERT(src);

#if USE_PRE_INCREMENT

	unsigned long v = val;
	
	for (p = (unsigned char *)src - 1, n++; --n;)
		if (*++p == v)
			return (void *)p;
	
#else
	
	unsigned char v = val;
	
	for (p = (unsigned char *)src, n++; --n;)
		if (*p++ == v)
			return (void *)(p-1);

#endif
	
	return NULL;
}

/*
 * The memrchr() function looks for the last occurrence of <val> in the first <n>
 * bytes of the memory pointed to by <src>.  Returns a pointer to the found byte, or
 * a null pointer (NULL) if not found.
 */
void *memrchr(const void *src, int val, size_t n)
{
	const unsigned char *p;
	
	ASSERT(src);

#if USE_PRE_INCREMENT

	unsigned long v = val;
	
	for (p = (unsigned char *)src + n, n++; --n;)
		if (*--p == v)
			return (void *)p;

	
#else
	
	unsigned char v = val;
	
	for (p = (unsigned char *)src + n, n++; --n;)
		if (*--p == v)
			return (void *)p;

#endif
	
	return nil;
}

/*
 * The memcmp() function compares the first <n> bytes of <src1> to <src2> one byte at
 * a time.  Returns zero if all <n> bytes pointed to by <src1> and <src2> are equal.
 * Returns a negative value if the first non-matching byte pointed to by <src1> is
 * less than the byte pointed to by <src2>.  Returns a positive value if the first
 * non-matching byte pointed to by <src1> is greater than the byte pointed to by
 * <src2>.
 */
int memcmp(const void *src1, const void *src2, size_t n)
{
	const unsigned char *p1;
	const unsigned char *p2;

	ASSERT(src1 && src2);

#if USE_PRE_INCREMENT
	
	for (p1 = (const unsigned char *)src1 - 1, p2 = (const unsigned char *)src2 - 1, n++; --n;)
		if (*++p1 != *++p2)
			return ((*p1 < *p2) ? -1 : +1);
	
#else
	
	for (p1 = (const unsigned char *)src1, p2 = (const unsigned char *)src2, n++; --n;)
		if (*p1++ != *p2++)
			return ((*--p1 < *--p2) ? -1 : +1);

#endif
	
	return 0;
}

/*
 * The strlen() function returns the number of characters in a character array not
 * including the terminating null character.
 */
 
#if USING_AW_NAMESPACE
 #ifdef strlen
  #undef strlen
 #endif
#endif
 
size_t strlen(const char *str)
{
	if (!str) return 0;
	
	unsigned long len = -1;
	
#if USE_PRE_INCREMENT
	
	unsigned char *p = (unsigned char *)str - 1;
	
	do
		len++;
	while (*++p);

#else
	
	do
		len++;
	while (*str++);

#endif
	
	return len;
}

#if USING_AW_NAMESPACE
 #define strlen ::strlen
#endif

/*
 * The strcpy() function copies the character array pointed to by <src> to the
 * character array pointed to <dst>.  The <src> argument must point to a null
 * terminated character array.  The resulting character array at <dst> is null
 * terminated as well.  If the arrays pointed to by <dst> and <src> overlap, the
 * operation of strcpy() is undefined.  Returns <dst>.
 */
char *strcpy(char *dst, const char *src)
{
	ASSERT(dst && src);
	
#if USE_PRE_INCREMENT
	
	const unsigned char *p = (unsigned char *)src - 1;
	unsigned char *q = (unsigned char *)dst - 1;
	
	while (*++q = *++p) ;
	
#else
	
	const char *p = src;
	char *q = dst;
	
	while (*q++ = *p++) ;

#endif
	
	return dst;
}

/*
 * The strncpy() function copies a maximum of <len> characters from the character
 * array pointed to by <src> to the character array pointed to by <dst>.  Neither
 * <dst> nor <src> must necessarily point to null terminated character arrays.
 * Also, <dst> and <src> must not overlap.  If a null character ('\0') is reached
 * in <src> before <len> characters have been copied, strncpy() continues padding
 * <dst> with null characters until <len> characters have been added to dest. The
 * function does not terminate <dst> with a null character if <len> characters are
 * copied from <src> before reaching a null character.  Returns <dst>.
 */
char *strncpy(char *dst, const char *src, size_t len)
{
	ASSERT(dst && src);

#if USE_PRE_INCREMENT

	const unsigned char *p = (const unsigned char *)src - 1;
	unsigned char *q = (unsigned char *)dst - 1;
	unsigned char zero = 0;
	
	len++;
	
	while (--len)
		if (!(*++q = *++p))
		{
			while (--len)
				*++q = 0;
			break;
		}

#else

	const char *p = src;
	char *q = dst;
	
	len++;
	
	while (--len)
		if (!(*q++ = *p++))
		{
			while (--len)
				*q++ = 0;
			break;
		}

#endif
	
	return dst;
}

/*
 * The strcat() function appends a copy of the character array pointed to by <src>
 * to the end of the character array pointed to by <dst>. The <dst> and <src>
 * arguments must both point to null terminated character arrays. strcat() null
 * terminates the resulting character array.  Returns <dst>.
 */
char *strcat(char *dst, const char *src)
{
	ASSERT(dst);

	if (src)
	{
	#if USE_PRE_INCREMENT

		const unsigned char *p = (unsigned char *)src - 1;
		unsigned char *q = (unsigned char *)dst - 1;
		
		while (*++q) ;
		
		q--;
		
		while (*++q = *++p) ;

	#else

		const char *p = src;
		char *q = dst;
		
		while (*q++) ;
		
		q--;
		
		while (*q++ = *p++) ;

	#endif
	}
	
	return dst;
}

/*
 * The strncat() function appends a maximum of <len> characters from the character
 * array pointed to by <src> to the character array pointed to by <dst>. The <dst>
 * argument must point to a null terminated character array. The <src> argument does
 * not necessarily have to point to a null terminated character array.  If a null
 * character is reached in <src> before <len> characters have been appended,
 * strncat() stops.  When done, strncat() terminates <dst> with a null character
 * ('\0').   Returns <dst>.
 */
char *strncat(char *dst, const char *src, size_t len)
{
	ASSERT(dst);

	if (src)
	{
	#if USE_PRE_INCREMENT

		const unsigned char *p = (unsigned char *)src - 1;
		unsigned char *q = (unsigned char *)dst - 1;
		
		while (*++q) ;
		
		q--; len++;
		
		while (--len)
			if (!(*++q = *++p))
			{
				q--;
				break;
			}
		
		*++q = 0;

	#else

		const char *p = src;
		char *q = dst;
		
		while (*q++) ;
		
		q--; len++;
		
		while (--len)
			if (!(*q++ = *p++))
			{
				q--;
				break;
			}
		
		*q = 0;

	#endif
	}
	
	return dst;
}

/*
 * The strcmp() function compares the character array pointed to by <str1> to the character
 * array pointed to by <str2>. Both <str1> and <str2> must point to null terminated
 * character arrays.  Returns zero if <str1> and <str2> are equal, a negative value if
 * <str1> is less than <str2>, or a positive value if <str1> is greater than <str2>.
 */
int strcmp(const char *str1, const char *str2)
{
	ASSERT(str1 && str2);

#if USE_PRE_INCREMENT
	
	const unsigned char *p1 = (unsigned char *)str1 - 1;
	const unsigned char *p2 = (unsigned char *)str2 - 1;
	unsigned long c1, c2;
		
	while ((c1 = *++p1) == (c2 = *++p2))
		if (!c1)
			return(0);

#else
	
	const unsigned char *p1 = (unsigned char *)str1;
	const unsigned char *p2 = (unsigned char *)str2;
	unsigned char c1, c2;
	
	while ((c1 = *p1++) == (c2 = *p2++))
		if (!c1)
			return(0);

#endif
	
	return (c1 - c2);
}

/*
 * The strncmp() function compares <len> characters of the character array pointed to
 * by <str1> to <len> characters of the character array pointed to by <str2>. Both
 * <str1> and <str2> do not necessarily have to be null terminated character arrays.
 * The function stops prematurely if it reaches a null character before <lan> characters
 * have been compared. Returns zero if the first <lane> characters of <str1> and <str2>
 * are equal, a negative value if <str1> is less than <str2>, or a positive value if 
 * <str1> is greater than <str2>.
 */
int strncmp(const char *str1, const char *str2, size_t len)
{
	ASSERT(str1 && str2);

#if USE_PRE_INCREMENT
	
	const unsigned char *p1 = (unsigned char *)str1 - 1;
	const unsigned char *p2 = (unsigned char *)str2 - 1;
	unsigned long c1, c2;
	
	len++;
	
	while (--len)
		if ((c1 = *++p1) != (c2 = *++p2))
			return (c1 - c2);
		else if (!c1)
			break;
	
#else
	
	const unsigned char *p1 = (unsigned char *)str1;
	const unsigned char *p2 = (unsigned char *)str2;
	unsigned char c1, c2;
	
	len++;
	
	while (--len)
		if ((c1 = *p1++) != (c2 = *p2++))
			return (c1 - c2);
		else if (!c1)
			break;

#endif
	
	return 0;
}

/*
 * The strchr() function searches for the first occurrence of the character <chr> in
 * the character array pointed to by <str>. The <str> argument must point to a null
 * terminated character array.  Returns a pointer to the found character, or a null
 * pointer if not found.
 */
char *strchr(const char *str, int chr)
{
	ASSERT(str);

#if USE_PRE_INCREMENT

	const unsigned char *p = (unsigned char *)str - 1;
	unsigned long c = (chr & 0xFF);
	unsigned long ch;
	
	while (ch = (*++p & 0xFF))
		if (ch == c)
			return (char *)p;
	
	return (c ? 0 : (char *)p);

#else

	const char *p = str;
	char c = chr;
	char ch;
	
	while (ch = *p++)
		if (ch == c)
			return ((char *)(p - 1));
	
	return (c ? 0 : (char *)(p - 1));

#endif
}

/*
 * The strchr() function searches for the last occurrence of the character <chr> in
 * the character array pointed to by <str>. The <str> argument must point to a null
 * terminated character array.  Returns a pointer to the found character, or a null
 * pointer if not found.
 */
char *strrchr(const char *str, int chr)
{
	ASSERT(str);

#if USE_PRE_INCREMENT

	const unsigned char *p = (unsigned char *)str - 1;
	const unsigned char *q = 0;
	unsigned long c = (chr & 0xff);
	unsigned long ch;
	
	while (ch = (*++p & 0xff))
		if (ch == c)
			q = p;
	
	if (q) return (char *)q;
	
	return (c ? 0 : (char *)p);

#else

	const char *p = str;
	const char *q = 0;
	char c = chr;
	char ch;
	
	while (ch = *p++)
		if (ch == c)
			q = p - 1;
	
	if (q) return (char *)q;
	
	return (c ? 0 : (char *)(p - 1));

#endif
}

typedef unsigned char char_map[32];

#define set_char_map(map, ch)	 map[ch>>3] |= (1 << (ch&7))
#define tst_char_map(map, ch) (map[ch>>3] &  (1 << (ch&7)))

/*
 * The strpbrk() function returns a pointer to the first character in <str> that matches
 * any character in <set>, or a null pointer if not found.
 */
char *strpbrk(const char *str, const char *set)
{
	ASSERT(str && set);

	const unsigned char *p;
	int c;
	char_map map = {0};
	
#if USE_PRE_INCREMENT
	
	p = (unsigned char *)set - 1;

	while (c = *++p)
		set_char_map(map, c);
	
	p = (unsigned char *)str - 1;
	
	while (c = *++p)
		if (tst_char_map(map, c))
			return (char *)p;
			
	return NULL;

#else
	
	p = (unsigned char *)set;

	while (c = *p++)
		set_char_map(map, c);
	
	p = (unsigned char *)str;
	
	while (c = *p++)
		if (tst_char_map(map, c))
			return((char *) (p - 1));
			
	return NULL;

#endif
}

/*
 * The strspn() function counts the initial number of characters in the character
 * array pointed to by <str> that contains characters in the character array pointed
 * to by <set>. The function starts counting characters at the beginning of <str>
 * and continues counting until it finds a character that is not in <set>. Both <str>
 * and <set> must point to null terminated character arrays.  Returns the number of
 * characters in <str> that match the characters in <set>.
 */
size_t strspn(const char *str, const char *set)
{
	ASSERT(str && set);

	const unsigned char *p;
	int	 c;
	char_map map = {0};
	
#if USE_PRE_INCREMENT
	
	p = (unsigned char *)set - 1;

	while (c = *++p)
		set_char_map(map, c);
	
	p = (unsigned char *)str - 1;
	
	while (c = *++p)
		if (!tst_char_map(map, c))
			break;
			
	return (p - (unsigned char *)str);

#else
	
	p = (unsigned char *)set;

	while (c = *p++)
		set_char_map(map, c);
	
	p = (unsigned char *)str;
	
	while (c = *p++)
		if (!tst_char_map(map, c))
			break;
			
	return (p - (unsigned char *)str - 1);

#endif
}

/*
 * The strcspn() function counts the initial length of the character array pointed
 * to by <str> that does not contain characters in the character array pointed to by
 * <set>. The function starts counting characters at the beginning of <str> and
 * continues counting until a character in <set> matches a character in <str>. 
 * Both <str> and <set> must point to null terminated character arrays.  Returns 
 * the length of characters in <str> that do not match any characters in <set>.
 */
size_t strcspn(const char *str, const char *set)
{
	ASSERT(str && set);

	const unsigned char *p;
	int	 c;
	char_map map = {0};
	
#if USE_PRE_INCREMENT
	
	p = (unsigned char *)set - 1;

	while (c = *++p)
		set_char_map(map, c);
	
	p = (unsigned char *)str - 1;
	
	while (c = *++p)
		if (tst_char_map(map, c))
			break;
			
	return (p - (unsigned char *)str);

#else
	
	p = (unsigned char *)set;

	while (c = *p++)
		set_char_map(map, c);
	
	p = (unsigned char *)str;
	
	while (c = *p++)
		if (tst_char_map(map, c))
			break;
			
	return (p - (unsigned char *)str - 1);

#endif
}

/*
Purpose
Extract tokens within a character array.

Prototype
char *strtok(char *str, const char *sep);

Remarks
The strtok() function tokenizes the character array pointed to by str. The sep argument
points to a character array containing token separator characters. The tokens in str are
extracted by successive calls to strtok().
The first call to strtok() causes it to search for the first character in str that does
not occur in sep. The function returns a pointer to the beginning of this first token.
If no such character can be found, strtok() returns a null pointer (NULL).
If, on the first call, strtok() finds a token, it searches for the next token.
The function searches by skipping characters in the token in str until a character in
sep is found. This character is overwritten with a null character to terminate the token
string, thereby modifying the character array contents. The function also keeps its own
pointer to the character after the null character for the next token. Subsequent token
searches continue in the same manner from the internal pointer.
Subsequent calls to strtok() with a NULL str argument cause it to return pointers to
subsequent tokens in the original str character array. If no tokens exist, strtok()
returns a null pointer. The sep argument can be different for each call to strtok().
Both str and sep must be null terminated character arrays.

Return
When first called strtok() returns a pointer to the first token in str or returns a null
pointer if no token can be found.
Subsequent calls to strtok() with a NULL str argument causes strtok() to return a pointer
to the next token or return a null pointer (NULL) when no more tokens exist.
strtok() modifies the character array pointed to by str.
*/
char *strtok(char *str, const char *set)
{
	ASSERT(str && set);

	unsigned char *p, *q;
	static unsigned char *n = (unsigned char *)"";
	static unsigned char *s = (unsigned char *)"";
	int c;
	char_map map	= {0};
	
	if (str)
		s = (unsigned char *)str;
	
#if USE_PRE_INCREMENT
	
	p = (unsigned char *)set - 1;

	while (c = *++p)
		set_char_map(map, c);
	
	p = s - 1;
	
	while (c = *++p)
		if (!tst_char_map(map, c))
			break;
	
	if (!c)
	{
		s = n;
		return NULL;
	}
	
	q = p;
	
	while (c = *++p)
		if (tst_char_map(map, c))
			break;
	
	if (!c)
		s = n;
	else
	{
		s  = p + 1;
		*p = 0;
	}
	
	return (char *)q;

#else
	
	p = (unsigned char *)set;

	while (c = *p++)
		set_char_map(map, c);
	
	p = s;
	
	while (c = *p++)
		if (!tst_char_map(map, c))
			break;
	
	if (!c)
	{
		s = n;
		return NULL;
	}
	
	q = p - 1;
	
	while (c = *p++)
		if (tst_char_map(map, c))
			break;
	
	if (!c)
		s = n;
	else
	{
		s    = p;
		*--p = 0;
	}
	
	return (char *)q;

#endif
}

/*
 * The strstr() function searches the character array pointed to by <str> for the
 * first occurrence of the character array pointed to by <pat>.  Both <str> and
 * <pat> must point to null terminated ('\0') character arrays.  Returns a pointer
 * to the first occurrence of <pat> in <str> and returns a null pointer (NULL) if
 * <pat> cannot be found.
 */
char *strstr(const char *str, const char *pat)
{
	ASSERT(str && pat);

#if USE_PRE_INCREMENT

	unsigned char *s1 = (unsigned char *)str-1;
	unsigned char *p1 = (unsigned char *)pat-1;
	unsigned long firstc, c1, c2;
	
	if (!(firstc = *++p1))
		return (char *)s1;

	while (c1 = *++s1)
		if (c1 == firstc)
		{
			const unsigned char *s2 = s1-1;
			const unsigned char *p2 = p1-1;
			
			while ((c1 = *++s2) == (c2 = *++p2) && c1) ;
			
			if (!c2)
				return (char *)s1;
		}
	
	return NULL;

#else

	unsigned char *s1 = (unsigned char *)str;
	unsigned char *p1 = (unsigned char *)pat;
	unsigned char firstc, c1, c2;
	
	if (!(firstc = *p1++))
		return (char *)s1;

	while (c1 = *s1++)
		if (c1 == firstc)
		{
			const unsigned char *s2 = s1;
			const unsigned char *p2 = p1;
			
			while ((c1 = *s2++) == (c2 = *p2++) && c1);
			
			if (!c2)
				return ((char *) s1 - 1);
		}
	
	return NULL;

#endif
}

// reverse a string in place
char *strrev(char *str)
{
	ASSERT(str);

	int SmallIndex = 0;
	int BigIndex = strlen(str) - 1;
	
	while (SmallIndex < BigIndex)
	{
		char Temp = str[SmallIndex];
		
		str[SmallIndex] = str[BigIndex];
		str[BigIndex] = Temp;
		
		SmallIndex++;
		BigIndex--;
	}
	
	return str;
}

unsigned char *pstrcpy(unsigned char *dst, const unsigned char *src)
{
	ASSERT(dst && src);
	
	// believe it or not, attempts to optimize this function only made it slower (PPC compiler, all optimizers ON)
	register unsigned int i;
	register unsigned int n = src[0];
	
	for (i = 0; i <= n; i++)
		dst[i] = src[i];
	
	return dst;
}

unsigned char *pstrncpy(unsigned char *dst, const unsigned char *src, size_t len)
{
	ASSERT(dst && src);
	
	// believe it or not, attempts to optimize this function only made it slower (PPC compiler, all optimizers ON)
	register unsigned int i;
	register unsigned int n = src[0];
	
	if (n > len) n = len;
	dst[0] = n;
	
	for (i = 1; i <= n; i++)
		dst[i] = src[i];
	
	return dst;
}

unsigned char *pstrcat(unsigned char *d, const unsigned char *s)
{
	register int i, j;

	ASSERT(d);

	if (s)
	{
		if (((j = s[0]) + d[0]) > 255)
			j = 255 - d[0];

		for (i = 0; i < j;)
			d[++d[0]] = s[++i];
	}
	
	return d;
}

unsigned char *pstrncat(unsigned char *dst, const unsigned char *src, size_t len)
{
	ASSERT(dst);

	if (src)
	{
		if (len > src[0]) len = src[0];
		
		if ((dst[0] + len) > 255)
			len = 255 - dst[0];
		
		unsigned char *dp = dst + dst[0];
		dst[0] += len;
		
		while (len--)
			*++dp = *++src;
	}
	
	return dst;
}

int pstrcmp(const unsigned char *s1, const unsigned char *s2)
{
	ASSERT(s1 && s2);

	register unsigned int	i;
	register int			cmp;
	unsigned int			l = s1[0];
	
	for( i = 0; i <= l; i++ )
	{
		cmp = *s1++ - *s2++;
		if (cmp) return cmp;
	}
	
	return 0;
}

int pstrncmp(const unsigned char *s1, const unsigned char *s2, size_t n)
{
	ASSERT(s1 && s2);

	register unsigned int	i;
	register int			cmp;
	unsigned int			l1 = s1[0],
							l2 = s2[0];
	
	for( i = 1; i <= l1 && i <= l2 && i <= n; i++ ) {
		cmp = *++s1 - *++s2;
		if( cmp ) {
			return( cmp );
		}
	}
	if( n <= l1 && n <= l2 ) {
		return( 0 );
	} else {
		return( l1 - l2 );
	}
}

unsigned char *pstrchr(const unsigned char *s, unsigned char c)
{
	ASSERT(s);

	register unsigned int	i;
	unsigned int			l = s[0];
	
	for( i = 1; i <= l; i++ )
	{
		if( *++s == c )
			return (unsigned char *) --s;
	}
	
	return NULL;
}

unsigned char *pstrrchr(const unsigned char *s, unsigned char c)
{
	ASSERT(s);

	register unsigned int	i;
	unsigned int			l = s[0];
	
	s = &s[s[0]];
	for( i = 1; i <= l; i++ )
	{
		if( *s-- == c )
			return (unsigned char *) ++s;
	}
	
	return NULL;
}

size_t pstrspn(const unsigned char *s1, const unsigned char *s2)
{
	ASSERT(s1 && s2);

	register unsigned int	i,
							j;
	unsigned int			l1 = s1[0],
							l2 = s2[0];
	
	if( l2 == 0 ) {
		return( 0 );
	}
	
	for( i = 1; i <= l1; i++ ) {
		for( j = 1; j <= l2; j++ ) {
			if( s1[i] != s2[j] ) {
				goto exit;
			}
		}
	}

exit:	
	return( --i );
}

size_t pstrcspn(const unsigned char *s1, const unsigned char *s2)
{
	ASSERT(s1 && s2);

	register unsigned int	i,
							j;
	unsigned int			l1 = s1[0],			
							l2 = s2[0];
	
	for( i = 1; i <= l1; i++ ) {
		for( j = 1; j <= l2; j++ ) {
			if( s1[i] == s2[j] ) {
				goto exit;
			}
		}
	}

exit:	
	return( --i );
}

long strtol(const char *nptr, char **endptr, int base)
{
	register char *s = (char *)nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (UText::isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)min_Int32 : max_Int32;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (UText::isdigit(c))
			c -= '0';
		else if (UText::isalpha(c))
			c -= UText::isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? min_Int32 : max_Int32;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = any ? s - 1 : (char *)nptr;
	return (acc);
}

unsigned long strtoul(const char *nptr, char **endptr, int base)
{
	register char *s = (char *)nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (UText::isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)max_Uint32 / (unsigned long)base;
	cutlim = (unsigned long)max_Uint32 % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (UText::isdigit(c))
			c -= '0';
		else if (UText::isalpha(c))
			c -= UText::isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = max_Uint32;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = any ? s - 1 : (char *)nptr;
	return (acc);
}

#if 0	// ****** haven't tested this
double strtod(const char *s, char **sret)
{
  double r;	/* result */
  int e;	/* exponent */
  double d;	/* scale */
  int sign;	/* +- 1.0 */
  int esign;
  int i;
  int flags=0;

  r = 0.0;
  sign = 1.0;
  e = 0;
  esign = 1;

  while ((*s == ' ') || (*s == '\t'))
    s++;

  if (*s == '+')
    s++;
  else if (*s == '-')
  {
    sign = -1;
    s++;
  }

  while ((*s >= '0') && (*s <= '9'))
  {
    flags |= 1;
    r *= 10.0;
    r += *s - '0';
    s++;
  }

  if (*s == '.')
  {
    d = 0.1;
    s++;
    while ((*s >= '0') && (*s <= '9'))
    {
      flags |= 2;
      r += d * (*s - '0');
      s++;
      d *= 0.1;
    }
  }

  if (flags == 0)
  {
    if (sret) *sret = s;
    return 0;
  }

  if ((*s == 'e') || (*s == 'E'))
  {
    s++;
    if (*s == '+')
      s++;
    else if (*s == '-')
    {
      *s++;
      esign = -1;
    }
    if ((*s < '0') || (*s > '9'))
    {
      if (sret) *sret = s;
      return r;
    }

    while ((*s >= '0') && (*s <= '9'))
    {
      e *= 10.0;
      e += *s - '0';
      s++;
    }
  }

  if (esign < 0)
    for (i = 1; i <= e; i++)
      r *= 0.1;
  else
    for (i = 1; i <= e; i++)
      r *= 10.0;

  if (sret) *sret = s;
  return r * sign;
}
#endif

// convert integer to string
char *ltostr(long val, char *str, int radix)
{
	char IsNegative = 0;
	long theNum = val;
	int StrIndex = 0;

	if (theNum < 0) {
		theNum = -theNum;
		IsNegative = 1;
	}
	
	do {
		long CurDigit = theNum % radix;
		if (CurDigit > 9)
			str[StrIndex++] = CurDigit + 'A' - 10;
		else
			str[StrIndex++] = CurDigit + '0';
		
		theNum /= radix;
	} while (theNum);
		
	if (IsNegative) {
		str[StrIndex++] = '-';
	}
	str[StrIndex++] = 0;
	
	// now reverse the string
	strrev(str);
	
	return str;
}

// convert unsigned integer to a string
char *ultostr(unsigned long val, char *str, int radix)
{
	unsigned long theNum = val;
	int StrIndex = 0;

	do {
		int CurDigit = theNum % radix;
		if (CurDigit > 9)
			str[StrIndex++] = CurDigit + 'A' - 10;
		else
			str[StrIndex++] = CurDigit + '0';
		
		theNum /= radix;
	} while (theNum);
		
	str[StrIndex++] = 0;
	
	// now reverse the string
	strrev(str);
		
	return str;
}

int sprintf(char *str, const char *fmt, ...)
{
#if WIN32
	
	va_list va;
	va_start(va, fmt);
	
	int n = ::wvsprintf(str, fmt, va);
	
	va_end(va);
	
	return n;
	
#else

	va_list va;
	va_start(va, fmt);
	
	int n = UText::FormatArg(str, max_Uint32, fmt, va);
	
	va_end(va);
	
	return n;

#endif
}

int vsprintf(char *str, const char *fmt, va_list arg)
{
#if WIN32

	return ::wvsprintf(str, fmt, arg);

#else

	return UText::FormatArg(str, max_Uint32, fmt, arg);

#endif
}

void __destroy_global_chain();

#define max_funcs	64

static void (*__atexit_funcs[max_funcs])(void);
static long	__atexit_curr_func = 0;

void exit(int status)
{
#if MACINTOSH
	#pragma unused(status)
#endif

	//__begin_critical_region(atexit_funcs_access);

	while (__atexit_curr_func > 0)
		(*__atexit_funcs[--__atexit_curr_func])();

	//__end_critical_region(atexit_funcs_access);
	
	//__kill_critical_regions();

#if MACINTOSH

	__destroy_global_chain();
	::ExitToShell();

#elif WIN32

	::ExitProcess(status);

#else

	#error "unknown OS"

#endif
}

int atexit(void (*func)(void))
{
	if (__atexit_curr_func == max_funcs)
		return -1;

	//__begin_critical_region(atexit_funcs_access);
		
	__atexit_funcs[__atexit_curr_func++] = func;
	
	//__end_critical_region(atexit_funcs_access);
	
	return 0;
}

void abort(void)
{
#if MACINTOSH

	::ExitToShell();

#elif WIN32

	::ExitProcess(1);

#else

	#error "unknown OS"

#endif
}

void *bsearch(const void *key, const void *table_base, size_t num_members, size_t member_size, _compare_func compare_members)
{
	size_t l, r, m;
	int c;
	char *mp;
	
	if (!key || !table_base || !num_members || !member_size || !compare_members)
		return NULL;
	
	mp = (char *)table_base;
	
	c = compare_members(key, mp);

	if (c == 0)
		return mp;
	else if (c < 0)
		return NULL;
	
	l = 1;
	r = num_members - 1;
	
	while (l <= r)
	{
		m = (l + r) / 2;
		
		mp = ((char *)table_base) + (member_size * m);
		
		c = compare_members(key, mp);

		if (c == 0)
			return mp;
		else if (c < 0)
			r = m - 1;
		else
			l = m + 1;
	}
	
	return NULL;
}

#define table_ptr(i)	(((char *)table_base) + (member_size * ((i) - 1)))

#if !__POWERPC__

	#define myinlineswap(dst, src, cnt)								\
	do {															\
		char *p;													\
		char *q;													\
		size_t n = cnt;												\
																	\
		unsigned char tmp;											\
																	\
		for (p = src, q = dst, n++; --n;)							\
		{															\
			tmp = *q;												\
			*q++ = *p;												\
			*p++ = tmp;												\
		}															\
	} while (0)
	
#else

	#define myinlineswap(dst, src, cnt)								\
	do {															\
		char *p;													\
		char *q;													\
		size_t n = cnt;												\
																	\
		unsigned long tmp;											\
																	\
		for (p = (char *)src - 1, q = (char *) dst - 1, n++; --n;)	\
		{															\
			tmp = *++q;												\
			*q = *++p;												\
			*p = tmp;												\
		}															\
	} while (0)

#endif

void qsort(void *table_base, size_t num_members, size_t member_size, _compare_func compare_members)
{
	// this could be made more efficient with special cases for member_size == 1, 2, 4, 8 bytes (the swap wouldn't need to loop)
	
	size_t l, r, /*i,*/ j;
	char *lp;
	char *rp;
	char *ip;
	char *jp;
	char *kp;
	
	if (num_members < 2) return;
	
	r = num_members;
	l = (r / 2) + 1;
	
	lp = table_ptr(l);
	rp = table_ptr(r);
	
	for (;;)
	{
		if (l > 1)
		{
			l--;
			lp -= member_size;
		}
		else
		{
			myinlineswap(lp, rp, member_size);
			
			if (--r == 1)
				return;
				
			rp -= member_size;
		}
			
		j = l;
		
		jp = table_ptr(j);
		
		while ((j * 2) <= r)
		{
			/*i = j;*/
			j *= 2;
			
			ip = jp;
			jp = table_ptr(j);
			
			if (j < r)
			{
				kp = jp + member_size;
				
				if (compare_members(jp, kp) < 0)
				{
					j++;
					jp = kp;
				}
			}
			
			if (compare_members(ip,jp) < 0)
				myinlineswap(ip, jp, member_size);	
			else
				break;
		}
	}
}

/*

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

unsigned char __ctype_map[256] = {
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
#elif BEOS
	// BeOS extended characters (127-255)
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, spac, punc, punc, punc, punc, punc,
	punc, punc, punc, punc, punc, punc, punc, punc, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
	ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl, ctrl,
#endif
};

unsigned char __lower_map[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '[',  '\\', ']',  '^',  '_',
	'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
	'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  '{',  '|',  '}',  '~',  0x7F,
#if MACINTOSH
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  0xFF
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

unsigned char __upper_map[256] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
	' ',  '!',  '"',  '#',  '$',  '%',  '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
	'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
	'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[',  '\\', ']',  '^',  '_',
	'`',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
	'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '{',  '|',  '}',  '~',  0x7F,
#if MACINTOSH
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',
	'�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  '�',  0xFF
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

_end_extern_c
#pragma warn_possunwant reset