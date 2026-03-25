#include "keyboard.h"

#define KEYBOARD_DATA_PORT 0x60

const char kbd_us[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

void keyboard_handler_main(void)
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    if (!(scancode & 0x80))
    {
        char c = kbd_us[scancode];
        if (c > 0)
        {
            mini_printf("%c", c);
        }
    }
}
