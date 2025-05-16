#include "./mock_value.h"
#include "mock_type.h"

int calls_to_freeValue = 0;
void mock_freeValue(ValueElement *value)
{
    calls_to_freeValue++;
    free(value);
}

int calls_to_freeValueData = 0;
void mock_freeValueData(ValueData *data)
{
    calls_to_freeValueData++;
    free_type(data->value_type);
    free(data);
}

int calls_to_evalValue = 0;
ValueData *mock_evalIntValue(ValueElement *value, EvalContext *context)
{
    calls_to_evalValue++;
    ValueData *data = malloc(sizeof(ValueData));
    data->free = mock_freeValueData;
    data->value_type = mock_intType();
    return data;
}
ValueData *mock_evalFloatValue(ValueElement *value, EvalContext *context)
{
    calls_to_evalValue++;
    ValueData *data = malloc(sizeof(ValueData));
    data->free = mock_freeValueData;
    data->value_type = mock_floatType();
    return data;
}

ValueElement *mock_newValue(MockValueType variant)
{
    ValueElement *value = malloc(sizeof(ValueElement));
    value->free = mock_freeValue;
    switch (variant)
    {
    case mock_IntValue:
        value->eval = mock_evalIntValue;
        break;
    case mock_FloatValue:
        value->eval = mock_evalFloatValue;
        break;
    }

    return value;
}