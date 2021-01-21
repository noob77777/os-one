#ifndef __RM_H
#define __RM_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>
#include <filesystem/filesystem.h>
#include <filesystem/dir.h>

class RM : public Program
{
    FileSystem *fs;

public:
    RM(FileSystem *fs) : Program("rm")
    {
        this->fs = fs;
    }

    virtual int run(int argc, char *const argv[], char *const env[])
    {
        if (argc == 1)
        {
            kprintf("rm: missing file operand\n");
            return 0;
        }
        if (argc >= 3)
        {
            kprintf("rm: too many files as argument\n");
            return 0;
        }

        return (fs->unlink(fs->open(argv[1])) != FileSystem::ERROR);
    }
};

#endif