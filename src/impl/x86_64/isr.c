#include "isr.h"

[[noreturn]]
void exception_handler(void)
{
    print_str("Exception called!");
    for(;;);
    asm volatile ("cli; hlt"); // Hang the CPU
}