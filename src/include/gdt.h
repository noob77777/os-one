#ifndef __GDT_H
#define __GDT_H

#include "types.h"

class SegmentDescriptor
{
private:
    uint16_t limit_lo;
    uint16_t base_lo;
    uint8_t base_hi;
    uint8_t type;
    uint8_t flags_limit_hi;
    uint8_t base_vhi;

public:
    SegmentDescriptor(uint32_t base = 0x0, uint32_t limit = 0x0, uint8_t type = 0x0)
    {
        uint8_t *target = (uint8_t *)this;

        limit = limit >> 12;
        target[6] = 0xC0;

        // limit
        target[0] = limit & 0xFF;
        target[1] = (limit >> 8) & 0xFF;
        target[6] |= (limit >> 16) & 0xF;

        // base
        target[2] = base & 0xFF;
        target[3] = (base >> 8) & 0xFF;
        target[4] = (base >> 16) & 0xFF;
        target[7] = (base >> 24) & 0xFF;

        // type
        target[5] = type;
    }
    uint32_t base()
    {
        uint8_t *target = (uint8_t *)this;

        uint32_t result = target[7];
        result = (result << 8) + target[4];
        result = (result << 8) + target[3];
        result = (result << 8) + target[2];

        return result;
    }
    uint32_t limit()
    {
        uint8_t *target = (uint8_t *)this;

        uint32_t result = target[6] & 0xF;
        result = (result << 8) + target[1];
        result = (result << 8) + target[0];
        result = (result << 12);

        return result;
    }
} __attribute__((packed));

class GlobalDescriptorTable
{
    const static int NUMBER_OF_SEGMENTS = 3;

    SegmentDescriptor gdt[NUMBER_OF_SEGMENTS];
    uint16_t limit;
    uint32_t base;

public:
    GlobalDescriptorTable()
    {
        gdt[0] = SegmentDescriptor();
        gdt[1] = SegmentDescriptor(0x0, 0xFFFFFFFF, 0x9A);
        gdt[2] = SegmentDescriptor(0x0, 0xFFFFFFFF, 0x92);

        limit = (sizeof(gdt) - 1);
        base = (uint32_t)this;

        __asm__ volatile("lgdt (%0)"
                         :
                         : "p"(((uint8_t *)&limit)));
    }
    uint32_t code_segment()
    {
        return 0x08;
    }

} __attribute__((packed));

#endif
