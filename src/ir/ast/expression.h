#if !defined(EXPRESSION_H__)
#define EXPRESSION_H__

#include "ir/eval/context.h"
#include "ir/eval/type.h"
#include "utils/error_utils.h"

typedef struct expressionData
{
    Type *type;
    char *generated_code;

    void (*free)(struct expressionData *);
} ExpressionData;

typedef struct expressionElement
{
    char *operation;
    location_t location;

    void (*free)(struct expressionElement *);
    ExpressionData *(*eval)(struct expressionElement *, EvalContext *);
} ExpressionElement;

ExpressionElement *newExpression(char *operation, location_t location);

#endif // EXPRESSION_H__
