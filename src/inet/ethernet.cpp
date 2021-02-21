#include <inet/ethernet.h>

namespace ethernet
{
    void receive_handler(uint8_t *buffer, int size)
    {
        Ethernet::receive(buffer, size);
    }
} // namespace ethernet

NICDriver *Ethernet::nic = 0;

void Ethernet::set_nic(NICDriver *nic)
{
    Ethernet::nic = nic;
}

void Ethernet::receive(uint8_t *buffer, int size)
{
    // call appropriate handler
    return;
}
