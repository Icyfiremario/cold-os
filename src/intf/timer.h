#pragma once

// STD
#include <stdint.h>

static uint64_t loops = 0;

static inline void update_loops(void)
{
    loops++;
}

uint64_t get_loops(void)
{
    return loops;
}
