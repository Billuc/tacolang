#if !defined(EXPRESSION_H__)
#define EXPRESSION_H__

#include "ir/eval/context.h"
#include "ir/eval/type.h"

typedef struct expressionData
{
    Type *type;

    void (*free)(struct expressionData *);
} ExpressionData;

typedef struct expressionElement
{
    char *operation;

    void (*free)(struct expressionElement *);
    ExpressionData *(*eval)(struct expressionElement *, EvalContext *);
} ExpressionElement;

ExpressionElement *newExpression(char *operation);

#endif // EXPRESSION_H__
