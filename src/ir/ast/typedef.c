#include <stdlib.h>
#include <string.h>
#include "typedef.h"

TypedefElement *newTypedef(ModifierLink *modifiers, char *name)
{
    TypedefElement *element = malloc(sizeof(TypedefElement));
    element->modifiers = modifiers;
    element->name = strdup(name);
    return element;
}

void freeTypedef(TypedefElement *element)
{
    freeModifierList(element->modifiers);
    free(element->name);
    free(element);
}

void evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable)
{
    // TODO
}