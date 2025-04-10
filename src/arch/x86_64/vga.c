#include "vga.h"
#include "memory.h"

#define VGA_BASE 0xb8000
#define LINE(X) ((X) / 80)


static unsigned short *vgaBuff = (unsigned short *)VGA_BASE;
static int width = 80;
static int height = 25;
static int cursor = 0;
static unsigned char color = FG(VGA_WHITE) | BG(VGA_BLACK);
static unsigned char warn_color = FG(VGA_YELLOW) | BG(VGA_BLACK);
static unsigned char error_color = FG(VGA_LIGHT_RED) | BG(VGA_BLACK);
static unsigned char debug_color = FG(VGA_LIGHT_GREEN) | BG(VGA_BLACK);






void scroll() {
	size_t i;
	for (i=0; i<height-1; i++)
		memcpy(vgaBuff + i * width, vgaBuff + (i+1) * width, width * sizeof(unsigned short));
	memset(vgaBuff + i * width, FG(VGA_BLACK) | BG(VGA_BLACK), width * sizeof(unsigned short));
	cursor = (height - 1) * width;
}

void VGA_display_char(char c, unsigned char col) {
	if (c == '\n') {
		cursor = (LINE(cursor) + 1) * width;
	}
	else if (c == '\r')
		cursor = LINE(cursor);
	else {
		vgaBuff[cursor] = (col << 8) | c;
		cursor++;
	}
	if (cursor >= width * height)
		scroll();
}

void VGA_clear(void) {
	memset(vgaBuff, FG(VGA_BLACK) | BG(VGA_BLACK), height * width * sizeof(unsigned short));
	cursor = 0;
}

void VGA_display_str(const char *s, int level) {
	unsigned char text_color = 0;
	while (*s) {
		switch (level) {
			case DEBUG:
				text_color = debug_color;
				break;
			case INFO:
				text_color = color;
				break;
			case WARN:
				text_color = warn_color;
				break;
			case ERROR:
				text_color = error_color;
				break;
			default:
				text_color = error_color;
				VGA_display_str("ERROR: Invalid color in VGA_display_str. Printing as error:\n", ERROR);
				break;
		}	
		VGA_display_char(*s, text_color);
		s++;
	}
}