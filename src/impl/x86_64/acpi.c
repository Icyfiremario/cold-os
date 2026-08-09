#include "acpi.h"

void *rsdp_addr = 0;
int is_xsdp = 0;

void parse_multiboot_acpi(void)
{
    struct multiboot_tag* tag;

    for (tag = (struct multiboot_tag *)(multiboot_ptr + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((uint8_t *) tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == MULTIBOOT_TAG_TYPE_ACPI_NEW)
        {
            struct multiboot_tag_acpi* acpi_tag = (struct multiboot_tag_acpi *) tag;
            rsdp_addr = (void *)acpi_tag->rsdp;
            is_xsdp = 1;
            break;
        }
        else if (tag->type == MULTIBOOT_TAG_TYPE_ACPI_OLD)
        {
            if (!rsdp_addr)
            {
                struct multiboot_tag_acpi* acpi_tag = (struct multiboot_tag_acpi*) tag;
                rsdp_addr = (void*)acpi_tag->rsdp;
                is_xsdp = 0;
            }
        }
    }
}
