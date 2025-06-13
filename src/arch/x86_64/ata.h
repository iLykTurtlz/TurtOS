#ifndef ATA_H
#define ATA_H

#include "block_device.h"
#include "context_switch.h"




struct ATARequest {
    struct ATARequest *next;
    uint64_t block;
    uint16_t data[256];
    struct PROC_list *blocked;
};


struct ATABlockDev {
    struct BlockDev dev;
    uint16_t io_base, ctrl_base; //ata_base becomes io_base, ata_master becomes ctrl_base
    uint8_t slave, irq;
    struct ATARequest *req_head, *req_tail;
};

typedef enum {PATAPI=0, SATAPI=1, PATA=2, SATA=3, UNKNOWN=4} device_type;

extern struct ATABlockDev *ATA_instance;


// void identify_primary();
void ATA_init();

void test_ATA_read(int blk_num);

#endif