#ifndef __MALLOC_H
#define __MALLOC_H

#include <types.h>
#include <drivers/display.h>

extern "C" char kernel_heap_start;

class MemoryManager
{
public:
    const static int SM = 512;             // 512 B
    const static int M = 4096;             // 4 KiB
    const static int L = 1024 * 1024;      // 1 MiB
    const static int TABLE_SIZE_SM = 4096; // 2 MiB
    const static int TABLE_SIZE_M = 512;   // 2 MiB
    const static int TABLE_SIZE_L = 4;     // 4 MiB
    const static int SM_OFFSET = 0;
    const static int M_OFFSET = 2 * 1024 * 1024;
    const static int L_OFFSET = 4 * 1024 * 1024;
    static uint32_t table_sm[TABLE_SIZE_SM];
    static uint32_t table_m[TABLE_SIZE_M];
    static uint32_t table_l[TABLE_SIZE_L];
    static void init()
    {
        for (int i = 0; i < TABLE_SIZE_SM; i++)
            table_sm[i] = i + 1;
        for (int i = 0; i < TABLE_SIZE_M; i++)
            table_m[i] = i + 1;
        for (int i = 0; i < TABLE_SIZE_L; i++)
            table_l[i] = i + 1;
    }
    static void *malloc(size_t size)
    {
        if (size == 0)
            return nullptr;
        if (size <= SM)
        {
            int free_index = table_sm[0];
            if (free_index == TABLE_SIZE_SM)
                return nullptr;
            table_sm[0] = table_sm[free_index];
            table_sm[free_index] = TABLE_SIZE_SM;
            return ((&kernel_heap_start) + SM_OFFSET + free_index * SM);
        }
        if (size <= M)
        {
            int free_index = table_m[0];
            if (free_index == TABLE_SIZE_M)
                return nullptr;
            table_m[0] = table_m[free_index];
            table_m[free_index] = TABLE_SIZE_M;
            return ((&kernel_heap_start) + M_OFFSET + free_index * M);
        }
        if (size <= L)
        {
            int free_index = table_l[0];
            if (free_index == TABLE_SIZE_L)
                return nullptr;
            table_l[0] = table_l[free_index];
            table_l[free_index] = TABLE_SIZE_L;
            return ((&kernel_heap_start) + L_OFFSET + free_index * L);
        }
        return nullptr;
    }
    static void free(void *ptr)
    {
        if (ptr == nullptr)
            return;
        int address = reinterpret_cast<int>(ptr);
        int base = reinterpret_cast<int>(&kernel_heap_start);
        int offset = address - base;
        if (offset < 0)
            return;
        if (offset < M_OFFSET)
        {
            int free_index = (offset - SM_OFFSET) / SM;
            table_sm[free_index] = table_sm[0];
            table_sm[0] = free_index;
            return;
        }
        if (offset < L_OFFSET)
        {
            int free_index = (offset - M_OFFSET) / M;
            table_m[free_index] = table_m[0];
            table_m[0] = free_index;
            return;
        }
        if (offset >= L_OFFSET && offset < 2 * L_OFFSET)
        {
            int free_index = (offset - L_OFFSET) / L;
            table_l[free_index] = table_l[0];
            table_l[0] = free_index;
            return;
        }
    }
};

uint32_t MemoryManager::table_sm[MemoryManager::TABLE_SIZE_SM];
uint32_t MemoryManager::table_m[MemoryManager::TABLE_SIZE_M];
uint32_t MemoryManager::table_l[MemoryManager::TABLE_SIZE_L];

void *operator new(size_t size)
{
    return MemoryManager::malloc(size);
}
void *operator new[](size_t size)
{
    return MemoryManager::malloc(size);
}

void *operator new(size_t size, void *ptr);
void *operator new[](size_t size, void *ptr);

void operator delete(void *ptr)
{
    MemoryManager::free(ptr);
}
void operator delete[](void *ptr)
{
    MemoryManager::free(ptr);
}

#endif
