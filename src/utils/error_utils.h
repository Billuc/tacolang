#if !defined(ERROR_UTILS_H__)
#define ERROR_UTILS_H__

#include <stdlib.h>

typedef struct
{
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    char *filename;
} location_t;

#define YYLTYPE_IS_DECLARED 1

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                             \
    do                                                              \
        if (N)                                                      \
        {                                                           \
            (Current).first_line = YYRHSLOC(Rhs, 1).first_line;     \
            (Current).first_column = YYRHSLOC(Rhs, 1).first_column; \
            (Current).last_line = YYRHSLOC(Rhs, N).last_line;       \
            (Current).last_column = YYRHSLOC(Rhs, N).last_column;   \
            (Current).filename = YYRHSLOC(Rhs, N).filename;         \
        }                                                           \
        else                                                        \
        {                                                           \
            (Current).first_line = (Current).last_line =            \
                YYRHSLOC(Rhs, 0).last_line;                         \
            (Current).first_column = (Current).last_column =        \
                YYRHSLOC(Rhs, 0).last_column;                       \
            (Current).filename = YYRHSLOC(Rhs, 0).filename;         \
        }                                                           \
    while (0)
#endif

void yyerror(const char *s);
void print_error(location_t loc, const char *message, ...);

#endif // ERROR_UTILS_H__
