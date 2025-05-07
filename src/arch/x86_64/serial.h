#ifndef SERIAL_H
#define SERIAL_H

#include "circular_queue.h"




void serial_init();
int serial_write(const char *buff, int len);

#endif