
void kprintf(const char *str)
{
    char *video_memory = (char *)0xb8000;
    for (int i = 0; str[i] != '\0'; i++)
    {
        video_memory[2 * i] = str[i];
    }
}

extern "C" void kernel_main(void *multiboot_struct, unsigned int magic_number)
{
    kprintf("OS-ONE (version 0.0.1-target=i386)");
    for (;;)
        ;
}
