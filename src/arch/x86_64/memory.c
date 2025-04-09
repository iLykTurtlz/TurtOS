#include "memory.h"
#include <stddef.h>

void *memcpy(void * restrict dest, const void * restrict src, size_t n) {
	const unsigned char *s = src;
	unsigned char *d = dest;
	while (n--) {
		*d++ = *s++;
	}
	return dest;
}

void *memset(void *b, int c, size_t len) {
	const unsigned char v = c;
	unsigned char *d = b;
	while (len--) {
		*d++ = v;
	}
	return b;
}
