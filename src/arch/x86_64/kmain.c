#include "vga.h"



void kmain(void) {
	VGA_clear();	



	char s1[] = "Yo dude!\n";
	char s2[] = "Sup?\n";

	for (int i=0; i<35; i++) {
		VGA_display_str(s1);
	
		for (long long i=0; i<99999999; i++);

		VGA_display_str(s2);

		for (long long i=0; i<99999999; i++);
	}


	VGA_clear();

	char s[] = "Turtles";
	for (int i=0; i<400; i++) {
		VGA_display_str(s);
		for (long long i=0; i<9999999; i++);
	}
	VGA_clear();
	
	while (1) {
		__asm__ volatile ("hlt");
	}
}
