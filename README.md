# TurtOs

A practice kernel for x86_64

## Purpose

This is a kernel for x86_64, created for practice and exploration of operating system implementation strategies.  It is experimental: a work in progress, developed using a QEMU emulator.  Nevertheless, it has many essential features including

* GDT, IDT, TSS
* UART serial driver with circular buffer 
* context switching, ISRs, cooperative multitasking
* round robin scheduling
* interrupt-driven keyboard
* 4-level page table with on demand paging
* heap allocator
* block device interface
* ATA driver for Ext2 file read
* ...to be continued

## Installation

Requires an appropriate cross-compiler as well as QEMU.  My compiler and linker were found in ```~/opt/cross/bin/``` and the Makefile is hardcoded as such.

Then ```make run```.
  
