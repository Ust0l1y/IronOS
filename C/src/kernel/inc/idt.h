#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idtg {
    uint16_t l;
    uint16_t s;
    uint8_t z;
    uint8_t f;
    uint16_t h;
} __attribute__((packed));

struct idtr {
    uint16_t l;
    uint32_t b;
} __attribute__((packed));

struct irqf {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t id, err;
    uint32_t eip, cs, eflags, useresp, ss;
};

void set_gate(uint8_t n, uint32_t b, uint16_t s, uint8_t f);
void load_idt(void);
void set_irq(uint8_t n, void (*handler)(struct irqf *r));

#endif // IDT_H