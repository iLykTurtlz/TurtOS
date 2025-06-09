#ifndef BLOCK_DEVICE_H
#define BLOCK_DEVICE_H

#include <stdint.h>


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


#endif