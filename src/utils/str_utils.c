#include <string.h>
#include <stdlib.h>

char *strdup(const char *s)
{
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p)
    {
        memcpy(p, s, size);
    }
    p[size - 1] = '\0';
    return p;
}

char *strndup(const char *s, size_t len)
{
    size_t size = len + 1;
    char *p = malloc(size);
    if (p)
    {
        memcpy(p, s, size);
    }
    p[size - 1] = '\0';
    return p;
}