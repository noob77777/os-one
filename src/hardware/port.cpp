#include <hardware/port.h>

Port::Port(uint16_t port)
{
    this->port = port;
}

inline uint8_t Port::read8(uint16_t _port)
{
    uint8_t result;
    __asm__ volatile("inb %1, %0"
                     : "=a"(result)
                     : "Nd"(_port));
    return result;
}

inline void Port::write8(uint16_t _port, uint8_t _data)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(_data), "Nd"(_port));
}

inline uint16_t Port::read16(uint16_t _port)
{
    uint16_t result;
    __asm__ volatile("inw %1, %0"
                     : "=a"(result)
                     : "Nd"(_port));
    return result;
}

inline void Port::write16(uint16_t _port, uint16_t _data)
{
    __asm__ volatile("outw %0, %1"
                     :
                     : "a"(_data), "Nd"(_port));
}

inline void Port::write8_pic(uint16_t _port, uint8_t _data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(_data), "Nd"(_port));
}

inline uint32_t Port::read32(uint16_t _port)
{
    uint32_t result;
    __asm__ volatile("inl %1, %0"
                     : "=a"(result)
                     : "Nd"(_port));
    return result;
}

inline void Port::write32(uint16_t _port, uint32_t _data)
{
    __asm__ volatile("outl %0, %1"
                     :
                     : "a"(_data), "Nd"(_port));
}

Port8bit::Port8bit(uint16_t port) : Port(port) { ; }

uint8_t Port8bit::read()
{
    return read8(port);
}

void Port8bit::write(uint8_t data)
{
    write8(port, data);
}

Port8bitPIC::Port8bitPIC(uint16_t port) : Port8bit(port) { ; }

void Port8bitPIC::write(uint8_t data)
{
    write8_pic(port, data);
}

Port16bit::Port16bit(uint16_t port) : Port(port) { ; }

uint16_t Port16bit::read()
{
    return read16(port);
}

void Port16bit::write(uint16_t data)
{
    write16(port, data);
}

Port32bit::Port32bit(uint16_t port) : Port(port) { ; }

uint32_t Port32bit::read()
{
    return read32(port);
}

void Port32bit::write(uint32_t data)
{
    write32(port, data);
}
