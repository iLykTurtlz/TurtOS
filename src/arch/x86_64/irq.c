#include "irq.h"
#include "idt.h"
#include "phys_memory.h"
#include <stdint.h>



#define IRQ_0   0x20
#define IRQ_8   0x28 // Not the same as PIC2, right??

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define PIC_EOI		0x20		/* End-of-interrupt command code */

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

//ICW := Initialization Command Word
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */



static inline void outb(uint16_t port, uint8_t val) {
    asm("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void IRQ_end_of_interrupt(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
	
	outb(PIC1_COMMAND,PIC_EOI);
}

static inline void io_wait(void) {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

void PIC_remap(int offset1, int offset2)
{
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	// io_wait();

	// Mask both PICs.
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}



// a
void pic_disable(void) {
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}

void IRQ_init(void)
{
    pic_disable();
    PIC_remap(IRQ_0, IRQ_8);
    
}

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    //sanity check
    kprintf("BEFORE set PIC #%d: %x\n", IRQline, inb(port));
    value = inb(port) | (1 << IRQline);
    outb(port, value);     
    kprintf("AFTER set PIC #%d: %x\n", IRQline, inb(port));   
}



void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    // kprintf("BEFORE clear PIC #%d: %x\n", IRQline, inb(port));
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);   
    // kprintf("AFTER clear PIC #%d: %x\n", IRQline, inb(port));     
}


// Helper func 
static uint16_t __pic_get_irq_reg(int ocw3)
{
    //OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
    //represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain 
    outb(PIC1_COMMAND, ocw3);
    outb(PIC2_COMMAND, ocw3);
    return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}

//Returns the combined value of the cascaded PICs irq request register 
uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

// Returns the combined value of the cascaded PICs in-service register 
uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}


struct irq_entry {
    irq_callback cb;
    void *arg;
} irq_h[256];

//irq_h[irq].cb(irq, err, irq_h[irq.arg])

void IRQ_set_handler(uint8_t irq, irq_callback cb, void *arg)
{
    irq_h[irq].cb = cb;
    irq_h[irq].arg = arg;
}

void IRQ_call(uint8_t irq, uint32_t error, void *arg)
{
    if (irq > IDT_MAX_DESCRIPTORS - 1) 
        return;
    if (irq_h[irq].cb != NULL) {
        if (arg != 0) { //if a memory address has been passed in from the assembly stub
            irq_h[irq].cb(irq, error, arg);
        } else {
            irq_h[irq].cb(irq, error, irq_h[irq].arg);
        }
    } else {
        kprintf("Unhandled IRQ: %d\n", irq);
    }
    if (0x20 <= irq && irq <= 0x2e) {
        IRQ_end_of_interrupt(irq - 0x20);
    }
}

/*
 	CF 	Carry Flag
1 	1 	Reserved
2 	PF 	Parity Flag
3 	0 	Reserved
4 	AF 	Auxiliary Carry Flag
5 	0 	Reserved
6 	ZF 	Zero Flag
7 	SF 	Sign Flag
8 	TF 	Trap Flag
9 	IF 	Interrupt Enable Flag
10 	DF 	Direction Flag
11 	OF 	Overflow Flag
12-13 	IOPL 	I/O Privilege Level
14 	NT 	Nested Task
15 	0 	Reserved
16 	RF 	Resume Flag
17 	VM 	Virtual-8086 Mode
18 	AC 	Alignment Check / Access Control
19 	VIF 	Virtual Interrupt Flag
20 	VIP 	Virtual Interrupt Pending
21 	ID 	ID Flag 
*/

static inline uint64_t get_flags()
{
    uint64_t flags;
    asm __volatile__("pushfq; pop %0" : "=r"(flags));
    return flags;
}

int interrupts_enabled()
{
    return (get_flags() & 0x200) > 0;
}






