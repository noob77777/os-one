#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include <types.h>
#include <drivers/display.h>
#include <drivers/ata.h>
#include <filesystem/mbr.h>
#include <filesystem/dir.h>

class FileSystem
{
    static const int FAT_BASE = 0x10;
    static const int BASE = 0x500;
    static const int FS_SIZE = 256; // 1 MiB

public:
    static const int CLUSTER_SIZE = 4096; // 4 KiB

private:
    static const int SECTOR_SIZE = 512;

public:
    static const int ERROR = 0xFF;

private:
    ATA *ata0m;

public:
    uint8_t dir_fd;

private:
    uint32_t get_lba(uint8_t fd);
    uint8_t get_next_cluster(uint8_t fd);
    uint8_t set_next_cluster(uint8_t fd, uint8_t next);
    uint8_t allocate();
    bool allocated(uint8_t fd);
    uint8_t free(uint8_t fd);

public:
    FileSystem(ATA *ata0m);

    uint8_t format_disk();
    uint8_t read(uint8_t fd, uint8_t *data, int n);
    uint8_t write(uint8_t fd, uint8_t *data, int n);
    uint8_t open(const char *file_name);
    uint8_t unlink(uint8_t fd);
};

#endif
