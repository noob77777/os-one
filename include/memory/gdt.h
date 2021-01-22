#ifndef __GDT_H
#define __GDT_H

#include <types.h>

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
    SegmentDescriptor(uint32_t base = 0x0, uint32_t limit = 0x0, uint8_t type = 0x0);
    uint32_t base();
    uint32_t limit();

} __attribute__((packed));

class GlobalDescriptorTable
{
    const static int NUMBER_OF_SEGMENTS = 3;

    SegmentDescriptor gdt[NUMBER_OF_SEGMENTS];
    uint16_t limit;
    uint32_t base;

public:
    GlobalDescriptorTable();

    uint32_t code_segment();

} __attribute__((packed));

#endif
