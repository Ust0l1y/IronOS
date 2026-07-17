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

void putc(char c) 
{
    if (c == '\n') 
    {
        column = 0;
        row++;
    } 
    else 
    {
        const size_t index = row * 80 + column;
        buffer[index] = ((uint16_t)color << 8) | (uint8_t)c;
        column++;
        if (column >= 80) 
        {
            column = 0;
            row++;
        }
    }
    set_cursor(column, row);
    scroll();
}

void scroll() 
{
    if (row >= 25) 
    {
        for (size_t r = 1; r < 25; r++) 
        {
            for (size_t c = 0; c < 80; c++) 
            {
                buffer[(r - 1) * 80 + c] = buffer[r * 80 + c];
            }
        }
        for (size_t c = 0; c < 80; c++) 
        {
            buffer[24 * 80 + c] = ((uint16_t)color << 8) | ' ';
        }
        row = 24;
    }
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

void printf(const char *format, ...) 
{
    va_list args;
    va_start(args, format);
    for (size_t i = 0; i < strlen(format); i++) 
    {
        if (format[i] == '%') 
        {
            i++;
            switch (format[i]) 
            {
                case 'c': 
                {
                    char c = (char)va_arg(args, int);
                    putc(c);
                    break;
                }
                case 'd': 
                {
                    int num = va_arg(args, int);
                    if (num < 0) 
                    {
                        putc('-');
                        num = -num;
                    }
                    char buffer[10];
                    size_t j = 0;
                    do 
                    {
                        buffer[j++] = (num % 10) + '0';
                        num /= 10;
                    } while (num > 0);
                    for (size_t k = 0; k < j / 2; k++) 
                    {
                        char temp = buffer[k];
                        buffer[k] = buffer[j - k - 1];
                        buffer[j - k - 1] = temp;
                    }
                    for (size_t k = 0; k < j; k++) 
                    {
                        putc(buffer[k]);
                    }
                    break;
                }
                case 'x': 
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char buffer[9];
                    size_t j = 0;
                    do 
                    {
                        unsigned int digit = num % 16;
                        buffer[j++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
                        num /= 16;
                    } while (num > 0);
                    for (size_t k = 0; k < j / 2; k++) 
                    {
                        char temp = buffer[k];
                        buffer[k] = buffer[j - k - 1];
                        buffer[j - k - 1] = temp;
                    }
                    for (size_t k = 0; k < j; k++) 
                    {
                        putc(buffer[k]);
                    }
                    break;
                }
                case 'C':
                {
                    uint8_t new_color = (uint8_t)va_arg(args, int);
                    color = ECOLOR(new_color, BLACK);
                    break;
                }
                case 's': 
                {
                    const char* str = va_arg(args, const char*);
                    for (size_t j = 0; j < strlen(str); j++) 
                    {
                        putc(str[j]);
                    }
                    break;
                }
                default:
                    putc(format[i]);
                    break;
            }
        } 
        else 
        {
            putc(format[i]);
        }
    }
    va_end(args);
}