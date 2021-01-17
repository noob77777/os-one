#ifndef __MALLOC_H
#define __MALLOC_H

#include <types.h>

extern "C" void *kernel_heap_start;

class memory_segment_index_entry
{
    uint32_t id;
    uint32_t prev;
    uint32_t next;

public:
    memory_segment_index_entry() { ; }
    memory_segment_index_entry(uint32_t id, uint32_t prev, uint32_t next)
    {
        this->id = id;
        this->prev = prev;
        this->next = next;
    }
} __attribute__((packed));

class MemoryManager
{
public:
    const static int SEGMENT_SIZE = 4096; // 4KiB
    const static int TABLE_SIZE = 2048; // 8MiB
    static memory_segment_index_entry table[TABLE_SIZE];
    static void init()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = memory_segment_index_entry(i, i - 1, i + 1);
    }
};

memory_segment_index_entry MemoryManager::table[MemoryManager::TABLE_SIZE];

void *malloc(size_t size);
void free(void *ptr);

void *operator new(size_t size);
void *operator new[](size_t size);

void *operator new(size_t size, void *ptr);
void *operator new[](size_t size, void *ptr);

void operator delete(void *ptr);
void operator delete[](void *ptr);

#endif
