#include <drivers/keyboard_driver.h>

namespace keyboard_driver
{
    void default_handler(char chr)
    {
        if (chr == '\b')
        {
            display::backspace();
            return;
        }
        const char string[2] = {chr, '\0'};
        kprintf(string);
    }
} // namespace keyboard_driver

void KeyboardDriver::keypress(char chr)
{
    key_press_handler(chr);
}

KeyboardDriver::KeyboardDriver() : DriverInterface(0x21), data_port(0x60),
                                   command_port(0x64), shift(0)
{
    while (command_port.read() & 0x1)
        data_port.read();

    command_port.write(0xAE);
    command_port.write(0x20);
    uint8_t status = (data_port.read() | 1) & ~0x10;
    command_port.write(0x60);
    data_port.write(status);
    data_port.write(0xF4);
    key_press_handler = keyboard_driver::default_handler;
}

void KeyboardDriver::on_key_press(void (*key_press_handler)(char))
{
    this->key_press_handler = key_press_handler;
}

void KeyboardDriver::reset()
{
    this->key_press_handler = keyboard_driver::default_handler;
}

uint32_t KeyboardDriver::handle_interrupt(uint32_t esp)
{
    uint8_t key = data_port.read();

    if (key == 0x2A || key == 0x36)
        shift++;
    else if (key == 0xAA || key == 0xB6)
        shift--;
    else if (key == 0x1D)
        shift = 0, ctrl = 1;
    else if (key == 0x9D)
        shift = 0, ctrl = 0;
    else if (key < 0x80)
    {
        switch (key)
        {
        case 0x29:
            keypress(shift ? '~' : '`');
            break;

        case 0x02:
            keypress(shift ? '!' : '1');
            break;
        case 0x03:
            keypress(shift ? '@' : '2');
            break;
        case 0x04:
            keypress(shift ? '#' : '3');
            break;
        case 0x05:
            keypress(shift ? '$' : '4');
            break;
        case 0x06:
            keypress(shift ? '%' : '5');
            break;
        case 0x07:
            keypress(shift ? '^' : '6');
            break;
        case 0x08:
            keypress(shift ? '&' : '7');
            break;
        case 0x09:
            keypress(shift ? '*' : '8');
            break;
        case 0x0A:
            keypress(shift ? '(' : '9');
            break;
        case 0x0B:
            keypress(shift ? ')' : '0');
            break;
        case 0x0C:
            keypress(shift ? '_' : '-');
            break;
        case 0x0D:
            keypress(shift ? '+' : '=');
            break;
        case 0x0E:
            keypress('\b');
            break;
        case 0x0F:
            keypress('\t');
            break;

        case 0x53:
            display::clear();
            break;

        case 0x10:
            keypress(shift ? 'Q' : 'q');
            break;
        case 0x11:
            keypress(shift ? 'W' : 'w');
            break;
        case 0x12:
            keypress(shift ? 'E' : 'e');
            break;
        case 0x13:
            keypress(shift ? 'R' : 'r');
            break;
        case 0x14:
            keypress(shift ? 'T' : 't');
            break;
        case 0x15:
            keypress(shift ? 'Y' : 'y');
            break;
        case 0x16:
            keypress(shift ? 'U' : 'u');
            break;
        case 0x17:
            keypress(shift ? 'I' : 'i');
            break;
        case 0x18:
            keypress(shift ? 'O' : 'o');
            break;
        case 0x19:
            keypress(shift ? 'P' : 'p');
            break;
        case 0x1A:
            keypress(shift ? '{' : '[');
            break;
        case 0x1B:
            keypress(shift ? '}' : ']');
            break;

        case 0x1E:
            keypress(shift ? 'A' : 'a');
            break;
        case 0x1F:
            keypress(shift ? 'S' : 's');
            break;
        case 0x20:
            keypress(shift ? 'D' : 'd');
            break;
        case 0x21:
            keypress(shift ? 'F' : 'f');
            break;
        case 0x22:
            keypress(shift ? 'G' : 'g');
            break;
        case 0x23:
            keypress(shift ? 'H' : 'h');
            break;
        case 0x24:
            keypress(shift ? 'J' : 'j');
            break;
        case 0x25:
            keypress(shift ? 'K' : 'k');
            break;
        case 0x26:
            keypress(shift ? 'L' : 'l');
            break;
        case 0x27:
            keypress(shift ? ':' : ';');
            break;
        case 0x28:
            keypress(shift ? '"' : '\'');
            break;
        case 0x2B:
            keypress(shift ? '|' : '\\');
            break;

        case 0x2C:
            keypress(shift ? 'Z' : 'z');
            break;
        case 0x2D:
            keypress(shift ? 'X' : 'x');
            break;
        case 0x2E:
            if (ctrl)
                SIGINT = 1;
            keypress(shift ? 'C' : 'c');
            break;
        case 0x2F:
            keypress(shift ? 'V' : 'v');
            break;
        case 0x30:
            keypress(shift ? 'B' : 'b');
            break;
        case 0x31:
            keypress(shift ? 'N' : 'n');
            break;
        case 0x32:
            keypress(shift ? 'M' : 'm');
            break;
        case 0x33:
            keypress(shift ? '<' : ',');
            break;
        case 0x34:
            keypress(shift ? '>' : '.');
            break;
        case 0x35:
            keypress(shift ? '?' : '/');
            break;

        case 0x1C:
            keypress('\n');
            break;
        case 0x39:
            keypress(' ');
            break;

        default:
            break;
        }
    }
    return esp;
}
