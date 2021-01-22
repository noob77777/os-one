#ifndef __ATA_H
#define __ATA_H

#include <types.h>
#include <hardware/port.h>
#include <drivers/display.h>

/*
 * ATA Disk Driver
 */
class ATA
{
    bool master;
    Port16bit data_port;
    Port8bit error_port;
    Port8bit sector_count_port;
    Port8bit lba_low_port;
    Port8bit lba_mid_port;
    Port8bit lba_hi_port;
    Port8bit device_port;
    Port8bit command_port;
    Port8bit control_port;

public:
    ATA(bool master, uint16_t port_base);

    uint8_t identify();
    uint8_t read(uint32_t sector_num, uint8_t *data, int count);
    uint8_t write(uint32_t sector_num, uint8_t *data, uint32_t count);
    uint8_t flush();
    static void ata_check();
};

#endif
