#include "idt.h"
#include "stdio.h"
#include "io.h"

struct idtg idt[256];
struct idtr ireg;
void *irqs[256] = {0};

extern uint32_t ints[48];

void set_irq(uint8_t n, void (*h)(struct irqf *r)) {
    irqs[n] = h;
}

__attribute__((noinline, used, cdecl)) 
void timer_stub(struct irqf *r) {
    __asm__ __volatile__("nop");
}

void load_idt(void) {
    volatile uint32_t* apic = (volatile uint32_t*)0xFEE00000;
    ireg.l = sizeof(idt) - 1;
    ireg.b = (uint32_t)&idt;

    for (int i = 0; i < 48; i++) {
        idt[i].l = ints[i] & 0xFFFF;
        idt[i].h = (ints[i] >> 16) & 0xFFFF;
        idt[i].s = 0x08;
        idt[i].z = 0;
        idt[i].f = 0x8E;
    }

    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);

    __asm__ __volatile__("lidt %0" : : "m"(ireg));

    set_irq(0, timer_stub);

    apic[0xF0/4] |= 0x100;
    apic[0x320/4] = 32 | 0x20000; 
    apic[0x3E0/4] = 0xB;
    apic[0x380/4] = 1000000;

    __asm__ __volatile__("sti");
    printf("%CIDT initialization and STI are enabled.%C\n", GREEN, WHITE);
}

__attribute__((noinline, used, cdecl))
void c_ih(struct irqf *r) {
    if (r->id < 32) {
        uint32_t cr2 = 0;
        if (r->id == 14) __asm__ __volatile__("mov %%cr2, %0" : "=r"(cr2));
        printf("INT:%d E:%x IP:%x CS:%x F:%x A:%x B:%x C:%x D:%x SP:%x BP:%x SI:%x DI:%x CR2:%x\n",
               r->id, r->err, r->eip, r->cs, r->eflags, r->eax, r->ebx, r->ecx, 
               r->edx, r->esp, r->ebp, r->esi, r->edi, cr2);
        while(1) __asm__ __volatile__("hlt");
    } else {
        void (*h)(struct irqf*) = irqs[r->id - 32];
        if (h) h(r);
        ((volatile uint32_t*)0xFEE00000)[0xB0/4] = 0;
    }
}