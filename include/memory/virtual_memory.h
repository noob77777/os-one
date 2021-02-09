#ifndef __VIRTUAL_MEMORY_H
#define __VIRTUAL_MEMORY_H

#include <types.h>

extern "C" void enable_virtual_memory(uint32_t cr3);

class PageDirectoryEntry
{
    uint32_t pde;

public:
    PageDirectoryEntry();
    PageDirectoryEntry(uint32_t base, uint8_t valid, uint8_t write_enable, uint8_t user_mode);

} __attribute__((packed));

class PageTableEntry
{
    uint32_t pte;

public:
    PageTableEntry();
    PageTableEntry(uint32_t base, uint8_t valid, uint8_t write_enable, uint8_t user_mode);

} __attribute__((packed));

class PageDirectory
{
    PageDirectoryEntry page_directory[1024];

public:
    PageDirectory();

    void set_entry(int i, PageDirectoryEntry pde);

} __attribute__((packed));

class PageTable
{
    PageTableEntry page_table[1024];

public:
    PageTable();

    void set_entry(int i, PageTableEntry pde);

} __attribute__((packed));

class VirtualMemory
{
protected:
    PageDirectory page_directory __attribute__((aligned(4096)));

    VirtualMemory();

public:
    virtual uint32_t cr3();
};

class IdentityVirtualMemory : public VirtualMemory
{
    PageTable page_table_array[16] __attribute__((aligned(4096))); // 64 MiB

public:
    IdentityVirtualMemory();
    virtual uint32_t cr3();
};

/*
 * 32 MiB with Base 0x00000000 => kernel
 * 16 MiB with Base 0xF0000000 => user loaded programs
 */
class SplitVirtualMemory : public VirtualMemory
{
    PageTable page_table_array[12] __attribute__((aligned(4096))); // 32 + 16 MiB
    const static int OFFSET = (0xF0000000 / (4096 * 1024));
public:
    SplitVirtualMemory();
    virtual uint32_t cr3();
};

#endif
