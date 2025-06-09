#ifndef STRING_H
#define STRING_H

#include <stddef.h>

size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
const char *strchr(const char *s, int c);
char *strcat(char *restrict dst, const char *restrict src);
char *strreverse(char *s);
char toupper(char c);
char tolower(char c);
char isupper(char c);
char islower(char c);
char *strdup(const char *s);

void *memcpy(void * restrict dest, const void * restrict src, size_t n);
void *memset(void *b, int c, size_t len);


#endif
