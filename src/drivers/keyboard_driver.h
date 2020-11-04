#ifndef __KEYBOARD_DRIVER_H
#define __KEYBOARD_DRIVER_H

#include "../include/types.h"
#include "../include/port.h"
#include "driver_interface.h"
#include "display.h"

class KeyboardDriver : public DriverInterface
{
    Port8bit data_port;
    Port8bit command_port;

public:
    KeyboardDriver() : DriverInterface(0x21), data_port(0x60),
                       command_port(0x64)
    {
        while (command_port.read() & 0x1)
            data_port.read();

        command_port.write(0xAE);
        command_port.write(0x20);
        uint8_t status = (data_port.read() | 1) & ~0x10;
        command_port.write(0x60);
        data_port.write(status);
        data_port.write(0xF4);
    }
    virtual uint32_t handle_interrupt(uint32_t esp)
    {
        uint8_t key = data_port.read();
        if (key < 0x80)
        {
            switch (key)
            {
            case 0x02:
                kprintf("1");
                break;
            case 0x03:
                kprintf("2");
                break;
            case 0x04:
                kprintf("3");
                break;
            case 0x05:
                kprintf("4");
                break;
            case 0x06:
                kprintf("5");
                break;
            case 0x07:
                kprintf("6");
                break;
            case 0x08:
                kprintf("7");
                break;
            case 0x09:
                kprintf("8");
                break;
            case 0x0A:
                kprintf("9");
                break;
            case 0x0B:
                kprintf("0");
                break;

            case 0x10:
                kprintf("q");
                break;
            case 0x11:
                kprintf("w");
                break;
            case 0x12:
                kprintf("e");
                break;
            case 0x13:
                kprintf("r");
                break;
            case 0x14:
                kprintf("t");
                break;
            case 0x15:
                kprintf("z");
                break;
            case 0x16:
                kprintf("u");
                break;
            case 0x17:
                kprintf("i");
                break;
            case 0x18:
                kprintf("o");
                break;
            case 0x19:
                kprintf("p");
                break;

            case 0x1E:
                kprintf("a");
                break;
            case 0x1F:
                kprintf("s");
                break;
            case 0x20:
                kprintf("d");
                break;
            case 0x21:
                kprintf("f");
                break;
            case 0x22:
                kprintf("g");
                break;
            case 0x23:
                kprintf("h");
                break;
            case 0x24:
                kprintf("j");
                break;
            case 0x25:
                kprintf("k");
                break;
            case 0x26:
                kprintf("l");
                break;

            case 0x2C:
                kprintf("y");
                break;
            case 0x2D:
                kprintf("x");
                break;
            case 0x2E:
                kprintf("c");
                break;
            case 0x2F:
                kprintf("v");
                break;
            case 0x30:
                kprintf("b");
                break;
            case 0x31:
                kprintf("n");
                break;
            case 0x32:
                kprintf("m");
                break;
                
            case 0x33:
                kprintf(",");
                break;
            case 0x34:
                kprintf(".");
                break;
            case 0x35:
                kprintf("-");
                break;

            case 0x1C:
                kprintf("\n");
                break;
            case 0x39:
                kprintf(" ");
                break;
            case 0x0E:
                backspace();
                break;

            default:
                kprintf_hex8(key);
                break;
            }
        }
        return esp;
    }
};

#endif
