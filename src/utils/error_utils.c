#include "utils/error_utils.h"
#include <stdarg.h>
#include <stdio.h>

void yyerror(const char *s)
{
    extern int yylineno;
    fprintf(stderr, "%s at line %d\n", s, yylineno);
}

void print_error(location_t loc, const char *message, ...)
{
    va_list args;
    va_start(args, message);

    char location_message[100];
    snprintf(location_message,
             sizeof(location_message),
             "%s:%d:%d: error: ",
             loc.filename,
             loc.first_line,
             loc.first_column);

    char error_message[2500];
    vsnprintf(error_message, sizeof(error_message), message, args);

    char full_message[2601];
    snprintf(full_message, sizeof(full_message), "%s%s", location_message, error_message);

    yyerror(full_message);

    va_end(args);
}