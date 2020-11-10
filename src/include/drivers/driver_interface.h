#ifndef __DRIVER_INTERFACE_H
#define __DRIVER_INTERFACE_H

#include <types.h>
#include <drivers/display.h>

class InterruptManager;

class DriverInterface
{
protected:
    uint8_t interrupt;
    DriverInterface(uint8_t interrupt)
    {
        this->interrupt = interrupt;
    }

public:
    virtual uint32_t handle_interrupt(uint32_t esp)
    {
        kprintf("DEBUG: Interrupt ");
        kprintf_hex8(interrupt);
        return esp;
    }
    friend class InterruptManager;
};

#endif
