#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modifier.h"

#define M_MUTABLE "mut"
#define M_REFERENCE "ref"
#define M_OPTIONAL "opt"

extern void yyerror(char *s);

ModifierElement *newModifier(char *type)
{
    ModifierElement *element = malloc(sizeof(ModifierElement));

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

void freeModifier(ModifierElement *element)
{
    if (element == NULL)
        return;

    free(element);
}

void evalModifier(ModifierElement *element, SymbolElement **symbolTable)
{
    // TODO
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
        freeModifier(iter->data);
        ModifierLink *next = iter->next;
        free(iter);
        iter = next;
    }
}

void evalModifierList(ModifierLink *list)
{
    // TODO
}