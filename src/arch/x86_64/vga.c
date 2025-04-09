#include "vga.h"
#include "memory.h"

#define VGA_BASE 0xb8000
#define LINE(X) ((X) / 80)
#define BG(COLOR) ((COLOR) << 4)
#define FG(COLOR) COLOR

#define VGA_BLACK 0
#define VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_WHITE 7
#define VGA_GRAY 8
#define VGA_LIGHT_BLUE 9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN 11
#define VGA_LIGHT_RED 12
#define VGA_LIGHT_MAGENTA 13
#define VGA_YELLOW 14
#define VGA_BRIGHT_WHITE 15


static unsigned short *vgaBuff = (unsigned short *)VGA_BASE;
static int width = 80;
static int height = 25;
static int cursor = 0;
static unsigned char color = FG(VGA_WHITE) | BG(VGA_BLACK);




void scroll() {
	size_t i;
	for (i=0; i<height-1; i++)
		memcpy(vgaBuff + i * width, vgaBuff + (i+1) * width, width * sizeof(unsigned short));
	memset(vgaBuff + i * width, FG(VGA_BLACK) | BG(VGA_BLACK), width * sizeof(unsigned short));
	cursor = (height - 1) * width;
}

void VGA_display_char(char c) {
	if (c == '\n') {
		cursor = (LINE(cursor) + 1) * width;
	}
	else if (c == '\r')
		cursor = LINE(cursor);
	else {
		vgaBuff[cursor] = (color << 8) | c;
		cursor++;
	}
	if (cursor >= width * height)
		scroll();
}

void VGA_clear(void) {
	memset(vgaBuff, FG(VGA_BLACK) | BG(VGA_BLACK), height * width * sizeof(unsigned short));
	cursor = 0;
}

void VGA_display_str(const char *s) {
	while (*s) {
		VGA_display_char(*s);
		s++;
	}
}