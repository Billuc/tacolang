#if !defined(DECLARE_H__)
#define DECLARE_H__

#include "value.h"
#include "typedef.h"
#include "common.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    char *symbolName;
    EvalType *symbolType;
} EvalDeclareData;

typedef struct declareElement
{
    char *name;
    TypedefElement *type;

    FREE_FUNC(struct declareElement, free);
    EVAL_FUNC(struct declareElement, EvalDeclareData *, eval);
} DeclareElement;

DeclareElement *newDeclare(char *name, TypedefElement *type);
// void freeDeclare(DeclareElement *declareEl);
// EvalDeclareData *evalDeclare(DeclareElement *declareElement, SymbolElement **symbolTable);

#endif // DECLARE_H__
