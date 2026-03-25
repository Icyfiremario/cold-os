// STD
#include <stdint.h>

// Local
#include "print.h"
#include "keyboard.h"
#include "io.h"
#include "pic.h"

struct interrupt_frame
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
    uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
    uint64_t vector_number;
    uint64_t error_code;
    uint64_t rip, cs, rflags, rsp, ss;
};

const char* error_id[] = {
    "Divide by zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Co-processor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "Security Exception"
};

void kernel_panic(struct interrupt_frame* frame)
{
    set_terminal_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLUE);
    terminal_clear();
    mini_printf("--- AUTO KERNEL PANIC ---\n");
    mini_printf("ISR VECTOR: %d %s\n", frame->vector_number, error_id[frame->vector_number]);
    mini_printf("ERROR CODE: %x\n", frame->error_code);
    mini_printf("Registers:\n");
    mini_printf(" R15:    %x\n", frame->r15);
    mini_printf(" R14:    %x\n", frame->r14);
    mini_printf(" R13:    %x\n", frame->r13);
    mini_printf(" R12:    %x\n", frame->r12);
    mini_printf(" R11:    %x\n", frame->r11);
    mini_printf(" R10:    %x\n", frame->r10);
    mini_printf(" R9:     %x\n", frame->r9);
    mini_printf(" R8:     %x\n", frame->r8);

    mini_printf(" RBP:    %x\n", frame->rbp);
    mini_printf(" RDI:    %x\n", frame->rdi);
    mini_printf(" RSI:    %x\n", frame->rsi);
    mini_printf(" RDX:    %x\n", frame->rdx);
    mini_printf(" RCX:    %x\n", frame->rcx);
    mini_printf(" RBX:    %x\n", frame->rbx);
    mini_printf(" RAX:    %x\n", frame->rax);

    mini_printf(" RIP:    %x\n", frame->rip);
    mini_printf(" CS:     %x\n", frame->cs);
    mini_printf(" RFLAGS: %x\n", frame->rflags);
    mini_printf(" RSP:    %x\n", frame->rsp);
    mini_printf(" SS:     %x\n", frame->ss);

    if (frame->vector_number == 14)
    {
        uint64_t cr2;
        __asm__ volatile("mov %%cr2, %0" : "=r"(cr2));
        mini_printf("CR2 (Faulting Addr): %x\n", cr2);
    }
    
    __asm__ volatile("cli");
    while(1)
    {
        __asm__ volatile("hlt");
    }
}

void idt_handler(struct interrupt_frame* frame)
{
    if (frame->vector_number < 32)
    {
        kernel_panic(frame);
    }
    else
    {
        switch (frame->vector_number)
        {
            case 0:
                kernel_panic(frame);
                break;
            case 33:
                keyboard_handler_main();
                break;
            default:
                break;
        }

        // EOI to PIC
        if (frame->vector_number >= 40)
        {
            outb(0xA0, 0x20); // Slave
        }
        outb(0x20, 0x20); // Master
    }
}
