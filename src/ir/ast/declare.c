#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "declare.h"
#include "../../utils/str_utils.h"

extern void yyerror(char *s);

DeclareElement *newDeclare(char *name, TypedefElement *type)
{
    DeclareElement *element = malloc(sizeof(DeclareElement));
    element->name = strdup(name);
    element->type = type;
    return element;
}

void freeDeclare(DeclareElement *element)
{
    free(element->name);
    freeTypedef(element->type);
    free(element);
}

EvalDeclareData *evalDeclare(DeclareElement *declareElement, SymbolElement **symbolTable)
{
    char *variableName = declareElement->name;
    SymbolData *symbol = getSymbol(*symbolTable, variableName);

    if (symbol != NULL)
    {
        char buf[100] = "";
        snprintf(buf, 100, "Variable '%s' has already been declared in this context", variableName);
        yyerror(buf);
        return NULL;
    }

    EvalTypedefData *typedefData = evalTypedef(declareElement->type, symbolTable);

    SymbolData *newSymbol = malloc(sizeof(SymbolData));
    newSymbol->name = strdup(declareElement->name);
    newSymbol->type = typedefData->type;
    SymbolElement *newHead = putSymbol(*symbolTable, newSymbol);
    *symbolTable = newHead;

    EvalType *evalType = malloc(sizeof(EvalType));
    memcpy(evalType, typedefData->type, sizeof(EvalType));

    EvalDeclareData *data = malloc(sizeof(EvalDeclareData));
    data->symbolName = strdup(variableName);
    data->symbolType = evalType;
    return data;
}