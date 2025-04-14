#include "idt.h"
#include <stdint.h>

static inline void lidt(void *base, uint16_t size)
{
    struct {
        uint16_t length;
        void *base;
    } __attribute__((packed)) IDTR = {size, base};

    asm("lidt %0" : : "m"(IDTR));
}