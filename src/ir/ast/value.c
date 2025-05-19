#include <stdlib.h>
#include "value.h"

static ValueData *evalValue(ValueElement *valueElement, EvalContext *context);

static const SimpleType intTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = U32},
    .modifiers = NULL,
};
static const SimpleType floatTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = F32},
    .modifiers = NULL,
};
static const SimpleType boolTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = Boolean},
    .modifiers = NULL,
};
static const SimpleType charTypeData = {
    .is_base_type = true,
    .type_data = {.base_type = U8},
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

ValueElement *newBooleanValue(bool value)
{
    ValueElement *element = malloc(sizeof(ValueElement));
    element->type = v_boolean;
    element->value.boolean = value;
    element->free = freeValue;
    element->eval = evalValue;
    return element;
}

ValueElement *newCharacterValue(char value)
{
    ValueElement *element = malloc(sizeof(ValueElement));
    element->type = v_character;
    element->value.character = value;
    element->free = freeValue;
    element->eval = evalValue;
    return element;
}

ValueElement *newFunctionCallValue(FunctionCallElement *value)
{
    ValueElement *element = malloc(sizeof(ValueElement));
    element->type = v_funccall;
    element->value.function_call = value;
    element->free = freeValue;
    element->eval = evalValue;
    return element;
}

void freeValue(ValueElement *valueEl)
{
    if (valueEl->type == v_expression)
        valueEl->value.expression->free(valueEl->value.expression);
    else if (valueEl->type == v_funccall)
        valueEl->value.function_call->free(valueEl->value.function_call);

    free(valueEl);
}

static void freeValueData(ValueData *valueData)
{
    free_type(valueData->value_type);
    free(valueData);
}

static ValueData *evalValue(ValueElement *valueElement, EvalContext *context)
{
    ValueData *valueData = malloc(sizeof(ValueData));
    Type *valueType;

    switch (valueElement->type)
    {
    case v_integer:
    {
        valueType = malloc(sizeof(Type));
        valueType->type_type = t_variable;
        TypeData int_td = {.variable_type = intTypeData};
        int_td.variable_type.modifiers = newTypeModifierList(); // We get segfault if we don't do this
        valueType->type_data = int_td;
        break;
    }
    case v_floating:
    {
        valueType = malloc(sizeof(Type));
        valueType->type_type = t_variable;
        TypeData float_td = {.variable_type = floatTypeData};
        float_td.variable_type.modifiers = newTypeModifierList(); // We get segfault if we don't do this
        valueType->type_data = float_td;
        break;
    }
    case v_boolean:
    {
        valueType = malloc(sizeof(Type));
        valueType->type_type = t_variable;
        TypeData bool_td = {.variable_type = boolTypeData};
        bool_td.variable_type.modifiers = newTypeModifierList(); // We get segfault if we don't do this
        valueType->type_data = bool_td;
        break;
    }
    case v_character:
    {
        valueType = malloc(sizeof(Type));
        valueType->type_type = t_variable;
        TypeData char_td = {.variable_type = charTypeData};
        char_td.variable_type.modifiers = newTypeModifierList(); // We get segfault if we don't do this
        valueType->type_data = char_td;
        break;
    }
    case v_expression:
    {
        ExpressionElement *expression = valueElement->value.expression;
        ExpressionData *expressionData = expression->eval(expression, context);
        valueType = copy_type(expressionData->type);
        expressionData->free(expressionData);
        break;
    }
    case v_funccall:
    {
        FunctionCallElement *functionCall = valueElement->value.function_call;
        FunctionCall *functionCallData = functionCall->eval(functionCall, context);

        if (functionCallData == NULL)
        {
            free(valueData);
            return NULL;
        }

        valueType = copy_type(functionCallData->returnType);
        functionCallData->free(functionCallData);
        break;
    }
    }

    valueData->value_type = valueType;
    valueData->free = freeValueData;
    return valueData;
}