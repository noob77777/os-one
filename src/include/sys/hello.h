#ifndef __HELLO_H
#define __HELLO_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>

class Hello : public Program
{
public:
    Hello() : Program("hello")
    {
        ;
    }

    virtual int run(int argc, char *const argv[], char *const env[])
    {
        kprintf("Hello!!!\n");
        return 0;
    }
};

#endif
