#pragma once

// STD
#include <stdint.h>
#include <stdbool.h>

// Local
#include "io.h"

#define IA32_APIC_BASE_MSR 0X1B
#define IA32_APIC_BASE_MSR_BSP 0x100 // Processor is a BSP flag
#define IA32_APIC_BASE_MSR_ENABLE 0x800

bool check_apic(void);
void cpu_set_apic_base(uintptr_t apic);
uintptr_t cpu_get_apic_base(void);
void apic_enable(void);
