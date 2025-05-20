#if !defined(TYPEDEF_H__)
#define TYPEDEF_H__

#include "./modifier.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"

typedef struct typedefData
{
    Type *type;

    void (*free)(struct typedefData *);
} TypedefData;

typedef struct typedefElement
{
    char *name;
    ModifierList *modifiers;
    location_t location;

    void (*free)(struct typedefElement *);
    TypedefData *(*eval)(struct typedefElement *, EvalContext *);
} TypedefElement;

TypedefElement *newTypedef(ModifierList *modifiers, char *name, location_t location);

#endif // TYPEDEF_H__
