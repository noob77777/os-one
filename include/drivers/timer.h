#ifndef __TIMER_H
#define __TIMER_H

#include <drivers/driver_interface.h>
#include <processmanager/processmanager.h>

class Timer : public DriverInterface
{
    ProcessManager *process_manager;
public:
    Timer(ProcessManager *process_manager);
    virtual uint32_t handle_interrupt(uint32_t esp);
};

#endif
