#include <stdlib.h>
#include <string.h>
#include "declare.h"

DeclareElement *newDeclare(char *name, TypedefElement *type, ValueElement *value)
{
    DeclareElement *element = malloc(sizeof(DeclareElement));
    element->name = strdup(name);
    element->type = type;
    element->value = value;
    return element;
}

void freeDeclare(DeclareElement *element)
{
    free(element->name);
    freeTypedef(element->type);
    freeValue(element->value);
    free(element);
}

void evalDeclare(DeclareElement *declareElement, SymbolElement **symbolTable)
{
    // TODO
}