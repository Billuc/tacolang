#include "./mock_variable.h"
#include "mock_type.h"

int calls_to_freeVariable = 0;
void mock_freeVariable(VariableElement *variable)
{
    calls_to_freeVariable++;
    free(variable);
}

int calls_to_freeVariableData = 0;
void mock_freeVariableData(VariableData *data)
{
    calls_to_freeVariableData++;
    free_type(data->variable_type);
    free(data);
}

int calls_to_evalVariable = 0;
VariableData *mock_evalDeclareVariable(VariableElement *variable, EvalContext *context)
{
    calls_to_evalVariable++;
    VariableData *data = malloc(sizeof(VariableData));
    data->free = mock_freeVariableData;
    data->is_declaration = true;
    data->variable_type = mock_intType();
    return data;
}
VariableData *mock_evalIdentiferVariable(VariableElement *variable, EvalContext *context)
{
    calls_to_evalVariable++;
    VariableData *data = malloc(sizeof(VariableData));
    data->free = mock_freeVariableData;
    data->is_declaration = false;
    data->variable_type = mock_floatType();
    return data;
}

VariableElement *mock_newVariable(MockVariableType variant)
{
    VariableElement *variable = malloc(sizeof(VariableElement));
    variable->free = mock_freeVariable;
    switch (variant)
    {
    case mock_DeclareVariable:
        variable->eval = mock_evalDeclareVariable;
        break;
    case mock_IdentifierVariable:
        variable->eval = mock_evalIdentiferVariable;
        break;
    }

    return variable;
}