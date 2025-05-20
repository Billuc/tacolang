#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assign.h"

extern void yyerror(const char *s);
static void freeAssign(AssignElement *assignElement);
static void evalAssign(AssignElement *assignElement, EvalContext *context);

AssignElement *newAssign(VariableElement *left, ValueElement *right, location_t loc)
{
    AssignElement *element = malloc(sizeof(AssignElement));
    element->left = left;
    element->right = right;
    element->location = loc;
    element->free = freeAssign;
    element->eval = evalAssign;
    return element;
}

static void freeAssign(AssignElement *assignElement)
{
    assignElement->left->free(assignElement->left);
    assignElement->right->free(assignElement->right);
    free(assignElement);
}

static bool canAssign(VariableData *variable)
{
    if (variable == NULL)
        return false;

    TypeModifierList *modifiers = variable->variable_type->type_data.variable_type.modifiers;
    if (modifiers == NULL || modifiers->size == 0)
        return false;

    TypeModifier *firstModifier = (TypeModifier *)modifiers->head->data;
    return firstModifier->modifier_type == tm_mutable;
}

static void evalAssign(AssignElement *assignElement, EvalContext *context)
{
    if (assignElement == NULL)
        return;

    VariableElement *variable = assignElement->left;
    VariableData *variableData = variable->eval(variable, context);

    if (variableData == NULL)
    {
        print_error(assignElement->location, "Couldn't evaluate variable correctly.");
        return;
    }

    if (!canAssign(variableData))
    {
        print_error(assignElement->location, "You are trying to assign a value to a non mutable variable.");
        variableData->free(variableData);
        return;
    }

    ValueElement *value = assignElement->right;
    ValueData *valueData = value->eval(value, context);
    if (valueData == NULL)
    {
        print_error(assignElement->location, "Couldn't evaluate value correctly.");
        variableData->free(variableData);
        return;
    }

    if (!compare_type(valueData->value_type, variableData->variable_type))
    {
        char *value_type_str = print_type(valueData->value_type);
        char *symbol_type_str = print_type(variableData->variable_type);

        print_error(assignElement->location, "Couldn't assign value of type '%s' to variable of type '%s'", value_type_str, symbol_type_str);

        free(value_type_str);
        free(symbol_type_str);
        valueData->free(valueData);
        variableData->free(variableData);
        return;
    }

    valueData->free(valueData);
    variableData->free(variableData);
}