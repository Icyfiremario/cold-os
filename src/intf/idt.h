#pragma once

// STD
#include <stdint.h>
#include <stddef.h>

#define MAX_IDT_ENTRIES 256

typedef struct
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

static idt_entry_t idt[MAX_IDT_ENTRIES];
static idtr_t idtr;

extern void* isr_stub_table[];

void set_idt_entry(uint8_t vector, void* handler, uint8_t dpl);
void init_idt(void);
