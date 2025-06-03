#ifndef KMALLOC_H
#define KMALLOC_H

#include <stddef.h>

void kfree(void *addr);
void *kmalloc(size_t size);

void kmalloc_init(void);

void test_kmalloc(void);

#endif