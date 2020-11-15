#include <types.h>
#include <memory/gdt.h>
#include <memory/virtual_memory.h>
#include <hardware/interrupts.h>
#include <drivers/display.h>
#include <drivers/keyboard_driver.h>
#include <drivers/ata.h>
#include <filesystem/fat.h>
#include <sys/program.h>
#include <sys/terminal.h>

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

    ATA::ata_check();
    ATA ataDisk(true, 0x01F0);
    FAT fs(&ataDisk);
    fs.init();

    // fs tests
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.free(2));
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.allocate());
    kprintf_hex(fs.allocate());

    kprintf("\n");
    ProgramManager program_manager;
    Terminal terminal(&program_manager, &keyboard);
    program_manager.add_program(&terminal);
    program_manager.start();

    for (;;)
        ;
}
