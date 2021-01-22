#ifndef __PROGRAM_H
#define __PROGRAM_H

#include <types.h>
#include <drivers/keyboard_driver.h>
#include <lib/string.h>

class Program
{
    const char *name;

protected:
    Program(const char *name);

public:
    virtual int main(int argc, char *const argv[], char *const env[]);
    const char *get_name();
};

class ProgramManager
{
    static const int PSMAX = 256;
    Program *ps[PSMAX];
    int ptr;

public:
    ProgramManager();
    void add_program(Program *program);
    void start();
    Program *program(const char *name);
};

#endif
