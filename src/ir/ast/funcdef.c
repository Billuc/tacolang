#include "ir/ast/funcdef.h"
#include "ir/ast/declare.h"
#include <stdlib.h>
#include "utils/str_utils.h"
#include "ir/eval/type.h"

extern void yyerror(char *s);

static void freeFunctionDefinition(FunctionDefinitionElement *functionDefinition);
static FunctionDefinitionData *evalFunctionDefinition(FunctionDefinitionElement *functionDefinition, EvalContext *context);

FunctionDefinitionElement *newFunctionDefinition(char *name, FunctionParameterList *parameters, TypedefElement *return_type, BlockElement *block)
{
    FunctionDefinitionElement *element = malloc(sizeof(FunctionDefinitionElement));
    element->name = name;
    if (parameters == NULL)
    {
        parameters = newFunctionParameterList();
    }
    element->parameters = parameters;
    element->return_type = return_type;
    element->block = block;
    element->free = freeFunctionDefinition;
    element->eval = evalFunctionDefinition;
    return element;
}

static void freeFunctionDefinition(FunctionDefinitionElement *functionDefinition)
{
    free(functionDefinition->name);
    freeLinkedList(functionDefinition->parameters);
    functionDefinition->return_type->free(functionDefinition->return_type);
    functionDefinition->block->free(functionDefinition->block);
    free(functionDefinition);
}

static void freeFunctionDefinitionData(FunctionDefinitionData *functionDefinitionData)
{
    if (functionDefinitionData == NULL)
        return;

    free_type(functionDefinitionData->functionType);
    free(functionDefinitionData);
}

static FunctionDefinitionData *evalFunctionDefinition(FunctionDefinitionElement *functionDefinition, EvalContext *context)
{
    if (functionDefinition == NULL)
        return NULL;

    FunctionDefinitionData *functionData = malloc(sizeof(FunctionDefinitionData));
    functionData->free = freeFunctionDefinitionData;
    functionData->functionType = malloc(sizeof(Type));
    functionData->functionType->type_type = t_function;
    FunctionType *functionType = &functionData->functionType->type_data.function_type;

    unshift(context->symbolTableStack, create_SymbolTable());

    LinkedListElement *current;
    for (current = functionDefinition->parameters->head; current != NULL; current = current->next)
    {
        FunctionParameterElement *parameter = (FunctionParameterElement *)current->data;
        FunctionParameterData *parameterData = parameter->eval(parameter, context);

        functionType->number_of_args++;
        if (functionType->number_of_args > MAX_NB_OF_ARGS)
        {
            yyerror("Too many parameters in function definition.");
            functionData->free(functionData);
            return NULL;
        }
        copy_simpleType(&parameterData->type->type_data.variable_type, &functionType->argument_types[functionType->number_of_args - 1]);

        SymbolData *symbol = malloc(sizeof(SymbolData));
        symbol->name = strdup(parameter->name);
        symbol->type = copy_type(parameterData->type);
        putSymbol(context, symbol);

        parameterData->free(parameterData);
    }

    TypedefData *returnTypeData = functionDefinition->return_type->eval(functionDefinition->return_type, context);
    functionType->return_type = returnTypeData->type->type_data.variable_type;

    functionDefinition->block->eval(functionDefinition->block, context);

    SymbolTable *table = shift(context->symbolTableStack);
    freeLinkedList(table);

    SymbolData *fnSymbol = malloc(sizeof(SymbolData));
    fnSymbol->name = strdup(functionDefinition->name);
    fnSymbol->type = copy_type(functionData->functionType);
    putSymbol(context, fnSymbol);

    return functionData;
}