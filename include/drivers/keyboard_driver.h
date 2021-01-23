#ifndef __KEYBOARD_DRIVER_H
#define __KEYBOARD_DRIVER_H

#include <types.h>
#include <hardware/port.h>
#include <drivers/driver_interface.h>
#include <drivers/display.h>

namespace keyboard_driver
{
    void default_handler(char chr);
} // namespace keyboard_driver

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4

class KeyboardDriver : public DriverInterface
{
    Port8bit data_port;
    Port8bit command_port;
    void (*key_press_handler)(char);

    void keypress(char chr);

public:
    uint8_t shift;
    uint8_t ctrl;
    uint8_t SIGINT = 0;

    KeyboardDriver();

    void on_key_press(void (*key_press_handler)(char));
    void reset();

    virtual uint32_t handle_interrupt(uint32_t esp);
};

#endif
