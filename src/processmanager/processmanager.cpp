#include <processmanager/processmanager.h>

Process::Process(GlobalDescriptorTable *GDT, void _start())
{
    state = (process_state *)(stack + STACK_SIZE - sizeof(process_state));

    state->eax = 0;
    state->ebx = 0;
    state->ecx = 0;
    state->edx = 0;
    state->esi = 0;
    state->edi = 0;
    state->ebp = 0;

    state->eip = (uint32_t)_start;
    state->cs = GDT->code_segment();
    state->eflags = 0x202;
}

ProcessManager::ProcessManager()
{
    current = 0;
}

bool ProcessManager::add_process(Process *process, int id)
{
    if (id < 0 || id >= MAX_PROCESSES)
        return false;
    if (processes[id] != nullptr)
        return false;

    processes[id] = process;
    return true;
}

bool ProcessManager::remove_process(int id)
{
    if (id <= 0 || id >= MAX_PROCESSES)
        return false;

    processes[id] = nullptr;
    return true;
}

process_state *ProcessManager::schedule(process_state *state)
{
    processes[current]->state = state;

    RoundRobinScheduler RR;
    current = RR.select_next_process(current, processes, MAX_PROCESSES);

    return processes[current]->state;
}
