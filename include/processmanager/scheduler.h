#ifndef __SCHEDULER_H
#define __SCHEDULER_H

class Process;

class RoundRobinScheduler
{
public:
    int select_next_process(int current, Process **queue, int QLEN);
};

#endif
