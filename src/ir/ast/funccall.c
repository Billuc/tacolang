#include "ir/ast/funccall.h"
#include "ir/ast/value.h"
#include <stdlib.h>
#include "utils/str_utils.h"
#include <string.h>
#include <stdio.h>

static void freeFunctionCallElement(FunctionCallElement *functionCall);
static FunctionCall *evalFunctionCallElement(FunctionCallElement *functionCall, EvalContext *context);

FunctionCallElement *newFunctionCall(char *functionName, FunctionArgumentList *arguments, location_t loc)
{
    FunctionCallElement *element = malloc(sizeof(FunctionCallElement));
    element->functionName = strdup(functionName);
    element->location = loc;
    if (arguments == NULL)
    {
        arguments = newFunctionArgumentList();
    }
    element->arguments = arguments;
    element->free = freeFunctionCallElement;
    element->eval = evalFunctionCallElement;
    return element;
}

static void freeFunctionCallElement(FunctionCallElement *functionCall)
{
    free(functionCall->functionName);
    freeLinkedList(functionCall->arguments);
    free(functionCall);
}

static void freeFunctionCall(FunctionCall *functionCall)
{
    if (functionCall == NULL)
        return;

    free_type(functionCall->returnType);
    free(functionCall);
}

static FunctionCall *evalFunctionCallElement(FunctionCallElement *functionCall, EvalContext *context)
{
    if (functionCall == NULL)
        return NULL;

    FunctionCall *call = malloc(sizeof(FunctionCall));
    call->free = freeFunctionCall;

    // find the function in the EvalContext
    SymbolData *funcdef = findSymbol(context, functionCall->functionName);
    if (funcdef == NULL)
    {
        print_error(functionCall->location, "Function '%s' not defined in this context", functionCall->functionName);
        free(call);
        return NULL;
    }

    // check if the function is a function type
    if (funcdef->type->type_type != t_function)
    {
        print_error(functionCall->location, "Symbol '%s' is not a function", functionCall->functionName);
        return NULL;
    }

    // check if the arguments have the same types as the parameters
    FunctionType *functionType = &funcdef->type->type_data.function_type;
    if (functionType->number_of_args != functionCall->arguments->size)
    {
        print_error(functionCall->location, "Function '%s' takes %d arguments, but %d were given", functionCall->functionName, functionType->number_of_args, functionCall->arguments->size);
        free(call);
        return NULL;
    }

    LinkedListElement *currentArg = functionCall->arguments->head;
    for (int i = 0; i < functionType->number_of_args; i++)
    {
        ValueElement *argument = (ValueElement *)currentArg->data;
        ValueData *argValue = argument->eval(argument, context);

        if (argValue->value_type->type_type != t_variable)
        {
            print_error(functionCall->location, "Argument %d of function '%s' is not a variable type", i + 1, functionCall->functionName);
            free(call);
            return NULL;
        }

        SimpleType argType = argValue->value_type->type_data.variable_type;
        SimpleType paramType = functionType->argument_types[i];

        if (compare_simpleType(argType, paramType) != 0)
        {
            char *argTypeStr = print_simpleType(argType);
            char *paramTypeStr = print_simpleType(paramType);
            print_error(argument->location, "Argument %d of function '%s' has type '%s', but parameter has type '%s'", i + 1, functionCall->functionName, argTypeStr, paramTypeStr);
            free(argTypeStr);
            free(paramTypeStr);
            free(call);
            return NULL;
        }
    }

    Type returnType = {.type_type = t_variable, .type_data = {.variable_type = functionType->return_type}};
    call->returnType = copy_type(&returnType);
    return call;
}

FunctionArgumentList *newFunctionArgumentList()
{
    return newLinkedList(freeValue);
}