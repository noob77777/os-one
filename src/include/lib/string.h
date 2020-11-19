#ifndef __STRING_H
#define __STRING_H

#include <types.h>

uint32_t strlen(const char *s)
{
    uint32_t n = 0;
    for (int i = 0; s[i] != '\0'; i++)
        n++;
    return n;
}

bool strcmp(const char *s1, const char *s2)
{
    if (strlen(s1) != strlen(s2))
        return false;
    for (int i = 0; s1[i] != '\0'; i++)
        if (s1[i] != s2[i])
            return false;
    return true;
}

void strcpy(char *dest, const char *source)
{
    for (int i = 0; source[i] != '\0'; i++)
    {
        dest[i] = source[i];
        dest[i + 1] = '\0';
    }
}

#endif
