#include <memory/malloc.h>

uint32_t MemoryManager::table_sm[MemoryManager::TABLE_SIZE_SM];
uint32_t MemoryManager::table_m[MemoryManager::TABLE_SIZE_M];
uint32_t MemoryManager::table_l[MemoryManager::TABLE_SIZE_L];
uint32_t MemoryManager::USAGE_SM = 0;
uint32_t MemoryManager::USAGE_M = 0;
uint32_t MemoryManager::USAGE_L = 0;

void MemoryManager::init()
{
    for (int i = 0; i < TABLE_SIZE_SM; i++)
        table_sm[i] = i + 1;
    for (int i = 0; i < TABLE_SIZE_M; i++)
        table_m[i] = i + 1;
    for (int i = 0; i < TABLE_SIZE_L; i++)
        table_l[i] = i + 1;
}

void *MemoryManager::malloc(size_t size)
{
    if (size == 0)
        return nullptr;
    if (size <= SM)
    {
        int free_index = table_sm[0];
        if (free_index == TABLE_SIZE_SM)
            return nullptr;
        USAGE_SM++;
        table_sm[0] = table_sm[free_index];
        table_sm[free_index] = TABLE_SIZE_SM;
        return ((&kernel_heap_start) + SM_OFFSET + free_index * SM);
    }
    if (size <= M)
    {
        int free_index = table_m[0];
        if (free_index == TABLE_SIZE_M)
            return nullptr;
        USAGE_M++;
        table_m[0] = table_m[free_index];
        table_m[free_index] = TABLE_SIZE_M;
        return ((&kernel_heap_start) + M_OFFSET + free_index * M);
    }
    if (size <= L)
    {
        int free_index = table_l[0];
        if (free_index == TABLE_SIZE_L)
            return nullptr;
        USAGE_L++;
        table_l[0] = table_l[free_index];
        table_l[free_index] = TABLE_SIZE_L;
        return ((&kernel_heap_start) + L_OFFSET + free_index * L);
    }
    return nullptr;
}

void MemoryManager::free(void *ptr)
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
        USAGE_SM--;
        int free_index = (offset - SM_OFFSET) / SM;
        table_sm[free_index] = table_sm[0];
        table_sm[0] = free_index;
        return;
    }
    if (offset < L_OFFSET)
    {
        USAGE_M--;
        int free_index = (offset - M_OFFSET) / M;
        table_m[free_index] = table_m[0];
        table_m[0] = free_index;
        return;
    }
    if (offset >= L_OFFSET && offset < 2 * L_OFFSET)
    {
        USAGE_L--;
        int free_index = (offset - L_OFFSET) / L;
        table_l[free_index] = table_l[0];
        table_l[0] = free_index;
        return;
    }
}
void MemoryManager::status()
{
    kprintf("USAGE_SM: ");
    kprintf_hex(USAGE_SM);
    kprintf("\n");
    kprintf("USAGE_M: ");
    kprintf_hex(USAGE_M);
    kprintf("\n");
    kprintf("USAGE_L: ");
    kprintf_hex(USAGE_L);
    kprintf("\n");
}

void *operator new(size_t size)
{
    return MemoryManager::malloc(size);
}
void *operator new[](size_t size)
{
    return MemoryManager::malloc(size);
}

void operator delete(void *ptr)
{
    MemoryManager::free(ptr);
}
void operator delete[](void *ptr)
{
    MemoryManager::free(ptr);
}
