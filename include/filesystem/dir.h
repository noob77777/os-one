#ifndef __DIR_H
#define __DIR_H

#include <types.h>
#include <lib/string.h>

class Directory;

class DirectoryEntry
{
    char file_name[31];
    uint8_t fd;

public:
    DirectoryEntry(const char *file_name = "", uint8_t fd = 0xFF);

    const char *name();

    friend class Directory;
} __attribute__((packed));

class Directory
{
public:
    const static int DIRECTORY_SIZE = 128;

private:
    DirectoryEntry dir[DIRECTORY_SIZE];

public:
    Directory();

    bool add(DirectoryEntry file);
    bool remove(uint8_t fd);
    uint8_t fd(const char *file_name);
    DirectoryEntry directory_entry(int i);

} __attribute__((packed));

#endif
