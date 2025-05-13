#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "utils/str_utils.h"
#include "utils/list_utils.h"

static void freeTypedef(TypedefElement *element);
static TypedefData *evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable);

TypedefElement *newTypedef(ModifierList *modifiers, char *name)
{
    TypedefElement *element = malloc(sizeof(TypedefElement));
    element->modifiers = modifiers;
    element->name = strdup(name);
    element->free = freeTypedef;
    element->eval = evalTypedef;
    return element;
}

static void freeTypedef(TypedefElement *element)
{
    freeLinkedList(element->modifiers);
    free(element->name);
    free(element);
}

static void freeTypedefData(TypedefData *data)
{
    free_evalType(data->type);
    free(data);
}

static TypedefData *evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable)
{
    TypedefData *evalData = malloc(sizeof(TypedefData));
    evalData->free = freeTypedefData;

    EvalType *evalType = malloc(sizeof(EvalType));
    evalType->type = t_variable;
    evalType->data.variableTypeData.modifiers;

    EvalModifierLinkData *modifiers = evalModifierList(typedefElement->modifiers, symbolTable);
    evalType->type = t_variable;
    evalType->data.variableTypeData.modifiers = modifiers->typeModifierList;

    int i;
    for (i = 0; i < BASE_TYPES_LEN; i++)
    {
        if (strcmp(BASE_TYPES[i].name, typedefElement->name) == 0)
        {
            evalType->data.variableTypeData.is_base_type = true;
            evalType->data.variableTypeData.type_data.baseType = BASE_TYPES[i].baseType;
            break;
        }
    }

    if (i == BASE_TYPES_LEN)
    {
        evalType->data.variableTypeData.is_base_type = false;
        evalType->data.variableTypeData.type_data.customType = strdup(typedefElement->name);
    }

    evalData->type = evalType;
    return evalData;
}