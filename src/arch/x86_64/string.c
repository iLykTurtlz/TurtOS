#include "string.h"
#include <stddef.h>

size_t strlen(const char *s)
{
	size_t res = 0;
	while (*s++)
		res++;
	return res;
}

char *strcpy(char *dest, const char *src)
{
	char *d = dest;
	while ((*d++ = *src++))
		;
	return dest;
}

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (unsigned char)*s1 - (unsigned char)*s2;
}

const char *strchr(const char *s, int c)
{
	char target = c;
	while (*s && *s != target)
		s++;
	return *s == target ? s : NULL;
}

char *strcat(char *restrict dst, const char *restrict src)
{
	char *ptr = dst + strlen(dst);
	while ((*ptr++ = *src++))
		;
	return dst;
}

char *strreverse(char *s)
{
	/* reverses the string in place, returns the same pointer */
	/* can only be called on writeable strings */
	char *lptr = s, *rptr = s + (strlen(s) - 1);
	while (lptr < rptr)
	{
		*lptr ^= *rptr;
		*rptr ^= *lptr;
		*lptr ^= *rptr;
		lptr++;
		rptr--;
	}
	return s;
}

char isupper(char c) {
	return 'A' <= c && c <= 'Z';
}

char islower(char c) {
	return 'a' <= c && c <= 'z';
}

char toupper(char c) {
	if (islower(c)) 
		return c - 32;
	return c;
}

char tolower(char c) {
	if (isupper(c))
		return c + 32;
	return c;
}


