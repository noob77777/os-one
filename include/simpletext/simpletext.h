#ifndef __SIMPLETEXT_H
#define __SIMPLETEXT_H

#include <drivers/display.h>
#include <drivers/keyboard_driver.h>
#include <filesystem/filesystem.h>
#include <sys/program.h>
#include <memory/malloc.h>
#include <sys/terminal.h>

namespace simpletext
{
    struct editor_state
    {
        int fd;
        char *buffer;
        int line;
        int idx;
        const char *filename;
    };
}

class SimpleText : public Program
{
    FileSystem *fs;
    KeyboardDriver *keyboard;
    simpletext::editor_state state;

public:
    SimpleText(FileSystem *fs, KeyboardDriver *keyboard);

    virtual int main(int argc, char *const argv[], char *const env[]);
};

#endif
