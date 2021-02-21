#include <inet/ethernet.h>

namespace ethernet
{
    void receive_handler(uint8_t *buffer, int size)
    {
        EthernetII::receive(buffer, size);
    }
} // namespace ethernet

NICDriver *EthernetII::nic = 0;

void EthernetII::set_nic(NICDriver *nic)
{
    EthernetII::nic = nic;
}

void EthernetII::receive(uint8_t *buffer, int size)
{
    // call appropriate handler
    return;
}
