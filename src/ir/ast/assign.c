#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assign.h"

extern void yyerror(char *s);
static void freeAssign(AssignElement *assignElement);
static void evalAssign(AssignElement *assignElement, SymbolElement **symbolTable);

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

static void evalAssign(AssignElement *assignElement, SymbolElement **symbolTable)
{
    if (assignElement == NULL)
        return;

    EvalVariableData *variableData = assignElement->left->eval(assignElement->left, symbolTable);
    if (variableData == NULL)
        return;

    TypeModifierLink *firstModifier = variableData->variableType->data.variableTypeData.modifiers;
    if (!variableData->isDeclaration && (!firstModifier || firstModifier->element.modifier_type != tm_mutable))
    {
        char buf[100] = "";
        snprintf(buf, 100, "You are trying to assign a value to a non mutable variable.");
        yyerror(buf);
        return;
    }

    EvalValueData *valueData = assignElement->right->eval(assignElement->right, symbolTable);
    if (valueData == NULL)
    {
        // free(variableData);
        return;
    }

    if (!eval_type_equals(valueData->valueType, variableData->variableType))
    {
        char buf[200] = "";
        char *value_type_str = print_type(valueData->valueType);
        char *symbol_type_str = print_type(variableData->variableType);

        snprintf(buf, 200, "Couldn't assign value of type '%s' to variable of type '%s'", value_type_str, symbol_type_str);
        yyerror(buf);

        free(value_type_str);
        free(symbol_type_str);
        return;
    }
}