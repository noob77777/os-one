#ifndef __MBR_H
#define __MBR_H

#include <types.h>

class MasterBootRecord
{
public:
    uint8_t bootloader[440];
    uint32_t signature;
    uint16_t unused;
    uint8_t primary_partition[64];
    uint16_t magicnumber;
    MasterBootRecord()
    {
        for (int i = 0; i < 440; i++)
            bootloader[i] = 0;
        signature = 7;
        unused = 0;
        for (int i = 0; i < 64; i++)
            primary_partition[i] = 0;
        magicnumber = 0x55AA;
    }
} __attribute__((packed));

#endif
