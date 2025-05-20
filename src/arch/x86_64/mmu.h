#ifndef MMU_H
#define MMU_H

#include "parse_multiboot.h"

void *MMU_pf_alloc(void);
void MMU_pf_free(void *pf);

void small_test_MMU(void);
void test_MMU(void);
void stress_test_MMU(void);

#endif