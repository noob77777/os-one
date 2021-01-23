#include <sys/system_software.h>

FDisk::FDisk(FileSystem *fs) : Program("fdisk")
{
    this->fs = fs;
}

int FDisk::main(int argc, char *const argv[], char *const env[])
{
    if (argc != 2)
    {
        kprintf("fdisk: use fdisk -format to format disk [irreversable operation]\n");
        return 1;
    }
    if (!strcmp(argv[1], "-format"))
    {
        kprintf("fdisk: use fdisk -format to format disk [irreversable operation]\n");
        return 1;
    }
    if (!fs->format_disk())
    {
        kprintf("Disk formatted\n");
    }
    else
    {
        kprintf("Operation failed\n");
    }
    return 0;
}
