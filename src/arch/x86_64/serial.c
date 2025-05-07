#include "serial.h"
#include <stdint.h>

#include "irq.h"
// #include "keyboard.h"



#define PORT 0x3f8          // COM1
#define IER (PORT+1)    //int enable reg
#define IIR (PORT+2) //int id reg
#define LCR (PORT+3) //line control reg
#define MCR (PORT+4)
#define LSR (PORT+5) //line status register

//IIR values
#define THRE 0x001 //transmitting holding register empty
#define LINE_STATUS_CHANGE 0x011
#define NO_INTERRUPT_PENDING 0x01

//LSR values
#define THR_EMPTY 0x20 //bit 5


//LCR options
#define ENABLE_DLAB 0x80    // for LCR
#define DISABLE_DLAB 0x00
#define DLL PORT            //only if DLAB enabled
#define DLM IER             //only if DLAB enabled

#define DATA_BITS(N) (N-5)
#define ONE_STOP_BIT 0
#define MULT_STOP_BITS 4
#define NO_PARITY 0
#define ODD_PARITY 8
#define EVEN_PARITY 20
#define HIGH_PARITY 40
#define LOW_PARITY 56
#define BREAK_SIGNAL_DISABLED 0
#define BREAK_SIGNAL_ENABLED 64

#define INTERRUPT_WHEN_EMPTY 2



#define SERIAL_BUFFER_SIZE 64
MAKE_CIRCULAR_QUEUE(char, SERIAL_BUFFER_SIZE)
// struct char_circular_queue;
// void char_circular_queue_init(struct char_circular_queue *q, void (*consume_next)(char));
// static void produce(struct char_circular_queue *q, char c);
// static void consume(struct char_circular_queue *q);


struct serial_state {
    struct char_circular_queue serial_buffer;
    int busy;
} state;

static inline void outb(uint16_t port, uint8_t val) {
    asm("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void consume_next(char c) {
    outb(PORT, c);
    state.busy = 1;
}


void handle_serial(uint8_t irq, uint32_t error, void *arg) {
    struct serial_state *s = (struct serial_state *)arg;
    uint8_t iir_value = inb(IIR);
    if (iir_value & NO_INTERRUPT_PENDING) {
        return;
    }
    uint8_t event = 0x07 & (iir_value >> 1);
    if (event == LINE_STATUS_CHANGE) { //LINE interrupt
        //clear LSR by reading it
        inb(LSR);
    }
    else if (event == THRE) { //TX interrupt
        //write to THR
        if (!consume(&s->serial_buffer)) //no setting s->busy again????
            s->busy = 0;
    }
    //acknowledge the interrupts with the PIC
    // IRQ_end_of_interrupt(irq);
}



int serial_write(const char *buff, int len) {
    size_t i=0;
    CLI;
    while (i < len && !state.serial_buffer.is_full) {
        
        if (!state.serial_buffer.is_full) {
            produce(&state.serial_buffer, buff[i++]);
        }
    }
    
    if (i > 0 && state.busy == 0) {
        consume(&state.serial_buffer);
        state.busy = 1;
    }
    STI;
    return i+1;
}

// irq_callback handle_serial;


// static inline void outb(uint16_t port, uint8_t val) {
//     asm("outb %0, %1" : : "a"(val), "Nd"(port));
// }

// static inline uint8_t inb(uint16_t port) {
//     uint8_t ret;
//     asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
//     return ret;
// }


void serial_init() {
    //state init
    state.busy = 0;
    char_circular_queue_init(&state.serial_buffer, consume_next);

    //hardware init
    outb(IER, 0x00);    // mask all interrupts
    outb(LCR, ENABLE_DLAB);
    outb(DLL, 0x03);    // lower byte 3 -> 38400 baud
    outb(DLM, 0x00);    // upper byte not set
    outb(LCR, DISABLE_DLAB);
    outb(LCR, 0x03); //8 bits no parity, one stop bit
    inb(IIR);   //read IIR to clear interrupts
    outb(IER, INTERRUPT_WHEN_EMPTY);

    //register interrupt handler
    IRQ_set_handler(0x24, handle_serial, &state);
    IRQ_clear_mask(4);
}

// IER, IIR
// PORT + 1 is interrupt enable register
// Read IIR to clear interrupt
// write to IER to enable interrupts - not writing the correct values
// 
// int init_serial() {
//    outb(PORT + 1, 0x00);    //  ok Disable all interrupts
//    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
//    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
//    outb(PORT + 1, 0x00);    //                  (hi byte)
//    outb(PORT + 3, 0x03);    //  ok 8 bits, no parity, one stop bit
//    outb(PORT + 2, 0xC7);    // XEnable FIFO, clear them, with 14-byte threshold
//    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
//    outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
//    outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

//    // Check if serial is faulty (i.e: not same byte as sent)
//    if(inb(PORT + 0) != 0xAE) {
//       return 1;
//    }

//    // If serial is not faulty set it in normal operation mode
//    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
//    outb(PORT + 4, 0x0F);
//    return 0;
// }

// int serial_received() {
//     return inb(PORT + 5) & 1;
//  }
 
// char serial_read() {
//     while (serial_received() == 0);
 
//     return inb(PORT);
//  }

// int is_transmit_empty() {
//     return inb(PORT + 5) & 0x20;
// }
 
// void write_serial(char a) {
//     while (is_transmit_empty() == 0);
//     outb(PORT,a);
// }

// void serial_write(char a) {
//     //while (is_transmit_empty() == 0);
//     outb(PORT,a);
// }










 