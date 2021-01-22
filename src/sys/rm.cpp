#include <sys/system_software.h>

RM::RM(FileSystem *fs) : Program("rm")
{
    this->fs = fs;
}

int RM::main(int argc, char *const argv[], char *const env[])
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
