#ifndef BLOCK_DEVICE_H
#define BLOCK_DEVICE_H

#include <stdint.h>
#include <stddef.h>


enum BlockDevType { MASS_STORAGE, PARTITION };



struct BlockDev {
    uint64_t tot_length;
    int (*read_block)(struct BlockDev *this, uint64_t blk_num, void *dst);
    uint32_t blk_size;
    enum BlockDevType type;
    const char *name;   //separate malloc?
    uint8_t fs_type;
    struct BlockDev *next;
};

struct BlockQueue {
    struct BlockDev *head;
    struct BlockDev *last;
    size_t len;
};

extern struct BlockQueue *BLK_devices_list;

void BLK_init(void);
int BLK_register(struct BlockDev *dev);


#endif