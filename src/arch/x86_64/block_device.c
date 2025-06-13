#include "block_device.h"
#include "string.h"
#include "kmalloc.h"
#include "phys_memory.h"



struct BlockQueue *BLK_devices_list = KERNEL_NULL;

void BLK_init(void) {
    BLK_devices_list = kmalloc(sizeof(*BLK_devices_list));
    memset(BLK_devices_list, 0, sizeof(*BLK_devices_list));
    BLK_devices_list->head = KERNEL_NULL;
    BLK_devices_list->last = KERNEL_NULL;
    BLK_devices_list->len = 0;
}

int BLK_register(struct BlockDev *dev) {
    if (BLK_devices_list->len > 0) {
        BLK_devices_list->last->next = dev;
    } else {
        BLK_devices_list->head = dev;
    }
    BLK_devices_list->last = dev;
    BLK_devices_list->len++;
    return 1;
}





// struct BlockDev *BlockDev_init(struct BlockDev *this, const char *name) {
//     // struct BlockDev *res = kmalloc(sizeof(struct BlockDev));
//     memset(this, 0, sizeof(struct BlockDev));
//     this->read_block = &read_block;
//     this->name = name;
//     return this;
// }

// struct ATABlockDev *ata_probe(uint16_t base, uint16_t master, uint8_t slave, const char *name, uint8_t irq) {
//     struct ATABlockDev *res = kmalloc(sizeof(struct ATABlockDev));
//     res->ata_base = base;
//     res->ata_master = master;
//     res->slave = slave;
//     res->irq = irq;
//     BlockDev_init(&res->dev, name);
//     // more work to do....read something?
//     return res;
// }