#ifndef __IDT_H
#define __IDT_H

#include "types.h"
#include "gdt.h"
#include "port.h"
#include "../drivers/display.h"

class GateDescriptor
{
    static const uint8_t IDT_DESC_PRESENT = 0x80;
    uint16_t handler_address_lo;
    uint16_t gdt_cs;
    uint8_t reserved;
    uint8_t access;
    uint16_t handler_address_hi;

public:
    GateDescriptor() { ; }
    GateDescriptor(void (*handler)(), uint16_t gdt_cs, uint8_t privilege_level, uint8_t type)
    {
        handler_address_lo = ((uint32_t)handler) & 0xFFFF;
        handler_address_hi = (((uint32_t)handler) >> 16) & 0xFFFF;
        this->gdt_cs = gdt_cs;
        access = IDT_DESC_PRESENT | ((privilege_level & 3) << 5) | type;
        reserved = 0;
    }
} __attribute__((packed));

class InterruptManager;

class InterruptDescriptorTable
{
    static const uint32_t NUMBER_OF_INTERRUPTS = 256;

    GateDescriptor idt[NUMBER_OF_INTERRUPTS];
    uint16_t limit;
    uint32_t base;

public:
    InterruptDescriptorTable()
    {
        limit = (sizeof(idt) - 1);
        base = (uint32_t)this;
        __asm__ volatile("lidt %0"
                         :
                         : "m"(limit));
    }

    friend class InterruptManager;
} __attribute__((packed));

extern "C" uint32_t handle_interrupt_main(uint8_t interrupt, uint32_t esp);

class InterruptManager
{
    const static uint8_t IDT_INTERRUPT_GATE = 0xE;

    static InterruptDescriptorTable IDT;

    static void InterruptIgnore();
    static void HandleInterruptRequest0x20();
    static void HandleInterruptRequest0x21();

    static uint32_t handleInterrupt(uint8_t interrupt, uint32_t esp);

    Port8bitSlow programmableInterruptControllerMasterCommandPort;
    Port8bitSlow programmableInterruptControllerMasterDataPort;
    Port8bitSlow programmableInterruptControllerSlaveCommandPort;
    Port8bitSlow programmableInterruptControllerSlaveDataPort;

public:
    InterruptManager(GlobalDescriptorTable *GDT) : programmableInterruptControllerMasterCommandPort(0x20),
                                                   programmableInterruptControllerMasterDataPort(0x21),
                                                   programmableInterruptControllerSlaveCommandPort(0xA0),
                                                   programmableInterruptControllerSlaveDataPort(0xA1)
    {
        uint32_t gdt_cs = GDT->code_segment();
        for (int i = 0; i < InterruptDescriptorTable::NUMBER_OF_INTERRUPTS; ++i)
        {
            IDT.idt[i] = GateDescriptor(&InterruptIgnore, gdt_cs, 0, IDT_INTERRUPT_GATE);
        }

        IDT.idt[0x20] = GateDescriptor(&HandleInterruptRequest0x20, gdt_cs, 0, IDT_INTERRUPT_GATE);
        IDT.idt[0x21] = GateDescriptor(&HandleInterruptRequest0x21, gdt_cs, 0, IDT_INTERRUPT_GATE);

        programmableInterruptControllerMasterCommandPort.write(0x11);
        programmableInterruptControllerSlaveCommandPort.write(0x11);

        programmableInterruptControllerMasterDataPort.write(0x20);
        programmableInterruptControllerSlaveDataPort.write(0x28);
        programmableInterruptControllerMasterDataPort.write(0x04);
        programmableInterruptControllerSlaveDataPort.write(0x02);
        programmableInterruptControllerMasterDataPort.write(0x01);
        programmableInterruptControllerSlaveDataPort.write(0x01);
        programmableInterruptControllerMasterDataPort.write(0x00);
        programmableInterruptControllerSlaveDataPort.write(0x00);
    }
    ~InterruptManager()
    {
        deactivate();
    }
    void activate()
    {
        __asm__ volatile("sti");
    }
    void deactivate()
    {
        __asm__ volatile("cli");
    }
    friend uint32_t handle_interrupt_main(uint8_t interrupt, uint32_t esp);
};

uint32_t handle_interrupt_main(uint8_t interrupt, uint32_t esp)
{
    return InterruptManager::handleInterrupt(interrupt, esp);
}

InterruptDescriptorTable InterruptManager::IDT;

uint32_t InterruptManager::handleInterrupt(uint8_t interrupt, uint32_t esp)
{
    kprintf("DEBUG: Interrupt ");
    kprintf_hex8(interrupt);

    return esp;
}

#endif
