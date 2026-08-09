#include "apic.h"

bool check_apic(void)
{
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);

    return (edx & (1 <<9)) != 0;
}

void cpu_set_apic_base(uintptr_t apic)
{
    uint32_t edx = 0;
    uint32_t eax = (apic & 0xFFFFF0000) | IA32_APIC_BASE_MSR_ENABLE;

#ifdef __PHYSICAL_MEMORY_EXTENSION__
    edx = (apic >> 32) & 0x0F;
#endif

    cpu_write_msr(IA32_APIC_BASE_MSR, eax, edx);

}

uintptr_t cpu_get_apic_base(void)
{
    uint32_t eax, edx;
    cpu_read_msr_ptr(IA32_APIC_BASE_MSR, &eax, &edx);

#ifdef __PHYSICAL_MEMORY_EXTENSION__
    return (eax & 0xFFFFF00) | ((edx & 0x0F) << 32);
#else
    return (eax & 0xFFFFF00);
#endif
}

void apic_enable(void)
{
    cpu_set_apic_base(cpu_get_apic_base());

    outb(0xF0, inb(0xF0) | 0x100);
}
