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
    MasterBootRecord();
} __attribute__((packed));

#endif
