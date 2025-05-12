#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modifier.h"

#define M_MUTABLE "mut"
#define M_REFERENCE "ref"
#define M_OPTIONAL "opt"

extern void yyerror(char *s);
static void freeModifier(ModifierElement *element);
static EvalModifierData *evalModifier(ModifierElement *element, SymbolElement **symbolTable);

ModifierElement *newModifier(char *type)
{
    ModifierElement *element = malloc(sizeof(ModifierElement));
    element->free = freeModifier;
    element->eval = evalModifier;

    if (!strcmp(type, M_MUTABLE))
        element->type = mutable;
    else if (!strcmp(type, M_REFERENCE))
        element->type = reference;
    else if (!strcmp(type, M_OPTIONAL))
        element->type = optional;
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

static EvalModifierData *evalModifier(ModifierElement *element, SymbolElement **symbolTable)
{
    EvalModifierData *data = malloc(sizeof(EvalModifierData));

    switch (element->type)
    {
    case mutable:
        data->typeModifier.modifier_type = tm_mutable;
        break;
    case reference:
        data->typeModifier.modifier_type = tm_reference;
        break;
    case optional:
        data->typeModifier.modifier_type = tm_optional;
        break;
    }

    return data;
}

ModifierLink *addModifier(ModifierLink *list, ModifierElement *newElement)
{
    ModifierLink *newHead = malloc(sizeof(ModifierLink));
    newHead->data = newElement;
    newHead->next = list;
    return newHead;
}

void freeModifierList(ModifierLink *list)
{
    ModifierLink *iter = list;

    while (iter != NULL)
    {
        iter->data->free(iter->data);
        ModifierLink *next = iter->next;
        free(iter);
        iter = next;
    }
}

EvalModifierLinkData *evalModifierList(ModifierLink *list, SymbolElement **symbolTable)
{
    EvalModifierLinkData *data = malloc(sizeof(EvalModifierLinkData));
    data->typeModifierList = NULL;
    ModifierLink *iter = list;

    while (iter != NULL)
    {
        EvalModifierData *iterData = evalModifier(iter->data, symbolTable);
        TypeModifierLink *newHead = malloc(sizeof(TypeModifierLink));
        newHead->element = iterData->typeModifier;
        newHead->next = data->typeModifierList;
        data->typeModifierList = newHead;
        iter = iter->next;
    }

    return data;
}