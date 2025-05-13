#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modifier.h"

#define M_MUTABLE "mut"
#define M_REFERENCE "ref"
#define M_OPTIONAL "opt"

extern void yyerror(char *s);
static void freeModifier(ModifierElement *element);
static ModifierData *evalModifier(ModifierElement *element, SymbolElement **symbolTable);

ModifierElement *newModifier(char *type)
{
    ModifierElement *element = malloc(sizeof(ModifierElement));
    element->free = freeModifier;
    element->eval = evalModifier;

    if (!strcmp(type, M_MUTABLE))
        element->type = m_mutable;
    else if (!strcmp(type, M_REFERENCE))
        element->type = m_reference;
    else if (!strcmp(type, M_OPTIONAL))
        element->type = m_optional;
    else
    {
        char buf[100];
        sprintf(buf, "Unknown modifier: %s", type);
        yyerror(buf);
        free(element);
        return NULL;
    }

    return element;
}

static void freeModifier(ModifierElement *element)
{
    if (element == NULL)
        return;

    free(element);
}

static void freeModifierData(ModifierData *data)
{
    if (data == NULL)
        return;

    if (data->typeModifier != NULL)
        data->typeModifier->free(data->typeModifier);

    free(data);
}

static ModifierData *evalModifier(ModifierElement *element, SymbolElement **symbolTable)
{
    ModifierData *data = malloc(sizeof(ModifierData));
    data->free = freeModifierData;

    switch (element->type)
    {
    case m_mutable:
        data->typeModifier->modifier_type = tm_mutable;
        break;
    case m_reference:
        data->typeModifier->modifier_type = tm_reference;
        break;
    case m_optional:
        data->typeModifier->modifier_type = tm_optional;
        break;
    }

    return data;
}

ModifierList *newModifierList()
{
    return newLinkedList(freeModifier);
}