#pragma once

// STD
#include <stdint.h>
#include <stdbool.h>

// Local
#include "print.h"
#include "panic.h"
#include "io.h"

#define PAGE_SIZE 4096
#define BITS_PER_WORD 64

// Externs
extern uint64_t multiboot_ptr;

extern uint64_t kernel_start;
extern uint64_t bitmap_start;
extern uint64_t kernel_end;

// Typedef
typedef uint64_t pt_entry;

#define PTE_PRESENT (1ULL << 0)
#define PTE_WRITEABLE (1ULL << 1)
#define PTE_USER (1ULL << 2)
#define PTE_FRAME 0x000FFFFFFFFFF000ULL

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

typedef struct malloc_header
{
    uint64_t size;
    bool is_free;
    struct malloc_header* next;
} malloc_header_t;

void* kmalloc(uint64_t size);
void kfree(void* ptr);
void* memset(void* dest, int ch, uint64_t count);

void mem_init(void);
