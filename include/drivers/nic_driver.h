#ifndef __NIC_DRIVER_H
#define __NIC_DRIVER_H

#include <types.h>

/*
 * Abstract class defination
 */
class NICDriver
{
protected:
    void (*receive_handler)(uint8_t *buffer, int size);
    NICDriver();

public:
    virtual void send(uint8_t *buffer, int size);
    virtual void receive();
    virtual uint64_t mac_address();
    virtual void set_receive_handler(void (*handler)(uint8_t *buffer, int size));
};

#endif
