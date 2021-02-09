#include <processmanager/scheduler.h>

int RoundRobinScheduler::select_next_process(int current, Process **queue, int QLEN)
{
    current = (current + 1) % QLEN;
    while (queue[current] == nullptr)
    {
        current = (current + 1) % QLEN;
    }
    return current;
}
