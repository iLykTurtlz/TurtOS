#include "tss.h"
#include <stdint.h>
#include <stddef.h>

#define TSS_OFFSET_KERNEL_CODE 16


typedef struct {
    uint16_t segment_limit0;
    uint32_t base_address0 : 24;
    uint32_t type : 4;
    uint32_t zero1 : 1;
    uint32_t dpl : 2;
    uint32_t presence : 1;
    uint16_t segment_limit25 : 4;
    uint16_t avl : 1;
    uint16_t nothing : 2;
    uint16_t g : 1;
    uint16_t base_address25 : 8;
    uint32_t base_address32;
    uint8_t reserved1;
    uint8_t zero2 : 5;
    uint8_t reserved2 : 3;
    uint16_t reserved3;
} __attribute__((packed)) tss_descriptor_t;

static struct {
    uint32_t reserved1;
    uint64_t rsp[3];
    uint64_t reserved2;
    uint64_t ist[7];
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t io_map_base_address;
} __attribute__((packed)) tss;

extern uint64_t gdt64[]; 

tss_descriptor_t *tss_descriptor = (tss_descriptor_t *)(gdt64 + 2); //

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];
static char stack3[STACK_SIZE];
static char stack4[STACK_SIZE];

void tss_init() {
    // fill out tss descriptor
    size_t segment_limit = sizeof(tss) - 1;

    tss_descriptor->segment_limit0 = segment_limit & 0xffff;
    tss_descriptor->segment_limit25 = (segment_limit >> 16) & 0xf;

    tss_descriptor->base_address0 = ((uint64_t)&tss) & 0xffffff;
    tss_descriptor->base_address25 = (((uint64_t)&tss) >> 24) & 0xff;
    tss_descriptor->base_address32 = ((uint64_t)&tss) >> 32;

    tss_descriptor->presence = 1;
    tss_descriptor->dpl = 0; //0 = kernel mode, 3 = user level

    //WHERE is this in the docs?!
    tss_descriptor->type = 9;

    //???avl, g, type???
    tss_descriptor->avl = 0;
    tss_descriptor->g = 0;
    tss_descriptor->nothing = 0;
    tss_descriptor->reserved1 = 0;
    tss_descriptor->reserved2 = 0;
    tss_descriptor->reserved3 = 0;
    
    tss_descriptor->zero1 = 0;
    tss_descriptor->zero2 = 0;
    
    // fill out tss
    tss.ist[0] = (uint64_t)&stack1[STACK_SIZE];
    tss.ist[1] = (uint64_t)&stack2[STACK_SIZE];
    tss.ist[2] = (uint64_t)&stack3[STACK_SIZE];
    tss.ist[3] = (uint64_t)&stack4[STACK_SIZE];


    // ???
    tss.rsp[0] = (uint64_t)&stack1[STACK_SIZE]; // ??? garbage - only need rsp for switch into ring 0 from nonzero
    tss.reserved1 = 0;
    tss.reserved2 = 0;
    tss.reserved3 = 0;
    tss.reserved4 = 0;
    tss.io_map_base_address = sizeof(tss); // ???


    uint16_t tss_selector = TSS_OFFSET_KERNEL_CODE;
    // ltr instruction
    __asm__ volatile ("ltr %0" : : "m"(tss_selector)); //offset in bytes
}