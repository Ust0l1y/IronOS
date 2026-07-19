MBOOT_MAGIC equ 0x1BADB002
MBOOT_FLAGS equ 0x00000003
align 4
dd MBOOT_MAGIC
dd MBOOT_FLAGS
dd -(MBOOT_MAGIC + MBOOT_FLAGS)

section .bss
align 16
sbot:
resb 16384
stop:

section .text
global _start
extern kentry

_start:
    cli
    mov esp, stop
    call kentry
.hang:
    hlt
    jmp .hang