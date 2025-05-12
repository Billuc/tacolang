#include <stdlib.h>
#include "value.h"

static void freeValue(ValueElement *valueEl);
static EvalValueData *evalValue(ValueElement *valueElement, SymbolElement **symbolTable);

static const TypeData intTypeData = {
    .is_base_type = true,
    .type_data = {.baseType = U32},
    .modifiers = NULL,
};
static const TypeData floatTypeData = {
    .is_base_type = true,
    .type_data = {.baseType = F32},
    .modifiers = NULL,
};

ValueElement *newExpressionValue(ExpressionElement *value)
{
    ValueElement *element = malloc(sizeof(ValueElement));
    element->type = v_expression;
    element->value.expression = value;
    element->free = freeValue;
    element->eval = evalValue;
    return element;
}

ValueElement *newIntegerValue(int value)
{
    ValueElement *element = malloc(sizeof(ValueElement));
    element->type = v_integer;
    element->value.integer = value;
    element->free = freeValue;
    element->eval = evalValue;
    return element;
}

ValueElement *newFloatValue(float value)
{
    ValueElement *element = malloc(sizeof(ValueElement));
    element->type = v_floating;
    element->value.floating = value;
    element->free = freeValue;
    element->eval = evalValue;
    return element;
}

static void freeValue(ValueElement *valueEl)
{
    if (valueEl->type == v_expression)
        valueEl->value.expression->free(valueEl->value.expression);

    free(valueEl);
}

static EvalValueData *evalValue(ValueElement *valueElement, SymbolElement **symbolTable)
{
    EvalValueData *valueData = malloc(sizeof(EvalValueData));
    EvalType *evalType;

    switch (valueElement->type)
    {
    case v_integer:
    {
        evalType = malloc(sizeof(EvalType));
        evalType->type = t_variable;
        EvalTypeData int_etd = {.variableTypeData = intTypeData};
        evalType->data = int_etd;
        break;
    }
    case v_floating:
    {
        evalType = malloc(sizeof(EvalType));
        evalType->type = t_variable;
        EvalTypeData float_etd = {.variableTypeData = floatTypeData};
        evalType->data = float_etd;
        break;
    }
    case v_expression:
        EvalExpressionData *expressionData = valueElement->value.expression->eval(valueElement->value.expression, symbolTable);
        evalType = expressionData->type;
        break;
    }

    valueData->valueType = evalType;
    return valueData;
}