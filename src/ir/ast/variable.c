#include "variable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern void yyerror(char *s);
static void freeVariable(VariableElement *variableEl);
static VariableData *evalVariable(VariableElement *variableElement, EvalContext *context);

VariableElement *newIdentifierVariable(char *identifier)
{
    VariableElement *variable = (VariableElement *)malloc(sizeof(VariableElement));
    if (!variable)
        return NULL;

    variable->free = freeVariable;
    variable->eval = evalVariable;
    variable->element_type = var_identifier;
    variable->element_data.identifier = strdup(identifier);

    if (!variable->element_data.identifier)
    {
        free(variable);
        return NULL;
    }

    return variable;
}

VariableElement *newDeclareVariable(DeclareElement *declare)
{
    VariableElement *variable = (VariableElement *)malloc(sizeof(VariableElement));
    if (!variable)
        return NULL;

    variable->free = freeVariable;
    variable->eval = evalVariable;
    variable->element_type = var_declare;
    variable->element_data.declare = declare;

    return variable;
}

static void freeVariable(VariableElement *variableEl)
{
    if (!variableEl)
        return;

    switch (variableEl->element_type)
    {
    case var_identifier:
        free(variableEl->element_data.identifier);
        break;
    case var_declare:
        variableEl->element_data.declare->free(variableEl->element_data.declare);
        break;
    }

    free(variableEl);
}

static void freeVariableData(VariableData *variableData)
{
    if (!variableData)
        return;

    free_type(variableData->variable_type);
    free(variableData);
}

static VariableData *evalVariable(VariableElement *variableElement, EvalContext *context)
{
    if (!variableElement)
        return NULL;

    VariableData *result = malloc(sizeof(VariableData));
    if (!result)
        return NULL;
    result->free = freeVariableData;

    switch (variableElement->element_type)
    {
    case var_identifier:
    {
        SymbolData *symbol = findSymbol(context, variableElement->element_data.identifier);
        if (!symbol)
        {
            char buf[100] = "";
            snprintf(buf, 100, "Variable '%s' is not declared in this context", variableElement->element_data.identifier);
            yyerror(buf);
            free(result);
            return NULL;
        }
        result->variable_type = copy_type(symbol->type);
        result->is_declaration = false;
        break;
    }
    case var_declare:
    {
        DeclareElement *declare = variableElement->element_data.declare;
        DeclareData *declareData = declare->eval(declare, context);
        if (!declareData)
        {
            free(result);
            return NULL;
        }
        result->variable_type = copy_type(declareData->symbol_type);
        result->is_declaration = true;

        declareData->free(declareData);
        break;
    }
    }

    return result;
}