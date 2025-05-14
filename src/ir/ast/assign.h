#if !defined(ASSIGN_H__)
#define ASSIGN_H__

#include "./value.h"
#include "./variable.h"
#include "ir/eval/context.h"

typedef struct assignElement
{
    VariableElement *left;
    ValueElement *right;

    void (*free)(struct assignElement *);
    void (*eval)(struct assignElement *, EvalContext *);
} AssignElement;

AssignElement *newAssign(VariableElement *left, ValueElement *right);

#endif // ASSIGN_H__
