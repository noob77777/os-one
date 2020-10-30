#ifndef GDT
#define GDT

#include "types.h"

struct gdt_entry
{
    uint32_t gdt_l, gdt_h;
};

struct gdt_descriptor
{
    uint16_t gdt_len;
    gdt_entry *gdt_ptr;
};

#endif
