#ifndef __DRIVER_INTERFACE_H
#define __DRIVER_INTERFACE_H

#include <types.h>
#include <drivers/display.h>

class InterruptManager;
class PCIController;
struct PCIDeviceDescriptor;

class DriverInterface
{
protected:
    uint8_t interrupt;
    DriverInterface(uint8_t interrupt);

public:
    virtual uint32_t handle_interrupt(uint32_t esp);
    friend class InterruptManager;
};

class PCIDriverInterface : public DriverInterface
{
protected:
    uint16_t vendor_id;
    uint16_t device_id;
    PCIDriverInterface(uint8_t interrupt, uint16_t vendor_id, uint16_t device_id);

public:
    virtual void init(PCIDeviceDescriptor *dev);
    friend class PCIController;
};

#endif
