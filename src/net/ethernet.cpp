#include <net/ethernet.h>

namespace ethernet
{
    void receive_handler(uint8_t *buffer, int size)
    {
        return;
    }
} // namespace ethernet

Ethernet::Ethernet(NICDriver *eth)
{
    this->eth = eth;
}

void Ethernet::send(uint64_t *dest, uint16_t *protocol, uint8_t *buffer, int size)
{
    return;
}
