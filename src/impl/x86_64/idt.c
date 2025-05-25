#include "idt.h"

void set_idt_entry(uint8_t vector, void *handler, uint8_t dpl)
{
    idt_entry_t* descriptor = &idt[vector];

    descriptor->offset_low = (uint64_t)handler & 0xFFFF;
    descriptor->offset_middle = ((uint64_t)handler >> 16) & 0xFFFF;
    descriptor->offset_high = ((uint64_t)handler >> 32) & 0xFFFFFFFF;
    descriptor->selector = 0x08; // Kernel code segment selector
    descriptor->ist = 0; // No IST
    descriptor->flags = dpl;
    descriptor->zero = 0;
}

void init_idt(void)
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * MAX_IDT_ENTRIES - 1;

    for (uint8_t i = 0; i < 32; i++)
    {
        set_idt_entry(i, isr_stub_table[i], 0x8E);
    }

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}
