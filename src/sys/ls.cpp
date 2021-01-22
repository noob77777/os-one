#include <sys/system_software.h>

LS::LS(FileSystem *fs) : Program("ls")
{
    this->fs = fs;
}

int LS::main(int argc, char *const argv[], char *const env[])
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

