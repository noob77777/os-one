#include <drivers/amd_am79c973.h>

amd_am79c973::amd_am79c973()
    : PCIDriverInterface(0, 0x1022, 0x2000), NICDriver(),
      mac_address0_port(0),
      mac_address2_port(0),
      mac_address4_port(0),
      register_data_port(0),
      register_address_port(0),
      reset_port(0),
      bus_control_register_data_port(0)
{
    ;
}

amd_am79c973::amd_am79c973(PCIDeviceDescriptor *dev)
    : PCIDriverInterface(dev->interrupt, 0x1022, 0x2000), NICDriver(),
      mac_address0_port(dev->port_base + APROM0),
      mac_address2_port(dev->port_base + APROM2),
      mac_address4_port(dev->port_base + APROM4),
      register_data_port(dev->port_base + RDP),
      register_address_port(dev->port_base + RAP),
      reset_port(dev->port_base + RESET),
      bus_control_register_data_port(dev->port_base + BDP)
{
    init(dev);
}

void amd_am79c973::init(PCIDeviceDescriptor *dev)
{
    this->interrupt = dev->interrupt;
    mac_address0_port = Port16bit(dev->port_base + APROM0);
    mac_address2_port = Port16bit(dev->port_base + APROM2);
    mac_address4_port = Port16bit(dev->port_base + APROM4);
    register_data_port = Port16bit(dev->port_base + RDP);
    register_address_port = Port16bit(dev->port_base + RAP);
    reset_port = Port16bit(dev->port_base + RESET);
    bus_control_register_data_port = Port16bit(dev->port_base + BDP);
    current_send_buffer = 0;
    current_recv_buffer = 0;

    uint64_t MAC0 = mac_address0_port.read() % 256;
    uint64_t MAC1 = mac_address0_port.read() / 256;
    uint64_t MAC2 = mac_address2_port.read() % 256;
    uint64_t MAC3 = mac_address2_port.read() / 256;
    uint64_t MAC4 = mac_address4_port.read() % 256;
    uint64_t MAC5 = mac_address4_port.read() / 256;

    uint64_t MAC = MAC5 << 40 | MAC4 << 32 | MAC3 << 24 | MAC2 << 16 | MAC1 << 8 | MAC0;

    // 32 bit mode
    register_address_port.write(0x14);
    bus_control_register_data_port.write(0x102);

    // stop reset
    register_address_port.write(0);
    register_data_port.write(0x04);

    // init_block
    init_block.mode = 0x0000; // promiscuous mode = false
    init_block.reserved1 = 0;
    init_block.transfer_length = 3;
    init_block.reserved2 = 0;
    init_block.receive_length = 3;
    init_block.physical_address = MAC;
    init_block.reserved3 = 0;
    init_block.logical_address = 0;

    send_buffer_descr = (BufferDescriptor *)((((uint32_t)&send_buffer_descr_memory[0]) + 15) & ~((uint32_t)0xF));
    init_block.transmit_descriptor = (uint32_t)send_buffer_descr;
    recv_buffer_descr = (BufferDescriptor *)((((uint32_t)&recv_buffer_descr_memory[0]) + 15) & ~((uint32_t)0xF));
    init_block.receive_descriptor = (uint32_t)recv_buffer_descr;

    for (uint8_t i = 0; i < 8; i++)
    {
        send_buffer_descr[i].address = (((uint32_t)&send_buffers[i]) + 15) & ~(uint32_t)0xF;
        send_buffer_descr[i].flags = 0x7FF | 0xF000;
        send_buffer_descr[i].flags2 = 0;
        send_buffer_descr[i].avail = 0;

        recv_buffer_descr[i].address = (((uint32_t)&recv_buffers[i]) + 15) & ~(uint32_t)0xF;
        recv_buffer_descr[i].flags = 0xF7FF | 0x80000000;
        recv_buffer_descr[i].flags2 = 0;
        recv_buffer_descr[i].avail = 0;
    }

    register_address_port.write(1);
    register_data_port.write((uint32_t)(&init_block) & 0xFFFF);
    register_address_port.write(2);
    register_data_port.write(((uint32_t)(&init_block) >> 16) & 0xFFFF);
}

void amd_am79c973::activate()
{
    register_address_port.write(0);
    register_data_port.write(0x41);

    register_address_port.write(0x4);
    uint32_t temp = register_data_port.read();
    register_address_port.write(0x4);
    register_data_port.write(temp | 0xC00);

    register_address_port.write(0);
    register_data_port.write(0x42);
}

void amd_am79c973::reset()
{
    reset_port.read();
    reset_port.write(0);
}

uint32_t amd_am79c973::handle_interrupt(uint32_t esp)
{
    register_address_port.write(0);
    uint32_t temp = register_data_port.read();

    if ((temp & 0x0100) == 0x0100)
        kprintf("AMD am79c973 Network Card Detected\n");
    if ((temp & 0x8000) == 0x8000)
        kprintf_notify("AMD am79c973 ERROR");
    if ((temp & 0x2000) == 0x2000)
        kprintf_notify("AMD am79c973 COLLISION ERROR");
    if ((temp & 0x1000) == 0x1000)
        kprintf_notify("AMD am79c973 MISSED FRAME");
    if ((temp & 0x0800) == 0x0800)
        kprintf_notify("AMD am79c973 MEMORY ERROR");
    if ((temp & 0x0400) == 0x0400)
        kprintf_notify("AMD am79c973 RECV"), receive();
    if ((temp & 0x0200) == 0x0200)
        kprintf_notify("AMD am79c973 DATA SENT");

    // acknowledge
    register_address_port.write(0);
    register_data_port.write(temp);

    return esp;
}

void amd_am79c973::send(uint8_t *buffer, int size)
{
    int send_descriptor = current_send_buffer;
    current_send_buffer = (current_send_buffer + 1) % 8;

    if (size > 1518)
        size = 1518;

    for (uint8_t *src = buffer + size - 1,
                 *dst = (uint8_t *)(send_buffer_descr[send_descriptor].address + size - 1);
         src >= buffer; src--, dst--)
        *dst = *src;

    send_buffer_descr[send_descriptor].avail = 0;
    send_buffer_descr[send_descriptor].flags2 = 0;
    send_buffer_descr[send_descriptor].flags = 0x8300F000 | ((uint16_t)((-size) & 0xFFF));
    register_address_port.write(0);
    register_data_port.write(0x48);
}

void amd_am79c973::receive()
{
    for (; (recv_buffer_descr[current_recv_buffer].flags & 0x80000000) == 0;
         current_recv_buffer = (current_recv_buffer + 1) % 8)
    {
        if (!(recv_buffer_descr[current_recv_buffer].flags & 0x40000000) && (recv_buffer_descr[current_recv_buffer].flags & 0x03000000) == 0x03000000)

        {
            uint32_t size = recv_buffer_descr[current_recv_buffer].flags2 & 0xFFF;
            uint8_t *buffer = (uint8_t *)(recv_buffer_descr[current_recv_buffer].address);

            if (receive_handler != nullptr)
                receive_handler(buffer, size);
        }

        recv_buffer_descr[current_recv_buffer].flags2 = 0;
        recv_buffer_descr[current_recv_buffer].flags = 0x8000F7FF;
    }
}

uint64_t amd_am79c973::mac_address()
{
    return init_block.physical_address;
}
