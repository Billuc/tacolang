#if !defined(ASSIGN_H__)
#define ASSIGN_H__

#include "value.h"
#include "variable.h"
#include "common.h"
#include "../symbol_table/symbol_table.h"

typedef struct assignElement
{
    VariableElement *left;
    ValueElement *right;

    FREE_FUNC(struct assignElement, free);
    EVAL_FUNC(struct assignElement, void, eval);
} AssignElement;

AssignElement *newAssign(VariableElement *left, ValueElement *right);
// void freeAssign(AssignElement *assignEl);
// void evalAssign(AssignElement *assignElement, SymbolElement **symbolTable);

#endif // ASSIGN_H__
