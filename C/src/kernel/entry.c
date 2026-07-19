#include "stdio.h"
#include "gdt.h"
#include "idt.h"

void kentry() {
    load_gdt();
    load_idt();
    printf("%C%s %C I am an Architect!",
       ECOLOR(LIGHT_RED, BLACK),
       "Hello,",
       ECOLOR(LIGHT_GREEN, BLACK));
    while(1) __asm__ __volatile__("hlt");
}