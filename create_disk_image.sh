#! /bin/bash
# $1 should contain the desired path of the ext2 disk image

# set -e
# set -x

BLOCK_SIZE=512
NB_BLOCKS=32648 #16MB in powers of 2
dd if=/dev/zero of="$1" bs="$BLOCK_SIZE" count="$NB_BLOCKS"
parted $1 mklabel msdos
parted $1 mkpart primary ext2 2048s 30720s
parted $1 set 1 boot on
max_loopback=$(losetup | grep -o '/dev/loop[0-9]\+' | sed 's/\/dev\/loop//' | sort -n | tail -1)
N0=$(($max_loopback+1))
N1=$(($max_loopback+2))
sudo losetup /dev/loop"$N0" $1
sudo losetup /dev/loop"$N1" $1 -o 1048576
sudo mkfs.ext2 /dev/loop"$N1"
mkdir -p /mnt/osfiles
sudo mount /dev/loop"$N1" /mnt/osfiles
sudo /home/paul/opt/cross/sbin/grub-install --root-directory=/mnt/osfiles --target=i386-pc --no-floppy --modules="normal part_msdos ext2 multiboot2" /dev/loop"$N0"
sudo cp -r $2 /mnt/osfiles
sudo umount /mnt/osfiles
sudo losetup -d /dev/loop"$N0"
sudo losetup -d /dev/loop"$N1"
