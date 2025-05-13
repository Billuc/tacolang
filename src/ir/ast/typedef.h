#if !defined(TYPEDEF_H__)
#define TYPEDEF_H__

#include "modifier.h"
#include "common.h"
#include "ir/eval/symbol_table.h"

typedef struct typedefData
{
    EvalType *type;

    FREE_FUNC(struct typedefData, free);
} TypedefData;

typedef struct typedefElement
{
    char *name;
    ModifierList *modifiers;

    FREE_FUNC(struct typedefElement, free);
    EVAL_FUNC(struct typedefElement, TypedefData *, eval);
} TypedefElement;

TypedefElement *newTypedef(ModifierList *modifiers, char *name);

#endif // TYPEDEF_H__
