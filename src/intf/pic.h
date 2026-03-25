#pragma once

// STD
#include <stdint.h>
#include <stdbool.h>

// Local
#include "io.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x11
#define ICW4_8086 0x01

void pic_init(int offset1, int offset2);
void pic_disable(void);
bool get_pic_status(void);
