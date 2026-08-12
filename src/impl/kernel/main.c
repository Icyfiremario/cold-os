// STD
#include <stdint.h>
#include <stdbool.h>

// local
#include "pic.h"
#include "acpi.h"
#include "apic.h"
#include "idt.h"
#include "memory.h"
#include "print.h"
#include "panic.h"

void kernel_main(void)
{
    terminal_clear();
    set_terminal_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);

    idt_init();
    mini_printf("IDT initalized.\n");
    
    pic_init(0x20, 0x28);
    mini_printf("Remapped PIC.\n");

    __asm__ volatile("sti");
    mini_printf("Interrupts enabled.\n");

    mem_init();
    mini_printf("Memory manager initialized\n");

    if (check_apic())
    {
        mini_printf("APIC Support found.\n");
    }

    set_terminal_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    mini_printf("OS Booted!\n");

    while(1)
    {
        __asm__ volatile("hlt");
    }
}
