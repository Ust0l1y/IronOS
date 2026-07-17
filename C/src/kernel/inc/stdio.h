#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>
enum color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};
#define ECOLOR(fg, bg) ((fg) | ((bg) << 4))
void printf(const char *format, ...);
void putc(char c);
void set_cursor(uint8_t x, uint8_t y);
void get_cursor(uint8_t *x, uint8_t *y);
void scroll();
#endif // STDIO_H