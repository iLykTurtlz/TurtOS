#include "mmu.h"
#include "stddef.h"
#include "kprint.h"


#define MAX_NB_PAGING_REGIONS 10

//flag to determine whether the next frame should come from memory_regions or from freelist
static int reading_memory_regions = 1;


size_t DEBUG_NB_ALLOCS = 0;

static struct page_frame *freelist = (struct page_frame *)MMU_NULL;


void *MMU_pf_alloc(void) {
    if (reading_memory_regions) {
        struct memory_region *ptr = memory_info_page_start;
        while ( (ptr->start != (struct memory_region *)MEMORY_REGION_NULL) && (ptr->end - ptr->next < PAGE_FRAME_SIZE) ) {
            ptr++;
        }
        if (ptr->start == (struct memory_region *)MEMORY_REGION_NULL) {
            reading_memory_regions = 0;
            MMU_pf_free(memory_info_page_start); //don't need this anymore
            return MMU_pf_alloc();
        }
        void *retval = ptr->next;
        ptr->next = (void *)((uint64_t)ptr->next + PAGE_FRAME_SIZE);
        DEBUG_NB_ALLOCS++;
        // kprintf("Number of pf allocs: %ld\n", DEBUG_NB_ALLOCS);
        return retval;
    } 
    else if (freelist != (struct page_frame *)MMU_NULL) {
        void *retval = freelist;
        freelist = freelist->next;
        DEBUG_NB_ALLOCS++;
        // kprintf("Number of pf allocs: %ld\n", DEBUG_NB_ALLOCS);
        return retval;
    }
    else {
        kprintf("ERROR: all pages have been allocated!");
        return (void *)MMU_NULL;
    }
}

void MMU_pf_free(void *pf) {
    struct page_frame *p = pf;
    p->next = freelist;
    freelist = p;
}

void *high_memory_start() {
    struct memory_region *ptr = memory_info_page_start;
    while ((ptr+1)->start != (struct memory_region *)MEMORY_REGION_NULL) {
        ptr++;
    }
    return ptr->start;
}

void small_test_MMU(void) {
    struct page_frame *arr[160];
    for (size_t i=0; i<160; i++) {
        arr[i] = MMU_pf_alloc();
        if (i < 5 || i > 155) {
            kprintf("Allocated page %ld @ %p\n", i, arr[i]);
        }
    }
    for (size_t i=0; i<160; i++) {
        MMU_pf_free(arr[i]);
    }
    kprintf("Small test complete.\n");
}


void test_MMU(void) {
    // I calculated 32564 pages may be allocated before the next alloc comes from the freelist
    // in total: 32565 pages available
    // CORRECTION: it keeps changing, so I need to calculate it dynamically.
    //low memory pages + high memory pages + first page

    //record the start of each section containing frames, the number of pages per frame, the number of sections
    struct page_frame *starts[MAX_NB_PAGING_REGIONS];
    size_t nb_pages[MAX_NB_PAGING_REGIONS];
    int nb_sections = 0;

    // walk through the memory regions to find this info
    struct memory_region *ptr = memory_info_page_start;
    while (ptr->start != (struct memory_region *)MEMORY_REGION_NULL) {
        size_t segment_size = (uint64_t)ptr->end - (uint64_t)ptr->next;
        if (segment_size / PAGE_FRAME_SIZE > 0) {
            starts[nb_sections] = ptr->next; // or ptr->start
            nb_pages[nb_sections] = segment_size / PAGE_FRAME_SIZE;
            nb_sections++;
        }
        ptr++;
    }

    // output page locations
    for (int i=0; i<nb_sections; i++) {
        kprintf("%ld frames starting at %p\n", nb_pages[i], starts[i]);
    }

    // allocate all pages (with some output)
    void *p;
    kprintf("---------\n");
    for (int i=0; i<nb_sections; i++) {
        for (size_t j=0; j<nb_pages[i]; j++) {
            p = MMU_pf_alloc();
            if (j < 3) {
                kprintf("Page %ld of section starting at %p, at %p\n", j, starts[i], p);
            }
            if (j == 3)
                kprintf("...\n");
            if (j > nb_pages[i] - 4) {
                kprintf("Page %ld of section starting at %p, at %p\n", j, starts[i], p);
            }
        }
        kprintf("---------\n");
    }

    struct page_frame *first_page = MMU_pf_alloc();
    kprintf("This page should be @ 0....%p\n", first_page);


    kprintf("Freeing these pages (in order): %p, %p\n", &starts[0][0], &starts[0][1]);
    MMU_pf_free(&starts[0][0]);
    MMU_pf_free(&starts[0][1]);
    struct page_frame *p1 = MMU_pf_alloc();
    struct page_frame *p2 = MMU_pf_alloc();
    kprintf("These pages were alloc'ed again (in order): %p, %p\n", p1, p2);
    MMU_pf_free(p1);
    MMU_pf_free(p2);



    //free them all
    MMU_pf_free(first_page);
    for (int i=0; i<nb_sections; i++) {
        for (size_t j=0; j<nb_pages[i]; j++) {
            MMU_pf_free(starts[i] + j);
        }
    }
    
    kprintf("All pages free'd\n");
    kprintf("First three addresses of freelist: %p, %p, %p\n", freelist, freelist->next, freelist->next->next);
}

void stress_test_MMU(void) {
    //record the start of each section containing frames, the number of pages per frame, the number of sections
    struct page_frame *starts[MAX_NB_PAGING_REGIONS];
    size_t nb_pages[MAX_NB_PAGING_REGIONS];
    int nb_sections = 0;

    // walk through the memory regions to find this info
    struct memory_region *ptr = memory_info_page_start;
    while (ptr->start != (struct memory_region *)MEMORY_REGION_NULL) {
        size_t segment_size = (uint64_t)ptr->end - (uint64_t)ptr->next;
        if (segment_size / PAGE_FRAME_SIZE > 0) {
            starts[nb_sections] = ptr->next; // or ptr->start
            nb_pages[nb_sections] = segment_size / PAGE_FRAME_SIZE;
            nb_sections++;
        }
        ptr++;
    }

    // allocate all pages (with some output)
    void *p;
    uint64_t *page_arr;
    for (int i=0; i<nb_sections; i++) {
        for (size_t j=0; j<nb_pages[i]; j++) {
            p = MMU_pf_alloc();
            page_arr = (uint64_t *)p;
            for (size_t k=0; k<PAGE_FRAME_SIZE/sizeof(uint64_t); k++) {
                page_arr[k] = (uint64_t)p;
            }
        }
    }

    struct page_frame *first_page = MMU_pf_alloc();
    kprintf("This page should be @ 0....%p\n", first_page);
    page_arr = (uint64_t *)first_page;
    for (size_t k=0; k<PAGE_FRAME_SIZE/sizeof(uint64_t); k++) {
        page_arr[k] = (uint64_t)first_page;
    }

    //check bit patterns
    for (size_t k=0; k<PAGE_FRAME_SIZE/sizeof(uint64_t); k++) {
        if (page_arr[k] != (uint64_t)first_page) {
            kprintf("Uh oh: page_arr[%ld]==%lx but page_arr==%p\n", k, page_arr[k], page_arr);
        }
    }

    for (int i=0; i<nb_sections; i++) {
        for (size_t j=0; j<nb_pages[i]; j++) {
            page_arr = (uint64_t *)(starts[i] + j); //jth page of the ith segment (large enough to contain pages)
            for (size_t k=0; k<PAGE_FRAME_SIZE/sizeof(uint64_t); k++) {
                if (page_arr[k] != (uint64_t)page_arr) {
                    kprintf("Uh oh: page_arr[%ld]==%lx but page_arr==%p\n", k, page_arr[k], page_arr);
                }
            }
        }
    }

    // free ALL pages;
    for (int i=0; i<nb_sections; i++) {
        for (size_t j=0; j<nb_pages[i]; j++) {
            p = starts[i] + j;
            MMU_pf_free(p);
        }
    }
    MMU_pf_free(first_page);
    // kprintf("First three addresses of freelist: %p, %p, %p\n", freelist, freelist->next, freelist->next->next);
    kprintf("Stress test complete!\n");
    // struct page_frame *low_start = (struct page_frame *)0x1000;
    // struct page_frame *high_start = high_memory_start();
    // size_t nb_low = (0x9fc00 - (uint64_t)low_start) / PAGE_FRAME_SIZE;
    // size_t nb_high = (0x7fe0000 - (uint64_t)high_start) / PAGE_FRAME_SIZE;

    // // kprintf("High memory start: %p\n", high_memory_start());

    // kprintf("Nb low memory pages: %ld\n", nb_low);
    // kprintf("Nb high memory pages: %ld\n", nb_high);

    // void *p;
    // for (size_t i=0; i<nb_low; i++) {
    //     p = MMU_pf_alloc();
    //     uint64_t *page_arr = (uint64_t *)p;
    //     for (size_t j = 0; j < PAGE_FRAME_SIZE / sizeof(uint64_t); j++) {
    //         page_arr[j] = (uint64_t)p;
    //     }
    // }
    // for (size_t i=0; i<nb_high; i++) {
    //     p = MMU_pf_alloc();
    //     uint64_t *page_arr = (uint64_t *)p;
    //     for (size_t j = 0; j < PAGE_FRAME_SIZE / sizeof(uint64_t); j++) {
    //         page_arr[j] = (uint64_t)p;
    //     }
    // }
    // struct page_frame *first_page = MMU_pf_alloc();
    // uint64_t *page_arr = (uint64_t *)first_page;
    // for (size_t i = 0; i < PAGE_FRAME_SIZE / sizeof(uint64_t); i++) {
    //     page_arr[i] = (uint64_t)first_page;
    // }

    // // verify bit pattern for first page
    // for (size_t i = 0; i < PAGE_FRAME_SIZE / sizeof(uint64_t); i++) {
    //     if (page_arr[i] != (uint64_t)first_page) {
    //         kprintf("Uh oh: page_arr[%ld]==%lx but page_arr==%p\n", i, page_arr[i], page_arr);
    //     }
    // }
    // // verify bit pattern for low pages
    // for (size_t i=0; i<nb_low; i++) {
    //     uint64_t *page_arr = (uint64_t *)&low_start[i];
    //     for (size_t j = 0; j < PAGE_FRAME_SIZE / sizeof(uint64_t); j++) {
    //         if (page_arr[j] != (uint64_t)page_arr) {
    //             kprintf("Uh oh: page_arr[%ld]==%lx but page_arr==%p\n", j, page_arr[j], page_arr);
    //         }
    //     }
    // }
    // // verify bit pattern for high pages
    // for (size_t i=0; i<nb_high; i++) {
    //     uint64_t *page_arr = (uint64_t *)&high_start[i];
    //     for (size_t j = 0; j < PAGE_FRAME_SIZE / sizeof(uint64_t); j++) {
    //         if (page_arr[j] != (uint64_t)page_arr) {
    //             kprintf("Uh oh: page_arr[%ld]==%lx but page_arr==%p\n", j, page_arr[j], page_arr);
    //         }
    //     }
    // }
    // // free first
    // MMU_pf_free(first_page);
    // // free low
    // for (size_t i=0; i<nb_low; i++) {
    //     MMU_pf_free(&low_start[i]);
    // }
    // // free high
    // for (size_t i=0; i<nb_high; i++) {
    //     MMU_pf_free(&high_start[i]);
    // }
    // kprintf("Stress test complete!\n");
}