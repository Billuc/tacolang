#if !defined(STR_UTILS_H__)
#define STR_UTILS_H__

#include <stddef.h>

char *strdup(const char *s);
char *strndup(const char *s, size_t size);
char *strremove(char *s, const char remove);

#endif // STR_UTILS_H__
