
#include "print.h"

__attribute__((noreturn))
void exception_handler(void);
void exception_handler()
{
    print_clear();
    print_str("Interrupted!");
    asm volatile ("cli; hlt");
}