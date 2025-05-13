#if !defined(EXPRESSION_H__)
#define EXPRESSION_H__

#include "common.h"
#include "ir/eval/symbol_table.h"
#include "ir/eval/type.h"

typedef struct expressionElement
{
    EvalType *type;

    FREE_FUNC(struct expressionElement, free);
} EvalExpressionData;

typedef struct expressionElement
{
    char *operation;

    FREE_FUNC(struct expressionElement, free);
    EVAL_FUNC(struct expressionElement, EvalExpressionData *, eval);
} ExpressionElement;

ExpressionElement *newExpression(char *operation);

#endif // EXPRESSION_H__
