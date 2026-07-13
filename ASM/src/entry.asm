[org 0x7c00]
mov ah, 0x0e
mov bx, mess

prints:
    mov al, [bx]
    test al, al
    je end
    int 0x10
    inc bx
    jmp prints
end:

mess: db "Hello, Im Architect", 0

times 510-($-$$) db 0
db 0x55, 0xaa