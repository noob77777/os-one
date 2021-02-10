#include <sys/exec.h>

namespace exec
{
    void test_loop(uint8_t *buffer)
    {
        // infinite loop [broken code]
        buffer[0] = 0x66;
        buffer[1] = 0xb8;
        buffer[2] = 0x01;
        buffer[3] = 0x00;
        buffer[4] = 0x00;
        buffer[5] = 0x00;
        buffer[6] = 0xeb;
        buffer[7] = 0xf8;
        buffer[8] = 0xc3;
    }

    void test_white(uint8_t *buffer)
    {
        // white
        buffer[0] = 0xb8;
        buffer[1] = 0x00;
        buffer[2] = 0x80;
        buffer[3] = 0x0b;
        buffer[4] = 0x00;
        buffer[5] = 0xbb;
        buffer[6] = 0xff;
        buffer[7] = 0xff;
        buffer[8] = 0xff;
        buffer[9] = 0xff;
        buffer[10] = 0x89;
        buffer[11] = 0x18;
        buffer[12] = 0xc3;
    }
} // namespace exec

Exec::Exec(GlobalDescriptorTable *gdt, KeyboardDriver *keyboard, FileSystem *fs, ProcessManager *process_manager) : Program("exec")
{
    this->fs = fs;
    this->process_manager = process_manager;
    this->gdt = gdt;
    this->keyboard = keyboard;
}

int Exec::main(int argc, char *const argv[], char *const env[])
{
    if (argc == 1)
    {
        kprintf("exec: missing file operand\n");
        return 0;
    }
    if (argc >= 3)
    {
        kprintf("exec: too many files as argument\n");
        return 0;
    }

    const char *filename = argv[1];
    Directory dir;
    uint8_t status = fs->read(fs->dir_fd, (uint8_t *)&dir, sizeof(dir));
    if (status)
    {
        kprintf("exec: filesystem error\n");
        return 1;
    }

    bool file_exists = false;
    for (int i = 0; i < Directory::DIRECTORY_SIZE; i++)
    {
        DirectoryEntry test = dir.directory_entry(i);
        if ((strcmp(test.name(), filename)))
            file_exists = true;
    }

    if (!file_exists)
    {
        kprintf("exec: ");
        kprintf(filename);
        kprintf(": No such file or directory\n");
        return 1;
    }

    int fd = fs->open(filename);
    if (fd == FileSystem::ERROR)
    {
        kprintf("exec: filesystem error\n");
        return 1;
    }

    uint8_t buffer[FileSystem::CLUSTER_SIZE];
    fs->read(fd, buffer, FileSystem::CLUSTER_SIZE);

    uint8_t *loader = (uint8_t *)LOAD_ADDR;
    for (int i = 0; i < FileSystem::CLUSTER_SIZE; i++)
    {
        *loader = buffer[i];
        loader++;
    }

    Process p1(gdt, jump_to_load_addr);
    process_manager->add_process(&p1, 0x07);

    for (;;)
    {
        if (keyboard->SIGINT)
            SIGKILL = 0x01;

        if (SIGKILL != 0)
        {
            keyboard->SIGINT = 0;
            process_manager->remove_process(0x07);
            kprintf("\n");
            break;
        }
    }

    return 0;
}
