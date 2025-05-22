#pragma once

// STD
#include <stdint.h>
#include <stdbool.h>

// Project
#include "print.h"

#define IDT_MAX_ENTRIES 256
#define GDT_KERNEL_CS 0x08

typedef struct 
{
    uint16_t isr_low; // Lower 16 bits of ISR address
    uint16_t kernel_cs; // GDT segment selector
    uint8_t ist;
    uint8_t attributes;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idt_entry_t;

typedef struct 
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256];

static bool vectors[IDT_MAX_ENTRIES];

extern void* isr_stub_table[];

void idt_init(void);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);