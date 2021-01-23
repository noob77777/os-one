#ifndef __SYSTEM_SOFTWARE_H
#define __SYSTEM_SOFTWARE_H

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
    LS(FileSystem *fs);

    virtual int main(int argc, char *const argv[], char *const env[]);
};

class RM : public Program
{
    FileSystem *fs;

public:
    RM(FileSystem *fs);

    virtual int main(int argc, char *const argv[], char *const env[]);
};

class Touch : public Program
{
    FileSystem *fs;

public:
    Touch(FileSystem *fs);

    virtual int main(int argc, char *const argv[], char *const env[]);
};

class FDisk : public Program
{
    FileSystem *fs;

public:
    FDisk(FileSystem *fs);

    virtual int main(int argc, char *const argv[], char *const env[]);
};

class Clear : public Program
{
public:
    Clear();
    virtual int main(int argc, char *const argv[], char *const env[]);
};

#endif
