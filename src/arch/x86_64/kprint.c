#include <stdarg.h>
#include "string.h"
#include "vga.h"

#define HEX 16
#define DEC 10
#define SIGNED 1
#define UNSIGNED 0


#define BUFSIZ 24 /*enough for any representable integral number*/


#define PRINT_NUM(TYPE, SUFFIX, HAS_SIGN)       \
int print_##SUFFIX(TYPE x, int base)            \
{                                               \
    if (x == 0) {                               \
        VGA_display_str("0", INFO);             \
        return 1;                               \
    }                                           \
    char buf[BUFSIZ] = {0};                     \
    char digit=0;                               \
    unsigned long num = (unsigned long)x;       \
    if (HAS_SIGN && x < 0)                      \
        num = ~num + 1;                         \
    size_t j=0;                                 \
    while (num > 0) {                           \
        digit = num % base;                     \
        if (base == HEX && digit > 9)           \
            digit = digit + 'a'- 10;            \
        else                                    \
            digit += '0';                       \
        buf[j++] = digit;                       \
        num /= base;                            \
    }                                           \
    if (HAS_SIGN && x < 0)                      \
        buf[j++] = '-';                         \
    buf[j] = '\0';                              \
    strreverse(buf);                            \
    VGA_display_str(buf, INFO);                 \
    return strlen(buf);                         \
}         


PRINT_NUM(short, short, SIGNED)
PRINT_NUM(int, int, SIGNED)
PRINT_NUM(long, long, SIGNED)
PRINT_NUM(long long, llong, SIGNED)

PRINT_NUM(unsigned short, ushort, UNSIGNED)
PRINT_NUM(unsigned int, uint, UNSIGNED)
PRINT_NUM(unsigned long, ulong, UNSIGNED)
PRINT_NUM(unsigned long long, ullong, UNSIGNED)

void print_char(char);
int print_str(const char *);
void print_error(const char *);


int kprintf(const char *fmt, ...) {
    va_list argptr;
    va_start(argptr, fmt);
    int count = 0;
    while (*fmt) {
        if (*fmt == '%') {
            switch (*(fmt+1)) {
                case '%':
                    print_char(*fmt);
                    count++;
                    fmt += 2;
                    break;
                case 's':
                    count += print_str(va_arg(argptr, char *));
                    fmt += 2;
                    break;
                case 'd':
                    count += print_int(va_arg(argptr, int), DEC);
                    fmt += 2;
                    break;
                case 'u':
                    count += print_uint(va_arg(argptr, unsigned int), DEC);
                    fmt += 2;
                    break;
                case 'x':
                    count += print_uint(va_arg(argptr, unsigned int), HEX);
                    fmt += 2;
                    break;
                case 'c':
                    print_char((char)va_arg(argptr, int));
                    count++;
                    fmt += 2;
                    break;
                case 'p':
                    count += print_ulong((unsigned long)va_arg(argptr, void *), HEX);
                    fmt += 2;
                    break;

                // short
                case 'h':
                    switch (*(fmt + 2)) {
                        case 'd':
                            count += print_short((short)va_arg(argptr, int), DEC);
                            fmt += 3;
                            break;
                        case 'u':
                            count += print_ushort((unsigned short)va_arg(argptr, unsigned int), DEC);
                            fmt += 3;
                            break;
                        case 'x':
                            count += print_ushort((unsigned short)va_arg(argptr, unsigned int), HEX);
                            fmt += 3;
                            break;
                        default:
                            // Error message?
                            print_error("\nERROR: Unsupported format specifier %h\n");
                            fmt += 2;
                            break;
                    }
                    break;
                case 'l':
                    switch (*(fmt + 2)) {
                        case 'd':
                            count += print_long(va_arg(argptr, long), DEC);
                            fmt += 3;
                            break;
                        case 'u':
                            count += print_ulong(va_arg(argptr, unsigned long), DEC);
                            fmt += 3;
                            break;
                        case 'x':
                            count += print_ulong(va_arg(argptr, unsigned long), HEX);
                            fmt += 3;
                            break;
                        default:
                            fmt += 2;
                            // Error message?
                            print_error("\nERROR: Unsupported format specifier %l\n");
                            break;
                    }
                    break;
                case 'q':
                    switch (*(fmt + 2)) {
                        case 'd':
                            count += print_llong(va_arg(argptr, long long), DEC);
                            fmt += 3;
                            break;
                        case 'u':
                            count += print_ullong(va_arg(argptr, unsigned long long), DEC);
                            fmt += 3;
                            break;
                        case 'x':
                            count += print_ullong(va_arg(argptr, unsigned long long), HEX);
                            fmt += 3;
                            break;
                        default:
                            fmt += 2;
                            // Error message?
                            print_error("\nERROR: Unsupported format specifier %q\n");
                            break;
                    }
                    break;
                default:
                    fmt += 2;
                    print_error("\nERROR: Unsupported format specifier\n");
                    break;
            }
        } else { 
            print_char(*fmt++);
            count++;
        }
    }
    va_end(argptr);
    return count;
}


void print_error(const char *str) {
    VGA_display_str(str, ERROR);
}


void print_char(char c) {
    VGA_display_char(c, FG(VGA_WHITE) | BG(VGA_BLACK));
}

int print_str(const char *str) {
    VGA_display_str(str, INFO);
    return strlen(str);
}
