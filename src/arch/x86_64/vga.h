#ifndef VGA_H
#define VGA_H

#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3

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

void VGA_clear(void);
void VGA_display_char(char, unsigned char);
void VGA_display_str(const char*, int);
void VGA_backspace(void);


#endif
