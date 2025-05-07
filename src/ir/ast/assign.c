#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assign.h"

AssignElement *newAssign(char *name, ValueElement *value)
{
    AssignElement *element = malloc(sizeof(AssignElement));
    element->name = strdup(name);
    element->value = value;
    return element;
}

void freeAssign(AssignElement *assignElement)
{
    free(assignElement->name);
    freeValue(assignElement->value);
    free(assignElement);
}

void evalAssign(AssignElement *assignElement, SymbolElement **symbolTable)
{
    char *variableName = assignElement->name;
    SymbolData *symbol = getSymbol(*symbolTable, variableName);

    if (symbol == NULL)
    {
        char buf[100] = "";
        snprintf(buf, 100, "Variable '%s' has not been declared in this context", variableName);
        yyerror(buf);
        return;
    }

    EvalValueData *valueData = evalValue(assignElement->value, symbolTable);

    if (symbol->type->type != t_variable)
    {
        char buf[100] = "";
        snprintf(buf, 100, "'%s' is not a variable ! You can't assign it a value.", variableName);
        yyerror(buf);
        return;
    }

    if (symbol->type->data.variableTypeData.modifiers->element->modifier_type != tm_mutable)
    {
        char buf[100] = "";
        snprintf(buf, 100, "'%s' is not mutable ! You can't assign it a value.", variableName);
        yyerror(buf);
        return;
    }

    if (!eval_type_equals(valueData->valueType, symbol->type))
    {
        char buf[200] = "";
        char *value_type_str = print_type(valueData->valueType);
        char *symbol_type_str = print_type(symbol->type);

        snprintf(buf, 200, "Couldn't assign value of type '%s' to variable '%s' of type '%s'", value_type_str, variableName, symbol_type_str);
        yyerror(buf);

        free(value_type_str);
        free(symbol_type_str);
        return;
    }
}