#ifndef PARSE_MULTIBOOT_H
#define PARSE_MULTIBOOT_H

#include <stdint.h>

#define PAGE_FRAME_SIZE 4096
#define MEMORY_REGION_NULL ((void *)UINTPTR_MAX)

struct fixed_header {
	uint32_t tag_size;
	uint32_t reserved;
}__attribute__((packed));

struct memory_region {
    void *start;
    void *end;
    void *next;
};

extern struct memory_region *memory_info_page_start;

void parse_multiboot(struct fixed_header *head);

#endif