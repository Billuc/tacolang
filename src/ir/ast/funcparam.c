#include "ir/ast/funcparam.h"
#include "utils/str_utils.h"
#include <stdlib.h>

static void freeFunctionParameter(FunctionParameterElement *functionParameter);
static FunctionParameterData *evalFunctionParameter(FunctionParameterElement *functionParameter, EvalContext *context);

FunctionParameterElement *newFunctionParameter(char *name, TypedefElement *type, location_t location)
{
    FunctionParameterElement *element = malloc(sizeof(FunctionParameterElement));
    element->name = strdup(name);
    element->type = type;
    element->location = location;
    element->free = freeFunctionParameter;
    element->eval = evalFunctionParameter;
    return element;
}

static void freeFunctionParameter(FunctionParameterElement *functionParameter)
{
    free(functionParameter->name);
    functionParameter->type->free(functionParameter->type);
    free(functionParameter);
}

static void freeFunctionParameterData(FunctionParameterData *functionParameterData)
{
    if (functionParameterData == NULL)
        return;

    free(functionParameterData->name);
    free_type(functionParameterData->type);
    free(functionParameterData);
}

static FunctionParameterData *evalFunctionParameter(FunctionParameterElement *functionParameter, EvalContext *context)
{
    if (functionParameter == NULL)
        return NULL;

    TypedefData *typedefData = functionParameter->type->eval(functionParameter->type, context);

    FunctionParameterData *data = malloc(sizeof(FunctionParameterData));
    data->name = strdup(functionParameter->name);
    data->type = copy_type(typedefData->type);
    data->free = freeFunctionParameterData;

    typedefData->free(typedefData);
    return data;
}

FunctionParameterList *newFunctionParameterList(void)
{
    return newLinkedList(freeFunctionParameter);
}