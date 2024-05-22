#pragma once

extern void* isr_stub_table[];

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_init(void);