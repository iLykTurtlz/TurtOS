#ifndef IRQ_H
#define IRQ_H
#include <stdint.h>
#include "idt.h"
#include "kprint.h"
#include <stddef.h>

#define CLI __asm__ volatile("cli");
#define STI __asm__ volatile("sti");



void IRQ_init(void); //remap interrupts
void IRQ_end_of_interrupt(uint8_t irq);
// void PIC_remap(int offset1, int offset2);
void IRQ_set_mask(uint8_t IRQline);
void IRQ_clear_mask(uint8_t IRQline);
// void IRQ_get_mask(uint8_t irq); //void really?

typedef void (*irq_callback)(uint8_t, uint32_t, void*);



void IRQ_set_handler(uint8_t irq, irq_callback cb, void *arg);
void IRQ_call(uint8_t irq, uint32_t error);




#endif