#include "stdio.h"
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "string.h"
#include "io.h"

size_t row = 0;
size_t column = 0;
uint8_t color = ECOLOR(LIGHT_GREY, BLACK);
uint16_t* buffer = (uint16_t*)0xB8000;

void scroll() 
{
    if (row >= 25) 
    {
        uint16_t *dst = buffer;
        uint16_t *src = buffer + 80;
        while (src < buffer + 2000) *dst++ = *src++;
        while (dst < buffer + 2000) *dst++ = ((uint16_t)color << 8) | ' ';
        row = 24;
        set_cursor(column, row);
    }
}

void putc(char c) 
{
    if (c == '\n') 
    {
        column = 0;
        row++;
    } 
    else 
    {
        if (row >= 25) scroll();
        buffer[row * 80 + column] = ((uint16_t)color << 8) | (uint8_t)c;
        if (++column >= 80) 
        {
            column = 0;
            row++;
        }
    }
    if (row >= 25) scroll();
    set_cursor(column, row);
}

void set_cursor(uint8_t x, uint8_t y) 
{
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void get_cursor(uint8_t *x, uint8_t *y) 
{
    outb(0x3D4, 0x0F);
    uint16_t pos = inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    *x = pos % 80;
    *y = pos / 80;
}

void printf(const char *fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
    while (*fmt) 
    {
        if (*fmt == '%') 
        {
            fmt++;
            switch (*fmt) 
            {
                case 'c': 
                    putc((char)va_arg(args, int));
                    break;
                case 'd': 
                {
                    int n = va_arg(args, int);
                    unsigned int u = n;
                    if (n < 0) 
                    {
                        putc('-');
                        u = -n;
                    }
                    char b[10];
                    char *p = b;
                    do 
                    {
                        *p++ = (u % 10) + '0';
                        u /= 10;
                    } while (u > 0);
                    while (p > b) putc(*--p);
                    break;
                }
                case 'x': 
                {
                    unsigned int u = va_arg(args, unsigned int);
                    char b[9];
                    char *p = b;
                    do 
                    {
                        unsigned int d = u % 16;
                        *p++ = (d < 10) ? (d + '0') : (d - 10 + 'a');
                        u /= 16;
                    } while (u > 0);
                    while (p > b) putc(*--p);
                    break;
                }
                case 'C':
                    color = (uint8_t)va_arg(args, int);
                    break;
                case 's': 
                {
                    const char *s = va_arg(args, const char*);
                    while (*s) putc(*s++);
                    break;
                }
                default:
                    putc(*fmt);
                    break;
            }
        } 
        else 
        {
            putc(*fmt);
        }
        fmt++;
    }
    va_end(args);
}