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
    uint8_t shift;

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
        shift = 0;
    }
    virtual uint32_t handle_interrupt(uint32_t esp)
    {
        uint8_t key = data_port.read();

        if (key == 0x2A || key == 0x36)
            shift++;
        else if (key == 0xAA || key == 0xB6)
            shift--;
        else if (key < 0x80)
        {
            switch (key)
            {
            case 0x29:
                kprintf(shift ? "~" : "`");
                break;

            case 0x02:
                kprintf(shift ? "!" :"1");
                break;
            case 0x03:
                kprintf(shift ? "@" :"2");
                break;
            case 0x04:
                kprintf(shift ? "#" :"3");
                break;
            case 0x05:
                kprintf(shift ? "$" :"4");
                break;
            case 0x06:
                kprintf(shift ? "%" :"5");
                break;
            case 0x07:
                kprintf(shift ? "^" :"6");
                break;
            case 0x08:
                kprintf(shift ? "&" :"7");
                break;
            case 0x09:
                kprintf(shift ? "*" :"8");
                break;
            case 0x0A:
                kprintf(shift ? "(" :"9");
                break;
            case 0x0B:
                kprintf(shift ? ")" :"0");
                break;
            case 0x0C:
                kprintf(shift ? "_" :"-");
                break;
            case 0x0D:
                kprintf(shift ? "+" :"=");
                break;
            case 0x0E:
                backspace();
                break;

            case 0x10:
                kprintf(shift ? "Q" :"q");
                break;
            case 0x11:
                kprintf(shift ? "W" :"w");
                break;
            case 0x12:
                kprintf(shift ? "E" :"e");
                break;
            case 0x13:
                kprintf(shift ? "R" :"r");
                break;
            case 0x14:
                kprintf(shift ? "T" :"t");
                break;
            case 0x15:
                kprintf(shift ? "Y" :"y");
                break;
            case 0x16:
                kprintf(shift ? "U" :"u");
                break;
            case 0x17:
                kprintf(shift ? "I" :"i");
                break;
            case 0x18:
                kprintf(shift ? "O" :"o");
                break;
            case 0x19:
                kprintf(shift ? "P" :"p");
                break;
            case 0x1A:
                kprintf(shift ? "{" :"[");
                break;
            case 0x1B:
                kprintf(shift ? "}" :"]");
                break;

            case 0x1E:
                kprintf(shift ? "A" :"a");
                break;
            case 0x1F:
                kprintf(shift ? "S" :"s");
                break;
            case 0x20:
                kprintf(shift ? "D" :"d");
                break;
            case 0x21:
                kprintf(shift ? "F" :"f");
                break;
            case 0x22:
                kprintf(shift ? "G" :"g");
                break;
            case 0x23:
                kprintf(shift ? "H" :"h");
                break;
            case 0x24:
                kprintf(shift ? "J" :"j");
                break;
            case 0x25:
                kprintf(shift ? "K" :"k");
                break;
            case 0x26:
                kprintf(shift ? "L" :"l");
                break;
            case 0x27:
                kprintf(shift ? ":" :";");
                break;
            case 0x28:
                kprintf(shift ? "\"" :"'");
                break;
            case 0x2B:
                kprintf(shift ? "|" :"\\");
                break;

            case 0x2C:
                kprintf(shift ? "Z" :"z");
                break;
            case 0x2D:
                kprintf(shift ? "X" :"x");
                break;
            case 0x2E:
                kprintf(shift ? "C" :"c");
                break;
            case 0x2F:
                kprintf(shift ? "V" :"v");
                break;
            case 0x30:
                kprintf(shift ? "B" :"b");
                break;
            case 0x31:
                kprintf(shift ? "N" :"n");
                break;
            case 0x32:
                kprintf(shift ? "M" :"m");
                break;
            case 0x33:
                kprintf(shift ? "<" :",");
                break;
            case 0x34:
                kprintf(shift ? ">" :".");
                break;
            case 0x35:
                kprintf(shift ? "?" :"/");
                break;

            case 0x1C:
                kprintf("\n");
                break;
            case 0x39:
                kprintf(" ");
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
