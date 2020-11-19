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
    DirectoryEntry(const char *file_name = "", uint8_t fd = 0xFF)
    {
        strcpy(this->file_name, file_name);
        this->fd = fd;
    }
    const char *name()
    {
        return file_name;
    }
    
    friend class Directory;
} __attribute__((packed));

class Directory
{
    const static int DIRECTORY_SIZE = 128;
    DirectoryEntry dir[DIRECTORY_SIZE];

public:
    Directory() { ; }

    bool add(DirectoryEntry file)
    {
        for (int i = 0; i < DIRECTORY_SIZE; i++)
        {
            if (dir[i].fd == 0xFF)
            {
                dir[i] = file;
                return true;
            }
        }
        return false;
    }

    bool remove(uint8_t fd)
    {
        for (int i = 0; i < DIRECTORY_SIZE; i++)
        {
            if (dir[i].fd == fd)
            {
                dir[i] = DirectoryEntry();
                return true;
            }
        }
        return false;
    }

    uint8_t fd(const char *file_name)
    {
        for (int i = 0; i < DIRECTORY_SIZE; i++)
        {
            if (strcmp(dir[i].file_name, file_name))
            {
                return dir[i].fd;
            }
        }
        return 0xFF;
    }

    DirectoryEntry directory_entry(int i)
    {
        return dir[i];
    }

} __attribute__((packed));

#endif
