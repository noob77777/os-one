#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "../include/types.h"

static const int WIDTH = 80;
static const int HEIGHT = 25;
static char *const VIDEO_MEMORY = (char *const)0xB8000;
static const char *hex = "0123456789ABCDEF";

static int x_coordinate = 0;
static int y_coordinate = 0;

namespace display
{
    void clear()
    {
        for (y_coordinate = 0; y_coordinate < HEIGHT; y_coordinate++)
            for (x_coordinate = 0; x_coordinate < WIDTH; x_coordinate++)
                VIDEO_MEMORY[2 * (80 * y_coordinate + x_coordinate)] = ' ';
        x_coordinate = 0;
        y_coordinate = 0;
    }

    void backspace()
    {
        if (x_coordinate > 0)
        {
            x_coordinate--;
            VIDEO_MEMORY[2 * (WIDTH * y_coordinate + x_coordinate)] = ' ';
        }
    }
} // namespace display

void kprintf(const char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
            x_coordinate = 0;
            y_coordinate++;
            break;
        case '\t':
            x_coordinate += 4;
            break;
        default:
            VIDEO_MEMORY[2 * (WIDTH * y_coordinate + x_coordinate)] = str[i];
            x_coordinate++;
            break;
        }

        if (x_coordinate >= WIDTH)
        {
            x_coordinate = 0;
            y_coordinate++;
        }

        if (y_coordinate >= HEIGHT)
        {
            for (y_coordinate = 0; y_coordinate < HEIGHT; y_coordinate++)
                for (x_coordinate = 0; x_coordinate < WIDTH; x_coordinate++)
                    VIDEO_MEMORY[2 * (80 * y_coordinate + x_coordinate)] = ' ';
            x_coordinate = 0;
            y_coordinate = 0;
        }
    }
}

void kprintf_hex8(uint8_t value, bool flag = true)
{
    char *tmp = {0};
    tmp[0] = hex[(value >> 4) & 0xF];
    tmp[1] = hex[value & 0xF];
    if (flag)
        kprintf("0x");
    kprintf(tmp);
}
void kprintf_hex(uint32_t value)
{
    kprintf("0x");
    kprintf_hex8((value >> 24) & 0xFF, false);
    kprintf_hex8((value >> 16) & 0xFF, false);
    kprintf_hex8((value >> 8) & 0xFF, false);
    kprintf_hex8(value & 0xFF, false);
}

#endif
