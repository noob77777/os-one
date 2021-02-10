#include <drivers/timer.h>

Timer::Timer(ProcessManager *process_manager) : DriverInterface(0x20)
{
    this->process_manager = process_manager;
}

uint32_t Timer::handle_interrupt(uint32_t esp)
{
    esp = (uint32_t)process_manager->schedule((process_state*)esp);
    return esp;
}
