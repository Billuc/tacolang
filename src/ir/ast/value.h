#if !defined(VALUE_H__)
#define VALUE_H__

#include "./expression.h"
#include "ir/eval/context.h"
#include "ir/eval/type.h"

typedef enum
{
    v_integer,
    v_floating,
    v_expression,
} ValueType;

typedef union
{
    int integer;
    float floating;
    ExpressionElement *expression;
} Value;

typedef struct valueData
{
    Type *value_type;

    void (*free)(struct valueData *);
} ValueData;

typedef struct valueElement
{
    ValueType type;
    Value value;

    void (*free)(struct valueElement *);
    ValueData *(*eval)(struct valueElement *, EvalContext *);
} ValueElement;

ValueElement *newExpressionValue(ExpressionElement *value);
ValueElement *newIntegerValue(int value);
ValueElement *newFloatValue(float value);

#endif // VALUE_H__
