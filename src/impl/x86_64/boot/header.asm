section .multiboot_header
align 8
header_start:
    ; magic number
    dd 0xE85250D6
    ; architecture
    dd 0 ; protected mode i386
    ; header length
    dd header_end - header_start
    ; checksum
    dd 0x100000000 - (0xE85250D6 + 0 + (header_end - header_start))

    ; --- INFORMATION REQUEST TAG ---
align 8
info_req_tag:
    dw 1    ; Type: 1 = Info request
    dw 1    ; Flags: 0 (1 for optional)
    dd info_req_tag_end - info_req_tag
    dd 14
    dd 15
info_req_tag_end:
    
    ; --- ENG TAG ---
    align 8
    dw 0    ; Type: 0 = End tag
    dw 0    ; Flags: 0
    dd 8    ; Size: 8 Bytes
header_end: