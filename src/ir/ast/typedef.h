#if !defined(TYPEDEF_H__)
#define TYPEDEF_H__

#include "modifier.h"
#include "common.h"
#include "../symbol_table/symbol_table.h"

typedef struct
{
    EvalType *type;
} EvalTypedefData;

typedef struct typedefElement
{
    char *name;
    ModifierLink *modifiers;

    FREE_FUNC(struct typedefElement, free);
    EVAL_FUNC(struct typedefElement, EvalTypedefData *, eval);
} TypedefElement;

TypedefElement *newTypedef(ModifierLink *modifiers, char *name);
// void freeTypedef(TypedefElement *typedefEl);
// EvalTypedefData *evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable);

#endif // TYPEDEF_H__
