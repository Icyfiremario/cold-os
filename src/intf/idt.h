#pragma once

// STD
#include <stdint.h>

struct idt_entry_t 
{
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__ ((packed));

struct idtr_t
{
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed));

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);

static inline void load_idt(struct idtr_t* ptr)
{
    __asm__ volatile ("lidt %0" : : "m"(*ptr));
}
