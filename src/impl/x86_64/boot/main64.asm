global long_mode_start
extern kernel_main
extern kernel_integrity_check

section .text
bits 64
long_mode_start:
    mov qword [0x0], 0x1010 ; move 0x1010 to be checked later for kernel integrity.

    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    call kernel_main
    hlt
