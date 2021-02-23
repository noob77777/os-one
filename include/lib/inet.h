#ifndef __INET_H
#define __INET_H

#include <types.h>

/*
 * invert endianess
 */
uint64_t reflect(uint64_t x);
uint32_t reflect(uint32_t x);
uint16_t reflect(uint16_t x);

#endif
