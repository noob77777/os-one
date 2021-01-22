#include <filesystem/filesystem.h>

uint32_t FileSystem::get_lba(uint8_t fd)
{
    return BASE + ((CLUSTER_SIZE / SECTOR_SIZE) * fd);
}

uint8_t FileSystem::get_next_cluster(uint8_t fd)
{
    uint32_t lba = FAT_BASE;
    uint8_t fat[FS_SIZE];
    uint8_t status = ata0m->read(lba, fat, FS_SIZE);
    if (status)
        return ERROR;
    return fat[fd];
}

uint8_t FileSystem::set_next_cluster(uint8_t fd, uint8_t next)
{
    uint32_t lba = FAT_BASE;
    uint8_t fat[FS_SIZE];
    uint8_t status = ata0m->read(lba, fat, FS_SIZE);
    if (status)
        return ERROR;
    fat[fd] = next;
    status = ata0m->write(lba, fat, FS_SIZE);
    if (status)
        return ERROR;
    status = ata0m->flush();
    if (status)
        return ERROR;
    return next;
}

uint8_t FileSystem::allocate()
{
    uint8_t nfree = get_next_cluster(0);
    if (nfree == ERROR)
        return ERROR;
    uint8_t nnfree = get_next_cluster(nfree);
    if (nnfree == ERROR)
        return ERROR;
    uint8_t status = set_next_cluster(nfree, 0);
    if (status == ERROR)
        return ERROR;
    status = set_next_cluster(0, nnfree);
    if (status == ERROR)
        return ERROR;
    if (nfree == 0)
        return ERROR;

    return nfree;
}

bool FileSystem::allocated(uint8_t fd)
{
    return (get_next_cluster(fd) == 0);
}

uint8_t FileSystem::free(uint8_t fd)
{
    if (!allocated(fd))
        return ERROR;

    uint8_t nfree = get_next_cluster(0);
    if (nfree == ERROR)
        return ERROR;
    uint8_t status = set_next_cluster(fd, nfree);
    if (status == ERROR)
        return ERROR;
    status = set_next_cluster(0, fd);
    if (status == ERROR)
        return ERROR;

    return 0;
}

FileSystem::FileSystem(ATA *ata0m)
{
    this->ata0m = ata0m;
    this->dir_fd = 1;

    uint8_t status = 0;
    MasterBootRecord mbr = MasterBootRecord();
    status = ata0m->read(0, (uint8_t *)&mbr, sizeof(mbr));
    if (status)
    {
        kprintf("Disk Error\n");
        return;
    }
    if (mbr.magicnumber == 0x55AA && mbr.signature == 7)
    {
        kprintf("Disk Ready!\n");
        return;
    }
    kprintf("Formatting Disk...\n");
    status = format_disk();
    if (!status)
        kprintf("Disk Ready!\n");
    else
        kprintf("Disk Error\n");
}

uint8_t FileSystem::format_disk()
{
    uint8_t status = 0;
    MasterBootRecord mbr = MasterBootRecord();
    status = ata0m->write(0, (uint8_t *)&mbr, sizeof(mbr));
    if (status)
        return ERROR;
    status = ata0m->flush();
    if (status)
        return ERROR;

    uint8_t fat[FS_SIZE];

    for (int i = 0; i < FS_SIZE; i++)
        fat[i] = i + 1;

    status = ata0m->write(FAT_BASE, fat, FS_SIZE);
    if (status)
        return ERROR;
    status = ata0m->flush();
    if (status)
        return ERROR;

    uint8_t fd = allocate();
    Directory dir = Directory();
    status = write(fd, (uint8_t *)&dir, sizeof(dir));

    return status;
}

uint8_t FileSystem::read(uint8_t fd, uint8_t *data, int n)
{
    if (!allocated(fd))
        return ERROR;

    uint32_t lba = get_lba(fd);
    for (int i = 0; i < n; i += SECTOR_SIZE)
    {
        uint8_t status = ata0m->read(lba, &data[i], min(SECTOR_SIZE, n - i));
        lba++;
        if (status)
            return ERROR;
    }
    return 0;
}

uint8_t FileSystem::write(uint8_t fd, uint8_t *data, int n)
{
    if (!allocated(fd))
        return ERROR;

    uint32_t lba = get_lba(fd);
    for (int i = 0; i < n; i += SECTOR_SIZE)
    {
        uint8_t status = ata0m->write(lba, &data[i], min(SECTOR_SIZE, n - i));
        lba++;
        if (status)
            return ERROR;
        status = ata0m->flush();
        if (status)
            return ERROR;
    }

    return 0;
}

uint8_t FileSystem::open(const char *file_name)
{
    Directory dir;
    uint8_t status = read(dir_fd, (uint8_t *)&dir, sizeof(dir));
    if (status)
        return ERROR;
    uint8_t fd = dir.fd(file_name);
    if (fd == ERROR)
    {
        fd = allocate();
        if (fd == ERROR)
            return ERROR;
        DirectoryEntry file(file_name, fd);
        dir.add(file);
        uint8_t status = write(dir_fd, (uint8_t *)&dir, sizeof(dir));

        if (status)
            return ERROR;
    }
    return fd;
}

uint8_t FileSystem::unlink(uint8_t fd)
{
    Directory dir;
    uint8_t status = read(dir_fd, (uint8_t *)&dir, sizeof(dir));
    if (status)
        return ERROR;
    dir.remove(fd);
    status = write(dir_fd, (uint8_t *)&dir, sizeof(dir));
    if (status)
        return ERROR;
    return free(fd);
}
