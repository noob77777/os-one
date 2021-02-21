#include <drivers/nic_driver.h>

NICDriver::NICDriver()
{
    this->receive_handler = nullptr;
}

void NICDriver::send(uint8_t *buffer, int size)
{
    return;
}

void NICDriver::receive()
{
    return;
}

uint64_t NICDriver::mac_address()
{
    return 0;
}

void NICDriver::set_receive_handler(void (*handler)(uint8_t *buffer, int size))
{
    this->receive_handler = handler;
}
