#include "keyboard.h"
#include <stdint.h>
#include <stddef.h>
#include "vga.h"
#include "kprint.h"
#include "string.h"
#include "serial.h"
#include "irq.h"
// #include "irq.h"
#include "context_switch.h"
#include "circular_queue.h"

#define PS2_DATA 0x60
#define PS2_CMD 0x64
#define PS2_STATUS PS2_CMD
#define PS2_STATUS_OUTPUT 1
#define PS2_STATUS_INPUT (1 << 1)

/* commands */
#define DISABLE1 0xad //keyboard
#define DISABLE2 0xa7 //auxiliary interface = mouse
#define ENABLE1 0xae
#define ENABLE2 0xa8
#define READ_CONFIG 0x20 //controller configuration byte
#define WRITE_CONFIG 0x60
#define RESET 0xff
#define PS2_SELF_TEST 0xaa
#define RESET_FAILURE 0xfc
#define SET_SCAN_CODE 0xf0
#define SCAN_CODE(i) i
#define GET_SCAN_CODE 0x00
#define ENABLE_KEYBOARD 0xf4


/* messages from controller */
#define ACK 0xfa
#define RESEND 0xfe
#define RESET_SUCCESS 0xaa
#define PS2_SELF_TEST_SUCCESS 0x55
#define USING_SCAN_CODE1 0x43
#define USING_SCAN_CODE2 0x41
#define USING_SCAN_CODE3 0x3f


/* driver parameters */
// #define COMMAND_QUEUE_SIZE 64


MAKE_CIRCULAR_QUEUE(char, 64)

struct char_circular_queue stdin;
static char from_kbd = 0;


// "list.h" doubly linked circular list of struct Process *
// MAKE_LIST(PROC, struct Process *)
// struct PROC_list *keyboard_wait;



struct config {
    uint8_t interrupt_port1 : 1;
    uint8_t interrupt_port2 : 1;
    uint8_t sysflag : 1;
    uint8_t zero1 : 1; // else command
    uint8_t clock1 : 1;
    uint8_t clock2 : 1;
    uint8_t translation_port1 : 1;
    uint8_t zero2 : 1;
} __attribute__((packed));

static inline void outb(uint16_t port, uint8_t val) {
    asm("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void write(uint16_t port, uint8_t val) {
    while (PS2_STATUS_INPUT & inb(PS2_STATUS))
        ;
    outb(port, val);
}

uint8_t read(uint16_t port) {
    while(!(PS2_STATUS_OUTPUT & inb(PS2_STATUS)))
        ;
    return inb(port);
}

int handshake(uint16_t port, uint8_t command, int nb_tries) {
    write(port, command);
    uint8_t response = read(PS2_DATA);
    for (int i=0; i<nb_tries && response != ACK; i++) {
        kprintf("Response: %hx\n", (uint16_t)response);
        kprintf("Resending...\n");
        write(port, command);
        response = read(PS2_DATA);
    }
    return response == ACK ? 1 : 0;
}

static void consume_stdin(char c) {
    from_kbd = c;
}


char KBD_read() {
    CLI;
    while (stdin.producer == stdin.consumer) {
        PROC_block_on(keyboard_wait, 1); //enable_ints = 1
        CLI;
    }
    consume(&stdin);
    STI;
    return from_kbd;
}


void KBD_io(void *arg)
{
    while (1)
        kprintf("%c", KBD_read());
}




void keyboard_init() {
    char_circular_queue_init(&stdin, consume_stdin);
    PROC_create_kthread(KBD_io, KERNEL_NULL);

    //TODO? determine whether the PS/2 controller exists???

    // disable devices on channel 1 and channel 2
    write(PS2_CMD, DISABLE1);
    write(PS2_CMD, DISABLE2);

    // // flush output buffer?
    // inb(0x60);

    // read the PS/2 config byte
    write(PS2_CMD, READ_CONFIG);
    uint8_t config_data = read(PS2_DATA);
    //kprintf("Config data: %hx\n", (unsigned short)config_data);

    // set config
    struct config c = *((struct config *)&config_data);
    c.interrupt_port1 = 0; //needs to be 0!!!
    // c.interrupt_port2 = 0;
    // c.sysflag = 1;
    // c.zero1 = 0;
    c.clock1 = 0; //0 means enable
    // c.clock2 = 0;
    c.translation_port1 = 0; //
    // c.zero2 = 0;
    config_data = *((uint8_t *)&c);

    //write config byte back out to the PS/2 controller
    write(PS2_CMD, WRITE_CONFIG);
    write(PS2_DATA, config_data);

    // controller self-test 
    write(PS2_CMD, PS2_SELF_TEST);
    uint8_t self_test_result = read(PS2_DATA);
    if (self_test_result != PS2_SELF_TEST_SUCCESS)
        VGA_display_str("Failed self test", ERROR);

    //Enable port 1 (port 2 remains disabled for now)
    write(PS2_CMD, ENABLE1);

    //reset keyboard
    write(PS2_DATA, RESET);
    uint8_t result1 = read(PS2_DATA);
    uint8_t result2 = read(PS2_DATA);
    if ((result1 == RESET_SUCCESS || result2 == RESET_SUCCESS) &&
         (result1 == ACK || result2 == ACK))
        kprintf("Reset keyboard\n");
    else if (result1 == RESET_FAILURE || result2 == RESET_FAILURE)
        kprintf("Failed to reset keyboard\n");
    else
        kprintf("Codes returned: %hx, %hx\n", (uint16_t)result1, (uint16_t)result2);

    // set scan code 1
    int code = 1;
    if (!handshake(PS2_DATA, SET_SCAN_CODE, 5)) {
        VGA_display_str("Error initiating scan code setup\n", ERROR);
    }
    if (!handshake(PS2_DATA, SCAN_CODE(code), 5)) {
        VGA_display_str("Error setting scan code\n", ERROR);
    } else {
        kprintf("Set scan code %d\n", code);
    }

    if (handshake(PS2_DATA, ENABLE_KEYBOARD, 2)) {
        kprintf("Scanning begins now.\n");
    } else {
        VGA_display_str("Error enabling keyboard\n", ERROR);
    }

    
    IRQ_set_handler(0x21, handle_keyboard, 0);
    IRQ_clear_mask(1);

    // // READ CONFIG BYTE
    // uint8_t read_val;
    // write(PS2_CMD, READ_CONFIG);
    // read_val = read(PS2_DATA);
    // kprintf("CONFIG BYTE BEFORE: %x\n", read_val);

    // NEW CONFIG
    c.interrupt_port1 = 1;
    config_data = *((uint8_t *)&c);
    // // config_data = 0xa8;
    // kprintf("CONFIG DATA TO BE WRITTEN: %x\n", config_data);

    // WRITE CONFIG BYTE
    write(PS2_CMD, WRITE_CONFIG);
    write(PS2_DATA, config_data);

    // READ CONFIG
    // write(PS2_CMD, READ_CONFIG);
    // read_val = read(PS2_DATA);
    // kprintf("CONFIG BYTE AFTER: %x\n", read_val);

}

// struct command_queue {
//     size_t first;
//     size_t last;
//     uint8_t q[COMMAND_QUEUE_SIZE];
// }; 


// enum Symbol {
//     ESC,

// };


// missing 0x54-0x56, 0x59-0x80, above 0xed
static const char basic_repr_table[] = {
    0,  0,  '1', '2', '3', '4', '5', '6', '7', '8',
    '9', '0', '-', '=', 0, '\t', 'q', 'w', 'e', 'r', 
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 
    'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, '7', '8', '9', '-', '4', '5', '6', '+', '1', 
    '2', '3', '0', '.', 0 , 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// TODO: fix \t: it doesn't work.
static const char shift_repr_table[] = {
    0,  0,  '!', '@', '#', '$', '%', '^', '&', '*',
    '(', ')', '_', '+', 0, '\t', 'Q', 'W', 'E', 'R', 
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 
    'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, '7', '8', '9', '-', '4', '5', '6', '+', '1', 
    '2', '3', '0', '.', 0 , 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};



void no_op(void) {}
void extend(void);



static void (*action_table[])(void) = {
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, VGA_backspace, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,

    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,

    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, extend, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
    no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op, no_op,
};

void extend(void) {
    action_table[read(PS2_DATA)]();
}


enum ReprState {
    BASIC, SHIFTED
};

#define LSHIFT 0x2a
#define RSHIFT 0x36
#define CAPSLOCK 0x3a
#define BACKSPACE 0x0e
#define UN_LSHIFT 0xaa
#define UN_RSHIFT 0xb6
#define UN_CAPSLOCK 0xba
#define UN_BACKSPACE 0x8e
#define EXTEND 0xe0
// TODO implement tab - should backspace undo tab?



// static const  jump_table[] = {
//     NULL,
//     ascii_table,


// };



void poll(void) {
    uint8_t input;
    enum ReprState state = BASIC;
    int capslock = 0;
    while (1) {
        input = read(PS2_DATA);
        if (input == LSHIFT || input == RSHIFT) {
            state = SHIFTED;
        }
        else if (input == UN_LSHIFT || input == UN_RSHIFT) {
            state = BASIC;
        }

        if (input == UN_CAPSLOCK) {
            capslock = !capslock;
        }

        //kprintf("\nextend = %p, action=%p, no_op=%p\n", extend, action_table[input], no_op);
        action_table[input]();
        // if (input == EXTEND)
        //     continue;

        char c;
        if (state == BASIC) {
            c = basic_repr_table[input];
        }
        else if (state == SHIFTED) {
            c = shift_repr_table[input];
        }
        if (capslock) {
            if (islower(c)) {
                c = toupper(c);
            } 
            else if (isupper(c)) {
                c = tolower(c);
            }
        }
        if (c != 0) {
            kprintf("%c",c);
        }
    }
}

struct keyboard_state {
    enum ReprState state;
    int capslock;
} ks;





void handle_keyboard(uint8_t irq, uint32_t err, void *arg)
{
    uint8_t input = read(PS2_DATA);
    if (input == LSHIFT || input == RSHIFT) {
        ks.state = SHIFTED;
    }
    else if (input == UN_LSHIFT || input == UN_RSHIFT) {
        ks.state = BASIC;
    }

    if (input == UN_CAPSLOCK) {
        ks.capslock = !ks.capslock;
    }

    //kprintf("\nextend = %p, action=%p, no_op=%p\n", extend, action_table[input], no_op);
    action_table[input]();
    // if (input == EXTEND)
    //     continue;

    char c;
    if (ks.state == BASIC) {
        c = basic_repr_table[input];
    }
    else if (ks.state == SHIFTED) {
        c = shift_repr_table[input];
    }
    if (ks.capslock) {
        if (islower(c)) {
            c = toupper(c);
        } 
        else if (isupper(c)) {
            c = tolower(c);
        }
    }
    if (c != 0) {
        // kprintf("%c",c);
        produce(&stdin, c);
    }
    PROC_unblock_all(keyboard_wait);
}













