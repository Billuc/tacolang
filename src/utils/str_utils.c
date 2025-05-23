#include <stdlib.h>
#include <string.h>

char *strdup(const char *s) {
  size_t size = strlen(s) + 1;
  char *p = malloc(size);
  if (p) {
    memcpy(p, s, size);
  }
  p[size - 1] = '\0';
  return p;
}

char *strndup(const char *s, size_t len) {
  size_t size = len + 1;
  char *p = malloc(size);
  if (p) {
    memcpy(p, s, size);
  }
  p[size - 1] = '\0';
  return p;
}

char *strremove(char *s, const char remove) {
  char *p = s;
  char *q = s;
  while (*p) {
    if (*p != remove) {
      *q++ = *p;
    }
    p++;
  }
  *q = '\0';
  return s;
}

char *strconcat(char *s1, char *s2) {
  size_t len1 = strlen(s1);
  size_t len2 = strlen(s2);
  char *result = malloc(len1 + len2 + 1);
  if (result) {
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2);
    result[len1 + len2] = '\0';
  }
  return result;
}

void strappend(char **s1, char *s2) {
  *s1 = realloc(*s1, strlen(*s1) + strlen(s2) + 1);
  if (*s1) {
    strcat(*s1, s2);
  }
}
