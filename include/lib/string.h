#ifndef __STRING_H
#define __STRING_H

#include <types.h>
#include <memory/malloc.h>

uint32_t strlen(const char *s);

bool strcmp(const char *s1, const char *s2);

void strcpy(char *dest, const char *source);

char **strtoken(const char *s, char delimeter, int *num);

#endif
