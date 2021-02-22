#include <inet/ethernet.h>

namespace ethernet
{
    void receive_handler(uint8_t *buffer, int size)
    {
        Ethernet::receive(buffer, size);
    }
} // namespace ethernet

NICDriver *Ethernet::nic = 0;

uint16_t Ethernet::protocol(uint8_t *buffer, int size)
{
    if (size < Ethernet::HEADER_LEN)
        return 0;
    uint16_t protocol = *((uint16_t *)&buffer[12]);
    return reflect(protocol);
}

uint64_t Ethernet::src_addr(uint8_t *buffer, int size)
{
    if (size < Ethernet::HEADER_LEN)
        return 0;
    uint64_t src = *((uint64_t *)&buffer[6]);
    src = reflect(src) >> 16;
    return src;
}

uint64_t Ethernet::dest_addr(uint8_t *buffer, int size)
{
    if (size < Ethernet::HEADER_LEN)
        return 0;
    uint64_t des = *((uint64_t *)&buffer[0]);
    des = reflect(des) >> 16;
    return des;
}

uint32_t Ethernet::checksum(uint8_t *buffer, int size)
{
    if (size < Ethernet::FOOTER_LEN)
        return 0;
    uint32_t crc = *((uint32_t *)&buffer[size - 4]);
    return reflect(crc);
}

void Ethernet::add_checksum(uint8_t *buffer, int size)
{
    return;
}

void Ethernet::add_dest_addr(uint8_t *buffer, int size, uint64_t dest)
{
    if (size < Ethernet::HEADER_LEN)
        return;
    dest = reflect(dest);
    dest = dest >> 16;
    for (int i = 0; i < 6; i++)
    {
        uint8_t byte = (dest & (0xff));
        buffer[i] = byte;
        dest = dest >> 8;
    }
}

void Ethernet::add_src_addr(uint8_t *buffer, int size)
{
    if (size < Ethernet::HEADER_LEN)
        return;
    if (Ethernet::nic == 0)
        return;

    uint64_t src = nic->mac_address();
    for (int i = 0; i < 6; i++)
    {
        uint8_t byte = (src & (0xff));
        buffer[i + 6] = byte;
        src = src >> 8;
    }
}

void Ethernet::add_protocol(uint8_t *buffer, int size, uint16_t protocol)
{
    if (size < Ethernet::HEADER_LEN)
        return;
    protocol = reflect(protocol);
    for (int i = 0; i < 2; i++)
    {
        uint8_t byte = (protocol & (0xff));
        buffer[i + 12] = byte;
        protocol = protocol >> 8;
    }
}

void Ethernet::set_nic(NICDriver *nic)
{
    Ethernet::nic = nic;
}

void Ethernet::send(uint64_t dest, uint16_t protocol, uint8_t *buffer, int size)
{
    if (Ethernet::nic == 0)
        return;
    int frame_size = size + Ethernet::HEADER_LEN + Ethernet::FOOTER_LEN;
    uint8_t frame[MAX_SZ];
    for (int i = 0; i < size; i++)
        frame[i + Ethernet::HEADER_LEN] = buffer[i];

    Ethernet::add_dest_addr(frame, frame_size, dest);
    Ethernet::add_src_addr(frame, frame_size);
    Ethernet::add_protocol(frame, frame_size, protocol);
    Ethernet::add_checksum(frame, frame_size);

    Ethernet::nic->send(frame, frame_size);
}

void Ethernet::receive(uint8_t *buffer, int size)
{
    if (Ethernet::nic == 0)
        return;
    uint64_t dest = reflect(Ethernet::dest_addr(buffer, size) << 16);
    uint64_t broadcast = 0xffffffffffff;
    if (dest == Ethernet::nic->mac_address() || dest == broadcast)
    {
        buffer = buffer + Ethernet::HEADER_LEN;
        size = size - Ethernet::HEADER_LEN - Ethernet::FOOTER_LEN;
        // check crc && call handler
        // default inline handler
        for (int i = 0; i < size; i++)
        {
            kprintf_hex8(buffer[i], false);
            kprintf(" ");
        }
    }
    return;
}
