#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "block_device.h"
#include <stdint.h>

typedef struct SuperBlock *(*FS_detect_cb)(struct BlockDev *dev);



void MBR_parse(struct BlockDev *dev);


struct Partition {
    uint8_t status;
    uint8_t first_sector_head;
    uint8_t first_sector_high_bits;
    uint8_t first_sector_low_bits;
    uint8_t type;
    uint8_t last_sector_head;
    uint8_t last_sector_high_bits;
    uint8_t last_sector_low_bits;
    uint32_t first_sector_lba;
    uint32_t nb_sectors;
}__attribute__((packed));

struct MBR_data {
    uint8_t signature[2];
    struct Partition parts[4];
};



struct FSImpl {
    FS_detect_cb probe;
    struct FSImpl *next;
};



struct FS_list {
    struct FSImpl *head;
    struct FSImpl *last;
};



extern struct FS_list *filesystems;

void FS_init(void);
extern void FS_register(FS_detect_cb probe);
// struct SuperBlock *FS_probe(struct BlockDev *dev);
int get_MBR_data(struct MBR_data *buff, struct BlockDev *dev);




#endif