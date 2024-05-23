#pragma once

//STD
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define IDT_MAX_DESCRIPTORS 256
#define GDT_OFFSET_KERNEL_CODE 0x08

extern void* isr_stub_table[];

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);