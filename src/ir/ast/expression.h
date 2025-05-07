#if !defined(EXPRESSION_H__)
#define EXPRESSION_H__

#include "../symbol_table/symbol_table.h"
#include "../eval/type.h"

typedef struct
{
    char *operation;
    // TODO
} ExpressionElement;

typedef struct
{
    EvalType *type;
} EvalExpressionData;

ExpressionElement *newExpression(char *operation);
void freeExpression(ExpressionElement *expressionEl);
EvalExpressionData *evalExpression(ExpressionElement *expressionElement, SymbolElement **symbolTable);

#endif // EXPRESSION_H__
