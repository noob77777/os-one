#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>

namespace terminal
{
    void keyboard_handler(char chr)
    {
        const char string[2] = {chr, '\0'};
        kprintf(string);
        if (chr == '\n')
        {
            kprintf("root@host: ");
        }
    }
} // namespace terminal

class Terminal : public Program
{
    ProgramManager *program_manager;

public:
    Terminal(ProgramManager *program_manager, KeyboardDriver *keyboard) : Program(keyboard)
    {
        this->program_manager = program_manager;
        keyboard->on_key_press(terminal::keyboard_handler);
    }

    virtual int run(int argc, char *const argv[], char *const env[])
    {
        kprintf("root@host: ");

        for (;;)
        {
            if (keyboard->SIGINT)
            {
                kprintf("\n^C\n");
                keyboard->reset();
                break;
            }
        }

        return 0;
    }
};

#endif
