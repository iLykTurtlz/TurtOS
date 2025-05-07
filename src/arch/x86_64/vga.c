#include "vga.h"
#include "memory.h"
#include "irq.h"

#define TAB_SIZE 4

#define VGA_BASE 0xb8000
#define LINE(X) ((X) / 80)
#define COL(X) ((X) % 80)
#define BLINK 0x80

static unsigned short *const vgaBuff = (unsigned short *)VGA_BASE;
static const int width = 80;
static const int height = 25;
static int cursor = 0;
static const unsigned char color = FG(VGA_WHITE) | BG(VGA_BLACK);
static const unsigned char warn_color = FG(VGA_YELLOW) | BG(VGA_BLACK);
static const unsigned char error_color = FG(VGA_LIGHT_RED) | BG(VGA_BLACK);
static const unsigned char debug_color = FG(VGA_LIGHT_GREEN) | BG(VGA_BLACK);
static const unsigned short empty = ((FG(VGA_BLACK) | BG(VGA_BLACK)) << 8) | '\0';
static const unsigned short blinking_cursor = ((BLINK | color) << 8) | ' ';
static unsigned short underneath = empty;


enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3,
};



// VGA_move_cursor(int direction) {
//     switch (direction) {
//     case 
//     }
// }

void VGA_clear(void)
{
    memset(vgaBuff, (char)empty, height * width * sizeof(unsigned short));
    cursor = 0;
    vgaBuff[cursor] = blinking_cursor;
}


void scroll()
{
    size_t i;
    for (i = 0; i < height - 1; i++)
    {
        memcpy(vgaBuff + i * width, vgaBuff + (i + 1) * width, width * sizeof(unsigned short));
    }
    memset(vgaBuff + i * width, (char)empty, width * sizeof(unsigned short));
    cursor = i * width;
}

void VGA_display_char(char c, unsigned char col)
{
    int enable_ints = 0;
    if (interrupts_enabled()) {
        enable_ints = 1;
        CLI;
    }

    static int previous_tab = 0;
    if (c == '\n')
    {
        if (previous_tab)
        {
            VGA_display_char(' ', col);
            previous_tab = 0;
        }
        vgaBuff[cursor] = empty;
        cursor = (LINE(cursor) + 1) * width;
    }
    else if (c == '\r')
    {
        vgaBuff[cursor] = empty;
        cursor = LINE(cursor) * width;
        previous_tab = 0;
    }
    else if (c == '\t')
    {
        VGA_display_char(' ', col);
        for (int i = 0; i < TAB_SIZE - 1; i++)
        {
            VGA_display_char('\0', FG(VGA_BLACK) | BG(VGA_BLACK));
        }
        previous_tab = 1;
    }
    else
    {
        vgaBuff[cursor] = (col << 8) | c;
        cursor++;
        previous_tab = 0;
    }
    if (cursor >= width * height)
        scroll();
    vgaBuff[cursor] = blinking_cursor;

    if (enable_ints)
        STI;
}



void VGA_display_str(const char *s, int level)
{
    unsigned char text_color = 0;
    switch (level)
    {
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
        VGA_display_str("\nERROR: Invalid color in VGA_display_str. Printing as error:\n", ERROR);
    }
    while (*s)
    {
        VGA_display_char(*s, text_color);
        s++;
    }
}

void VGA_backspace(void)
{
    int start_line, end_line;
    start_line = LINE(cursor);
    vgaBuff[cursor] = underneath;
    if (cursor > 0)
        cursor--;

    // find previous nonempty (which could be a space) or else 0
    while (cursor > 0 && vgaBuff[cursor] == empty)
    {
        cursor--;
    }
    end_line = LINE(cursor);

    // same line or end of line -> erase the last character
    if (start_line == end_line || (cursor + 1) % width == 0)
    {
        vgaBuff[cursor] = blinking_cursor;
    }

    // more than a line apart or nothing above -> go to start of previous line
    else if (start_line - end_line > 1 || vgaBuff[cursor] == empty)
    {
        cursor = (start_line - 1) * width;
        vgaBuff[cursor] = blinking_cursor;
    }

    // tab case
    else if (vgaBuff[cursor] == ((color << 8) | ' '))
        vgaBuff[cursor] = blinking_cursor;

    // otherwise erase the invisible, unwritten '\n'
    else
    {
        vgaBuff[++cursor] = blinking_cursor;
    }
}
