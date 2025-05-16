#if !defined(MOCK_VALUE_H__)
#define MOCK_VALUE_H__

#include "ir/ast/value.h"

typedef enum
{
    mock_FloatValue,
    mock_IntValue,
} MockValueType;

extern int calls_to_freeValue;
extern int calls_to_freeValueData;
extern int calls_to_evalValue;

ValueElement *mock_newValue(MockValueType variant);

#endif // MOCK_VALUE_H__
