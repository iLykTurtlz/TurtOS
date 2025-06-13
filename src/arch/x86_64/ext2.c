#include "ext2.h"
#include "phys_memory.h"
#include "kprint.h"
#include "kmalloc.h"
#include "string.h"
#include "math.h"
#include "irq.h"
#include "md5.h"

//for testing
#include "ata.h"

#define ATA_BLK_SIZE 512
#define EXT2_BLK_SIZE 1024
#define VALID 0x80
#define EXT2 0x83
#define ROOT_INODE_NUM 2

#define REGULAR_FILE 0x8000
#define DIR 0x4000
#define EXT2_S_IFMT 0xF000

#define EXT2_FT_DIR 2



void read_ext2_block(struct VFS_Ext2SuperBlock *sb, uint64_t block_num, void *dst) {
    // uint64_t part_start_lba = sb->part_start_lba;
    uint64_t lba = sb->part_start_lba + (sb->super.block_size / sb->super.dev->blk_size) * block_num;
    sb->super.dev->read_block(sb->super.dev, lba, dst);
    sb->super.dev->read_block(sb->super.dev, lba + 1, ((uint8_t *)dst) + sb->super.dev->blk_size);
    
    // assertions
    if ((sb->super.block_size / sb->super.dev->blk_size) != 2) {
        kprintf("read_ext2_block: Block size ratio is %ld\n", (sb->super.block_size / sb->super.dev->blk_size));
        asm volatile ("hlt");
    }
}


// void read_ext2_block(struct BlockDev *dev, uint64_t lba, void *dst) {
//     dev->read_block(dev, lba, dst);
//     dev->read_block(dev, lba + 1, ((uint8_t *)dst) + ATA_BLK_SIZE);
// }




// void find_inode_location(uint64_t inode_num, struct InodeLocationInfo *loc, struct VFS_Ext2SuperBlock *sb) {
//     loc->group_num = (inode_num - 1) / sb->inodes_per_group;
//     uint64_t idx = (inode_num - 1) % sb->inodes_per_group;

//     uint64_t inode_table_block = sb->groups[loc->group_num].starting_block_lba;

//     uint64_t offset = idx * sb->inode_size;
//     uint64_t block_offset = offset / sb->super.block_size;
//     uint64_t offset_in_block = offset % sb->super.block_size;

//     loc->start_block_lba = inode_table_block + block_offset;
//     loc->byte_offset = offset_in_block;

//     uint64_t end_offset = (idx + 1) * sb->inode_size;
//     uint64_t end_block_offset = (end_offset + sb->super.block_size - 1) / sb->super.block_size;

//     loc->nb_blocks = end_block_offset - block_offset;
// }


// data needed to read in the right data and find the inode within it
// struct InodeLocationInfo {
//     uint64_t group_num;
//     uint64_t start_block_lba;
//     uint64_t byte_offset; //within the starting block
//     uint64_t nb_blocks; //device blocks, not inode blocks
// };

struct InodeLocationInfo {
    uint64_t block_num; //ext2 block
    uint64_t byte_offset; //in block
};



void find_inode_location(uint64_t inode_num, struct InodeLocationInfo *loc, struct VFS_Ext2SuperBlock *sb) {
    uint64_t group_num = (inode_num - 1) / sb->inodes_per_group;
    uint64_t idx = (inode_num - 1) % sb->inodes_per_group;
    uint64_t block = idx * sb->inode_size / sb->super.block_size;
    uint64_t inodes_per_block = sb->super.block_size / sb->inode_size;

    if (inodes_per_block != (1024 / 256)) {
        kprintf("Wrong inodes per block: %ld\n", inodes_per_block);
        asm volatile ("hlt");
    }

    loc->block_num = sb->groups[group_num].starting_block_lba + block;
    loc->byte_offset = (idx % inodes_per_block) * sb->inode_size;




    // // logical block 512
    // uint64_t block = idx * sb->inode_size / sb->super.dev->blk_size;//sb->super.block_size;

    // uint64_t table_start = sb->groups[loc->group_num].starting_block_lba;
    // // kprintf("SANITY CHECK - SHOULD BE 2: %d\n", sb->super.block_size / sb->super.dev->blk_size);
    // loc->start_block_lba = table_start + block;
    // loc->byte_offset = (2 * idx * sb->inode_size) % sb->super.dev->blk_size;
    // uint64_t end_block = ( 2 * idx + 2) * sb->inode_size / sb->super.dev->blk_size;
    // loc->nb_blocks = end_block - block + 1;

    // uint64_t block = (idx * sb->inode_size) / sb->super.block_size; // 1024 block size
    // kprintf("Block for inode %ld is %ld\n", inode_num, block);
    // loc->byte_offset = (idx * sb->inode_size) % sb->super.block_size; // offset in an inode block
    // if (loc->byte_offset >= 512) {
    //     loc->byte_offset -= 512;
    //     loc->start_block_lba = 2 * block + 1;
    // } else {
    //     loc->start_block_lba = 2 * block;
    // }
    // loc->nb_blocks = 1;
}

struct VFS_Inode *read_inode(struct VFS_SuperBlock *this, uint64_t inode_num) {
    struct InodeLocationInfo info;
    find_inode_location(inode_num, &info, (struct VFS_Ext2SuperBlock *)this);

    uint8_t *buff = kmalloc(this->block_size);
    read_ext2_block((struct VFS_Ext2SuperBlock *)this, info.block_num, buff);


    struct VFS_Inode *res = kmalloc(sizeof(struct VFS_Inode));

    struct Inode *inode = (struct Inode *)(buff + info.byte_offset);

    // kprintf("Root inode?\n");
    // for (uint8_t *ptr = (uint8_t *)inode; ptr<(uint8_t *)inode + sizeof(struct Inode); ptr++) {
    //     if (*ptr > 15) {
    //         kprintf("%x", *ptr);
    //     } else {
    //         kprintf("0%x", *ptr);
    //     }
    // }

    // kprintf("\nEnd of inode\n");
    // kprintf("Type: %x\n", inode->type_and_permissions & EXT2_S_IFMT);

    // asm volatile ("hlt");
   

    res->dev = this->dev;
    res->parent = KERNEL_NULL; //should be set by the caller (unless root)
    res->parent_superblock = this;
    res->type_and_permissions = inode->type_and_permissions;

    uint64_t size = 0;
    size |= inode->low_size;
    size |= (((uint64_t)inode->upper_size) << 32);
    res->size = size;
    memcpy(res->direct_blocks, inode->direct_blocks, 12 * sizeof(uint32_t));
    res->single_indirection = inode->single_indirection;
    res->double_indirection = inode->double_indirection;
    res->triple_indirection = inode->triple_indirection;



    kfree(buff);
    return res;
}



//premature stopping after first superblock found
// TODO: support multiple partitions
struct VFS_SuperBlock *ext2_probe(struct BlockDev *dev) {
    struct MBR_data mbr;
    if (get_MBR_data(&mbr, dev) == 0) {
        kprintf("ext2_probe: problem getting MBR data");
        asm volatile ("hlt");
    }
    int nb_parts = 0;
    for (int i=0; i<4; i++) {
        if (mbr.parts[i].status == VALID && mbr.parts[i].type == EXT2) {
            nb_parts++;
            uint64_t superblock_lba = mbr.parts[i].first_sector_lba + 2; //1024 (2 blocks) past the beginning of the volume
            
            uint8_t block_buff[EXT2_BLK_SIZE];
            // read_ext2_block(dev, superblock_lba, block_buff); // I don't like this: what if I need to support a different ext2 block size?
            dev->read_block(dev, superblock_lba, block_buff);
            dev->read_block(dev, superblock_lba, block_buff + ATA_BLK_SIZE);


            struct Ext2SuperBlock *superblock = (struct Ext2SuperBlock *)block_buff;
            
            struct VFS_Ext2SuperBlock *ret = kmalloc(sizeof(struct VFS_Ext2SuperBlock));
            ret->part_start_lba = mbr.parts[i].first_sector_lba;
            ret->nb_inodes = superblock->nb_inodes;
            ret->nb_blocks = superblock->nb_blocks;
            ret->log_block_size = superblock->log_block_size;
            ret->blocks_per_group = superblock->blocks_per_group;
            ret->inodes_per_group = superblock->inodes_per_group;
            ret->major_version_nb = superblock->major_version_nb;
            ret->inode_size = superblock->inode_size;
            ret->super.block_size = pow_base_2(10 + superblock->log_block_size);
            ret->super.nb_blocks = superblock->nb_blocks;

            int nb_groups = superblock->nb_blocks / superblock->blocks_per_group;
            if (superblock->nb_blocks % superblock->blocks_per_group != 0) {
                nb_groups++;
            }
            ret->nb_groups = nb_groups;
            if (nb_groups *sizeof(struct BlkGrpDescTbl) > EXT2_BLK_SIZE) {
                kprintf("ext2 probe: implausible nb_groups = %d\n", nb_groups);
                asm volatile ("hlt");
            }

            // block buff no longer contains superblock
            // SANITY CHECK
            kprintf("SANITY CHECK: sizeof(ext2 block): %ld, sizeof(dev block): %d\n", ret->super.block_size, dev->blk_size);
            kprintf("\tratio: %ld\n", (ret->super.block_size / dev->blk_size));

            // same issue as before...unknown ext2 block size on principle
            dev->read_block(dev, superblock_lba + 2, block_buff);
            dev->read_block(dev, superblock_lba + 3, block_buff + ATA_BLK_SIZE);


            kprintf("superblock lba: %ld\n", superblock_lba);
            kprintf("group descriptor table: %ld\n", superblock_lba + 2);

            // BGDT OK!!! :)
            struct BlkGrpDescTbl *group_table = (struct BlkGrpDescTbl *)block_buff;
            ret->groups = kmalloc(nb_groups * sizeof(struct VFS_Ext2BlockGroup));
            for (int i=0; i<nb_groups; i++) {
                ret->groups[i].blk_usage_bitmap_lba = group_table->block_usage_bitmap_lba;
                ret->groups[i].inode_usage_bitmap_lba = group_table->inode_usage_bitmap_lba;
                ret->groups[i].starting_block_lba = group_table->start_block;
                ret->groups[i].nb_unallocated_blocks = group_table->nb_unallocated_blocks;
                ret->groups[i].nb_unallocated_inodes = group_table->nb_unallocated_inodes;
                ret->groups[i].nb_directories = group_table->nb_directories;
                group_table++;
            }
            

            // SANITY CHECKS
            // kprintf("Ext2 Probe Sanity Check: block size = %ld (should be 1024)\n", ret->super.block_size);
            // kprintf("Ext2 Probe Sanity Check: nb groups = %d\n", nb_groups);

            // INFO
            // kprintf("nb blocks: %d\n", ret->nb_blocks);
            // kprintf("nb inodes: %d, inode size: %d\n", ret->nb_inodes, ret->inode_size);
            // kprintf("blocks per group: %d\n", ret->blocks_per_group);
            // kprintf("major version nb: %d\n", ret->major_version_nb);

            // // find the root
            // size_t root_group_num = (ROOT_INODE_NUM - 1) / ret->inodes_per_group;
            // kprintf("root group num: %ld\n", root_group_num);
            
            ret->super.read_inode = &read_inode;
            ret->super.dev = dev;
            ret->super.type = strdup("ext2");
            ret->super.name = strdup("root superblock");

            ret->root = ret->super.read_inode((struct VFS_SuperBlock *)ret, ROOT_INODE_NUM);
            ret->root->parent = KERNEL_NULL; //since it's the root

            return (struct VFS_SuperBlock *)ret;
        }
    }    
    return KERNEL_NULL;
}



// TODO: get rid of hardcoded read_ext2_block (malloc once, use every time)
void get_inode_block_data(struct VFS_Inode *inode, uint64_t block_index, void *dst) {
    uint8_t *block_buffer = kmalloc(inode->parent_superblock->block_size);
    if (inode->parent_superblock->block_size != 1024) {
        kprintf("get_inode_block_data: wrong block size\n");
        asm volatile ("hlt");
    }
    uint32_t block;
    uint64_t arr_size = inode->parent_superblock->block_size / sizeof(uint32_t);
    if (arr_size != 256) {
        kprintf("Miscalculated indirection array size as %ld\n", arr_size);
        asm volatile ("hlt");
    }
    if (block_index < 12) {
        block = inode->direct_blocks[block_index];
    }
    else if (block_index < 12 + arr_size) {
        read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, inode->single_indirection, block_buffer);
        // block_lba = *((uint32_t *)(block_buffer + (32 * (block_index - 12))));
        block = ((uint32_t *)block_buffer)[block_index - 12];
    }
    else if (block_index < 12 + arr_size + arr_size * arr_size) {
        read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, inode->double_indirection, block_buffer);
        uint64_t remaining_idx = block_index - (12 + arr_size);
        uint64_t first_indirection_idx = remaining_idx / arr_size;
        // uint32_t first_indirection_lba = *((uint32_t *)(block_buffer + (32 * first_indirection_idx))); 
        uint32_t first_indirection_block = ((uint32_t *)block_buffer)[first_indirection_idx];
        read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, first_indirection_block, block_buffer);
        uint64_t second_indirection_idx = remaining_idx % arr_size;
        // block_lba = *((uint32_t *)(block_buffer + (32 * second_indirection_idx)));
        block = ((uint32_t *)block_buffer)[second_indirection_idx];
    }
    else {
        read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, inode->triple_indirection, block_buffer);
        uint64_t remaining_idx = block_index - (12 + arr_size + arr_size * arr_size);
        uint64_t first_indirection_idx = remaining_idx / (arr_size * arr_size);
        uint32_t first_indirection_block = ((uint32_t *)block_buffer)[first_indirection_idx];
        read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, first_indirection_block, block_buffer);
        //index in remaining subtree
        remaining_idx = remaining_idx % (arr_size * arr_size);
        uint64_t second_indirection_idx = remaining_idx / arr_size;
        uint32_t second_indirection_block = ((uint32_t *)block_buffer)[second_indirection_idx];
        read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, second_indirection_block, block_buffer);
        uint64_t third_indirection_idx = remaining_idx % arr_size;
        block = ((uint32_t *)block_buffer)[third_indirection_idx];
    }
    read_ext2_block((struct VFS_Ext2SuperBlock *)inode->parent_superblock, block, dst);
    kfree(block_buffer);
}



int readdir(struct VFS_Inode *inode, readdir_cb cb, void *p);

struct TreeState {
    int level;
    struct VFS_SuperBlock *sb;
};

typedef int (*readdir_cb)(const char *name, int inode_num, void *arg);

int tree(const char *name, int inode_num, void *arg) {
    // kprintf("IN TREE\n");
    struct TreeState *ts = arg;
    for (int i=0; i<ts->level; i++) {
        kprintf("\t");
    }
    kprintf("%s\n", name);
    if (strcmp(".", name) != 0 && strcmp("..", name) != 0) {
        struct TreeState next_ts;
        next_ts.level = ts->level + 1;
        next_ts.sb = ts->sb;
        struct VFS_Inode *inode = read_inode(ts->sb, inode_num);
        uint32_t type = inode->type_and_permissions & EXT2_S_IFMT;
        if (type == DIR)
            readdir(inode, &tree, &next_ts);
        kfree(inode);
    }
    return 1;
}

int ls(const char *name, int inode_num, void *arg) {
    // kprintf("IN LS\n");
    // struct VFS_Inode *inode = read_inode(arg, inode_num);
    kprintf("%s\n", name);
    // readdir(inode, &ls, arg);
    // kfree(inode);
    return 1;
}

struct SearchState {
    const char *remaining_path;
    uint32_t inode_num;
    struct VFS_SuperBlock *sb;
};


int search(const char *name, int inode_num, void *arg) {
    struct SearchState *state = (struct SearchState *)arg;
    const char *remaining_path = state->remaining_path;
    if (state->inode_num != 0 || strlen(name) == 0) {
        return 0;
    }
    const char *next_delim = strchr(state->remaining_path, '/');
    if (next_delim == KERNEL_NULL) {
        if (strcmp(name, state->remaining_path) == 0) {
            state->inode_num = inode_num;
            return 1;
        }
    } 
    else {
        size_t token_length = next_delim - state->remaining_path;
        char *buffer = kmalloc(token_length + 1);
        strncpy(buffer, state->remaining_path, token_length);
        buffer[token_length] = '\0';
        if (strcmp(buffer, name) == 0 && strcmp(".", name) != 0 && strcmp("..", name) != 0) {
            struct VFS_Inode *inode = read_inode(state->sb, inode_num);
            uint32_t type = inode->type_and_permissions & EXT2_S_IFMT;
            if (type == DIR) {
                state->remaining_path = next_delim + 1;
                readdir(inode, &search, state);
                state->remaining_path = remaining_path;
            }
            kfree(inode);
        }
        kfree(buffer);
    }
    return 0;
}

struct DirEntry {
    uint32_t inode_nb;
    uint16_t size;
    uint8_t name_length; //lsb of name length
    uint8_t type;
}__attribute__((packed));

struct DirEntry *next_dir_entry(struct DirEntry *curr_entry_start) {
    uint16_t size = curr_entry_start->size;
    uint8_t *ptr = (uint8_t *)curr_entry_start;
    if (size % 4 == 0) {
        return (struct DirEntry *)(ptr + size);
    }
    kprintf("WRONG SIZE: %d\n", size);
    asm volatile ("hlt");
    return (struct DirEntry *)(ptr + (size - (size % 4) + 4));
    // kprintf("\tptr before = %p, size = %d, ptr after = %p\n", ptr, size, ptr+size);
    // return (struct DirEntry *)(ptr + size);
}


int readdir(struct VFS_Inode *inode, readdir_cb cb, void *p) {
    uint32_t type = inode->type_and_permissions & EXT2_S_IFMT;
    // kprintf("type and permissions: %x\n", inode->type_and_permissions);
    if (type != DIR) {
        // kprintf("type: %x\n", type);
        return 0;
    }
    // kprintf("INODE SIZE: %ld\n", inode->size);
    uint64_t total_bytes_read = 0;
    uint64_t block_idx = 0;
    uint8_t block_buff[EXT2_BLK_SIZE];
    char strbuff[256];
    while (total_bytes_read < inode->size) {
        get_inode_block_data(inode, block_idx++, block_buff);
        struct DirEntry *ptr = (struct DirEntry *)block_buff;
        uint8_t *block_end = block_buff + EXT2_BLK_SIZE;
        // uint64_t bytes_read = total_bytes_read;
        // 12 is minimum entry size?
        while((uint8_t *)ptr < block_end) {
            // 4 is minimum name length
            if ((uint8_t *)ptr + sizeof(struct DirEntry) + 4 > block_end) {
                break;
            }
            if (ptr->size < 12 || ((uint8_t *)ptr + ptr->size > block_end)) {
                break;
            }
 
            if (ptr->inode_nb != 0) {
                // strncpy(strbuff, (char *)(ptr + 1), ptr->name_length);
                memcpy(strbuff, ptr + 1, ptr->name_length);
                strbuff[ptr->name_length] = '\0';
                // kprintf("BEFORE CALLBACK\n");
                // kprintf("Reading entry at %p: '%s' (inode %d) in block %lu\n", ptr, strbuff, ptr->inode_nb, block_idx);
                cb(strbuff, ptr->inode_nb, p);
            }
            ptr = next_dir_entry(ptr);
        }
        total_bytes_read += EXT2_BLK_SIZE;
    }
    return 1;
}



void test_readdir(struct VFS_SuperBlock *sb) {
    // // ls test
    // readdir(((struct VFS_Ext2SuperBlock *)sb)->root, &ls, KERNEL_NULL);

    // tree test
    struct TreeState ts;
    ts.level = 0;
    ts.sb = sb;
    readdir(((struct VFS_Ext2SuperBlock *)sb)->root, &tree, &ts);
    kprintf("END OF TEST\n");
}


void free(struct VFS_Inode **inode) {
    kfree(*inode);
    *inode = KERNEL_NULL;
}

int close(struct File **file) {
    free(&(*file)->inode);
    *file = KERNEL_NULL;
    return 1;
}


int read_file(struct File *file, char *dst, int len) {
    char *block_buffer = kmalloc(file->inode->parent_superblock->block_size);
    size_t block_size = file->inode->parent_superblock->block_size;
    if (block_size != 1024) {
            kprintf("read: wrong block size\n");
            asm volatile ("hlt");
    }
    int idx = 0;
    while (idx < len && file->offset < file->inode->size) {
        uint32_t block_idx = file->offset / block_size;
        uint32_t block_offset = file->offset % block_size;
        get_inode_block_data(file->inode, block_idx, block_buffer);
        size_t nb_bytes = min(block_size - block_offset, file->inode->size - file->offset);
        nb_bytes = min(nb_bytes, len - idx);
        memcpy(&dst[idx], block_buffer + block_offset, nb_bytes);
        idx += nb_bytes;
        file->offset += nb_bytes;
    }
    kfree(block_buffer);
    return (int)idx;
}

int lseek(struct File *file, off_t offset) {
    file->offset = min(offset, file->inode->size);
    return 1;
}


struct File *open(struct Ext2Filesystem *this, uint32_t inode) {
    struct File *f = kmalloc(sizeof(struct File));
    f->inode = read_inode((struct VFS_SuperBlock *)this->sb, inode);
    f->offset = 0;
    f->free = &free;
    f->close = &close;
    f->read = &read_file;
    f->lseek = &lseek;
    return f;
}

struct Ext2Filesystem *Ext2Filesystem_new() {
    struct Ext2Filesystem *fs = kmalloc(sizeof(struct Ext2Filesystem));
    struct VFS_SuperBlock *sb = ext2_probe((struct BlockDev *)ATA_instance);
    fs->sb = (struct VFS_Ext2SuperBlock *)sb;
    fs->open = &open;
    return fs;
}

void hexdump(char *buff, size_t len) {
    kprintf("\n");
    for (size_t i=0; i<len; i++) {
      
        if (buff[i] < 16) {
            kprintf("0%x", buff[i]);
        } else {
            kprintf("%x", buff[i]);
        }
        if (i & 1) {
            kprintf(" ");
        }
        if (i % 16 == 15)
            kprintf("\n");
        
    }
    kprintf("\n");
}



void test_file_read(struct Ext2Filesystem *fs) {
    struct SearchState state;

    // CHOICES OF FILES TO READ
    // state.remaining_path = "boot/grub/grub.cfg"
    state.remaining_path = "boot/kernel.bin";


    state.sb = (struct VFS_SuperBlock *)fs->sb;
    state.inode_num = 0; //sentinel value
    readdir(fs->sb->root, &search, &state);
    kprintf("Result: %s at inode %d\n", state.remaining_path, state.inode_num);
    if (state.inode_num != 0) {
        struct File *f = fs->open(fs, state.inode_num);
        kprintf("File size: %ld\n", f->inode->size);
        char *buff = kmalloc(f->inode->size + 512);
        memset(buff, 0, f->inode->size + 512);

        // // read in two parts
        // uint64_t stop = 10;
        // f->read(f, buff, stop);
        // f->read(f, buff + stop, f->inode->size); //read to EOF

        // // read in 512 byte chunks
        // size_t read_so_far=0;
        // while (read_so_far < f->inode->size) {
        //     f->read(f, buff + read_so_far, 512);
        //     read_so_far += 512;
        // }

        // // same as above, using the return value of read
        size_t read_so_far=0;
        while (read_so_far < f->inode->size) {
            read_so_far += f->read(f, buff + read_so_far, 512);
        }


        // read whole file at once
        // f->read(f, buff, f->inode->size);


        hexdump(buff, min(f->inode->size, 120));

        // MD5
        // void md5(char *input, size_t len, uint8_t *result);
        uint8_t result[16];
        md5Bytes(buff, f->inode->size, result);

        kprintf("MD5: ");
        for (int i=0; i<16; i++) {
            if (result[i] < 16) {
                kprintf("0%x", result[i]);
            } else {
                kprintf("%x", result[i]);
            }
        }
        kprintf("\n");


        f->close(&f);
        kfree(buff);
    } else {
        kprintf("Target not found\n");
    }
}
