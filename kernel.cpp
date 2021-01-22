#include <types.h>
#include <memory/gdt.h>
#include <memory/virtual_memory.h>
#include <memory/malloc.h>
#include <hardware/interrupts.h>
#include <drivers/display.h>
#include <drivers/keyboard_driver.h>
#include <drivers/ata.h>
#include <filesystem/filesystem.h>
#include <sys/program.h>
#include <sys/terminal.h>
#include <sys/hello.h>
#include <sys/system_software.h>

void os_one()
{
    kprintf(" _____  _______      _____  __   _ _______\n");
    kprintf("|     | |______ ___ |     | | \\  | |______\n");
    kprintf("|_____| ______|     |_____| |  \\_| |______\n");
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
    IdentityVirtualMemory vm = IdentityVirtualMemory();
    enable_virtual_memory(vm.cr3());

    InterruptManager interruptManager(&GDT);
    KeyboardDriver keyboard;
    interruptManager.add_driver(&keyboard);
    interruptManager.activate();

    display::clear();
    kprintf("OS-ONE (version 0.0.1-target=i386)\n");

    ATA ata0m(true, 0x01F0);
    FileSystem fs(&ata0m);

    MemoryManager::init();

    kprintf("\n");
    os_one();
    kprintf("\n");

    ProgramManager program_manager;
    Terminal terminal(&program_manager, &keyboard);
    program_manager.add_program(&terminal);
    Hello hello;
    program_manager.add_program(&hello);
    LS ls(&fs);
    program_manager.add_program(&ls);
    Touch touch(&fs);
    program_manager.add_program(&touch);
    RM rm(&fs);
    program_manager.add_program(&rm);
    Clear clear;
    program_manager.add_program(&clear);
    program_manager.start();

    for (;;)
        ;
}
