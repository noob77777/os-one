#ifndef __LS_H
#define __LS_H

#include <types.h>
#include <sys/program.h>
#include <drivers/keyboard_driver.h>
#include <drivers/display.h>
#include <filesystem/filesystem.h>
#include <filesystem/dir.h>

class LS : public Program
{
    FileSystem *fs;

public:
    LS(FileSystem *fs) : Program("ls")
    {
        this->fs = fs;
    }

    virtual int run(int argc, char *const argv[], char *const env[])
    {
        Directory dir;
        uint8_t status = fs->read(fs->dir_fd, (uint8_t *)&dir, sizeof(dir));
        if (status)
            return 1;

        for (int i = 0; i < Directory::DIRECTORY_SIZE; i++)
        {
            DirectoryEntry test = dir.directory_entry(i);
            if (!(strcmp(test.name(), "")))
            {
                kprintf(test.name());
                kprintf("\n");
            }
        }

        return 0;
    }
};

#endif
