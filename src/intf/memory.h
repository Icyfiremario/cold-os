#pragma once

// STD
#include <stdint.h>
#include <stdbool.h>

// Externs
extern uint64_t multiboot_ptr;

struct multiboot_mmap_entry
{
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
};

struct multiboot_tag
{
    uint32_t type;
    uint32_t size;
};

struct multiboot_tag_mmap
{
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct multiboot_mmap_entry entries[0];
};

void kmalloc(void);
void kfree(void);
