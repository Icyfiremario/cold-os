#include "memory.h"

void init_pmm()
{
    struct multiboot_tag* tag;

    for (tag = (struct multiboot_tag*)(multiboot_ptr + 8); tag->type != 0; tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == 6)
        {
            struct multiboot_tag_mmap* mmap = (struct multiboot_tag_mmap*)tag;
            for (int i = 0; i < (mmap->size - sizeof(*mmap)) / mmap->entry_size; i++)
            {
                if (mmap->entries[i].type == 1)
                {
                    
                }
            }
        }
    }
}
