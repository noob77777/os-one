#ifndef __EXEC_H
#define __EXEC_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>
#include <filesystem/filesystem.h>
#include <processmanager/processmanager.h>

/*
 * Do not change this value.
 * To change LOAD_ADDR also need to change virtual memory setup
 * and exec_loader
 */
const static int LOAD_ADDR = 0xF0000000;

/*
 * defined in exec_loader
 */
extern "C" void jump_to_load_addr();

/*
 * SIGKILL defined in exec_loader
 */
extern "C" uint32_t SIGKILL;

class Exec : public Program
{
    FileSystem *fs;
    ProcessManager *process_manager;
    GlobalDescriptorTable *gdt;
    KeyboardDriver *keyboard;

public:
    Exec(GlobalDescriptorTable *gdt, KeyboardDriver *keyboard, FileSystem *fs, ProcessManager *process_manager);
    virtual int main(int argc, char *const argv[], char *const env[]);
};

#endif
