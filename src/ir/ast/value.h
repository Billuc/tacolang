#if !defined(VALUE_H__)
#define VALUE_H__

#include "expression.h"
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
    ValueType type;
    ValueData value;
} ValueElement;

typedef struct
{
    EvalType *valueType;
} EvalValueData;

ValueElement *newExpressionValue(ExpressionElement *value);
ValueElement *newIntegerValue(int value);
ValueElement *newFloatValue(float value);
void freeValue(ValueElement *valueEl);
EvalValueData *evalValue(ValueElement *valueElement, SymbolElement **symbolTable);

#endif // VALUE_H__
