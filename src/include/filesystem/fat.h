#ifndef __FAT_H
#define __FAT_H

#include <types.h>
#include <drivers/display.h>
#include <drivers/ata.h>
#include <filesystem/mbr.h>

class FAT
{
public:
    ATA *ataDisk;

    uint8_t format_disk()
    {
        uint8_t status = 0;
        MasterBootRecord mbr = MasterBootRecord();
        status = ataDisk->write(1, (uint8_t *)&mbr, sizeof(mbr));
        if (status)
            return status;

        uint32_t fat[1024 * 1024];
        fat[0] = 2;
        fat[1] = 0;

        for (int i = 2; i < 1024 * 1024; i++)
            fat[i] = i + 1;

        int lba = 8;
        // for (int i = 0; i < 1024 * 1024; i += 128)
        //     status = ataDisk->write(lba++, (uint8_t *)&fat[i], 512);

        status = ataDisk->flush();

        return status;
    }

    uint32_t get_lba(uint32_t cluster_number)
    {
        return 8 * cluster_number;
    }

    uint8_t read_cluster(uint32_t cluster_number, uint8_t *data, int count)
    {
        uint32_t lba = get_lba(cluster_number);

        if (count != 4096)
            return 1;

        uint8_t status = 0;
        for (int i = 0; i < 8; i++)
            status = ataDisk->read(lba++, (data + (i * 512)), 512);

        return status;
    }

    uint8_t write_cluster(uint32_t cluster_number, uint8_t *data, int count)
    {
        uint32_t lba = get_lba(cluster_number);

        uint8_t status = 0;
        for (int i = 0; i < 8; i++)
        {
            uint32_t wdata = (count >= 512 ? 512 : count);
            status = ataDisk->write(lba++, (data + (i * 512)), wdata);
            count = count - wdata;
        }

        status = ataDisk->flush();
        return status;
    }

    uint32_t get_next_cluster(uint32_t cluster_number)
    {
        uint32_t lba = 8 + (cluster_number / 128);
        uint32_t fat[128];
        uint8_t status = ataDisk->read(lba, (uint8_t *)fat, 512);
        if (status)
            return 0xFFFFFFFF;
        return fat[cluster_number % 128];
    }

    uint32_t set_next_cluster(uint32_t cluster_number, uint32_t next)
    {
        uint32_t lba = 8 + (cluster_number / 128);
        uint32_t fat[128];
        uint8_t status = ataDisk->read(lba, (uint8_t *)fat, 512);
        fat[cluster_number % 128] = next;
        status = ataDisk->write(lba, (uint8_t *)fat, 512);
        status = ataDisk->flush();
        if (status)
            return 0xFFFFFFFF;
        return next;
    }

    uint32_t allocate()
    {
        uint32_t nfree = get_next_cluster(0);
        if (nfree == 0xFFFFFFFF)
            return nfree;
        uint32_t nnfree = get_next_cluster(nfree);
        if (nnfree == 0xFFFFFFFF)
            return nnfree;
        uint32_t status = set_next_cluster(nfree, 0);
        if (status == 0xFFFFFFFF)
            return status;
        status = set_next_cluster(0, nnfree);
        if (status == 0xFFFFFFFF)
            return status;
        if (nfree == 0)
            return 0xFFFFFFFF;
        return nfree;
    }

    uint32_t free(uint32_t cluster_number)
    {
        uint32_t nfree = get_next_cluster(0);
        if (nfree == 0xFFFFFFFF)
            return nfree;
        uint32_t status = set_next_cluster(cluster_number, nfree);
        if (status == 0xFFFFFFFF)
            return status;
        status = set_next_cluster(0, cluster_number);
        if (status == 0xFFFFFFFF)
            return status;
        return 0;
    }

public:
    FAT(ATA *ataDisk)
    {
        this->ataDisk = ataDisk;
    }

    void init()
    {
        uint8_t status = 0;
        MasterBootRecord mbr = MasterBootRecord();
        status = ataDisk->read(1, (uint8_t *)&mbr, sizeof(mbr));
        if (mbr.magicnumber == 0x55AA && mbr.signature == 7)
        {
            kprintf("Disk Ready!\n");
            return;
        }
        kprintf("Formatting Disk...\n");
        status = format_disk();
        if (!status)
        {
            kprintf("Disk Ready!\n");
        }
        else
        {
            kprintf("Disk Error\n");
        }
    }

    uint32_t read(uint32_t fd, uint8_t *data, int count)
    {
        return 0;
    }

    uint32_t write(uint32_t fd, uint8_t *data, int count)
    {
        return 0;
    }

    uint8_t seek(uint32_t fd, int offset)
    {
        return 0;
    }

};

#endif
