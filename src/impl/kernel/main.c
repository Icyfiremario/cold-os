#include "print.h"
#include "idt.h"

void kernel_main()
{
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);

    idt_init();

    print_str("Welcome to the OS goob!\n");
}