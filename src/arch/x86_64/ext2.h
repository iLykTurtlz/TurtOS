#ifndef EXT2_H
#define EXT2_H

#include "filesystem.h"


typedef uint64_t off_t;
typedef int (*readdir_cb)(const char *name, int inode_num, void *arg);

struct Inode {
    uint16_t type_and_permissions;
    uint16_t uid;
    uint32_t low_size;
    uint32_t last_access;
    uint32_t creation_time;
    uint32_t last_modification_time;
    uint32_t deletion_time;
    uint16_t gid;
    uint16_t hard_link_count;
    uint32_t ext2_disk_sector_count;
    uint32_t flags;
    uint32_t os_specific_value;
    uint32_t direct_blocks[12];
    uint32_t single_indirection;
    uint32_t double_indirection;
    uint32_t triple_indirection;
    uint32_t nfs_generation_nb;
    uint32_t acl; //if version >= 1
    uint32_t upper_size; //if version >= 1
}__attribute__((packed));

struct VFS_Inode;

struct VFS_SuperBlock {
    const char *name;
    const char *type;
    // FS_detect_cb probe;
    size_t block_size;
    size_t nb_blocks;
    struct BlockDev *dev;
    struct VFS_Inode *(*read_inode)(struct VFS_SuperBlock *this, uint64_t inode_nb);
};


struct Ext2SuperBlock {
    uint32_t nb_inodes;
    uint32_t nb_blocks;
    uint32_t nb_reserved_blocks;
    uint32_t nb_unallocated_blocks;
    uint32_t nb_unallocated_inodes;
    uint32_t superblock_block_nb;
    uint32_t log_block_size;
    uint32_t log_fragment_size;
    uint32_t blocks_per_group;
    uint32_t fragments_per_group;
    uint32_t inodes_per_group;
    uint32_t last_mount_time;
    uint32_t last_written_time;
    uint16_t mounts_since_last_fsck;
    uint16_t mounts_allowed_before_fsck;
    uint16_t signature; // 0xef53
    uint16_t fs_state;
    uint16_t error_action;
    uint16_t minor_version_nb;
    uint32_t time_since_last_fsck;
    uint32_t max_time_between_fsck;
    uint32_t os_id;
    uint32_t major_version_nb;
    uint16_t uid_for_reserved_blocks;
    uint16_t gid_for_reserved_blocks;
    uint32_t first_non_reserved_inode;
    uint32_t inode_size;
}__attribute__((packed));

struct BlkGrpDescTbl {
    // block addresses
    uint32_t block_usage_bitmap_lba; 
    uint32_t inode_usage_bitmap_lba;
    uint32_t start_block;

    // "in a group"
    uint16_t nb_unallocated_blocks;
    uint16_t nb_unallocated_inodes;
    uint16_t nb_directories;
    uint64_t padding1;
    uint32_t padding2;
    uint16_t padding3;
}__attribute__((packed));


struct VFS_Ext2BlockGroup {
    uint32_t blk_usage_bitmap_lba;
    uint32_t inode_usage_bitmap_lba;
    uint32_t starting_block_lba;
    uint16_t nb_unallocated_blocks;
    uint16_t nb_unallocated_inodes;
    uint16_t nb_directories;
};






struct VFS_Inode {
    struct BlockDev *dev;
    struct VFS_Inode *parent;
    struct VFS_SuperBlock *parent_superblock;
    uint16_t type_and_permissions;
    uint64_t size;
    uint32_t direct_blocks[12];
    uint32_t single_indirection;
    uint32_t double_indirection;
    uint32_t triple_indirection;
};


//inherit struct SuperBlock (cf. filesystem.h)
struct VFS_Ext2SuperBlock {
    struct VFS_SuperBlock super;
    struct VFS_Inode *root;
    uint32_t nb_inodes;
    uint32_t nb_blocks;
    uint32_t log_block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    // uint16_t signature;
    uint32_t major_version_nb;
    uint32_t inode_size; //bytes, not log
    uint32_t nb_groups;
    struct VFS_Ext2BlockGroup *groups; //dynamically allocated array;
    uint32_t part_start_lba;
};




struct Directory {
    struct VFS_Inode *inode;
    int (*readdir)(struct VFS_Inode *inode, readdir_cb cb, void *p);
};

struct File {
    struct VFS_Inode *inode;
    off_t offset;
    void (*free)(struct VFS_Inode **inode);
    int (*close)(struct File **file);
    int (*read)(struct File *file, char *dst, int len);
    // int (*write)(struct File *file, char *dst, int len);
    int (*lseek)(struct File *file, off_t offset);
    // int (*mmap)(struct File *file, void *addr);
};


struct Ext2Filesystem {
    struct VFS_Ext2SuperBlock *sb;
    struct File *(*open)(struct Ext2Filesystem *this, uint32_t inode_num);
    // struct File *(*open_path)(const char *path);
};


struct VFS_SuperBlock *ext2_probe(struct BlockDev *dev);
void get_inode_block_data(struct VFS_Inode *inode, uint64_t block_index, void *dst);
void test_readdir(struct VFS_SuperBlock *sb);
void test_file_read(struct Ext2Filesystem *fs);

struct Ext2Filesystem *Ext2Filesystem_new();







#endif
