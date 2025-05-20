#! /bin/bash
BLOCK_SIZE=512
NB_BLOCKS=32648 #16MB in powers of 2
dd if=/dev/zero of="$1" bs="$BLOCK_SIZE" count="$NB_BLOCKS"
parted build/grub_no_kernel.img mklabel msdos

max_loopback=$(losetup | grep -o '/dev/loop[0-9]\+' | sed 's/\/dev\/loop//' | sort -n | tail -1)
N0=$(($max_loopback+1))
sudo losetup /dev/loop"$N0" $1
sudo losetup /dev/loop"$N1" $1 -o 1048576

sudo /home/paul/opt/cross/sbin/grub-install --root-directory=/build/ --target=i386-pc --no-floppy --modules="normal part_msdos ext2 multiboot2" /dev/loop"$N0"

