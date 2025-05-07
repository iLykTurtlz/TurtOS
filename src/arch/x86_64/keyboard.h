#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>



void keyboard_init();
void poll();
void handle_keyboard(uint8_t irq, uint32_t err, void *arg);
// void handle_serial(uint8_t irq, uint32_t err, void *arg);
// void serial_keyboard_init();
#endif