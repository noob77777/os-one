#include <lib/string.h>

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

char **strtoken(const char *s, char delimeter, int *num)
{
    int n = 0;
    if (strlen(s))
        n++;
    for (int i = 1; i < strlen(s); i++)
    {
        if (s[i] == delimeter && s[i - 1] != delimeter)
            n++;
    }
    *num = n;
    char **res = new char *[n];
    int ptr = 0;
    for (int i = 0; i < n; i++)
    {
        while (s[ptr] == delimeter)
            ptr++;

        int delta = 0;
        for (int j = ptr; (s[j] != delimeter && s[j] != 0); j++)
            delta = j - ptr + 1;

        res[i] = new char[delta];
        int lc = 0;
        for (int j = ptr; (s[j] != delimeter && s[j] != 0); j++)
            res[i][lc++] = s[j];
        res[i][lc] = 0;

        ptr += lc;
    }
    return res;
}
