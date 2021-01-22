#include <filesystem/dir.h>

DirectoryEntry::DirectoryEntry(const char *file_name, uint8_t fd)
{
    strcpy(this->file_name, file_name);
    this->fd = fd;
}
const char *DirectoryEntry::name()
{
    return file_name;
}

Directory::Directory() { ; }

bool Directory::add(DirectoryEntry file)
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

bool Directory::remove(uint8_t fd)
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

uint8_t Directory::fd(const char *file_name)
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

DirectoryEntry Directory::directory_entry(int i)
{
    return dir[i];
}
