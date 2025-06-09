#ifndef ATA_H
#define ATA_H

#include "block_device.h"

struct ATABlockDev {
    struct BlockDev dev;
    uint16_t io_base, ctrl_base;
    uint8_t slave, irq;
};

typedef enum {PATAPI=0, SATAPI=1, PATA=2, SATA=3, UNKNOWN=4} device_type;

// void identify_primary();
void init_ata();

#endif