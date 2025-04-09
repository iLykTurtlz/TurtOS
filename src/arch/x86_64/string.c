#include "string.h"
#include <stddef.h>

size_t strlen(const char *s) {
	size_t res = 0;
	while (*(s++)) 
		res++;
	return res;
}

char *strcpy(char *dest, const char *src) {
	char *d = dest;
	while ((*(d++) = *(src++)));
	return dest;
}

int strcmp(const char *s1, const char *s2) {
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return (unsigned char)*s1 - (unsigned char)*s2; 
}

const char *strchr(const char *s, int c) {
	char target = c;
	while (*s && *s != target)
		s++;
	return *s == target ? s : NULL;
}
