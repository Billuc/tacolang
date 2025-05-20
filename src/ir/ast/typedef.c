#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "utils/str_utils.h"
#include "utils/list_utils.h"

static void freeTypedef(TypedefElement *element);
static TypedefData *evalTypedef(TypedefElement *typedefElement, EvalContext *context);

TypedefElement *newTypedef(ModifierList *modifiers, char *name, location_t location)
{
    TypedefElement *element = malloc(sizeof(TypedefElement));
    element->modifiers = modifiers;
    element->location = location;
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
    free_type(data->type);
    free(data);
}

static TypedefData *evalTypedef(TypedefElement *typedefElement, EvalContext *context)
{
    TypedefData *result = malloc(sizeof(TypedefData));
    result->free = freeTypedefData;

    Type *defined_type = malloc(sizeof(Type));
    defined_type->type_type = t_variable;

    SimpleType *simpleType = &defined_type->type_data.variable_type;
    simpleType->modifiers = newTypeModifierList();

    LinkedListElement *current;
    for (current = typedefElement->modifiers->head; current != NULL; current = current->next)
    {
        ModifierElement *modifier = (ModifierElement *)current->data;
        ModifierData *m_data = modifier->eval(modifier, context);
        TypeModifier *tm_copy = copy_typeModifier(m_data->typeModifier);
        push(simpleType->modifiers, tm_copy);
        m_data->free(m_data);
    }

    int i;
    for (i = 0; i < BASE_TYPES_LEN; i++)
    {
        if (strcmp(BASE_TYPES[i].name, typedefElement->name) == 0)
        {
            simpleType->is_base_type = true;
            simpleType->type_data.base_type = BASE_TYPES[i].baseType;
            break;
        }
    }

    if (i == BASE_TYPES_LEN)
    {
        simpleType->is_base_type = false;
        simpleType->type_data.custom_type = strdup(typedefElement->name);
    }

    result->type = defined_type;
    return result;
}