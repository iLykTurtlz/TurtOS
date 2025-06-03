#include "vga.h"
#include <limits.h>
#include <stdint.h>
#include "kprint.h"
#include "string.h"
#include "keyboard.h"
#include "idt.h"
#include "irq.h"
#include "tss.h"
#include "serial.h"
#include "parse_multiboot.h"
#include "virt_memory.h"
#include "phys_memory.h"
#include "kmalloc.h"
#include "context_switch.h"
#include "snakes.h"



void kmain(struct fixed_header *multiboot2_start)
{

	CLI;
	
#ifdef debug
	int j;
	j=1;
	while(j);
#endif
	
	parse_multiboot(multiboot2_start);
	MMU_init();

	
	IDT_init();
	IRQ_init(); //sets mask for each PIC interrupt
	
	
	tss_init();

	// kmalloc requires page fault handling, i.e. a tss
	kmalloc_init();
	
	serial_init();
	// serial_keyboard_init();
	keyboard_init(); //unmasks 1

	PROC_init();
	
	// int nums[5] = {0,1,2,3,4};
	// PROC_create_kthread(test_entry, &nums[0]);
	// PROC_create_kthread(test_entry, &nums[1]);
	// PROC_create_kthread(test_entry, &nums[2]);
	// PROC_create_kthread(test_entry, &nums[3]);
	// PROC_create_kthread(test_entry, &nums[4]);

	STI;

	// test_paging(); //PAGING TEST
	// test_kmalloc();



  
	// this test only works for 4K identity map pages
	// kprintf("Result of 1-1 mapping: 0x1234567: %lx\n", test_page_table(0x1234567));

	// parse_multiboot(multiboot2_start);
	// small_test_MMU();
	// test_MMU();
	// int *p = MMU_alloc_page();
	// kprintf("p = %p\n", p);
	// kprintf("test: %lx\n", test_page_table((uint64_t)p));
	// kprintf("*p = %d\n", *p);

	// p[0] = 5;
	// kprintf("p[0] = %d\n", p[0]);

	// test_MMU();
	// stress_test_MMU();
	
	// stress_test_MMU();

	// turns out keyboard_init needs to be last
	

	// IRQ_set_mask(0);
	
	// for (int i=0; i<15; i++) {
	// 	if (i == 1 || i == 4) { //COM1 is IRQ4, PS2 is IRQ1
	// 		IRQ_clear_mask(i);
	// 	} else {
	// 		IRQ_set_mask(i);
	// 	}
	// }
	// STI;

	// char *words = "dfgjklsdfghjkladhsjfkhasdklfjklahsdkf sadfjklsadfjkl";
	// serial_write(words, 20);
	//serial_write('X');


	// unmask here
	// int serial_faulty = serial_init();
	// kprintf("Serial faulty: |%d|\n", serial_faulty);
	// serial_keyboard_init();
	
	
	// serial_write('X');
	
	// serial_write('b');
	


	
	

	// int *big_addr = (int *)-1;
	// *big_addr = 3;


	


	// // __asm__ volatile ("int $0x20");
	// int * silly = (int *)-1;
	// *silly = 3;

	// j=1;
	// while(j);
	// VGA_clear();
	// keyboard_init();
	// poll();
	


	

	// char s[] = "Turtles";
	// for (int i = 0; i < 400; i++)
	// {
	// 	VGA_display_str(s, DEBUG);
	// 	// kprintf("%s",s);
	// 	for (long long i = 0; i < 1999999; i++)
	// 		;
	// }
/*
	int count = 0;
	kprintf("%c\n", 'a');				  // should be "a"
	kprintf("%c\n", 'Q');				  // should be "Q"
	kprintf("%c\n", 256 + '9');			  // Should be "9"
	kprintf("%s\n", "test string");		  // "test string"
	kprintf("foo%sbar\n", "blah");		  // "fooblahbar"
	kprintf("foo%%sbar\n");				  // "foo%bar" SHOULDN'T IT BE foo%sbar??
	kprintf("BEFORE\n");
	kprintf("%d\n", INT_MIN);			  // "-2147483648"
	kprintf("AFTER\n");
	kprintf("%d\n", INT_MAX);			  // "2147483647"
	kprintf("%u\n", 0);					  // "0"
	kprintf("%u\n", UINT_MAX);			  // "4294967295"
	kprintf("%x\n", 0xDEADbeef);		  // "deadbeef"
	kprintf("%p\n", (void *)UINTPTR_MAX); // "0xFFFFFFFFFFFFFFFF"
	kprintf("%hd\n", 0x8000);			  // "-32768"
	kprintf("%hd\n", 0x7FFF);			  // "32767"
	kprintf("%hu\n", 0xFFFF);			  // "65535"
	count = kprintf("%ld\n", LONG_MIN);	  // "-9223372036854775808"
	kprintf("count = %d\n", count);
	kprintf("%ld\n", LONG_MAX);						 // "9223372036854775807"
	kprintf("%lu\n", ULONG_MAX);					 // "18446744073709551615"
	kprintf("%qd\n", (long long)LONG_MIN);			 // "-9223372036854775808"
	kprintf("%qd\n", (long long)LONG_MAX);			 // "9223372036854775807"
	kprintf("%qu\n", (unsigned long long)ULONG_MAX); // "18446744073709551615"

	count = kprintf("%qu, %hd, %s stuffstuffstuff %x\n", (unsigned long long)ULONG_MAX, 0x7FFF, "blah", 0xca11ab1e);
	kprintf("count = %d\n", count);
	
	*/

	// kprintf("%f", 3.14); //WEIRD: it all starts over when this runs!

	// for (long int i=0; i<999999999; i++);
	// kprintf("%lu\n", strlen("stringy"));

	// kprintf("%qe\n", (unsigned long long)ULONG_MAX); error

	/*
	char s[] = "abcdefghijklmnopqrstuvwxyz";
	int count = 0;
	for (int i=1; i<10; i++) {
		s[i] = '\0';
		count = kprintf("%s", s);
		kprintf("%d\n", count);
		count = kprintf("%s", s+(i+1));
		kprintf("%d\n", count);
		s[i] = 'X';
	}
	*/

	// int i=1;
	while (1)
	{
		setup_snakes(1);
		PROC_run();
		// if (i) {
		// 	kprintf("FINISHED FIRST RUN\n");
		// 	PROC_test();
		// 	i=0;
		// }
		// __asm__ volatile("hlt");
	}
}
