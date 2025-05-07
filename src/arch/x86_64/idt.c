#include "idt.h"
#include <stdint.h>

#define GDT_OFFSET_KERNEL_CODE 8 // ???????, 0x08????

#define DOUBLE_FAULT 8
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT 14

#define DF_STACK 1
#define GPF_STACK 2
#define PF_STACK 3


struct idt_entry {
	uint16_t isr_low;      // The lower 16 bits of the ISR's address
	uint16_t target_selector;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t ist : 3;          // The IST in the TSS (task state segment) that the CPU will load into RSP; set to zero for now
    uint8_t reserved1 : 5;
	uint8_t type : 4;   // Type and attributes; 0xf is trap, 0xe is interrupt
	uint8_t zero : 1;
    uint8_t dpl : 2;
    uint8_t presence : 1;
    uint16_t isr_mid;      // The higher 16 bits of the lower 32 bits of the ISR's address
	uint32_t isr_high;     // The higher 32 bits of the ISR's address
	uint32_t reserved2;     // Set to zero
}__attribute__((packed));

// another data structure for today - need for stacks
// requires additional GDT descriptor = TSS


__attribute__((aligned(0x10))) 
static struct idt_entry idt[IDT_MAX_DESCRIPTORS];


void exception_handler() {
    // __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;






void idt_set_descriptor(uint8_t vector, void *isr, uint8_t type)
{
    struct idt_entry *descriptor = &idt[vector];
    descriptor->isr_low = (uint64_t)isr & 0xffff;
    descriptor->target_selector = GDT_OFFSET_KERNEL_CODE;

    if (vector == PAGE_FAULT)
        descriptor->ist = PF_STACK;
    else if (vector == DOUBLE_FAULT)
        descriptor->ist = DF_STACK;
    else if (vector == GENERAL_PROTECTION_FAULT)
        descriptor->ist = GPF_STACK;
    else
        descriptor->ist = 0;

    descriptor->reserved1 = 0; //??
    descriptor->type = type; // 0xe interrupt, 0xf trap
    descriptor->zero = 0;
    descriptor->dpl = 0; //'assuming dpl is zero'- OS Wiki IDT page
    descriptor->presence = 1;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xffff;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xffffffff; //WHY??? &0xffffffff???
    descriptor->reserved2 = 0;
}

static int vectors[IDT_MAX_DESCRIPTORS];

extern void *isr_stub_table[];


void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

    // for (uint8_t vector = 0; vector < 32; vector++) {
    //     idt_set_descriptor(vector, isr_stub_table[vector], 0x8e);
    //     vectors[vector] = 1; //true
    // }

    //test
    for (uint16_t vector=0; vector < IDT_MAX_DESCRIPTORS; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0xe); //8f is trap, 8e is interrupt
        vectors[vector] = 1;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    // __asm__ volatile ("int $0x20");
    // __asm__ volatile ("sti"); // set the interrupt flag
}



// int number assembly number

// static inline void lidt(void *base, uint16_t size)
// {
//     struct {
//         uint16_t length;
//         void *base;
//     } __attribute__((packed)) IDTR = {size, base};

//     asm("lidt %0" : : "m"(IDTR));
// }