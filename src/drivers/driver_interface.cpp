#include <drivers/driver_interface.h>

DriverInterface::DriverInterface(uint8_t interrupt)
{
    this->interrupt = interrupt;
}

uint32_t DriverInterface::handle_interrupt(uint32_t esp)
{
    kprintf("DEBUG: Interrupt ");
    kprintf_hex8(interrupt);
    return esp;
}
