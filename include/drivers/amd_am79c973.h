#ifndef __AMD_AM79C973_H
#define __AMD_AM79C973_H

#include <types.h>
#include <drivers/display.h>
#include <drivers/nic_driver.h>
#include <drivers/driver_interface.h>
#include <hardware/pci.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>

class amd_am79c973 : public PCIDriverInterface, public NICDriver
{
    const static int APROM0 = 0x00;
    const static int APROM2 = 0x02;
    const static int APROM4 = 0x04;
    const static int RDP = 0x10;
    const static int RAP = 0x12;
    const static int RESET = 0x14;
    const static int BDP = 0x16;

    struct InitializationBlock
    {
        uint16_t mode;
        unsigned reserved1 : 4;
        unsigned receive_length : 4;
        unsigned reserved2 : 4;
        unsigned transfer_length : 4;
        uint64_t physical_address : 48;
        uint16_t reserved3;
        uint64_t logical_address;
        uint32_t receive_descriptor;
        uint32_t transmit_descriptor;
    } __attribute__((packed));

    struct BufferDescriptor
    {
        uint32_t address;
        uint32_t flags;
        uint32_t flags2;
        uint32_t avail;
    } __attribute__((packed));

    Port16bit mac_address0_port;
    Port16bit mac_address2_port;
    Port16bit mac_address4_port;
    Port16bit register_data_port;
    Port16bit register_address_port;
    Port16bit reset_port;
    Port16bit bus_control_register_data_port;

    InitializationBlock init_block;

    BufferDescriptor *send_buffer_descr;
    uint8_t send_buffer_descr_memory[2 * 1024 + 15];
    uint8_t send_buffers[8][2 * 1024 + 15];
    uint8_t current_send_buffer;

    BufferDescriptor *recv_buffer_descr;
    uint8_t recv_buffer_descr_memory[2 * 1024 + 15];
    uint8_t recv_buffers[8][2 * 1024 + 15];
    uint8_t current_recv_buffer;

public:
    amd_am79c973();
    amd_am79c973(PCIDeviceDescriptor *dev);

    void activate();
    void reset();
    virtual uint32_t handle_interrupt(uint32_t esp);
    virtual void init(PCIDeviceDescriptor *dev);

    virtual void send(uint8_t *buffer, int size);
    virtual void receive();
};

#endif
