arch ?= x86_64
kernel := build/kernel-$(arch).bin
ext2_img := build/os-$(arch).img
debug_option := -d int
CFLAGS := -g -Wall -Werror -ffreestanding


create_disk_image := ./create_disk_image.sh
linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_src := $(wildcard src/arch/$(arch)/*.asm)
assembly_obj := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%.o, $(assembly_src))
c_src := $(wildcard src/arch/$(arch)/*.c)
c_obj := $(patsubst src/arch/$(arch)/%.c, \
	build/arch/$(arch)/%.o, $(c_src))


.PHONY: all clean run ext2_img debug

all: $(kernel)

clean:
	rm -r build

run: $(ext2_img)
	qemu-system-x86_64 -drive format=raw,file=$(ext2_img)

debug: CFLAGS += -Ddebug
debug: clean $(ext2_img) 
	qemu-system-x86_64 $(debug_option) -s -drive format=raw,file=$(ext2_img)

ext2_img: $(ext2_img)

$(ext2_img): $(kernel) $(grub_cfg)
	mkdir -p .img
	mkdir -p build/ext2/boot/grub
	cp $(kernel) build/ext2/boot/kernel.bin
	cp $(grub_cfg) build/ext2/boot/grub
	$(create_disk_image) $(ext2_img) build/ext2/*

$(kernel): $(assembly_obj) $(c_obj) $(linker_script)
	~/opt/cross/bin/x86_64-elf-ld -g -n -T $(linker_script) -o $(kernel) $(assembly_obj) $(c_obj)


# compile assembly object files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.asm
	mkdir -p $(shell dirname $@)
	nasm -f elf64 -g -F dwarf $< -o $@
	

# compile C object files
build/arch/$(arch)/%.o: src/arch/$(arch)/%.c
	mkdir -p $(shell dirname $@)
	~/opt/cross/bin/x86_64-elf-gcc $(CFLAGS) -c $< -o $@
