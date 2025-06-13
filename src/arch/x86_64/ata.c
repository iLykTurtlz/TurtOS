#include "ata.h"
#include <stdint.h>
#include <stddef.h>
#include "kprint.h"
#include "kmalloc.h"
#include "phys_memory.h"
#include "irq.h"
#include "string.h"

#define BLK_SIZE 512


#define IRQ_PRIMARY 14
#define IRQ_SECONDARY 15

//ports
#define IO_BASE_PRIMARY 0x1f0 //through 0x1f7 => 8 ports
#define IO_BASE_SECONDARY 0x170 //through 0x177 => 8 ports

#define DATA_OFFSET 0
#define ERROR_OFFSET 1 // read
#define FEATURES_OFFSET 1 //write
#define SECTORCOUNT_OFFSET 2
#define LBA_LO_OFFSET 3
#define LBA_MID_OFFSET 4
#define LBA_HI_OFFSET 5
#define DRIVE_SELECT_OFFSET 6
#define CMD_OFFSET 7 //read
#define STATUS_OFFSET 7 //write


#define CONTROL_BASE_PRIMARY 0x3f6
#define CONTROL_BASE_SECONDARY 0x376

#define DCR_PRIMARY 0x3f6 // device control register / alternate status ports
#define DCR_SECONDARY 0x376

#define ALTERNATE_STATUS_PRIMARY 0x3f6
#define ALTERNATE_STATUS_SECONDAY 0x376



#define DATA_PRIMARY 0x1f0
#define SECTORCOUNT_PRIMARY 0x1f2
#define LBA_LO_PRIMARY 0x1f3
#define LBA_MID_PRIMARY 0x1f4
#define LBA_HI_PRIMARY 0x1f5
#define DRIVE_SELECT_PRIMARY 0x1f6
#define CMD_PRIMARY 0x1f7
#define STATUS_PRIMARY 0x1f7

#define DATA_SECONDARY 0x170
#define SECTORCOUNT_SECONDARY 0x172
#define LBA_LO_SECONDARY 0x173
#define LBA_MID_SECONDARY 0x174
#define LBA_HI_SECONDARY 0x175
#define DRIVE_SELECT_SECONDARY 0x176
#define CMD_SECONDARY 0x177
#define STATUS_SECONDARY 0x177




//commands
#define MASTER 0xa0 // = selectMaster
#define SLAVE 0xb0 // = selectSlave

#define IDENTIFY 0xec
#define FLUSH 0xe7

#define READ_MULTIPLE 0xc4
#define WRITE_MULTIPLE 0xc5

#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

#define SOFT_RESET 4

// ATAPI commands
#define      ATAPI_CMD_READ       0xA8
#define      ATAPI_CMD_EJECT      0x1B

// for reading from identification space
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

// for specifying interface type and whether master or slave
#define IDE_ATA        0x00
#define IDE_ATAPI      0x01

#define ATA_MASTER     0x00
#define ATA_SLAVE      0x01



//io register bits
#define LBA 0x40




// status register bits
#define ERR 0x01
#define IDX 0x02 //index always 0
#define CORR 0x04 //corrected data always 0
#define DRQ 0x08 // set if drive has PIO data to transfer or is ready to recv PIO data
#define SRV 0x10 // overlapped mode service request
#define DF 0x20 // drive fault error (does not set ERR)
#define RDY 0x40 //0 after error or when drive is spun down
#define BSY 0x80 // preparing send/recv (wait for clear) - other bits meaningless if BSY set


// error register bits
#define AMNF 0x01 // address mark not found
#define TKZNF 0x02 // track zero not found
#define ABRT 0x04 // aborted command
#define MCR 0x08 // media change request
#define IDNF 0x10 // ID not found
#define MC 0x20 // media changed
#define UNC 0x40 // uncorrectable data error
#define BBK 0x80 // bad block detected


struct ATABlockDev *ATA_instance;


static inline void outb(uint16_t port, uint8_t val) {
    asm("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// static void wait_and_send_command(uint16_t port, uint8_t val) {
//     while (inb(port) & (BSY | DRQ))
//         ;
//     while (inb(port) & (ERR | DF)) // may still be set accidentally?
//         ;
//     outb(port, val);
// }


void ATA_soft_reset(uint16_t ctrl_base) {
    uint8_t status = inb(ctrl_base);
    outb(ctrl_base, status | (1 << 2));
    inb(ctrl_base);
    inb(ctrl_base);
    outb(ctrl_base, status);
}



int detect_device_type(struct ATABlockDev *d) {
    ATA_soft_reset(d->ctrl_base); // = device control register / alternate status ports
    outb(d->io_base + DRIVE_SELECT_OFFSET, 0xa0 | d->slave << 4);
    inb(d->ctrl_base); // 400 ns wait ;)
    inb(d->ctrl_base);
    inb(d->ctrl_base);
    inb(d->ctrl_base);

    uint8_t cl=inb(d->io_base + LBA_MID_OFFSET); // "signature" bytes
    uint8_t ch=inb(d->io_base + LBA_HI_OFFSET);

    if (cl==0x14 && ch==0xEB) {
        return PATAPI;
    }
	if (cl==0x69 && ch==0x96) {
        return SATAPI;
    }
	if (cl==0 && ch == 0) {
        return PATA;
    }
	if (cl==0x3c && ch==0xc3) {
        return SATA;
    }
	return UNKNOWN;
}




static uint16_t identify_device_data[256];
// volatile unsigned static char ide_irq_invoked = 0;
// unsigned static char atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


// struct ide_device {
// 	unsigned char  reserved;    // 0 (Empty) or 1 (This Drive really exists).
// 	unsigned char  channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
// 	unsigned char  drive;       // 0 (Master Drive) or 1 (Slave Drive).
// 	unsigned short type;        // 0: ATA, 1:ATAPI.
// 	unsigned short sign;	    // Drive Signature
// 	unsigned short capabilities;// Features.
// 	unsigned int   commandsets; // Command Sets Supported.
// 	unsigned int   size;	    // Size in Sectors.
// 	unsigned char  model[41];   // Model in string.
// } ide_devices[4];


uint64_t device_length(struct ATABlockDev *d) {
    uint8_t status;
    uint16_t identify_data[256];
    status = inb(d->ctrl_base); // == alternate_status register
    outb(d->io_base + SECTORCOUNT_OFFSET, 0);
    outb(d->io_base + LBA_LO_OFFSET, 0);
    outb(d->io_base + LBA_MID_OFFSET, 0);
    outb(d->io_base + LBA_HI_OFFSET, 0);

    outb(d->io_base + CMD_OFFSET, IDENTIFY);
    status = inb(d->ctrl_base);
    if (status & ERR) {
        kprintf("device_length: Aborted\n");
        asm volatile ("hlt");
    }

    if (status == 0) {
        kprintf("Driver does not exist\n");
        asm volatile ("hlt");
    }
    status = inb(d->ctrl_base + STATUS_OFFSET);
    while (status & BSY) {
        status = inb(d->ctrl_base + STATUS_OFFSET); //alternate status reg
    }

    if (inb(d->io_base + LBA_MID_OFFSET) == 0 && inb(d->io_base + LBA_HI_OFFSET) == 0) {
        while (!(status & (ERR | DRQ))) {
            status = inb(d->ctrl_base);
        }
        if (status & ERR) {
            kprintf("ERR bit set\n");
            asm volatile ("hlt");
        } else {
            for (size_t i=0; i<256; i++) {
                identify_data[i] = inb(DATA_PRIMARY);
            }
            if (identify_device_data[83] & (1 << 10)) {
                kprintf("LBA48 mode supported\n");
            }

            // kprintf("vals: %d, %d, %d, %d\n", identify_data[103], identify_data[102], identify_data[101], identify_data[100]);
            uint64_t retval = 0;
            retval |= ((uint64_t)identify_data[103] << 48);
            retval |= ((uint64_t)identify_data[102] << 32);
            retval |= ((uint64_t)identify_data[101] << 16);
            retval |= identify_data[100];
            return retval;

            // kprintf("60 and 61: %d, %d\n", identify_device_data[60], identify_device_data[61]);
            // kprintf("100-103: %d, %d, %d, %d\n",
            //     identify_data[100],
            //     identify_data[101],
            //     identify_device_data[102],
            //     identify_device_data[103]);
        }

    } else {
        kprintf("Device is not ATA\n");
        asm volatile ("hlt");
    }
    return 0; // error case
}


// IRQ 14 on the PIC, 15


void identify_primary() {
    uint8_t status;

    // select primary master as target drive
    // outb(DRIVE_SELECT_PRIMARY, MASTER);

    // for (int i=0; i<15; i++) {
    //     status = inb(ALTERNATE_STATUS_PRIMARY);
    // }
    // kprintf("15th read of alt status: %x\n", status);
    status = inb(ALTERNATE_STATUS_PRIMARY);

    // read
    // set four io ports to 0
    outb(SECTORCOUNT_PRIMARY, 0);
    outb(LBA_LO_PRIMARY, 0);
    outb(LBA_MID_PRIMARY, 0);
    outb(LBA_HI_PRIMARY, 0);

    // IDENTIFY cmd
    outb(CMD_PRIMARY, IDENTIFY);

    // read status
    status = inb(ALTERNATE_STATUS_PRIMARY);

    // device aborts
    if (status & ERR) {
        if (inb(LBA_MID_PRIMARY) == 0x14 && inb(LBA_HI_PRIMARY) == 0xeb) {
            kprintf("Device is ATAPI - aborted\n");
            asm volatile ("hlt");
        }
        // TODO: look at code example
    }
    if (status == 0) {
        kprintf("Driver does not exist\n");
        asm volatile ("hlt");
    }
    while (status & BSY) {
        status = inb(ALTERNATE_STATUS_PRIMARY);
    }
    // for ATAPI drivers that don't follow spec
    if (inb(LBA_MID_PRIMARY) == 0 && inb(LBA_HI_PRIMARY) == 0) {
        // 0x09 : bit 0 is ERR, bit 3 is DRQ
        while (!(status & (ERR | DRQ))) {
            status = inb(ALTERNATE_STATUS_PRIMARY);
        }
        if (status & ERR) {
            kprintf("ERR bit set\n");
        } else {
            for (size_t i=0; i<256; i++) {
                identify_device_data[i] = inb(DATA_PRIMARY);
            }
            if (identify_device_data[83] & (1 << 10)) {
                kprintf("LBA48 mode supported\n");
            }
            kprintf("60 and 61: %d, %d\n", identify_device_data[60], identify_device_data[61]);
            kprintf("100-103: %d, %d, %d, %d\n",
                identify_device_data[100],
                identify_device_data[101],
                identify_device_data[102],
                identify_device_data[103]);

        }
    } else {
        kprintf("Not an ATA drive\n");
        asm volatile ("hlt");
    }

}

void handle_ATA_read(uint8_t irq, uint32_t err, void *arg) {
// Check ATA status (make sure it's DRQ and not an error)
// Read the data from 0x1F0
// Set a flag or wake a waiting thread
// Send EOI to the PIC

// if (!(inb(0x1F7) & 0x08)) // error
    struct ATABlockDev *ata = (struct ATABlockDev *)arg;

    uint8_t status = inb(ata->io_base + STATUS_OFFSET);
    if (status & ERR) {
        uint8_t error = inb(ata->io_base + ERROR_OFFSET);
        kprintf("ATA handler error num: %x\n", error);
        asm volatile ("hlt");
    }
    // if ((status & DRQ) == 0) {
    //     kprintf("ATA handler: DRQ bit not set in status = %x\n", status);
    //     asm volatile ("hlt");
    // }

    // now safe to read
    struct ATARequest *req = ata->req_head;

    for (int i = 0; i < BLK_SIZE / 2; i++) {
        req->data[i] = inw(ata->io_base);
    }

    PROC_unblock_all(req->blocked);
}


typedef union lba {
    uint64_t addr;
    struct {
        uint8_t low0;
        uint8_t low1;
        uint8_t low2;
        uint8_t high0;
        uint8_t high1;
        uint8_t high2;
        uint16_t unused;
    } parts;
} lba_t;


void ATA_add_request(struct ATABlockDev *ata, struct ATARequest *req) {
    if (req == KERNEL_NULL)
        return;
    req->next = KERNEL_NULL;
    if (ata->req_head == KERNEL_NULL) {
        ata->req_head = req;
    } else {
        ata->req_tail->next = req;
    }
    ata->req_tail = req;
}

void ATA_pop_head_request(struct ATABlockDev *ata) {
    if (ata->req_head == KERNEL_NULL) {
        return;
    }
    struct ATARequest *head = ata->req_head;
    ata->req_head = ata->req_head->next;
    if (ata->req_head == KERNEL_NULL) {
        ata->req_tail = KERNEL_NULL;
    }
    // free the former head
    kfree(head->blocked);
    kfree(head);
}


void ATA_48_read_cmd(struct ATABlockDev *ata, uint64_t blk_num) {
    lba_t lba;
    lba.addr = blk_num;
        
    // a little polling OK?
    while (inb(ata->io_base + STATUS_OFFSET) & BSY)
        ;

    // Send 0x40 for the "master" or 0x50 for the "slave" to port 0x1F6
    outb(ata->io_base + DRIVE_SELECT_OFFSET, LBA | (ata->slave << 4)); // 0ce0 or LBA
    

    uint16_t sectorcount = 0x0001;
    // write upper 3 bytes
    outb(ata->io_base + SECTORCOUNT_OFFSET, (uint8_t)(sectorcount >> 8));
    outb(ata->io_base + LBA_LO_OFFSET, lba.parts.high0);
    outb(ata->io_base + LBA_MID_OFFSET, lba.parts.high1);
    outb(ata->io_base + LBA_HI_OFFSET, lba.parts.high2);
    // write lower 3 bytes
    outb(ata->io_base + SECTORCOUNT_OFFSET, (uint8_t)sectorcount);
    outb(ata->io_base + LBA_LO_OFFSET, lba.parts.low0);
    outb(ata->io_base + LBA_MID_OFFSET, lba.parts.low1);
    outb(ata->io_base + LBA_HI_OFFSET, lba.parts.low2);

    // Send the "READ SECTORS EXT" command (0x24) to port 0x1F7
    outb(ata->io_base + CMD_OFFSET, ATA_CMD_READ_PIO_EXT);
}



int ATA_48_read_block(struct BlockDev *this, uint64_t blk_num, void *dst) {
    // start
    struct ATABlockDev *ata = (struct ATABlockDev *)this; // DOWNCAST OK?
    // lba_t lba;
    // lba.addr = blk_num;



    // allocate node
    // add to list
    struct ATARequest *req = kmalloc(sizeof(struct ATARequest));
    memset(req, 0, sizeof(struct ATARequest));
    req->block = blk_num;
    req->next = KERNEL_NULL;
    req->blocked = PROC_list_new();

    CLI;
    // if the req queue was already empty -> read cmd
    int was_empty = (ata->req_head == KERNEL_NULL);

    ATA_add_request(ata, req);
    

    if (was_empty) {
        ATA_48_read_cmd(ata, blk_num);
    }
    STI;

    PROC_block_on(req->blocked, 1); // enable ints = true???

    // copy data to dst
    memcpy(dst, req->data, BLK_SIZE);

    // free the request
    ATA_pop_head_request(ata);

    if (ata->req_head != KERNEL_NULL) {
        ATA_48_read_cmd(ata, ata->req_head->block);
    }

    // TODO retval depends on success or failure 
    return 0; 
}





void display_device_type(device_type t) {
    switch (t) {
        case PATAPI:
            kprintf("PATAPI");
            break;
        case SATAPI:
            kprintf("SATAPI");
            break;
        case PATA:
            kprintf("PATA");
            break;
        case SATA:
            kprintf("SATA");
            break;
        case UNKNOWN:
            kprintf("UNKNOWN");
            break;
        default:
            kprintf("unrecognized device type");
    }
}


struct BlockDev *ATA_probe(uint16_t base, uint16_t master, uint8_t slave, uint8_t irq) {
    if (inb(STATUS_PRIMARY) == 0xff) {
        kprintf("'Floating' primary bus\n");
        asm volatile ("hlt");
    }
    if (inb(STATUS_SECONDARY) == 0xff) {
        kprintf("'Floating' secondary bus\n");
        asm volatile ("hlt");
    }

    struct ATABlockDev *ata;
    ata = kmalloc(sizeof(*ata));
    memset(ata, 0, sizeof(*ata));

    // subclass init //fs type???
    ata->io_base = base;
    ata->ctrl_base = master;
    ata->slave = slave;
    ata->irq = irq;
    ata->dev.read_block = ATA_48_read_block;
    ata->req_head = KERNEL_NULL;
    ata->req_tail = KERNEL_NULL;

    // superclass init
    ata->dev.blk_size = 512;
    ata->dev.fs_type = MASS_STORAGE;
    ata->dev.name = strdup("ata");
    ata->dev.next = KERNEL_NULL;
    ata->dev.tot_length = device_length(ata);

    // verify device type
    int dev_type = detect_device_type(ata); // NEED TO UPCAST?
    if (dev_type != PATA && dev_type != SATA) {
        kprintf("Wrong device type = ");
        display_device_type(dev_type);
        kprintf("\n");
        asm volatile ("hlt");
    }

    // It's PATA - OK?
    kprintf("device type = ");
    display_device_type(dev_type);
    kprintf("\n");


    // set interrupt handler (0x20 is the irq for the PIC base)
    IRQ_set_handler(ata->irq, &handle_ATA_read, ata);

    
    // unmask PIC
    // IRQ_clear_mask(2);
    // IRQ_clear_mask(ata->irq - 0x20);
    // kprintf("Cleared PIC nb: %d\n", ata->irq - 0x20);


    // for (int i=2; i<15; i++) {
    //     IRQ_clear_mask(i);
    // }

    return (struct BlockDev *)ata;
}





void ATA_init() {
    uint8_t slave = 0;

    //0x20 is the base IRQ of the PIC
    struct BlockDev *dev = ATA_probe(IO_BASE_PRIMARY, CONTROL_BASE_PRIMARY, slave, 0x20 + IRQ_PRIMARY);
    
    kprintf("nb sectors: %ld\n", dev->tot_length);

    // register the ATA block device
    BLK_register(dev);

    // THERE HAS TO BE A BETTER WAY
    ATA_instance = (struct ATABlockDev *)dev;

    // enable interrupts
    outb(DCR_PRIMARY, 0x00);
}


uint16_t ip_checksum(const void *data, size_t length) {
    const uint8_t *bytes = data;
    uint32_t sum = 0;

    // Sum all 16-bit words
    for (size_t i = 0; i + 1 < length; i += 2) {
        uint16_t word = (bytes[i] << 8) | bytes[i + 1];
        sum += word;
    }

    // Handle odd byte at the end (if any)
    if (length & 1) {
        uint16_t last_byte = bytes[length - 1] << 8;
        sum += last_byte;
    }

    // Fold 32-bit sum to 16 bits
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    // One's complement
    return (uint16_t)(~sum);
}

void test_ATA_read(int blk_num) {
    uint16_t buff[BLK_SIZE/2];
	ATA_instance->dev.read_block(&ATA_instance->dev, blk_num, buff);
	kprintf("Block %d:\n", blk_num);
	for (int i=0; i<BLK_SIZE/2; i++) {
		// endian-ness correction
		uint8_t *x = (uint8_t *)(buff + i);
		uint8_t *y = x + 1;

		*x ^= *y;
		*y ^= *x;
		*x ^= *y;
	}

	for (int i=0; i< 256; i++) {
		kprintf("%x", buff[i]);
	}
	kprintf("\n");

	uint16_t checksum = ip_checksum(buff, BLK_SIZE);
	kprintf("checksum: 0x%x (%d)", checksum, checksum);
}

