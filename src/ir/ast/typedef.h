#if !defined(TYPEDEF_H__)
#define TYPEDEF_H__

#include "modifier.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    char *name;
    ModifierLink *modifiers;
} TypedefElement;

typedef struct
{
    EvalType *type;
} EvalTypedefData;

TypedefElement *newTypedef(ModifierLink *modifiers, char *name);
void freeTypedef(TypedefElement *typedefEl);
EvalTypedefData *evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable);

#endif // TYPEDEF_H__
