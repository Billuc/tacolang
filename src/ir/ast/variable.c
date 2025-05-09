#include "variable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern void yyerror(char *s);

VariableElement *newIdentifierVariable(char *identifier)
{
    VariableElement *variable = (VariableElement *)malloc(sizeof(VariableElement));
    if (!variable)
        return NULL;

    variable->type = var_identifier;
    variable->variable.identifier = strdup(identifier);
    if (!variable->variable.identifier)
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

    variable->type = var_declare;
    variable->variable.declare = declare;

    return variable;
}

void freeVariable(VariableElement *variableEl)
{
    if (!variableEl)
        return;

    switch (variableEl->type)
    {
    case var_identifier:
        free(variableEl->variable.identifier);
        break;
    case var_declare:
        freeDeclare(variableEl->variable.declare);
        break;
    }

    free(variableEl);
}

EvalVariableData *evalVariable(VariableElement *variableElement, SymbolElement **symbolTable)
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
        SymbolData *symbol = getSymbol(*symbolTable, variableElement->variable.identifier);
        if (!symbol)
        {
            char buf[100] = "";
            snprintf(buf, 100, "Variable '%s' is not declared in this context", variableElement->variable.identifier);
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
        EvalDeclareData *declareData = evalDeclare(variableElement->variable.declare, symbolTable);
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