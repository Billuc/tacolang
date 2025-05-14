#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assign.h"

extern void yyerror(char *s);
static void freeAssign(AssignElement *assignElement);
static void evalAssign(AssignElement *assignElement, EvalContext *context);

AssignElement *newAssign(VariableElement *left, ValueElement *right)
{
    AssignElement *element = malloc(sizeof(AssignElement));
    element->left = left;
    element->right = right;
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
    if (variable->is_declaration)
        return true;

    TypeModifierList *modifiers = variable->variable_type->type_data.variable_type.modifiers;
    if (modifiers->size == 0)
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

    if (!canAssign(variableData))
    {
        char buf[100] = "";
        snprintf(buf, 100, "You are trying to assign a value to a non mutable variable.");
        yyerror(buf);
        variableData->free(variableData);
        return;
    }

    ValueElement *value = assignElement->right;
    ValueData *valueData = value->eval(value, context);
    if (valueData == NULL)
    {
        char buf[100] = "";
        snprintf(buf, 100, "Incorrect value.");
        yyerror(buf);
        variableData->free(variableData);
        return;
    }

    if (!compare_type(valueData->value_type, variableData->variable_type))
    {
        char buf[200] = "";
        char *value_type_str = print_type(valueData->value_type);
        char *symbol_type_str = print_type(variableData->variable_type);

        snprintf(buf, 200, "Couldn't assign value of type '%s' to variable of type '%s'", value_type_str, symbol_type_str);
        yyerror(buf);

        free(value_type_str);
        free(symbol_type_str);
        valueData->free(valueData);
        variableData->free(variableData);
        return;
    }

    valueData->free(valueData);
    variableData->free(variableData);
}