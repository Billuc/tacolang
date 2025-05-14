#if !defined(TYPEDEF_H__)
#define TYPEDEF_H__

#include "./modifier.h"
#include "ir/eval/context.h"

typedef struct typedefData
{
    Type *type;

    void (*free)(struct typedefData *);
} TypedefData;

typedef struct typedefElement
{
    char *name;
    ModifierList *modifiers;

    void (*free)(struct typedefElement *);
    TypedefData *(*eval)(struct typedefElement *, EvalContext *);
} TypedefElement;

TypedefElement *newTypedef(ModifierList *modifiers, char *name);

#endif // TYPEDEF_H__
