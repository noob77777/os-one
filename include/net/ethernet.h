#ifndef __ETHERNET_H
#define __ETHERNET_H

#include <types.h>
#include <drivers/nic_driver.h>

namespace ethernet
{
    void receive_handler(uint8_t *buffer, int size);
} // namespace ethernet

class Ethernet
{
    NICDriver *eth;

public:
    Ethernet(NICDriver *eth);
    void send(uint64_t *dest, uint16_t *protocol, uint8_t *buffer, int size);
};

#endif
