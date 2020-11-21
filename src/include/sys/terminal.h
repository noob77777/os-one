#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>

namespace terminal
{
    ProgramManager *program_manager = nullptr;

    void keyboard_handler(char chr)
    {
        static char terminal_buffer[80] = "";
        static int idx = 0;
        if (chr == '\b')
        {
            if (idx > 0)
            {
                idx--;
                terminal_buffer[idx] = 0;
                display::backspace();
            }
        }
        else if (chr == '\n')
        {
            kprintf("\n");
            if (strlen(terminal_buffer) != 0)
            {

                Program *ps = program_manager->program(terminal_buffer);
                if (ps == nullptr)
                {
                    kprintf(terminal_buffer);
                    kprintf(": command not found\n");
                }
                else
                {
                    ps->run(0, nullptr, nullptr);
                }
            }
            kprintf("root@host: ");
            idx = 0;
            terminal_buffer[idx] = 0;
        }
        else
        {
            const char string[2] = {chr, '\0'};
            kprintf(string);
            terminal_buffer[idx++] = chr;
            idx = idx % 80;
            terminal_buffer[idx] = '\0';
        }
    }
} // namespace terminal

class Terminal : public Program
{
    KeyboardDriver *keyboard;

public:
    Terminal(ProgramManager *program_manager, KeyboardDriver *keyboard) : Program("terminal")
    {
        this->keyboard = keyboard;
        terminal::program_manager = program_manager;
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
