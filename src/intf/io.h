#pragma once

// STD
#include <stdint.h>

// Defines a bunch of simple IO functions. (outb, inb, wrmsr, etc.)

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}


static inline void cpu_write_msr(uint32_t msr, uint32_t low, uint32_t high)
{
    __asm__ volatile (
        "wrmsr"
        :
        : "c"(msr), "a"(low), "d"(high)
        : "memory"
    );
}

static inline uint64_t cpu_read_msr(uint32_t msr)
{
    uint32_t low, high;

    __asm__ volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(msr)
    );

    return ((uint64_t)high << 32) | low;
}

static inline uint64_t get_current_cr3(void)
{
    uint64_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

static inline void set_current_cr3(uint64_t cr3)
{
    __asm__ volatile("mov %0, %%cr3" : : "r"(cr3) : "memory");
}

static inline void flush_tlb(uint64_t addr)
{
    __asm__ volatile("invlpg (%0)" : : "r"(addr) : "memory");
}
