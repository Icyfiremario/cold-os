#pragma once

// STD
#include <stdlib.h>

// Macros
#define MULTIBOOT_TAG_TYPE_END 0
#define MULTIBOOT_TAG_TYPE_ACPI_OLD 14
#define MULTIBOOT_TAG_TYPE_ACPI_NEW 15

// Externs
extern uint64_t multiboot_ptr;

struct multiboot_tag
{
    uint32_t type;
    uint32_t size;
};
