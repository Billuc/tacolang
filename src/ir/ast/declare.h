#if !defined(DECLARE_H__)
#define DECLARE_H__

#include "./value.h"
#include "./typedef.h"
#include "ir/eval/context.h"

typedef struct declareData
{
    char *symbol_name;
    Type *symbol_type;

    void (*free)(struct declareData *);
} DeclareData;

typedef struct declareElement
{
    char *name;
    TypedefElement *type;

    void (*free)(struct declareElement *);
    DeclareData *(*eval)(struct declareElement *, EvalContext *);
} DeclareElement;

DeclareElement *newDeclare(char *name, TypedefElement *type);

#endif // DECLARE_H__
