#include "variable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern void yyerror(char *s);
static void freeVariable(VariableElement *variableEl);
static EvalVariableData *evalVariable(VariableElement *variableElement, SymbolElement **symbolTable);

VariableElement *newIdentifierVariable(char *identifier)
{
    VariableElement *variable = (VariableElement *)malloc(sizeof(VariableElement));
    if (!variable)
        return NULL;

    variable->free = freeVariable;
    variable->eval = evalVariable;
    variable->type = var_identifier;
    variable->data.identifier = strdup(identifier);
    if (!variable->data.identifier)
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
    variable->type = var_declare;
    variable->data.declare = declare;

    return variable;
}

static void freeVariable(VariableElement *variableEl)
{
    if (!variableEl)
        return;

    switch (variableEl->type)
    {
    case var_identifier:
        free(variableEl->data.identifier);
        break;
    case var_declare:
        variableEl->data.declare->free(variableEl->data.declare);
        break;
    }

    free(variableEl);
}

static EvalVariableData *evalVariable(VariableElement *variableElement, SymbolElement **symbolTable)
{
    if (!variableElement)
        return NULL;

    EvalVariableData *evalData = (EvalVariableData *)malloc(sizeof(EvalVariableData));
    if (!evalData)
        return NULL;

    switch (variableElement->type)
    {
    case var_identifier:
    {
        SymbolData *symbol = getSymbol(*symbolTable, variableElement->data.identifier);
        if (!symbol)
        {
            char buf[100] = "";
            snprintf(buf, 100, "Variable '%s' is not declared in this context", variableElement->data.identifier);
            yyerror(buf);
            free(evalData);
            return NULL;
        }
        evalData->variableType = symbol->type;
        evalData->isDeclaration = false;
        break;
    }
    case var_declare:
    {
        EvalDeclareData *declareData = variableElement->data.declare->eval(variableElement->data.declare, symbolTable);
        if (!declareData)
        {
            free(evalData);
            return NULL;
        }
        evalData->variableType = declareData->symbolType;
        evalData->isDeclaration = true;
        break;
    }
    }

    return evalData;
}