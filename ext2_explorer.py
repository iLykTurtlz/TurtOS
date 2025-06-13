#!/usr/bin/env python3

import argparse
import stat
import struct
from dataclasses import dataclass
from datetime import datetime

# --- Constants ---

# MBR
MBR_OFFSET = 0
PARTITION_TABLE_OFFSET = 446  # 0x1BE
PARTITION_ENTRY_SIZE = 16
NUM_PARTITION_ENTRIES = 4
LINUX_PARTITION_TYPE = 0x83
SECTOR_SIZE = 512

# ext2
SUPERBLOCK_PARTITION_OFFSET = 1024
SUPERBLOCK_READ_SIZE = 1024  # Read the entire standard superblock block
SUPERBLOCK_MAGIC = 0xEF53
BLOCK_GROUP_DESCRIPTOR_SIZE = 32
INODE_ROOT_DIRECTORY = 2
EXT2_GOOD_OLD_REV = 0
EXT2_DYNAMIC_REV = 1
EXT2_GOOD_OLD_INODE_SIZE = 128


# --- Dataclasses for Filesystem Structures ---


@dataclass
class PartitionEntry:
    """Represents a single MBR partition entry."""

    status: int
    type: int
    start_lba: int
    num_sectors: int


@dataclass
class Superblock:
    """Represents the parsed ext2 superblock."""

    total_inodes: int
    total_blocks: int
    reserved_blocks: int
    free_blocks: int
    free_inodes: int
    first_data_block: int
    block_size: int
    fragment_size: int
    blocks_per_group: int
    fragments_per_group: int
    inodes_per_group: int
    mount_time: datetime
    write_time: datetime
    mount_count: int
    max_mount_count: int
    magic: int
    state: int
    errors: int
    minor_revision: int
    last_check: datetime
    check_interval: int
    creator_os: int
    revision_level: int
    default_resuid: int
    default_resgid: int
    # --- EXT2_DYNAMIC_REV Specific ---
    first_inode: int
    inode_size: int
    volume_name: str


@dataclass
class BlockGroupDescriptor:
    """Represents an ext2 Block Group Descriptor."""

    block_bitmap: int
    inode_bitmap: int
    inode_table: int
    free_blocks_count: int
    free_inodes_count: int
    used_dirs_count: int


@dataclass
class Inode:
    """Represents a parsed ext2 Inode."""

    mode: int
    uid: int
    size: int
    access_time: datetime
    change_time: datetime
    modification_time: datetime
    deletion_time: datetime
    gid: int
    links_count: int
    blocks: int
    flags: int
    block_pointers: list[int]  # Array of 15 block pointers


# --- Parsing Functions ---


def find_ext2_partition(disk_image_path: str) -> PartitionEntry | None:
    """Parses the MBR of a disk image to find the first Linux partition."""
    print(f"[*] Reading MBR from '{disk_image_path}'...")
    with open(disk_image_path, "rb") as f:
        f.seek(PARTITION_TABLE_OFFSET)
        partition_table_data = f.read(
            PARTITION_ENTRY_SIZE * NUM_PARTITION_ENTRIES
        )

        for i in range(NUM_PARTITION_ENTRIES):
            entry_start = i * PARTITION_ENTRY_SIZE
            entry_data = partition_table_data[
                entry_start : entry_start + PARTITION_ENTRY_SIZE
            ]
            status, p_type, start_lba, num_sectors = struct.unpack(
                "<B3xB3xII", entry_data
            )

            if p_type == LINUX_PARTITION_TYPE:
                print(
                    f"[+] Found Linux partition (type 0x{p_type:02x}) at entry {i+1}"
                )
                return PartitionEntry(
                    status=status,
                    type=p_type,
                    start_lba=start_lba,
                    num_sectors=num_sectors,
                )
    return None


def parse_superblock(data: bytes) -> Superblock:
    """Parses the raw 1024-byte data of an ext2 superblock."""
    # This format string parses the first 90 bytes of the superblock
    fmt = "<IIIIIIIiIIIIIHHHHHHIIIIHH" + "IH"
    # Use unpack_from to parse from the larger buffer at offset 0
    fields = struct.unpack_from(fmt, data, 0)

    # Parse volume name from its fixed offset
    volume_name_bytes = struct.unpack_from("<16s", data, 120)[0]
    volume_name = volume_name_bytes.decode("latin-1").strip("\x00")

    log_block_size = fields[6]
    block_size = 1024 << log_block_size

    log_frag_size = fields[7]
    fragment_size = (
        (1024 << log_frag_size)
        if log_frag_size >= 0
        else (1024 >> -log_frag_size)
    )

    return Superblock(
        total_inodes=fields[0],
        total_blocks=fields[1],
        reserved_blocks=fields[2],
        free_blocks=fields[3],
        free_inodes=fields[4],
        first_data_block=fields[5],
        block_size=block_size,
        fragment_size=fragment_size,
        blocks_per_group=fields[8],
        fragments_per_group=fields[9],
        inodes_per_group=fields[10],
        mount_time=datetime.fromtimestamp(fields[11]),
        write_time=datetime.fromtimestamp(fields[12]),
        mount_count=fields[13],
        max_mount_count=fields[14],
        magic=fields[15],
        state=fields[16],
        errors=fields[17],
        minor_revision=fields[18],
        last_check=datetime.fromtimestamp(fields[19]),
        check_interval=fields[20],
        creator_os=fields[21],
        revision_level=fields[22],
        default_resuid=fields[23],
        default_resgid=fields[24],
        first_inode=fields[25],
        inode_size=fields[26],
        volume_name=volume_name,
    )


def parse_block_group_descriptor(data: bytes) -> BlockGroupDescriptor:
    """Parses a 32-byte block group descriptor."""
    (
        block_bitmap,
        inode_bitmap,
        inode_table,
        free_blocks_count,
        free_inodes_count,
        used_dirs_count,
    ) = struct.unpack("<IIIHHH14x", data)
    return BlockGroupDescriptor(
        block_bitmap,
        inode_bitmap,
        inode_table,
        free_blocks_count,
        free_inodes_count,
        used_dirs_count,
    )


def parse_inode(data: bytes) -> Inode:
    """Parses raw inode data into an Inode object."""
    fmt = "<HHIiIIIIHHII"
    base_fields = struct.unpack_from(fmt, data, 0)
    block_pointers = list(struct.unpack_from("<15I", data, 40))

    return Inode(
        mode=base_fields[0],
        uid=base_fields[1],
        size=base_fields[2],
        access_time=datetime.fromtimestamp(base_fields[3]),
        change_time=datetime.fromtimestamp(base_fields[4]),
        modification_time=datetime.fromtimestamp(base_fields[5]),
        deletion_time=datetime.fromtimestamp(base_fields[6]),
        gid=base_fields[7],
        links_count=base_fields[8],
        blocks=base_fields[9],
        flags=base_fields[10],
        block_pointers=block_pointers,
    )


# --- Logic and Printing Functions ---


def get_inode_size(sb: Superblock) -> int:
    """Determines inode size based on filesystem revision."""
    if sb.revision_level == EXT2_DYNAMIC_REV and sb.inode_size > 0:
        return sb.inode_size
    return EXT2_GOOD_OLD_INODE_SIZE


def find_and_parse_inode(
    disk_image_path: str,
    partition: PartitionEntry,
    sb: Superblock,
    inode_num: int,
):
    """Finds, reads, and parses a specific inode from the filesystem."""
    if not (1 <= inode_num <= sb.total_inodes):
        print(
            f"\n[!] Error: Inode number {inode_num} is out of range. "
            f"Valid range is 1 to {sb.total_inodes}."
        )
        return

    print(f"\n[*] Attempting to look up Inode {inode_num}...")

    partition_start_byte = partition.start_lba * SECTOR_SIZE
    inode_size = get_inode_size(sb)

    group_index = (inode_num - 1) // sb.inodes_per_group
    print(f"[*] Inode {inode_num} is in Block Group {group_index}")

    bgdt_start_block = sb.first_data_block + 1
    bgdt_start_byte = partition_start_byte + (bgdt_start_block * sb.block_size)
    descriptor_offset = bgdt_start_byte + (
        group_index * BLOCK_GROUP_DESCRIPTOR_SIZE
    )
    print(f"[*] Block Group Descriptor Table starts at byte {bgdt_start_byte}")

    with open(disk_image_path, "rb") as f:
        f.seek(descriptor_offset)
        bgd_data = f.read(BLOCK_GROUP_DESCRIPTOR_SIZE)
        bgd = parse_block_group_descriptor(bgd_data)
        print(f"[+] Inode Table for this group starts at block {bgd.inode_table}")

        local_inode_index = (inode_num - 1) % sb.inodes_per_group
        inode_table_start_byte = partition_start_byte + (
            bgd.inode_table * sb.block_size
        )
        print(f"[*] Inode Table starts at byte {inode_table_start_byte}")
        inode_offset_in_table = local_inode_index * inode_size
        final_inode_offset = inode_table_start_byte + inode_offset_in_table

        print(f"[*] Calculated Inode offset: {final_inode_offset} bytes")
        f.seek(final_inode_offset)
        inode_data = f.read(inode_size)

    inode = parse_inode(inode_data)
    print_inode_info(inode, inode_num)


def print_superblock_info(sb: Superblock):
    """Prints the superblock information in a formatted way."""
    print("\n--- ext2 Superblock Details ---")
    print(f"Volume Name:            {sb.volume_name}")
    print(f"Magic Number:           {hex(sb.magic)} (Correct: 0xEF53)")
    print("-" * 31)
    print(f"Total Inodes:           {sb.total_inodes:,}")
    print(f"Total Blocks:           {sb.total_blocks:,}")
    print(f"Block Size:             {sb.block_size} bytes")
    print(f"Inode Size:             {get_inode_size(sb)} bytes")
    print(f"Free Blocks:            {sb.free_blocks:,}")
    print(f"Free Inodes:            {sb.free_inodes:,}")
    print("-" * 31)
    print(f"Blocks per Group:       {sb.blocks_per_group:,}")
    print(f"Inodes per Group:       {sb.inodes_per_group:,}")
    print("-" * 31)
    print(f"Last Mount Time:        {sb.mount_time}")
    print(f"Last Write Time:        {sb.write_time}")
    print(f"Last Check Time:        {sb.last_check}")
    print("--- End of Superblock ---")


def print_inode_info(inode: Inode, inode_num: int):
    """Prints the inode information in a formatted way."""

    def get_file_type(mode: int) -> str:
        if stat.S_ISDIR(mode):
            return "Directory"
        if stat.S_ISREG(mode):
            return "Regular File"
        if stat.S_ISLNK(mode):
            return "Symbolic Link"
        if stat.S_ISCHR(mode):
            return "Character Device"
        if stat.S_ISBLK(mode):
            return "Block Device"
        if stat.S_ISFIFO(mode):
            return "FIFO/Pipe"
        if stat.S_ISSOCK(mode):
            return "Socket"
        return "Unknown"

    print(f"\n--- Inode {inode_num} Details ---")
    print(f"File Type:              {get_file_type(inode.mode)}")
    print(f"Permissions:            {stat.filemode(inode.mode)}")
    print(f"UID/GID:                {inode.uid}/{inode.gid}")
    print(f"File Size:              {inode.size:,} bytes")
    print(f"Link Count:             {inode.links_count}")
    print(f"Blocks Allocated:       {inode.blocks} (512-byte blocks)")
    print("-" * 31)
    print(f"Modification Time:      {inode.modification_time}")
    print(f"Access Time:            {inode.access_time}")
    print(f"Change Time (metadata): {inode.change_time}")
    print("-" * 31)
    print("Block Pointers:")
    for i, ptr in enumerate(inode.block_pointers[:12]):
        if ptr != 0:
            print(f"  Direct[{i:02d}]:           Block {ptr}")
    if inode.block_pointers[12] != 0:
        print(f"  Single Indirect:      Block {inode.block_pointers[12]}")
    if inode.block_pointers[13] != 0:
        print(f"  Double Indirect:      Block {inode.block_pointers[13]}")
    if inode.block_pointers[14] != 0:
        print(f"  Triple Indirect:      Block {inode.block_pointers[14]}")
    print("--- End of Inode ---")


def main():
    """Main function to drive the script."""
    parser = argparse.ArgumentParser(
        description="An ext2 filesystem explorer to parse the superblock and inodes.",
        formatter_class=argparse.RawTextHelpFormatter,
    )
    parser.add_argument(
        "disk_image", help="Path to the MBR-partitioned disk image file."
    )
    parser.add_argument(
        "--inode",
        type=int,
        help="Optional: The inode number to look up and display.",
    )
    args = parser.parse_args()

    try:
        partition = find_ext2_partition(args.disk_image)
        if not partition:
            print("\n[!] Could not find a Linux (type 0x83) partition.")
            return

        partition_start_byte = partition.start_lba * SECTOR_SIZE
        superblock_abs_offset = (
            partition_start_byte + SUPERBLOCK_PARTITION_OFFSET
        )

        with open(args.disk_image, "rb") as f:
            f.seek(superblock_abs_offset)
            # Read the whole 1024-byte block, as per the better approach
            superblock_data = f.read(SUPERBLOCK_READ_SIZE)

        if len(superblock_data) < SUPERBLOCK_READ_SIZE:
            print(
                f"\n[!] Error: Could not read full superblock. "
                f"Expected {SUPERBLOCK_READ_SIZE}, got {len(superblock_data)}."
            )
            return

        sb = parse_superblock(superblock_data)

        if sb.magic != SUPERBLOCK_MAGIC:
            print(
                f"\n[!] Error: Invalid ext2 magic number: {hex(sb.magic)}."
            )
            return

        print_superblock_info(sb)

        if args.inode:
            find_and_parse_inode(args.disk_image, partition, sb, args.inode)

    except FileNotFoundError:
        print(f"[!] Error: The file '{args.disk_image}' was not found.")
    except Exception as e:
        print(f"[!] An unexpected error occurred: {e}")


if __name__ == "__main__":
    main()
