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
    // for (int i = 0; i < 1024; i++) {
    //     uint32_t cluster = 0;
    //     kprintf_hex(cluster = fs.allocate());
    //     char write[4096] = "Test Data\n";
    //     fs.write_cluster(cluster, (uint8_t *)write, 4096);
    //     char test[4096];
    //     for(int i = 0; i < 4096; i++) test[i] = 0;
    //     fs.read_cluster(cluster, (uint8_t *)test, 4096);
    //     kprintf(test);
    // }

    kprintf("\n");
    ProgramManager program_manager;
    Terminal terminal(&program_manager, &keyboard);
    program_manager.add_program(&terminal);
    program_manager.start();

    for (;;)
        ;
}
