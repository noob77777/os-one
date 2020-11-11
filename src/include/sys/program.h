#ifndef __PROGRAM_H
#define __PROGRAM_H

#include <types.h>
#include <drivers/keyboard_driver.h>

class Terminal;

class Program
{
protected:
    KeyboardDriver *keyboard;
    Program(KeyboardDriver *keyboard)
    {
        this->keyboard = keyboard;
    }

public:
    virtual int run(int argc, char *const argv[], char *const env[])
    {
        return 0;
    }
};

class ProgramManager
{
    static const int PSMAX = 256;
    Program *ps[PSMAX];
    int ptr;

public:
    ProgramManager()
    {
        ptr = 0;
    }
    void add_program(Program *program)
    {
        ps[ptr++] = program;
    }
    void start()
    {
        ps[0]->run(0, nullptr, nullptr);
    }
    friend class Terminal;
};

#endif
