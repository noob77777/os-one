#include <types.h>
#include <memory/gdt.h>
#include <memory/virtual_memory.h>
#include <memory/malloc.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/display.h>
#include <drivers/keyboard_driver.h>
#include <drivers/ata.h>
#include <drivers/amd_am79c973.h>
#include <filesystem/filesystem.h>
#include <sys/program.h>
#include <sys/terminal.h>
#include <sys/hello.h>
#include <sys/system_software.h>
#include <simpletext/simpletext.h>

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
    display::clear();

    IdentityVirtualMemory vm = IdentityVirtualMemory();
    enable_virtual_memory(vm.cr3());

    MemoryManager::init();

    InterruptManager interrupt_manager(&GDT);
    KeyboardDriver keyboard;
    interrupt_manager.add_driver(&keyboard);
    interrupt_manager.activate();

    display::clear();
    kprintf("OS-ONE (version 1.0-target=i386)\n");

    amd_am79c973 eth0 = amd_am79c973();
    PCIDriverInterface *pci_device_drivers[1];
    pci_device_drivers[0] = &eth0;
    PCIController PCI;
    PCI.init(pci_device_drivers, 1, &interrupt_manager);
    eth0.activate();

    ATA ata0m(true, 0x01F0);
    FileSystem fs(&ata0m);

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
    FDisk fdisk(&fs);
    program_manager.add_program(&fdisk);
    Clear clear;
    program_manager.add_program(&clear);
    SimpleText simpletext(&fs, &keyboard);
    program_manager.add_program(&simpletext);
    program_manager.start();

    for (;;)
        ;
}
