#pragma once

// Local
#include "print.h"

static inline void call_panic(const char* message)
{
    set_terminal_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLUE);
    terminal_clear();
    mini_printf("--- MANUAL KERNEL PANIC ---\n");
    mini_printf("REASON: %s\n", message);

    __asm__ volatile("cli");
    while (1)
    {
        __asm__ volatile("hlt");
    }
}
