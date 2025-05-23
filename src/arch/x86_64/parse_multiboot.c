#include "parse_multiboot.h"
#include <stddef.h>
#include "kprint.h"





struct generic_tag {
    uint32_t type;
    uint32_t tag_size;
    uint64_t data;
}__attribute__((packed));

struct basic_mem_info {
    uint32_t type;
    uint32_t tag_size;
    uint32_t low_memory_size;
    uint32_t high_memory_size;
}__attribute__((packed));

struct bios_info {
    uint32_t type;
    uint32_t tag_size;
    uint32_t bios_boot_device;
    uint32_t bios_boot_partition;
    uint32_t bios_boot_subpartition;
}__attribute__((packed));

struct boot_command_line {
    uint32_t type;
    uint32_t tag_size;
    char first_char;
}__attribute__((packed));

struct bootloader_name {
    uint32_t type;
    uint32_t tag_size;
    char bootloader_name;
}__attribute__((packed));

struct memory_info_entry {
    uint64_t start_address;
    uint64_t len;
    uint32_t type;
    uint32_t reserved;
}__attribute__((packed));

struct memory_map {
    uint32_t type;
    uint32_t tag_size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct memory_info_entry first_entry;
}__attribute__((packed));

struct elf_section_header {
    uint32_t section_name; //index into the string table
    uint32_t section_type;
    uint64_t flags;
    uint64_t segment_address; //in memory
    uint64_t segment_offset; //on disk
    uint64_t segment_size; //in bytes
    uint32_t table_index_link;
    uint32_t extra_info;
    uint64_t address_alignment; //powers of 2
    uint64_t fixed_entry_size; //IFF section holds fixed entries
}__attribute__((packed));

struct elf_symbols {
    uint32_t type;
    uint32_t tag_size;
    uint32_t nb_entries;
    uint32_t entry_size;
    uint32_t string_table_idx;
    struct elf_section_header first_entry;
}__attribute__((packed));

struct memory_region *memory_info_page_start;

int min(const int a, const int b) {
    return a < b ? a : b;
}

// assign first free memory region large enough for a page to memory_info_page_start
void find_first_free_page(struct memory_info_entry *meminfo_arr, size_t meminfo_arr_len, struct elf_section_header *elfheader_arr, size_t elfheader_arr_len)
{
    // intervals a are available, intervals b are taken
    size_t meminfo_idx=0, elf_idx=0;
    uint64_t a_start, a_end, b_start, b_end;

    // might need an extra sometimes
    // uint64_t a_start2, a_end2;

    while (meminfo_idx < meminfo_arr_len) {
        //is it a valid type?
        if (meminfo_arr[meminfo_idx].type != 1) {
            meminfo_idx++;
            continue;
        }

        a_start = meminfo_arr[meminfo_idx].start_address;
        a_end = meminfo_arr[meminfo_idx].start_address + meminfo_arr[meminfo_idx].len;

        while (elfheader_arr[elf_idx].segment_address < a_end && elf_idx < elfheader_arr_len) {
            b_start = elfheader_arr[elf_idx].segment_address;
            b_end = elfheader_arr[elf_idx].segment_address + elfheader_arr[elf_idx].segment_size;

            //empty constraints can be ignored
            if (b_start == b_end) {
                elf_idx++;
                continue;
            }

            //if the intersection isn't empty
            if (b_end > a_start) {
                // first segment should be clear
                if (b_start - a_start >= PAGE_FRAME_SIZE) {
                    memory_info_page_start = (struct memory_region *)a_start;
                    return;
                }

                

                // what remains of the free memory segment
                a_start = min(a_end, b_end);

                // nothing left
                if (a_start == a_end) {
                    break;
                }
            }
            elf_idx++;
        }

        // if there's enough left after checking constraints
        if (a_end - a_start >= PAGE_FRAME_SIZE) {
            memory_info_page_start = (struct memory_region *)a_start;
            return;
        }
        meminfo_idx++;
    }


}

// UNUSED for now
inline int has_nonempty_intersection(uint64_t a_start, uint64_t a_end, uint64_t b_start, uint64_t b_end)
{
    return (a_start != a_end && b_start != b_end) &&
           ((a_end > b_start && b_end > a_start) || (b_end > a_start && a_end > b_start));
}




void write_free_info_page(struct memory_info_entry *meminfo_arr, size_t meminfo_arr_len, struct elf_section_header *elfheader_arr, size_t elfheader_arr_len) {
    struct memory_region *output = memory_info_page_start;

    // the output_page will be considered allocated until all other regions have been allocated
    // then it will be free'd
    uint64_t output_page_start = (uint64_t)memory_info_page_start;
    uint64_t output_page_end = (uint64_t)memory_info_page_start + PAGE_FRAME_SIZE;
    
    
    size_t meminfo_idx=0, elf_idx=0;

    // intervals a are available, intervals b are taken
    uint64_t a_start, a_end, b_start, b_end;

    // might need an extra sometimes
    uint64_t a_start2, a_end2;

    while (meminfo_idx < meminfo_arr_len) {
        //is it a valid type?
        if (meminfo_arr[meminfo_idx].type != 1) {
            meminfo_idx++;
            continue;
        }

        a_start = meminfo_arr[meminfo_idx].start_address;
        a_end = meminfo_arr[meminfo_idx].start_address + meminfo_arr[meminfo_idx].len;

        while (elfheader_arr[elf_idx].segment_address < a_end && elf_idx < elfheader_arr_len) {
            b_start = elfheader_arr[elf_idx].segment_address;
            b_end = elfheader_arr[elf_idx].segment_address + elfheader_arr[elf_idx].segment_size;

            //empty constraints can be ignored
            if (b_start == b_end) {
                elf_idx++;
                continue;
            }

            //if the intersection isn't empty
            if (b_end > a_start) {
                // first segment should be clear...need to check that it doesn't include the allocated page
                if (b_start > a_start) {
                    a_start2 = a_start;
                    a_end2 = b_start;

                    if (output_page_start < a_end2 && output_page_end > a_start2) {
                        if (output_page_start > a_start2) {
                            output->start = output->next = (void *)a_start2;
                            output->end = (void *)output_page_start;
                            output++;
                        }
                        if (output_page_end < a_end2) {
                            output->start = output->next = (void *)output_page_end;
                            output->end = (void *)a_end2;
                            output++;
                        }
                    } else {
                        output->start = output->next = (void *)a_start2;
                        output->end = (void *)a_end2;
                        output++;
                    }

                }

                // what remains of the free memory segment
                a_start = min(a_end, b_end);

                // nothing left
                if (a_start == a_end) {
                    break;
                }

            }
            elf_idx++;
        }

        //register whatever is left from the previous memory segment
        if (a_start < a_end) {
            // need to avoid the allocated page
            if (output_page_start < a_end && output_page_end > a_start) {
                // anything before
                if (output_page_start > a_start) {
                    output->start = output->next = (void *)a_start;
                    output->end = (void *)output_page_start;
                    output++;
                }
                // anything after
                if (output_page_end < a_end) {
                    output->start = output->next = (void *)output_page_end;
                    output->end = (void *)a_end;
                    output++;
                }
            } else {
                // no intersection
                output->start = output->next = (void *)a_start;
                output->end = (void *)a_end;
                output++;
            }
        }
        meminfo_idx++;
    }

    //write 'null' terminator to the array
    output->start = (void *)MEMORY_REGION_NULL; // = 0xffffffffffffffff;
}

// for handling the various sizes of multiboot2 tags: rounds up to the next 8-byte-aligned address
void *next_tag(void *tag) {
    struct generic_tag *super = (struct generic_tag *)tag;
    size_t size = super->tag_size;
    size_t padded_size;
    if (size % 8 != 0) {
        padded_size = size - (size % 8) + 8;
    } else {
        padded_size = size;
    }
    void *next = (((uint8_t *)tag) + padded_size);
    return next;
}

// for DEBUG only
void verify_memory_regions() {
    struct memory_region *ptr = memory_info_page_start;
    kprintf("Free memory regions:\n");
    while (ptr->start != (void *)((uint64_t)-1)) {
        if (ptr->next != ptr->start) {
            kprintf("ASSERTION FAILED: MEMORY_REGIONS\n");
        }
        kprintf("[%lx, %lx] ", (uint64_t)ptr->start, (uint64_t)ptr->end);
        ptr++;
    }
    kprintf("\n");
}

// for DEBUG only
void parse_memory_info_entries(struct memory_info_entry *arr, size_t len) {
    kprintf("Parsing memory info array...\n");
    kprintf("LEN %ld\n", len);
    for (size_t i=0; i<len; i++) {
        if (arr[i].type == 1) {
            kprintf("\t%ld bytes of memory @%p\n", arr[i].len, (void *)(arr[i].start_address));
        }
    }
}

// for DEBUG only
void parse_elf_section_headers(struct elf_section_header *arr, size_t len, size_t string_table_idx) {
    char *string_table = (char *)arr[string_table_idx].segment_address;
    for (size_t i=0; i<len; i++) {
        kprintf("ELF: %s\n", (char *)(string_table + arr[i].section_name));
        kprintf("\taddress %p size %ld alignment %ld\n", (void *)(arr[i].segment_address), arr[i].segment_size, arr[i].address_alignment);
    }
}

void parse_multiboot(struct fixed_header *head) {
    struct memory_info_entry *memory_info_array;
    struct elf_section_header *elf_header_array;
    size_t meminfo_array_len, elf_array_len;

    void *tag = head + 1;
    int type=-1;
    struct generic_tag *super = tag;
    type = super->type;
    while (type != 0) {
        switch (type) {
            case 1:
            {
                // struct boot_command_line *p = tag;
                // kprintf("Boot command line string: %s\n",&p->first_char);
                break;
            }
            case 2:
            {
                // struct bootloader_name *p = tag;
                // kprintf("Bootloader name: %s\n", &p->bootloader_name);
                break;
            }
            case 4:
            {
                // struct basic_mem_info *p = tag;
                // kprintf("Basic memory info:\n");
                // kprintf("\tLow memory size %d\n\tHigh memory size %d\n", p->low_memory_size, p->high_memory_size);
                break;
            }
            case 5:
            {
                // struct bios_info *p = tag;
                // kprintf("BIOS Boot Device Info:\n");
                // kprintf("\tBIOS boot device: %d\n", p->bios_boot_device);
                // kprintf("\tBIOS boot partition: %d\n", p->bios_boot_partition);
                // kprintf("\tBIOS boot subpartition: %d\n", p->bios_boot_subpartition);
                break;
            }
            case 6:
            {
                // kprintf("#####Found the memory map tag!#####\n");
                struct memory_map *p = tag;
                // kprintf("\tSanity check: type should be 6... %d\n", p->type);
                // kprintf("\tSanity check: memory entry size should be 24... %d\n", p->entry_size);
                // kprintf("\tSanity check: memory info entry version should be 0... %d\n", p->entry_version);
                // kprintf("Total size of memory map tag: %d\n", p->tag_size);
                int meminfo_array_size = p->tag_size - 16;
                // kprintf("Meminfo array size should be divisible by 24: %d\n", meminfo_array_size);
                // if (meminfo_array_size % 24 == 0) {
                // kprintf("Success!\n");
                meminfo_array_len = meminfo_array_size / 24;
                memory_info_array = &p->first_entry;
                
                // parse_memory_info_entries(memory_info_array, meminfo_array_len);
                // }
                // kprintf("####################\n");
                break;
            }
            case 9:
            {
                struct elf_symbols *p = tag;
                // kprintf("#####Found the elf symbols!#####");
                // kprintf("\ttag size = %d, nb entries = %d, sizeof(entry) = %d, index of section with string table = %d",
                //     p->tag_size, p->nb_entries, p->entry_size, p->string_table_idx
                // );
                
                // size_t metadata_size = sizeof(struct elf_symbols) - sizeof(struct elf_section_header);
                // size_t array_size = (p->tag_size - metadata_size);
                // size_t array_len = array_size / p->nb_entries;
                // kprintf("\tmetadata_size = %ld, array_size = %ld, total_size = %d\n",
                // metadata_size, array_size, p->tag_size);
                // kprintf("\tarray_len = %ld, nb_entries=%d\n", array_len, p->nb_entries);
                // kprintf("######################\n");
                elf_header_array = &p->first_entry;
                elf_array_len = p->nb_entries;
                // parse_elf_section_headers(elf_header_array, p->nb_entries, p->string_table_idx);
                break;
            }
            default:
            {
                // struct generic_tag *p = tag;
                // kprintf("Unknown tag of type %d, size %d\n", p->type, p->tag_size);
            }
        }
        tag = next_tag(tag);
        super = tag;
        type = super->type;
    }
    find_first_free_page(memory_info_array, meminfo_array_len, elf_header_array, elf_array_len);
    write_free_info_page(memory_info_array, meminfo_array_len, elf_header_array, elf_array_len);
    // verify_memory_regions();
    // kprintf("MEMORY INFO PAGE @%p\n", memory_info_page_start);
}




