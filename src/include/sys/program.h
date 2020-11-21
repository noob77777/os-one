#ifndef __PROGRAM_H
#define __PROGRAM_H

#include <types.h>
#include <drivers/keyboard_driver.h>
#include <lib/string.h>

class Terminal;

class Program
{
    const char *name;

protected:
    Program(const char *name)
    {
        this->name = name;
    }

public:
    virtual int run(int argc, char *const argv[], char *const env[])
    {
        return 0;
    }

    const char *get_name()
    {
        return this->name;
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
    Program *program(const char *name)
    {
        for (int i = 0; i < ptr; i++)
            if (strcmp(ps[i]->get_name(), name))
                return ps[i];

        return nullptr;
    }
};

#endif
