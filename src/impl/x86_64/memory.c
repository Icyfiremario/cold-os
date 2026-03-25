#include "memory.h"

uint64_t* bitmap;
uint64_t total_pages;

void pmm_set_bit(uint64_t index)
{
    bitmap[index / BITS_PER_WORD] |= (1ULL << (index % BITS_PER_WORD));
}

void pmm_clear_bit(uint64_t index)
{
    bitmap[index / BITS_PER_WORD] &= ~(1ULL << (index % BITS_PER_WORD));
}

bool pmm_test_bit(uint64_t index)
{
    return bitmap[index / BITS_PER_WORD] & (1ULL << (index % BITS_PER_WORD));
}

uint64_t pmm_alloc_frame(void)
{
    for (uint64_t i = 0; i < total_pages; i++)
    {
        if (!pmm_test_bit(i))
        {
            pmm_set_bit(i);
            return i * PAGE_SIZE;
        }
    }

    return 0;
}

void pmm_free_frame(uint64_t addr)
{
    uint64_t index = addr / PAGE_SIZE;
    pmm_clear_bit(index);
}

void reserve_critical_regions(void)
{
    for (uint64_t addr = 0; addr < 0x200000; addr += PAGE_SIZE)
    {
        pmm_set_bit(addr / PAGE_SIZE);
    }

    uint64_t bitmap_size = (total_pages / 8);
    uint64_t bitmap_end = 0x200000 + bitmap_size;
    for (uint64_t addr = 0x200000; addr < bitmap_end; addr += PAGE_SIZE)
    {
        pmm_set_bit(addr / PAGE_SIZE);
    }

    struct multiboot_tag* full_info = (struct multiboot_tag*)multiboot_ptr;
    for (uint64_t addr = multiboot_ptr; addr < multiboot_ptr + full_info->size; addr += PAGE_SIZE)
    {
        pmm_set_bit(addr / PAGE_SIZE);
    }
}

void init_pmm(void)
{
    struct multiboot_tag* tag;
    struct multiboot_tag_mmap* mmap = NULL;

    for (tag = (struct multiboot_tag*)(multiboot_ptr + 8); tag->type != 0; tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == 6)
        {
            mmap = (struct multiboot_tag_mmap*)tag;
            break;
        }
    }

    if (!mmap) return;

    bitmap = (uint64_t*)&bitmap_start;

    uint32_t max_addr = 0;
    uint32_t entry_count = (mmap->size - sizeof(struct multiboot_tag_mmap)) / mmap->entry_size;

    for (uint32_t i = 0; i < entry_count; i++)
    {
        if (mmap->entries[i].type == 1)
        {
            uint64_t end = mmap->entries[i].addr + mmap->entries[i].len;
            if (end > max_addr) max_addr = end;
        }
    }

    total_pages = max_addr / PAGE_SIZE;

    for (uint64_t i = 0; i < (total_pages / BITS_PER_WORD) + 1; i++)
    {
        bitmap[i] = 0xFFFFFFFFFFFFFFFF;
    }

    for (uint32_t i = 0; i < entry_count; i++)
    {
        if (mmap->entries[i].type == 1)
        {
            for (uint64_t addr = mmap->entries[i].addr; addr < mmap->entries[i].addr + mmap->entries[i].len; addr += PAGE_SIZE)
            {
                pmm_clear_bit(addr / PAGE_SIZE);
            }
        }
    }

    reserve_critical_regions();
}

void init_vmm(void)
{
    
}

void mem_init(void)
{
    init_pmm();
}
