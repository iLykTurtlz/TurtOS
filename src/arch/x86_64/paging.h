#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>


void MMU_init();

void *MMU_alloc_page(void);
void *MMU_alloc_pages(int num);
void MMU_free_page(void *);
void MMU_free_pages(void *, int num);



struct virtual_address {
    uint64_t phys_offset : 12;
    uint64_t L1_offset : 9;
    uint64_t L2_offset : 9;
    uint64_t L3_offset : 9;
    uint64_t L4_offset : 9;
    uint64_t sign_extension : 16;
}__attribute__((packed));

union vaddr {
    uint64_t value;
    struct virtual_address addr;
};

void test_paging(void);

// struct virtual_address {
//     uint64_t sign_extension : 16;
//     uint64_t L4_offset : 9;
//     uint64_t L3_offset : 9;
//     uint64_t L2_offset : 9;
//     uint64_t L1_offset : 9;
//     uint64_t phys_offset : 12;
// }__attribute__((packed));


// uint64_t test_page_table(struct virtual_address addr);
// uint64_t test_page_table(uint64_t vaddr);


#endif