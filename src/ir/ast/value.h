#if !defined(VALUE_H__)
#define VALUE_H__

#include "expression.h"
#include "common.h"
#include "../symbol_table/symbol_table.h"
#include "../eval/type.h"

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
} ValueData;

typedef struct
{
    EvalType *valueType;
} EvalValueData;

typedef struct valueElement
{
    ValueType type;
    ValueData value;

    FREE_FUNC(struct valueElement, free);
    EVAL_FUNC(struct valueElement, EvalValueData *, eval);
} ValueElement;

ValueElement *newExpressionValue(ExpressionElement *value);
ValueElement *newIntegerValue(int value);
ValueElement *newFloatValue(float value);
// void freeValue(ValueElement *valueEl);
// EvalValueData *evalValue(ValueElement *valueElement, SymbolElement **symbolTable);

#endif // VALUE_H__
