#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct __attribute__((packed)) GDTEntry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
};

struct __attribute__((packed)) GDTPtr
{
    uint16_t limit;
    uint32_t base;
};

void load_gdt(void);

#endif