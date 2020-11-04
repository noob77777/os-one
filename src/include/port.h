#ifndef __PORT_H
#define __PORT_H

#include "types.h"

class Port
{
protected:
    uint16_t port;
    Port(uint16_t port)
    {
        this->port = port;
    }

public:
    static inline uint8_t read8(uint16_t _port)
    {
        uint8_t result;
        __asm__ volatile("inb %1, %0"
                         : "=a"(result)
                         : "Nd"(_port));
        return result;
    }
    static inline void write8(uint16_t _port, uint8_t _data)
    {
        __asm__ volatile("outb %0, %1"
                         :
                         : "a"(_data), "Nd"(_port));
    }
    static inline uint16_t read16(uint16_t _port)
    {
        uint16_t result;
        __asm__ volatile("inw %1, %0"
                         : "=a"(result)
                         : "Nd"(_port));
        return result;
    }
    static inline void write16(uint16_t _port, uint16_t _data)
    {
        __asm__ volatile("outw %0, %1"
                         :
                         : "a"(_data), "Nd"(_port));
    }
    static inline void write8_slow(uint16_t _port, uint8_t _data)
    {
        __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                         :
                         : "a"(_data), "Nd"(_port));
    }
};

class Port8bit : public Port
{
public:
    Port8bit(uint16_t port) : Port(port)
    {
        ;
    }
    uint8_t read()
    {
        return read8(port);
    }
    void write(uint8_t data)
    {
        write8(port, data);
    }
};

class Port8bitSlow : public Port8bit
{
public:
    Port8bitSlow(uint16_t port) : Port8bit(port)
    {
        ;
    }
    void write(uint8_t data)
    {
        write8_slow(port, data);
    }
};

class Port16bit : public Port
{
public:
    Port16bit(uint16_t port) : Port(port)
    {
        ;
    }
    uint16_t read()
    {
        return read16(port);
    }
    void write(uint16_t data)
    {
        write16(port, data);
    }
};

#endif
