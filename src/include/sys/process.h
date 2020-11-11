#ifndef __PROCESS_H
#define __PROCESS_H

#include <types.h>
#include <drivers/keyboard_driver.h>

class Terminal;

class Process
{
protected:
    KeyboardDriver *keyboard;
    Process(KeyboardDriver *keyboard)
    {
        this->keyboard = keyboard;
    }

public:
    virtual int run(int argc, char *const argv[], char *const env[])
    {
        return 0;
    }
};

class ProcessManager
{
    static const int PSMAX = 256;
    Process *ps[PSMAX];
    int ptr;

public:
    ProcessManager()
    {
        ptr = 0;
    }
    void add_process(Process *process)
    {
        ps[ptr++] = process;
    }
    void start()
    {
        ps[0]->run(0, nullptr, nullptr);
    }
    friend class Terminal;
};

class Test : public Process
{

public:
    Test(KeyboardDriver *keyboard) : Process(keyboard) { ; }
    virtual int run(int argc, char *const argv[], char *const env[])
    {
        kprintf("Test\n");
        return 0;
    }
};

#endif
