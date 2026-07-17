#include "gdt.h"
#include "stdio.h"
extern void gdt_flush(uint32_t);

static struct GDTEntry gdt[3];
static struct GDTPtr gp;

static void gdt_set_gate(
    int num,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t gran)
{
    gdt[num].base_low    = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    gdt[num].limit_low   = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    gdt[num].access      = access;

    printf("GDT[%d] B:%x:%x:%x L:%x:%x A:%x G:%x\n",
       num,
       gdt[num].base_low,
       gdt[num].base_middle,
       gdt[num].base_high,
       gdt[num].limit_low,
       gdt[num].granularity & 0x0F,
       gdt[num].access,
       gdt[num].granularity);
}

void load_gdt(void)
{
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1,
                 0,
                 0xFFFFF,
                 0x9A,
                 0xCF);
    gdt_set_gate(2,
                 0,
                 0xFFFFF,
                 0x92,
                 0xCF);

    gdt_flush((uint32_t)&gp);
    printf("%CGDT loaded successfully! %C\n",
           ECOLOR(LIGHT_GREEN, BLACK),
           ECOLOR(LIGHT_GREY, BLACK));
}