#if !defined(DECLARE_H__)
#define DECLARE_H__

#include "value.h"
#include "typedef.h"
#include "utils/common.h"
#include "ir/eval/symbol_table.h"

typedef struct evalDeclareData
{
    char *symbolName;
    EvalType *symbolType;

    FREE_FUNC(struct evalDeclareData, free);
} EvalDeclareData;

typedef struct declareElement
{
    char *name;
    TypedefElement *type;

    FREE_FUNC(struct declareElement, free);
    EVAL_FUNC(struct declareElement, EvalDeclareData *, eval);
} DeclareElement;

DeclareElement *newDeclare(char *name, TypedefElement *type);

#endif // DECLARE_H__
