#if !defined(EXPRESSION_H__)
#define EXPRESSION_H__

#include "common.h"
#include "../symbol_table/symbol_table.h"
#include "../eval/type.h"

typedef struct
{
    EvalType *type;
} EvalExpressionData;

typedef struct expressionElement
{
    char *operation;

    FREE_FUNC(struct expressionElement, free);
    EVAL_FUNC(struct expressionElement, EvalExpressionData *, eval);
} ExpressionElement;

ExpressionElement *newExpression(char *operation);
// void freeExpression(ExpressionElement *expressionEl);
// EvalExpressionData *evalExpression(ExpressionElement *expressionElement, SymbolElement **symbolTable);

#endif // EXPRESSION_H__
