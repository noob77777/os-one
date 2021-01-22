#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <types.h>
#include <memory/gdt.h>
#include <hardware/port.h>
#include <drivers/driver_interface.h>

class GateDescriptor
{
    static const uint8_t IDT_DESC_PRESENT = 0x80;
    uint16_t handler_address_lo;
    uint16_t gdt_cs;
    uint8_t reserved;
    uint8_t access;
    uint16_t handler_address_hi;

public:
    GateDescriptor();
    GateDescriptor(void (*handler)(), uint16_t gdt_cs, uint8_t privilege_level, uint8_t type);
    
} __attribute__((packed));

class InterruptManager;

class InterruptDescriptorTable
{
    static const uint32_t NUMBER_OF_INTERRUPTS = 256;

    GateDescriptor idt[NUMBER_OF_INTERRUPTS];
    uint16_t limit;
    uint32_t base;

public:
    InterruptDescriptorTable();

    friend class InterruptManager;
} __attribute__((packed));

extern "C" uint32_t
handle_interrupt_main(uint8_t interrupt, uint32_t esp);

class InterruptManager
{
    const static uint8_t IDT_INTERRUPT_GATE = 0xE;

    static InterruptDescriptorTable IDT;
    static DriverInterface *drivers[InterruptDescriptorTable::NUMBER_OF_INTERRUPTS];

    static void InterruptIgnore();
    static void HandleInterruptRequest0x20();
    static void HandleInterruptRequest0x21();

    static uint32_t handleInterrupt(uint8_t interrupt, uint32_t esp);

    static Port8bitPIC picMasterCommandPort;
    static Port8bitPIC picMasterDataPort;
    static Port8bitPIC picSlaveCommandPort;
    static Port8bitPIC picSlaveDataPort;

public:
    InterruptManager(GlobalDescriptorTable *GDT);
    ~InterruptManager();

    void activate();
    void deactivate();
    void add_driver(DriverInterface *driver);
    friend uint32_t handle_interrupt_main(uint8_t interrupt, uint32_t esp);

};

uint32_t handle_interrupt_main(uint8_t interrupt, uint32_t esp);

#endif
