#ifndef KMALLOC_H
#define KMALLOC_H

#include <stddef.h>

extern void kfree(void *addr);
extern void *kmalloc(size_t size);

#endif