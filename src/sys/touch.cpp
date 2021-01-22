#include <sys/system_software.h>

Touch::Touch(FileSystem *fs) : Program("touch")
{
    this->fs = fs;
}

int Touch::main(int argc, char *const argv[], char *const env[])
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
