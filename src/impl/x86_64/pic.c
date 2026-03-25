#include "pic.h"

static bool pic_enabled = false;

void pic_init(int offset1, int offset2)
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT);
    io_wait();

    outb(PIC1_DATA, offset1);
    io_wait();
    outb(PIC2_DATA, offset2);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);

    pic_enabled = true;
}

void pic_disable(void)
{
    outb(PIC1_COMMAND, 0xFF);
    io_wait();
    outb(PIC2_COMMAND, 0xFF);
    io_wait();

    pic_enabled = false;
}

bool get_pic_status(void)
{
    return pic_enabled;
}
