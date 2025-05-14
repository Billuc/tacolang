#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "declare.h"
#include "utils/str_utils.h"

extern void yyerror(char *s);
static void freeDeclare(DeclareElement *element);
static DeclareData *evalDeclare(DeclareElement *declareElement, EvalContext *context);

DeclareElement *newDeclare(char *name, TypedefElement *type)
{
    DeclareElement *element = malloc(sizeof(DeclareElement));
    element->name = strdup(name);
    element->type = type;
    element->free = freeDeclare;
    element->eval = evalDeclare;
    return element;
}

void freeDeclare(DeclareElement *element)
{
    free(element->name);
    element->type->free(element->type);
    free(element);
}

static void freeDeclareData(DeclareData *data)
{
    free(data->symbol_name);
    free_type(data->symbol_type);
    free(data);
}

DeclareData *evalDeclare(DeclareElement *declareElement, EvalContext *context)
{
    char *variableName = declareElement->name;
    SymbolData *symbol = findSymbol(context, variableName);

    if (symbol != NULL)
    {
        char buf[100] = "";
        snprintf(buf, 100, "Variable '%s' has already been declared in this context", variableName);
        yyerror(buf);
        return NULL;
    }

    TypedefData *typedefData = declareElement->type->eval(declareElement->type, context);

    SymbolData *newSymbol = malloc(sizeof(SymbolData));
    newSymbol->name = strdup(declareElement->name);
    newSymbol->type = copy_type(typedefData->type);
    putSymbol(context, newSymbol);

    DeclareData *data = malloc(sizeof(DeclareData));
    data->free = freeDeclareData;
    data->symbol_name = strdup(variableName);
    data->symbol_type = copy_type(typedefData->type);

    typedefData->free(typedefData);

    return data;
}