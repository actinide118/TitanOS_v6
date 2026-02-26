[BITS 32]
[ORG 0x1000]

start:
    mov edi, (0xB8000+(24*80+79)*2)
    mov al, "g"
    mov ah, 0x0F
    mov [edi], ax
    mov dx, 0x0e
    mov al, 0x3d4
    out dx, al
    
    jmp hang

hang:
    jmp hang
       