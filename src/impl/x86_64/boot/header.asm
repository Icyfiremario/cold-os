section .multiboot_header
header_start:
    ; magic number
    dd 0xE85250D6
    ; architecture
    dd 0 ; protected mode i386
    ; header length
    dd header_end - header_start
    ; checksum
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))
    
    ; end tag
    dw 0
    dw 0
    dd 0
header_end: