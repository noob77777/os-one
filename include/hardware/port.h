#ifndef __PORT_H
#define __PORT_H

#include <types.h>

class Port
{
protected:
    uint16_t port;
    Port(uint16_t port);

public:
    static inline uint8_t read8(uint16_t _port);
    static inline void write8(uint16_t _port, uint8_t _data);
    static inline uint16_t read16(uint16_t _port);
    static inline void write16(uint16_t _port, uint16_t _data);
    static inline void write8_pic(uint16_t _port, uint8_t _data);
};

class Port8bit : public Port
{
public:
    Port8bit(uint16_t port);

    uint8_t read();
    void write(uint8_t data);
};

class Port8bitPIC : public Port8bit
{
public:
    Port8bitPIC(uint16_t port);

    void write(uint8_t data);
};

class Port16bit : public Port
{
public:
    Port16bit(uint16_t port);

    uint16_t read();
    void write(uint16_t data);
};

#endif
