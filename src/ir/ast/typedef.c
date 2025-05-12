#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "../../utils/str_utils.h"

static void freeTypedef(TypedefElement *element);
static EvalTypedefData *evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable);

TypedefElement *newTypedef(ModifierLink *modifiers, char *name)
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
    freeModifierList(element->modifiers);
    free(element->name);
    free(element);
}

static EvalTypedefData *evalTypedef(TypedefElement *typedefElement, SymbolElement **symbolTable)
{
    EvalTypedefData *evalData = malloc(sizeof(EvalTypedefData));
    EvalType *evalType = malloc(sizeof(EvalType));

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