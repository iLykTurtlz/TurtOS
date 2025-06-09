#include "block_device.h"
#include "string.h"
#include "kmalloc.h"



struct ATABlockDev {
    struct BlockDev dev;
    uint16_t ata_base, ata_master;
    uint8_t slave, irq;
};

int read_block(struct BlockDev *this, uint64_t blk_num, void *dst) {
    return 0;
}

struct BlockDev *BlockDev_init(struct BlockDev *this, const char *name) {
    // struct BlockDev *res = kmalloc(sizeof(struct BlockDev));
    memset(this, 0, sizeof(struct BlockDev));
    this->read_block = &read_block;
    this->name = name;
    return this;
}

struct ATABlockDev *ata_probe(uint16_t base, uint16_t master, uint8_t slave, const char *name, uint8_t irq) {
    struct ATABlockDev *res = kmalloc(sizeof(struct ATABlockDev));
    res->ata_base = base;
    res->ata_master = master;
    res->slave = slave;
    res->irq = irq;
    BlockDev_init(&res->dev, name);
    // more work to do....read something?
    return res;
}