#ifndef __PROCESSMANAGER_H
#define __PROCESSMANAGER_H

#include <types.h>
#include <memory/gdt.h>
#include <drivers/display.h>
#include <processmanager/scheduler.h>

namespace processmanager
{
    void test1();
    void test2();
    void testmain();
} // namespace processmanager

struct process_state
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    uint32_t error;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
} __attribute__((packed));

class Process
{
    const static int STACK_SIZE = 4096; // 4 KiB

    uint8_t stack[STACK_SIZE];
    process_state *state;

public:
    Process(GlobalDescriptorTable *GDT, void _start());
    friend class ProcessManager;
};

class ProcessManager
{
    const static int MAX_PROCESSES = 8;
    Process *processes[MAX_PROCESSES];
    int current;

public:
    ProcessManager();
    bool add_process(Process *process, int id);
    bool remove_process(int id);
    process_state *schedule(process_state *state);
};

#endif
