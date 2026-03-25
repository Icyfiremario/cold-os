#include "memory.h"

uint64_t* bitmap = &bitmap_start;
uint64_t total_pages;

uint64_t heap_virtual_top = 0x40000000;

static malloc_header_t* heap_start = NULL;

uint64_t next_free_byte = 0;
uint64_t current_page_limit = 0;

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
    uint64_t real_end = (uint64_t)&kernel_end + bitmap_size;
    for (uint64_t addr = (uint64_t)&kernel_end; addr < real_end; addr += PAGE_SIZE)
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

    if (!mmap)
    {
        call_panic("Multiboot2 Memory map not found!");
    }

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

void* memset(void* dest, int ch, uint64_t count)
{
    uint8_t* ptr = (uint8_t*)dest;
    while (count--)
    {
        *ptr++ = (uint8_t)ch;
    }

    return dest;
}

pt_entry* get_next_table(pt_entry* entry, bool create)
{
    if (*entry & PTE_PRESENT)
    {
        return (pt_entry*)(*entry & PTE_FRAME);
    }

    if (!create) return NULL;

    uint64_t new_table_phys = pmm_alloc_frame();
    memset((void*)new_table_phys, 0, PAGE_SIZE);
    *entry = new_table_phys | PTE_PRESENT | PTE_WRITEABLE;

    return (pt_entry*)new_table_phys;
}

void vmm_map_page(uint64_t virtual_addr, uint64_t physical_addr, uint64_t flags)
{
    uint64_t pml4_phys = get_current_cr3() & ~0xFFFULL;
    uint64_t* pml4 = (uint64_t*)pml4_phys;

    uint64_t pml4_idx = (virtual_addr >> 39) & 0x1FF;
    uint64_t pdpt_idx = (virtual_addr >> 30) & 0x1FF;
    uint64_t pd_idx = (virtual_addr >> 21) & 0x1FF;
    uint64_t pt_idx = (virtual_addr >> 12) & 0x1FF;

    //pt_entry* pml4 = (pt_entry*)get_current_cr3();
    pt_entry* pdpt = get_next_table(&pml4[pml4_idx], true);
    pt_entry* pd = get_next_table(&pdpt[pdpt_idx], true);
    pt_entry* pt = get_next_table(&pd[pd_idx], true);

    pt[pt_idx] = physical_addr | flags | PTE_PRESENT;

    flush_tlb(virtual_addr);
}

void* vmm_alloc_pages(uint64_t count)
{
    void* v_addr_start = (void*)heap_virtual_top;

    for (uint64_t i = 0; i < count; i++)
    {
        uint64_t phys_frame = pmm_alloc_frame();
        if (phys_frame == 0) return NULL;

        vmm_map_page(heap_virtual_top, phys_frame, 0x3);

        heap_virtual_top += PAGE_SIZE;
    }

    return v_addr_start;
}

void* kmalloc(uint64_t size)
{
    size = (size + 7) & ~7;

    malloc_header_t* current = heap_start;
    while (current)
    {
        if (current->is_free && current->size >= size)
        {
            current->is_free = false;
            return (void*)(current + 1);
        }

        if (!current->next) break;
        current = current->next;
    }

    uint64_t pages_needed = ((size + sizeof(malloc_header_t)) / PAGE_SIZE) + 1;
    malloc_header_t* new_block = (malloc_header_t*)vmm_alloc_pages(pages_needed);

    if (!new_block) return NULL;

    new_block->size = (pages_needed * PAGE_SIZE) - sizeof(malloc_header_t);
    new_block->is_free = false;
    new_block->next = NULL;

    if (!heap_start)
    {
        heap_start = new_block;
    }
    else
    {
        current->next = new_block;
    }

    return (void*)(new_block + 1);
}

void kfree(void* ptr)
{
    if (!ptr) return;

    malloc_header_t* header = (malloc_header_t*)ptr - 1;
    header->is_free = true;
}

void mem_init(void)
{
    init_pmm();
}
