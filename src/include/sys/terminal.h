#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>

class Terminal : public Program
{
    ProgramManager *program_manager;

public:
    Terminal(ProgramManager *program_manager, KeyboardDriver *keyboard) : Program(keyboard)
    {
        this->program_manager = program_manager;
    }
    virtual int run(int argc, char *const argv[], char *const env[])
    {
        kprintf("root@host: ");
        for (;;)
            ;
        return 0;
    }
};

#endif
