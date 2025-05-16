#include "mock_typedef.h"
#include "mock_type.h"
#include <stdlib.h>

int calls_to_freeTypedef = 0;
static void mock_freeTypedef(TypedefElement *typedefEl)
{
    calls_to_freeTypedef++;
    free(typedefEl);
}

int calls_to_freeTypedefData = 0;
static void mock_freeTypedefData(TypedefData *data)
{
    calls_to_freeTypedefData++;
    free_type(data->type);
    free(data);
}

int calls_to_evalTypedef = 0;
static TypedefData *mock_evalTypedef(TypedefElement *typedefEl, EvalContext *context)
{
    calls_to_evalTypedef++;
    TypedefData *data = malloc(sizeof(TypedefData));
    data->type = mock_intType();
    data->free = mock_freeTypedefData;
    return data;
}

TypedefElement *mock_newTypedef(void)
{
    TypedefElement *typedefEl = malloc(sizeof(TypedefElement));
    typedefEl->free = mock_freeTypedef;
    typedefEl->eval = mock_evalTypedef;
    return typedefEl;
}