#include "include/types.h"
#include "include/gdt.h"
#include "drivers/display.h"

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
