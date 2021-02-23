#include <lib/inet.h>
#include <drivers/display.h>

uint64_t reflect(uint64_t x)
{
    uint8_t *b = (uint8_t *)&x;
    for (int i = 0; i < 4; i++)
    {
        uint8_t tmp = b[i];
        b[i] = b[7 - i];
        b[7 - i] = tmp;
    }
    uint64_t res = *((uint64_t *)b);
    return res;
}

uint32_t reflect(uint32_t x)
{
    uint8_t *b = (uint8_t *)&x;
    for (int i = 0; i < 2; i++)
    {
        uint8_t tmp = b[i];
        b[i] = b[3 - i];
        b[3 - i] = tmp;
    }
    uint32_t res = *((uint32_t *)b);
    return res;
}

uint16_t reflect(uint16_t x)
{
    uint8_t *b = (uint8_t *)&x;
    for (int i = 0; i < 1; i++)
    {
        uint8_t tmp = b[i];
        b[i] = b[1 - i];
        b[1 - i] = tmp;
    }
    uint16_t res = *((uint16_t *)b);
    return res;
}
