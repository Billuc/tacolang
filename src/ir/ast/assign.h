#if !defined(ASSIGN_H__)
#define ASSIGN_H__

#include "value.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    char *name;
    ValueElement *value;
} AssignElement;

AssignElement *newAssign(char *name, ValueElement *value);
void freeAssign(AssignElement *assignEl);
void evalAssign(AssignElement *assignElement, SymbolElement **symbolTable);

#endif // ASSIGN_H__
