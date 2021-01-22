#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <types.h>

static const int WIDTH = 80;
static const int HEIGHT = 25;
static char *const VIDEO_MEMORY = (char *const)0x000B8000;
static const char *hex = "0123456789ABCDEF";

static int x_coordinate = 0;
static int y_coordinate = 0;

namespace display
{
    void clear();
    void backspace();
} // namespace display

void kprintf(const char *str);
void kprintf_hex8(uint8_t value, bool flag = true);
void kprintf_hex(uint32_t value);

#endif
