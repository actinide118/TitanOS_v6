[BITS 16]
[ORG 0x7C00]   ; Adresse où le BIOS charge le bootloader

start:
    mov [disk_number], dl
    ; Configuration initiale du segment
    cli                ; Désactiver les interruptions
    xor ax, ax         ; AX = 0
    mov ds, ax         ; DS = 0 (segment de données à 0)
    mov es, ax         ; ES = 0 (segment extra à 0)
    
    mov ah, 0x0
    mov al, 0x3
    int 0x10

    mov si, message
    call print_string

    ; Charger le noyau à partir du secteur 2 (secteur 1 est le bootloader)
    mov bx, 0x1000     ; Charger le noyau à l'adresse 0x1000
    mov ax, 0x0000
    mov es, ax
    mov dh, 0x00       ; Numéro de tête 0 (première tête)
    mov dl, [disk_number]      ; Périphérique 0 (disque)
    mov ah, 0x02       ; Fonction BIOS 0x02 : Lire des secteurs
    mov al, 0x30     ; Nombre de secteurs à lire
    mov ch, 0x00       ; Cylindre 0
    mov cl, 0x02       ; Secteur 2 (le premier secteur est le bootloader)
    int 0x13           ; Appel du BIOS pour lire le secteur
    ; Vérifier si la lecture a échoué
    jc alert_error

    mov ah, 0x0
    mov al, 0x3
    int 0x10

    mov ax, ds
    mov es, ax

    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp dword CODE_SEG:_32bit_kernel_loader

    ;jmp 0x0000:0x1000     ; Sauter à l'adresse où le noyau a été chargé

alert_error:
    mov si, error_occured
    call print_string
    jmp read_error

read_error:
    ; Boucle infinie en cas d'erreur
    hlt
    jmp read_error


print_string:
    mov ah, 0x0E
.next_char:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .next_char
.done:
    ret

[BITS 32]
_32bit_kernel_loader:
    mov ax, DATA_SEG            ; Charger le segment de données
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    mov bp, 0x9000
    mov sp,bp
    jmp dword 0x08:0x1000
error_hang:
    jmp error_hang    

message db 'Starting TitouanOS... please waiting for the kernel', 0
error_occured db 'Error loading kernel',0
kernel_segment dw 0x1000, 0
kernel_offset db 0x0000,0

disk_number db 80,0

gdt_start:
    dd 0x0
    dd 0x0

    ; Descripteur pour le segment de code (base = 0x00000000, limite = 4 GB)
gdt_code:
    dw 0xFFFF                   ; Limite basse
    dw 0x0000                   ; Base basse
    db 0x00                     ; Base médiane
    db 10011010b                ; Type : segment de code, exécutable, lisible
    db 11001111b                ; Limite haute et flags (32 bits)
    db 0x00                     ; Base haute

    ; Descripteur pour le segment de données (base = 0x00000000, limite = 4 GB)
gdt_data:
    dw 0xFFFF                   ; Limite basse
    dw 0x0000                   ; Base basse
    db 0x00                     ; Base médiane
    db 10010010b                ; Type : segment de données, lisible, accessible
    db 11001111b                ; Limite haute et flags (32 bits)
    db 0x00                     ; Base haute

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; Taille de la GDT
    dd gdt_start 
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
;VIDEO_SEG equ gdt_video - gdt_start

stack_size equ 4096 



times 510-($-$$) db 0  ; Remplir jusqu'à 510 octets (taille d'un secteur 512 octets)
dw 0xAA55              ; Signature du secteur de démarrage
