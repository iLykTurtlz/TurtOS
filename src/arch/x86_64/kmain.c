#include "vga.h"
#include <limits.h>
#include <stdint.h>
#include "kprint.h"
#include "string.h"
#include "keyboard.h"
#include "idt.h"
#include "irq.h"

void kmain(void)
{
	CLI;
	
#ifdef debug
	int j;
	j=1;
	while(j);
#endif
	idt_init();
	IRQ_init();

	// __asm__ volatile ("int $0x20");
	int * silly = (int *)-1;
	*silly = 3;

	// j=1;
	// while(j);
	VGA_clear();
	keyboard_init();
	poll();
	


	

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

	while (1)
	{
		__asm__ volatile("hlt");
	}
}
