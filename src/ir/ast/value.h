#if !defined(VALUE_H__)
#define VALUE_H__

#include <stdbool.h>
#include "./expression.h"
#include "ir/eval/context.h"
#include "ir/eval/type.h"
#include "ir/ast/funccall.h"

typedef enum
{
    v_integer,
    v_floating,
    v_boolean,
    v_character,
    v_expression,
    v_funccall,
} ValueType;

typedef union
{
    int integer;
    float floating;
    bool boolean;
    char character;
    ExpressionElement *expression;
    FunctionCallElement *function_call;
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
ValueElement *newBooleanValue(bool value);
ValueElement *newCharacterValue(char value);
ValueElement *newFunctionCallValue(FunctionCallElement *value);

// freeValue isn't static because it is used for the argument list in funccall.c
void freeValue(ValueElement *value);

#endif // VALUE_H__
