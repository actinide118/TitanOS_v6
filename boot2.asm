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
    jmp read
    DAPACK:
	db	0x10
	db	0
blkcnt:	dw	100		; int 13 resets this to # of blocks actually read/written
db_add:	dw	0x8000		; memory buffer destination address (0:7c00)
	dw	0		; in memory page zero
d_lba:	dd	1		; put the lba to read in this spot
	dd	0		; more storage bytes only for big lba's ( > 4 bytes )
read:
	mov si, DAPACK		; address of "disk address packet"
	mov ah, 0x42		; AL is unused
	mov dl, 0x80		; drive number 0 (OR the drive # with 0x80)
	int 0x13
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
    mov bp, 0x7a00
    mov sp,bp
    jmp dword 0x08:0x8000
error_hang:
    jmp error_hang    

message db 'Starting TitouanOS... please wait for the kernel', 0
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