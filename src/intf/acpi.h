#pragma once

// STD
#include <stdint.h>

// Local
#include "multiboot.h"
#include "print.h"

struct multiboot_tag_acpi
{
    uint32_t type;
    uint32_t size;
    uint8_t rsdp[8];
};

//void parse_multiboot_acpi(void);
void* get_acpi_ptr(void);
