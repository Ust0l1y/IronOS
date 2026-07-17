#include "stdio.h"
#include "gdt.h"

void kentry() {
    load_gdt();
    printf("%C%s %C I am an Architect!",
       ECOLOR(LIGHT_RED, BLACK),
       "Hello,",
       ECOLOR(LIGHT_GREEN, BLACK));
}