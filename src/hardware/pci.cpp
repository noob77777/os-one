#include <hardware/pci.h>

PCIController::PCIController()
    : data_port(0xCFC),
      command_port(0xCF8)
{
    ;
}

uint32_t PCIController::read(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset)
{
    uint32_t id =
        0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (register_offset & 0xFC);
    command_port.write(id);
    uint32_t result = data_port.read();
    return result >> (8 * (register_offset % 4));
}

void PCIController::write(uint16_t bus, uint16_t device, uint16_t function, uint32_t register_offset, uint32_t value)
{
    uint32_t id =
        0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (register_offset & 0xFC);
    command_port.write(id);
    data_port.write(value);
}

bool PCIController::device_has_all_functions(uint16_t bus, uint16_t device)
{
    return read(bus, device, 0, 0x0E) & (1 << 7);
}

void PCIController::init(PCIDriverInterface **drivers, int num_drivers, InterruptManager *interrupt_manager)
{
    for (int bus = 0; bus < NUM_BUSES; bus++)
    {
        for (int device = 0; device < NUM_DEVICES; device++)
        {
            for (int function = 0; function < NUM_FUNCTIONS; function++)
            {
                PCIDeviceDescriptor dev = get_device_descriptor(bus, device, function);

                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                for (int bar_num = 0; bar_num < MAX_BAR; bar_num++)
                {
                    BaseAddressRegister bar = get_base_address_register(bus, device, function, bar_num);
                    if (bar.address && (bar.type == INPUT_OUTPUT))
                        dev.port_base = (uint32_t)bar.address;
                }

                /*
                kprintf("PCI BUS ");
                kprintf_hex8(bus & 0xFF);

                kprintf(", DEVICE ");
                kprintf_hex8(device & 0xFF);

                kprintf(", FUNCTION ");
                kprintf_hex8(function & 0xFF);

                kprintf(" = VENDOR ");
                kprintf_hex8((dev.vendor_id & 0xFF00) >> 8);
                kprintf_hex8(dev.vendor_id & 0xFF, false);

                kprintf(", DEVICE ");
                kprintf_hex8((dev.device_id & 0xFF00) >> 8);
                kprintf_hex8(dev.device_id & 0xFF, false);
                kprintf("\n");
                */

                for (int i = 0; i < num_drivers; i++)
                {
                    if (drivers[i]->device_id == dev.device_id && drivers[i]->vendor_id == dev.vendor_id)
                    {
                        drivers[i]->init(&dev);
                        interrupt_manager->add_driver((DriverInterface *)drivers[i]);
                    }
                }
            }
        }
    }
}

BaseAddressRegister PCIController::get_base_address_register(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;
    uint32_t header_type = read(bus, device, function, 0x0E) & 0x7F;
    int max_bar = 6 - (4 * header_type);
    if (bar >= max_bar)
        return result;

    uint32_t bar_value = read(bus, device, function, 0x10 + 4 * bar);
    result.type = (bar_value & 0x1) ? INPUT_OUTPUT : MEMORY_MAPPING;

    if (result.type == MEMORY_MAPPING)
    {
        ;
    }
    else
    {
        result.address = (uint8_t *)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
}

PCIDeviceDescriptor PCIController::get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PCIDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = read(bus, device, function, 0x00);
    result.device_id = read(bus, device, function, 0x02);

    result.class_id = read(bus, device, function, 0x0b);
    result.subclass_id = read(bus, device, function, 0x0a);
    result.interface_id = read(bus, device, function, 0x09);

    result.revision = read(bus, device, function, 0x08);
    result.interrupt = read(bus, device, function, 0x3c) + PCI_INT_BASE;

    return result;
}
