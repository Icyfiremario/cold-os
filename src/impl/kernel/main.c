#include <stdint.h>

#include "print.h"
#include "idt.h"

void kernel_main(void)
{
    init_idt();

    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);

    print_str("Welcome to the OS goob!\n");

    while (1)
    {
        asm volatile("hlt");
    }
}
