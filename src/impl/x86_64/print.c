#include "print.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint8_t VGA_COLOR  = 0x0F;

static int terminal_row = 0;
static int terminal_col = 0;

uint16_t volatile* vga_buffer = (void*)0xB8000;
int cursor_pos = 0;

void terminal_scroll(void)
{
    for (int y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            const int current_idx = y * VGA_WIDTH + x;
            const int next_row_idx = (y + 1) * VGA_WIDTH + x;
            vga_buffer[current_idx] = vga_buffer[next_row_idx];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++)
    {
        const int last_row_idx = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        vga_buffer[last_row_idx] = (uint16_t)' ' | (uint16_t)VGA_COLOR << 8;
    }

    terminal_row = VGA_HEIGHT - 1;
}

void put_char(char c)
{
    switch (c)
    {
        case '\n':
            terminal_col = 0;
            terminal_row++;
            break;
        case '\r':
            terminal_col = 0;
            break;
        case '\t':
            terminal_col += 4;
            break;
        case '\b':
        {
            terminal_col--;
            const int index = terminal_row * VGA_WIDTH + terminal_col;
            vga_buffer[index] = (uint16_t)' ' | (uint16_t)VGA_COLOR << 8;
            break;
        }
        default:
        {
            const int index = terminal_row * VGA_WIDTH + terminal_col;
            vga_buffer[index] = (uint16_t)c | (uint16_t)VGA_COLOR << 8;
            terminal_col++;
            break;
        }
    }

    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }

    if (terminal_row >= VGA_HEIGHT)
    {
        terminal_scroll();
    }
}

char* itoa(int64_t value, char* buffer, int base)
{
    char* ptr = buffer;
    char* ptr1 = buffer;
    char tmp_char;
    uint64_t tmp_value;

    if (value == 0)
    {
        *ptr++ = '0';
        *ptr = '\0';
        return buffer;
    }

    int negative = 0;
    if (value < 0 && base == 10)
    {
        negative = 1;
        value = -value;
    }

    while (value != 0)
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789ABCDEF"[tmp_value % base];
    }

    if (negative)
    {
        *ptr++ = '-';
    }

    *ptr-- = '\0';

    while (ptr1 < ptr)
    {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return buffer;
}

void mini_printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char buf[64];

    for (int i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            i++;
            switch (format[i])
            {
                case 'c':
                {
                    char c = (char)va_arg(args, int);
                    put_char(c);
                    break;
                }
                case 's':
                {
                    char* s = va_arg(args, char*);
                    while (*s) put_char(*s++);
                    break;
                }
                case 'd':
                {
                    itoa(va_arg(args, int), buf, 10);
                    for (int j = 0; buf[j]; j++) put_char(buf[j]);
                    break;
                }
                case 'x':
                {
                    uint64_t val = va_arg(args, uint64_t);
                    itoa(val, buf, 16);

                    put_char('0');
                    put_char('x');

                    char* p = buf;
                    while (*p)
                    {
                        put_char(*p);
                        p++;
                    }

                    break;
                }
                default:
                    break;
            }
        }
        else
        {
            put_char(format[i]);
        }
    }
    va_end(args);
}

void terminal_clear(void)
{
    terminal_row = 0;
    terminal_col = 0;

    const uint16_t blank = (uint16_t)' ' | (uint16_t)VGA_COLOR << 8;

    for (int y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            const int index = y * VGA_WIDTH + x;
            vga_buffer[index] = blank;
        }
    }
}

void set_terminal_color(uint8_t foreground, uint8_t background)
{
    VGA_COLOR = foreground + (background << 4);
}
