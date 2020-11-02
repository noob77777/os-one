#include "include/types.h"
#include "include/gdt.h"

static int ptr = 0;

void kprintf(const char *str)
{
    uint8_t *video_memory = (uint8_t *)0xB8000;
    for (int i = 0; str[i] != '\0'; i++)
    {
        video_memory[2 * ptr++] = str[i];
    }
}

void kprintf(uint32_t value)
{
    uint8_t *video_memory = (uint8_t *)0xB8000;
    for (int bit = 0; bit < 32; bit++)
    {
        video_memory[2 * ptr++] = ((value & (1 << bit) ? '1' : '0'));
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void init_constructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

GlobalDescriptorTable GDT;

extern "C" void kernel_main(uint32_t arg)
{
    kprintf("OS-ONE (version 0.0.1-target=i386)");
    for (;;)
        ;
}
