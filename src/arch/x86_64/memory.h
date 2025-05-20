#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void *memcpy(void * restrict dest, const void * restrict src, size_t n);
void *memset(void *b, int c, size_t len);

#endif
