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
    static void HandleInterruptRequest0x22();
    static void HandleInterruptRequest0x23();
    static void HandleInterruptRequest0x24();
    static void HandleInterruptRequest0x25();
    static void HandleInterruptRequest0x26();
    static void HandleInterruptRequest0x27();
    static void HandleInterruptRequest0x28();
    static void HandleInterruptRequest0x29();
    static void HandleInterruptRequest0x2A();
    static void HandleInterruptRequest0x2B();
    static void HandleInterruptRequest0x2C();
    static void HandleInterruptRequest0x2D();
    static void HandleInterruptRequest0x2E();
    static void HandleInterruptRequest0x2F();
    static void HandleInterruptRequest0x80();

    static void HandleException0x00();
    static void HandleException0x01();
    static void HandleException0x02();
    static void HandleException0x03();
    static void HandleException0x04();
    static void HandleException0x05();
    static void HandleException0x06();
    static void HandleException0x07();
    static void HandleException0x08();
    static void HandleException0x09();
    static void HandleException0x0A();
    static void HandleException0x0B();
    static void HandleException0x0C();
    static void HandleException0x0D();
    static void HandleException0x0E();
    static void HandleException0x0F();
    static void HandleException0x10();
    static void HandleException0x11();
    static void HandleException0x12();
    static void HandleException0x13();

    static uint32_t handleInterrupt(uint8_t interrupt, uint32_t esp);

    static Port8bitPIC pic_master_command_port;
    static Port8bitPIC pic_master_data_port;
    static Port8bitPIC pic_slave_command_port;
    static Port8bitPIC pic_slave_data_port;

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
