#ifndef __TOUCH_H
#define __TOUCH_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>
#include <filesystem/filesystem.h>
#include <filesystem/dir.h>

class Touch : public Program
{
    FileSystem *fs;

public:
    Touch(FileSystem *fs) : Program("touch")
    {
        this->fs = fs;
    }

    virtual int run(int argc, char *const argv[], char *const env[])
    {
        if (argc == 1)
        {
            kprintf("touch: missing file operand\n");
            return 0;
        }
        if (argc >= 3)
        {
            kprintf("touch: too many files as argument\n");
            return 0;
        }

        if (fs->open(argv[1]) == FileSystem::ERROR)
            return 1;
        return 0;
    }
};

#endif
