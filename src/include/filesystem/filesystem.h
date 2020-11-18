#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <types.h>
#include <drivers/display.h>
#include <drivers/ata.h>
#include <filesystem/mbr.h>

class FileSystem
{
    static const int BASE = 0x10;
    static const int FS_SIZE = 256; // 128 KiB
    ATA *ataDisk;

public:
    uint8_t format_disk()
    {
        uint8_t status = 0;
        MasterBootRecord mbr = MasterBootRecord();
        status = ataDisk->write(0, (uint8_t *)&mbr, sizeof(mbr));
        status = ataDisk->flush();
        if (status)
            return status;

        uint8_t fat[FS_SIZE];

        for (int i = 0; i < FS_SIZE; i++)
            fat[i] = i + 1;

        status = ataDisk->write(BASE, fat, FS_SIZE);
        status = ataDisk->flush();

        return status;
    }

    uint32_t get_lba(uint8_t fd)
    {
        return 0x20 + fd;
    }

    uint8_t get_next_cluster(uint8_t fd)
    {
        uint32_t lba = BASE;
        uint8_t fat[FS_SIZE];
        uint8_t status = ataDisk->read(lba, fat, FS_SIZE);
        if (status)
            return 0xFF;
        return fat[fd];
    }

    uint8_t set_next_cluster(uint8_t fd, uint8_t next)
    {
        uint32_t lba = BASE;
        uint8_t fat[FS_SIZE];
        uint8_t status = ataDisk->read(lba, fat, FS_SIZE);
        fat[fd] = next;
        status = ataDisk->write(lba, fat, FS_SIZE);
        status = ataDisk->flush();
        if (status)
            return 0xFF;
        return next;
    }

    uint8_t allocate()
    {
        uint8_t nfree = get_next_cluster(0);
        if (nfree == 0xFF)
            return nfree;
        uint8_t nnfree = get_next_cluster(nfree);
        if (nnfree == 0xFF)
            return nnfree;
        uint8_t status = set_next_cluster(nfree, 0);
        if (status == 0xFF)
            return status;
        status = set_next_cluster(0, nnfree);
        if (status == 0xFF)
            return status;
        if (nfree == 0)
            return 0xFF;
        return nfree;
    }

    uint8_t free(uint8_t fd)
    {
        uint8_t nfree = get_next_cluster(0);
        if (nfree == 0xFF)
            return nfree;
        uint8_t status = set_next_cluster(fd, nfree);
        if (status == 0xFF)
            return status;
        status = set_next_cluster(0, fd);
        if (status == 0xFF)
            return status;
        return 0;
    }

    bool allocated(uint8_t fd)
    {
        return (get_next_cluster(fd) == 0);
    }

public:
    FileSystem(ATA *ataDisk)
    {
        this->ataDisk = ataDisk;
    }

    void init()
    {
        uint8_t status = 0;
        MasterBootRecord mbr = MasterBootRecord();
        status = ataDisk->read(0, (uint8_t *)&mbr, sizeof(mbr));
        // if (mbr.magicnumber == 0x55AA && mbr.signature == 7)
        // {
        //     kprintf("Disk Ready!\n");
        //     return;
        // }
        kprintf("Formatting Disk...\n");
        status = format_disk();
        if (!status)
            kprintf("Disk Ready!\n");
        else
            kprintf("Disk Error\n");
    }

    uint8_t read(uint8_t fd, uint8_t *data, int count)
    {
        if (!allocated(fd)) return 0xFF;
        uint32_t lba = get_lba(fd);
        uint8_t status = ataDisk->read(lba, data, count);
        return status;
    }

    uint8_t write(uint8_t fd, uint8_t *data, int count)
    {
        if (!allocated(fd)) return 0xFF;
        uint32_t lba = get_lba(fd);
        uint8_t status = ataDisk->write(lba, data, count);
        status = ataDisk->flush();
        return status;
    }

    uint8_t unlink(uint8_t fd)
    {
        return free(fd);
    }
};

#endif
