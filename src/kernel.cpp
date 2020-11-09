#include "include/types.h"
#include "include/gdt.h"
#include "include/interrupts.h"
#include "drivers/display.h"
#include "drivers/keyboard_driver.h"
#include "include/virtual_memory.h"

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
    display::clear();

    IdentityVirtualMemory vm = IdentityVirtualMemory();
    enable_virtual_memory(vm.cr3());

    InterruptManager interruptManager(&GDT);
    KeyboardDriver keyboard;
    interruptManager.add_driver(&keyboard);
    interruptManager.activate();

    kprintf("OS-ONE (version 0.0.1-target=i386)\n");
    
    for (;;)
        ;
}
