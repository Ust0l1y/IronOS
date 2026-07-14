MBOOT_HEADER_MAGIC equ 0x1BADB002
MBOOT_HEADER_FLAGS equ 0x00000003
dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .text
global _start
extern kentry

_start:
    cli
    call kentry
.hang:
    hlt
    jmp .hang