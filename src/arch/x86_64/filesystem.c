#include "filesystem.h"
#include "kprint.h"
#include "phys_memory.h"
#include "kmalloc.h"
#include "string.h"

#define BLK_SIZE 512
#define BOOT_SIGNATURE_OFFSET 510
#define PART_1_OFFSET 446
#define PART_2_OFFSET 462
#define PART_3_OFFSET 478
#define PART_4_OFFSET 494

#define STATUS_OFFSET 0
#define FIRST_HEAD_OFFSET 1
#define FIRST_HIGH_BITS_OFFSET 2
#define FIRST_LOW_BITS_OFFSET 3
#define PART_TYPE_OFFSET 4
#define LAST_HEAD_OFFSET 5
#define LAST_HIGH_BITS_OFFSET 6
#define LAST_LOW_BITS_OFFSET 7
#define FIRST_LBA_OFFSET 8
#define NB_SECTORS_OFFSET 12

uint32_t change_endian_32(uint32_t val) {
    uint8_t *tab = (uint8_t *)&val;

    tab[0] ^= tab[3];
    tab[3] ^= tab[0];
    tab[0] ^= tab[3];

    tab[1] ^= tab[2];
    tab[2] ^= tab[1];
    tab[1] ^= tab[2];

    return val;
}


void MBR_parse(struct BlockDev *dev) {
    size_t lba_off, nb_sectors_off;

    uint8_t blk_data[BLK_SIZE];
    dev->read_block(dev, 0, blk_data);

    // verify block 0 signature
    if (blk_data[BOOT_SIGNATURE_OFFSET] != 0x55 || blk_data[BOOT_SIGNATURE_OFFSET+1] != 0xaa) {
        kprintf("Wrong signature in MBR: 0x%x, 0x%x\n", blk_data[BOOT_SIGNATURE_OFFSET], blk_data[BOOT_SIGNATURE_OFFSET+1]);
        asm volatile ("hlt");
    }


    kprintf("LITTLE ENDIAN:\n");
    kprintf("Partition 1:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_1_OFFSET + STATUS_OFFSET]); //0x80 is good, else invalid/inactive
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_1_OFFSET + PART_TYPE_OFFSET]); // 0x83 is ext2
    lba_off = PART_1_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", *((uint32_t *)(blk_data + lba_off)));
    nb_sectors_off = PART_1_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", *((uint32_t *)(blk_data + nb_sectors_off)));

    kprintf("Partition 2:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_2_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_2_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_2_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", *((uint32_t *)(blk_data + lba_off)));
    nb_sectors_off = PART_2_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", *((uint32_t *)(blk_data + nb_sectors_off)));

    kprintf("Partition 3:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_3_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_3_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_3_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", *((uint32_t *)(blk_data + lba_off)));
    nb_sectors_off = PART_3_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", *((uint32_t *)(blk_data + nb_sectors_off)));

    kprintf("Partition 4:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_4_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_4_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_4_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", *((uint32_t *)(blk_data + lba_off)));
    nb_sectors_off = PART_4_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", *((uint32_t *)(blk_data + nb_sectors_off)));


    kprintf("BIG ENDIAN:\n");
    kprintf("Partition 1:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_1_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_1_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_1_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + lba_off))));
    nb_sectors_off = PART_1_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + nb_sectors_off))));

    kprintf("Partition 2:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_2_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_2_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_2_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + lba_off))));
    nb_sectors_off = PART_2_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + nb_sectors_off))));

    kprintf("Partition 3:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_3_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_3_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_3_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + lba_off))));
    nb_sectors_off = PART_3_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + nb_sectors_off))));

    kprintf("Partition 4:\n");
    kprintf("\tstatus: 0x%x\n", blk_data[PART_4_OFFSET + STATUS_OFFSET]);
    kprintf("\tpartition type: 0x%x\n", blk_data[PART_4_OFFSET + PART_TYPE_OFFSET]);
    lba_off = PART_4_OFFSET + FIRST_LBA_OFFSET;
    kprintf("\tfirst sector LBA: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + lba_off))));
    nb_sectors_off = PART_4_OFFSET + NB_SECTORS_OFFSET;
    kprintf("\tnb sectors: 0x%x\n", change_endian_32(*((uint32_t *)(blk_data + nb_sectors_off))));

}


struct FS_list *filesystems;

void FS_init() {
    filesystems = kmalloc(sizeof(struct FS_list));
    memset(filesystems, 0, sizeof(struct FS_list));
    filesystems->head = KERNEL_NULL;
    filesystems->last = KERNEL_NULL;
}


void FS_register(FS_detect_cb probe) {
    struct FSImpl *fs = kmalloc(sizeof(struct FSImpl));
    fs->probe = probe;
    fs->next = KERNEL_NULL;

    // append to tail
    if (filesystems->last != KERNEL_NULL) {
        filesystems->last->next = fs;
    } else {
        filesystems->head = fs;
    }
    filesystems->last = fs;
}

int get_MBR_data(struct MBR_data *buff, struct BlockDev *dev) {
    uint8_t blk_data[BLK_SIZE];
    dev->read_block(dev, 0, blk_data);
    buff->signature[0] = blk_data[BOOT_SIGNATURE_OFFSET];
    buff->signature[1] = blk_data[BOOT_SIGNATURE_OFFSET+1];
    if (buff->signature[0] != 0x55 || buff->signature[1] != 0xaa) {
        return 0;
    }
    memcpy(&buff->parts[0], &blk_data[PART_1_OFFSET], sizeof(struct Partition));
    memcpy(&buff->parts[1], &blk_data[PART_2_OFFSET], sizeof(struct Partition));
    memcpy(&buff->parts[2], &blk_data[PART_3_OFFSET], sizeof(struct Partition));
    memcpy(&buff->parts[3], &blk_data[PART_4_OFFSET], sizeof(struct Partition));
    return 1;
}


