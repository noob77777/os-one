#include <hardware/interrupts.h>

GateDescriptor::GateDescriptor() { ; }

GateDescriptor::GateDescriptor(void (*handler)(), uint16_t gdt_cs, uint8_t privilege_level, uint8_t type)
{
    handler_address_lo = ((uint32_t)handler) & 0xFFFF;
    handler_address_hi = (((uint32_t)handler) >> 16) & 0xFFFF;
    this->gdt_cs = gdt_cs;
    access = IDT_DESC_PRESENT | ((privilege_level & 3) << 5) | type;
    reserved = 0;
}

InterruptDescriptorTable::InterruptDescriptorTable()
{
    limit = (sizeof(idt) - 1);
    base = (uint32_t)this;
    __asm__ volatile("lidt %0"
                     :
                     : "m"(limit));
}

InterruptDescriptorTable InterruptManager::IDT;
DriverInterface *InterruptManager::drivers[] = {0};
Port8bitPIC InterruptManager::picMasterCommandPort(0x20);
Port8bitPIC InterruptManager::picMasterDataPort(0x21);
Port8bitPIC InterruptManager::picSlaveCommandPort(0xA0);
Port8bitPIC InterruptManager::picSlaveDataPort(0xA1);

InterruptManager::InterruptManager(GlobalDescriptorTable *GDT)
{
    uint32_t gdt_cs = GDT->code_segment();
    for (int i = 0; i < InterruptDescriptorTable::NUMBER_OF_INTERRUPTS; ++i)
    {
        IDT.idt[i] = GateDescriptor(&InterruptIgnore, gdt_cs, 0, IDT_INTERRUPT_GATE);
    }

    IDT.idt[0x00] = GateDescriptor(&HandleException0x00, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x01] = GateDescriptor(&HandleException0x01, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x02] = GateDescriptor(&HandleException0x02, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x03] = GateDescriptor(&HandleException0x03, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x04] = GateDescriptor(&HandleException0x04, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x05] = GateDescriptor(&HandleException0x05, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x06] = GateDescriptor(&HandleException0x06, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x07] = GateDescriptor(&HandleException0x07, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x08] = GateDescriptor(&HandleException0x08, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x09] = GateDescriptor(&HandleException0x09, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x0A] = GateDescriptor(&HandleException0x0A, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x0B] = GateDescriptor(&HandleException0x0B, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x0C] = GateDescriptor(&HandleException0x0C, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x0D] = GateDescriptor(&HandleException0x0D, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x0E] = GateDescriptor(&HandleException0x0E, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x0F] = GateDescriptor(&HandleException0x0F, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x10] = GateDescriptor(&HandleException0x10, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x11] = GateDescriptor(&HandleException0x11, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x12] = GateDescriptor(&HandleException0x12, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x13] = GateDescriptor(&HandleException0x13, gdt_cs, 0, IDT_INTERRUPT_GATE);

    IDT.idt[0x20] = GateDescriptor(&HandleInterruptRequest0x20, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x21] = GateDescriptor(&HandleInterruptRequest0x21, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x22] = GateDescriptor(&HandleInterruptRequest0x22, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x23] = GateDescriptor(&HandleInterruptRequest0x23, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x24] = GateDescriptor(&HandleInterruptRequest0x24, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x25] = GateDescriptor(&HandleInterruptRequest0x25, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x26] = GateDescriptor(&HandleInterruptRequest0x26, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x27] = GateDescriptor(&HandleInterruptRequest0x27, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x28] = GateDescriptor(&HandleInterruptRequest0x28, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x29] = GateDescriptor(&HandleInterruptRequest0x29, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x2A] = GateDescriptor(&HandleInterruptRequest0x2A, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x2B] = GateDescriptor(&HandleInterruptRequest0x2B, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x2C] = GateDescriptor(&HandleInterruptRequest0x2C, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x2D] = GateDescriptor(&HandleInterruptRequest0x2D, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x2E] = GateDescriptor(&HandleInterruptRequest0x2E, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x2F] = GateDescriptor(&HandleInterruptRequest0x2F, gdt_cs, 0, IDT_INTERRUPT_GATE);
    IDT.idt[0x80] = GateDescriptor(&HandleInterruptRequest0x80, gdt_cs, 0, IDT_INTERRUPT_GATE);

    picMasterCommandPort.write(0x11);
    picSlaveCommandPort.write(0x11);

    picMasterDataPort.write(0x20);
    picSlaveDataPort.write(0x28);
    picMasterDataPort.write(0x04);
    picSlaveDataPort.write(0x02);
    picMasterDataPort.write(0x01);
    picSlaveDataPort.write(0x01);
    picMasterDataPort.write(0x00);
    picSlaveDataPort.write(0x00);
}

InterruptManager::~InterruptManager()
{
    deactivate();
}

void InterruptManager::activate()
{
    __asm__ volatile("sti");
}

void InterruptManager::deactivate()
{
    __asm__ volatile("cli");
}

void InterruptManager::add_driver(DriverInterface *driver)
{
    drivers[driver->interrupt] = driver;
}

uint32_t InterruptManager::handleInterrupt(uint8_t interrupt, uint32_t esp)
{
    if (drivers[interrupt] != 0)
    {
        esp = drivers[interrupt]->handle_interrupt(esp);
    }

    if (0x20 <= interrupt && interrupt < 0x30)
    {
        picMasterCommandPort.write(0x20);
        if (0x28 <= interrupt)
            picSlaveCommandPort.write(0x20);
    }
    
    return esp;
}

uint32_t handle_interrupt_main(uint8_t interrupt, uint32_t esp)
{
    return InterruptManager::handleInterrupt(interrupt, esp);
}
