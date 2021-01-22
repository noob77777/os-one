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
    static uint32_t USAGE_SM;
    static uint32_t USAGE_M;
    static uint32_t USAGE_L;

    static void init();
    static void *malloc(size_t size);
    static void free(void *ptr);
    static void status();
};

void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *ptr);
void operator delete[](void *ptr);

#endif
