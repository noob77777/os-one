#include "include/types.h"

void kprintf(const char *str)
{
    uint8_t *video_memory = (uint8_t *)0xb8000;
    for (int i = 0; str[i] != '\0'; i++)
    {
        video_memory[2 * i] = str[i];
    }
}

extern "C" void kernel_main(void *multiboot_struct, uint32_t magic_number)
{
    kprintf("OS-ONE (version 0.0.1-target=i386)");
    for (;;)
        ;
}
