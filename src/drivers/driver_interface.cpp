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

PCIDriverInterface::PCIDriverInterface(uint8_t interrupt, uint16_t vendor_id, uint16_t device_id)
    : DriverInterface(interrupt)
{
    this->vendor_id = vendor_id;
    this->device_id = device_id;
}

void PCIDriverInterface::init(PCIDeviceDescriptor *dev)
{
    ;
}
