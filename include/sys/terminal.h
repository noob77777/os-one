#ifndef __TERMINAL_H
#define __TERMINAL_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>
#include <memory/malloc.h>

namespace terminal
{
    void keyboard_handler(char chr);
} // namespace terminal

class Terminal : public Program
{
    KeyboardDriver *keyboard;

public:
    Terminal(ProgramManager *program_manager, KeyboardDriver *keyboard);

    virtual int main(int argc, char *const argv[], char *const env[]);
};

#endif
