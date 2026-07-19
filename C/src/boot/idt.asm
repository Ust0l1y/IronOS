[bits 32]
extern c_ih
global ints

%macro no_err_stub 1
isr_stub_%1:
    push byte 0
    push byte %1
    jmp irq_common
%endmacro

%macro err_stub 1
isr_stub_%1:
    push byte %1
    jmp irq_common
%endmacro

no_err_stub 0
no_err_stub 1
no_err_stub 2
no_err_stub 3
no_err_stub 4
no_err_stub 5
no_err_stub 6
no_err_stub 7
err_stub    8
no_err_stub 9
err_stub    10
err_stub    11
err_stub    12
err_stub    13
err_stub    14
no_err_stub 15
no_err_stub 16
err_stub    17
no_err_stub 18
no_err_stub 19
no_err_stub 20
no_err_stub 21
no_err_stub 22
no_err_stub 23
no_err_stub 24
no_err_stub 25
no_err_stub 26
no_err_stub 27
no_err_stub 28
no_err_stub 29
err_stub    30
no_err_stub 31

%assign i 32
%rep 16
    no_err_stub i
%assign i i+1
%endrep

irq_common:
    pushad
    push esp
    call c_ih
    add esp, 4
    popad
    add esp, 8
    iret
section .data
ints:
%assign i 0
%rep 48
    dd isr_stub_%[i]
%assign i i+1
%endrep