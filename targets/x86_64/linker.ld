ENTRY(start)

SECTIONS
{
    . = 1M;
    kernel_start = .;

    .boot :
    {
        KEEP(*(.multiboot_header))
    }

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
        *(COMMON)
    }

    . = 2M;
    bitmap_start = .;

    kernel_end = .;
}