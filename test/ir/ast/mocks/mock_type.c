#include "mock_type.h"

Type *mock_intType(void)
{
    Type *type = malloc(sizeof(Type));
    type->type_type = t_variable;
    type->type_data.variable_type.is_base_type = true;
    type->type_data.variable_type.type_data.base_type = U16;
    return type;
}

Type *mock_floatType(void)
{
    Type *type = malloc(sizeof(Type));
    type->type_type = t_variable;
    type->type_data.variable_type.is_base_type = true;
    type->type_data.variable_type.type_data.base_type = F32;
    return type;
}