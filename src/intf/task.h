#pragma once

// STD
#include <stdint.h>

typedef struct {
    struct {
        uint64_t    rax; // +0x00
        uint64_t    rbx; // +0x08
        uint64_t    rcx; // +0x10
        uint64_t    rdx; // +0x18
        uint64_t    rdi; // +0x20
        uint64_t    rsi; // +0x28
        uint64_t    r8; // +0x30
        uint64_t    r9; // +0x38
        uint64_t    r10; // +0x40
        uint64_t    r11; // +0x48
        uint64_t    r12; // +0x50
        uint64_t    r13; // +0x58
        uint64_t    r14; // +0x60
        uint64_t    r15; // +0x68
    } registers;
    
    struct {
        uint64_t rsp; // +0x70
        uint64_t rbp; // +0x78
    } stack;

    uint64_t rip;   // instruction pointer +0x80
    uint64_t cr3;   // page table +0x88
} __attribute__((__packed__)) task_context_t;