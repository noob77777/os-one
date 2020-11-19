#include <types.h>
#include <memory/gdt.h>
#include <memory/virtual_memory.h>
#include <hardware/interrupts.h>
#include <drivers/display.h>
#include <drivers/keyboard_driver.h>
#include <drivers/ata.h>
#include <filesystem/filesystem.h>
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
    ATA ata0m(true, 0x01F0);
    FileSystem fs(&ata0m);
    fs.format_disk();

    // fs.open("testfile");

    // Directory dir;
    // fs.read(1, (uint8_t *)&dir);

    // for(int i = 0; i < 16; i++)
    // {
    //     kprintf(dir.directory_entry(i).name());
    //     kprintf("\n");
    // }

    for (int i = 0; i < 250; i++)
    {
        kprintf_hex8(fs.get_next_cluster(i));
        kprintf("\n");
    }

    for (int i = 0; i < 250; i++) {
        uint8_t fd = fs.allocate();
        kprintf_hex8(fd);
        char text[4096] = "test-disk";
        fs.write(fd, (uint8_t *)text);
        char read[4096] = "";
        fs.read(fd, (uint8_t *)read);
        kprintf(read);
        kprintf("\n");
    }

    kprintf("\n");
    ProgramManager program_manager;
    Terminal terminal(&program_manager, &keyboard);
    program_manager.add_program(&terminal);
    program_manager.start();

    for (;;)
        ;
}
