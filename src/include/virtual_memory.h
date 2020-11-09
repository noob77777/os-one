#ifndef __VIRTUAL_MEMORY_H
#define __VIRTUAL_MEMORY_H

#include "types.h"
#include "../drivers/display.h"

class PageDirectoryEntry
{
    uint32_t pde;

public:
    PageDirectoryEntry() { pde = 0; }
    PageDirectoryEntry(uint32_t base, uint8_t write_enable, uint8_t user_mode)
    {
        pde = (base & 0xFFFFF001);
        if (write_enable)
            pde = (pde | 0x00000002);
        if (user_mode)
            pde = (pde | 0x00000004);
    }
} __attribute__((packed));

class PageTableEntry
{
    uint32_t pte;

public:
    PageTableEntry() { pte = 0; }
    PageTableEntry(uint32_t base, uint8_t write_enable, uint8_t user_mode)
    {
        pte = (base & 0xFFFFF001);
        if (write_enable)
            pte = (pte | 0x00000002);
        if (user_mode)
            pte = (pte | 0x00000004);
    }
} __attribute__((packed));

class VirtualMemory;

class PageDirectory
{
    PageDirectoryEntry page_directory[1024];

public:
    PageDirectory()
    {
    }
    void set_entry(int i, PageDirectoryEntry pde)
    {
        page_directory[i] = pde;
    }
} __attribute__((packed));

class PageTable
{
    PageTableEntry page_table[1024];

public:
    PageTable()
    {
    }
    void set_entry(int i, PageTableEntry pde)
    {
        page_table[i] = pde;
    }
} __attribute__((packed));

class VirtualMemory
{
protected:
    static const int SZ = 1024;
    PageDirectory page_directory __attribute__((aligned(4096)));
    PageTable page_table_array[SZ] __attribute__((aligned(4096)));

    VirtualMemory() { ; }

public:
    virtual uint32_t cr3() { return 0; }
};

class IdentityVirtualMemory : public VirtualMemory
{

public:
    IdentityVirtualMemory() : VirtualMemory()
    {
        kprintf_hex((uint32_t)&page_directory);
        kprintf_hex((uint32_t)&page_table_array);

        for (int i = 0; i < SZ; i++)
        {
            for (int j = 0; j < SZ; j++)
                page_table_array[i].set_entry(j, PageTableEntry(((i * SZ) + j) << 12, 1, 0));

            page_directory.set_entry(i, PageDirectoryEntry((uint32_t)&page_table_array[i], 1, 0));
        }
    }
    virtual uint32_t cr3()
    {
        return ((uint32_t)&page_directory);
    }
};

extern "C" void enable_virtual_memory(uint32_t cr3);

#endif
