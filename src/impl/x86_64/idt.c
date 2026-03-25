#include "idt.h"

// Exceptions
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr30(void);

// Hardware
extern void isr32(void);
extern void isr33(void);

__attribute__((aligned(0x10))) struct idt_entry_t idt[256];
struct idtr_t idtr_ptr;

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    struct idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low = (uint64_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->isr_mid = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->isr_high = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->reserved = 0;
}

void idt_init(void)
{
    idtr_ptr.limit = (sizeof(struct idt_entry_t) * 256) - 1;
    idtr_ptr.base = (uint64_t)&idt;

    // Exception interrupts
    idt_set_descriptor(0, (void*)isr0, 0x8E);
    idt_set_descriptor(1, (void*)isr1, 0x8E);
    idt_set_descriptor(2, (void*)isr2, 0x8E);
    idt_set_descriptor(3, (void*)isr3, 0x8E);
    idt_set_descriptor(4, (void*)isr4, 0x8E);
    idt_set_descriptor(5, (void*)isr5, 0x8E);
    idt_set_descriptor(6, (void*)isr6, 0x8E);
    idt_set_descriptor(7, (void*)isr7, 0x8E);
    idt_set_descriptor(8, (void*)isr8, 0x8E);
    idt_set_descriptor(9, (void*)isr9, 0x8E);
    idt_set_descriptor(10, (void*)isr10, 0x8E);
    idt_set_descriptor(11, (void*)isr11, 0x8E);
    idt_set_descriptor(12, (void*)isr12, 0x8E);
    idt_set_descriptor(13, (void*)isr13, 0x8E);
    idt_set_descriptor(14, (void*)isr14, 0x8E);
    idt_set_descriptor(15, (void*)isr15, 0x8E);
    idt_set_descriptor(16, (void*)isr16, 0x8E);
    idt_set_descriptor(17, (void*)isr17, 0x8E);
    idt_set_descriptor(18, (void*)isr18, 0x8E);
    idt_set_descriptor(30, (void*)isr30, 0x8E);

    // Hardware interrupts
    idt_set_descriptor(32, (void*)isr32, 0x8E);
    idt_set_descriptor(33, (void*)isr33, 0x8E);

    load_idt(&idtr_ptr);
}
