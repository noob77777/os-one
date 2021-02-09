#include <memory/virtual_memory.h>

PageDirectoryEntry::PageDirectoryEntry() { pde = 0; }

PageDirectoryEntry::PageDirectoryEntry(uint32_t base, uint8_t valid, uint8_t write_enable, uint8_t user_mode)
{
    pde = (base & 0xFFFFF000);
    if (valid)
        pde = (pde | 0x00000001);
    if (write_enable)
        pde = (pde | 0x00000002);
    if (user_mode)
        pde = (pde | 0x00000004);
}

PageTableEntry::PageTableEntry() { pte = 0; }

PageTableEntry::PageTableEntry(uint32_t base, uint8_t valid, uint8_t write_enable, uint8_t user_mode)
{
    pte = (base & 0xFFFFF000);
    if (valid)
        pte = (pte | 0x00000001);
    if (write_enable)
        pte = (pte | 0x00000002);
    if (user_mode)
        pte = (pte | 0x00000004);
}

PageDirectory::PageDirectory() { ; }

void PageDirectory::set_entry(int i, PageDirectoryEntry pde)
{
    page_directory[i] = pde;
}

PageTable::PageTable() { ; }

void PageTable::set_entry(int i, PageTableEntry pde)
{
    page_table[i] = pde;
}

VirtualMemory::VirtualMemory() { ; }

uint32_t VirtualMemory::cr3() { return 0; }

IdentityVirtualMemory::IdentityVirtualMemory() : VirtualMemory()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 1024; j++)
            page_table_array[i].set_entry(j, PageTableEntry(((i * 1024) + j) * 0x1000, 1, 1, 0));

        page_directory.set_entry(i, PageDirectoryEntry((uint32_t)&page_table_array[i], 1, 1, 0));
    }
}

uint32_t IdentityVirtualMemory::cr3()
{
    return ((uint32_t)&page_directory);
}

#include <drivers/display.h>

SplitVirtualMemory::SplitVirtualMemory() : VirtualMemory()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 1024; j++)
            page_table_array[i].set_entry(j, PageTableEntry(((i * 1024) + j) * 0x1000, 1, 1, 0));

        page_directory.set_entry(i, PageDirectoryEntry((uint32_t)&page_table_array[i], 1, 1, 0));
    }

    for (int i = 8; i < 12; i++)
    {
        for (int j = 0; j < 1024; j++)
            page_table_array[i].set_entry(j, PageTableEntry(((i * 1024) + j) * 0x1000, 1, 1, 0));

        page_directory.set_entry(i + OFFSET - 8, PageDirectoryEntry((uint32_t)&page_table_array[i], 1, 1, 0));
    }
}

uint32_t SplitVirtualMemory::cr3()
{
    return ((uint32_t)&page_directory);
}
