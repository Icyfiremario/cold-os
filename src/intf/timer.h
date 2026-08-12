#pragma once

// STD
#include <stdint.h>

// Local
#include "panic.h"
#include "print.h"

// Externs
extern uint64_t kernel_integrity_check;

static uint64_t loops = 0;

static inline void update_loops(void)
{
    loops++;

    if (*(volatile uint64_t *)(0x0) != 0x1010)
    {
        call_panic("KERNEL INTEGRITY VIOLATED");

        __asm__ volatile("cli");
        while (1)
        {
            __asm__ volatile("hlt");
        }
    }
}

uint64_t get_loops(void)
{
    return loops;
}
