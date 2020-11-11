#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <types.h>
#include <sys/process.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>

class Terminal : public Process
{
    ProcessManager *process_manager;

public:
    Terminal(ProcessManager *process_manager, KeyboardDriver *keyboard) : Process(keyboard)
    {
        this->process_manager = process_manager;
    }
    virtual int run(int argc, char *const argv[], char *const env[])
    {
        kprintf("Terminal\n");
        for (;;)
            ;
        return 0;
    }
};

#endif
