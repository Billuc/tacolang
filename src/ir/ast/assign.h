#if !defined(ASSIGN_H__)
#define ASSIGN_H__

#include "value.h"
#include "variable.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    VariableElement *left;
    ValueElement *right;
} AssignElement;

AssignElement *newAssign(VariableElement *left, ValueElement *right);
void freeAssign(AssignElement *assignEl);
void evalAssign(AssignElement *assignElement, SymbolElement **symbolTable);

#endif // ASSIGN_H__
