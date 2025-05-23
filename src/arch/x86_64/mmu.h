#ifndef MMU_H
#define MMU_H

#include "parse_multiboot.h"
#include <stddef.h>

#define MMU_NULL 1
#define KERNEL_NULL 0xf000000000000000

extern size_t DEBUG_NB_ALLOCS;

struct page_frame {
    struct page_frame *next;
    char empty[PAGE_FRAME_SIZE - sizeof(struct page_frame *)]; //PAGE_FRAME_SIZE = 4096
}__attribute__((packed));

void *MMU_pf_alloc(void);
void MMU_pf_free(void *pf);

void small_test_MMU(void);
void test_MMU(void);
void stress_test_MMU(void);

#endif