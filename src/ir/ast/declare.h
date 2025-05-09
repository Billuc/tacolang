#if !defined(DECLARE_H__)
#define DECLARE_H__

#include "value.h"
#include "typedef.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    char *name;
    TypedefElement *type;
} DeclareElement;

typedef struct
{
    char *symbolName;
    EvalType *symbolType;
} EvalDeclareData;

DeclareElement *newDeclare(char *name, TypedefElement *type);
void freeDeclare(DeclareElement *declareEl);
EvalDeclareData *evalDeclare(DeclareElement *declareElement, SymbolElement **symbolTable);

#endif // DECLARE_H__
