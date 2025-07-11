#if !defined(STR_UTILS_H__)
#define STR_UTILS_H__

#include <stddef.h>

char *strdup(const char *s);
char *strndup(const char *s, size_t size);
char *strremove(char *s, const char remove);
char *strconcat(char *s1, char *s2);
void strappend(char **s1, char *s2);

#endif // STR_UTILS_H__
