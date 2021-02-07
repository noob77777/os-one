#ifndef __PCI_H
#define __PCI_H

#include <types.h>
#include <hardware/port.h>
#include <hardware/interrupts.h>
#include <drivers/display.h>
#include <drivers/driver_interface.h>

enum BaseAddressRegisterType
{
    MEMORY_MAPPING,
    INPUT_OUTPUT
};

struct BaseAddressRegister
{
    bool prefetchable;
    uint8_t *address;
    uint32_t size;
    BaseAddressRegisterType type;
};

struct PCIDeviceDescriptor
{
    uint32_t port_base;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;
};

class PCIController
{
    Port32bit data_port;
    Port32bit command_port;
    static const int NUM_BUSES = 8;
    static const int NUM_DEVICES = 32;
    static const int NUM_FUNCTIONS = 8;
    static const int MAX_BAR = 6;
    static const int PCI_INT_BASE = 0x20;

public:
    PCIController();

    uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset);
    void write(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset, uint32_t value);
    bool device_has_all_functions(uint16_t bus, uint16_t device);

    void init(PCIDriverInterface *drivers[], int num_drivers, InterruptManager *interrupt_manager);
    PCIDeviceDescriptor get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function);
    BaseAddressRegister get_base_address_register(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);
};

#endif
