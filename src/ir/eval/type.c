#include <string.h>
#include <stdlib.h>
#include "type.h"
#include "../../utils/str_utils.h"

const struct baseTypeData BASE_TYPES[] = {
    {U8, "U8"},
    {U16, "U16"},
    {U32, "U32"},
    {U64, "U64"},
    {I8, "I8"},
    {I16, "I16"},
    {I32, "I32"},
    {I64, "I64"},
    {F32, "F32"},
    {F64, "F64"},
    {Boolean, "Boolean"},
    {None, "None"},
};
const int BASE_TYPES_LEN = sizeof(BASE_TYPES) / sizeof(BASE_TYPES[0]);

bool type_data_equals(TypeData typeData1, TypeData typeData2)
{
    TypeModifierLink *iter1 = typeData1.modifiers;
    TypeModifierLink *iter2 = typeData2.modifiers;

    while (iter1 != NULL && iter2 != NULL)
    {
        if (!type_modifier_equals(iter1->element, iter2->element))
            return false;
    }
    if (iter1 != NULL || iter2 != NULL)
        return false;

    if (typeData1.is_base_type != typeData2.is_base_type)
        return false;

    if (typeData1.is_base_type)
        return typeData1.type_data.baseType == typeData2.type_data.baseType;
    else
        return strcmp(typeData1.type_data.customType, typeData2.type_data.customType) == 0;
}

bool function_type_data_equals(FunctionTypeData typeData1, FunctionTypeData typeData2)
{
    if (typeData1.number_of_args != typeData2.number_of_args)
        return false;

    int i;
    for (i = 0; i < typeData1.number_of_args; i++)
    {
        if (!type_data_equals(typeData1.argumentTypes[i], typeData2.argumentTypes[i]))
            return false;
    }

    return type_data_equals(typeData1.returnType, typeData2.returnType);
}

bool struct_type_data_equals(StructTypeData typeData1, StructTypeData typeData2)
{
    if (typeData1.number_of_fields != typeData2.number_of_fields)
        return false;
    if (strcmp(typeData1.name, typeData2.name) != 0)
        return false;

    int i;
    for (i = 0; i < typeData1.number_of_fields; i++)
    {
        if (!type_data_equals(typeData1.fields[i].type, typeData2.fields[i].type) || strcmp(typeData1.fields[i].name, typeData2.fields[i].name) != 0)
            return false;
    }

    return true;
}

bool eval_type_equals(EvalType *type1, EvalType *type2)
{
    if (type1->type != type2->type)
        return false;

    switch (type1->type)
    {
    case t_variable:
        return type_data_equals(type1->data.variableTypeData, type2->data.variableTypeData);
    default:
        break;
    }
}

static char *print_base_type(BaseType baseType)
{
    switch (baseType)
    {
    case U8:
        return strdup("U8");
    case U16:
        return strdup("U16");
    case U32:
        return strdup("U32");
    case U64:
        return strdup("U64");
    case I8:
        return strdup("I8");
    case I16:
        return strdup("I16");
    case I32:
        return strdup("I32");
    case I64:
        return strdup("I64");
    case F32:
        return strdup("F32");
    case F64:
        return strdup("F64");
    case Boolean:
        return strdup("Boolean");
    case None:
        return strdup("None");
    }
}

static char *print_type_data(TypeData type)
{
    char buf[50] = "";

    TypeModifierLink *iter = type.modifiers;
    while (iter != NULL)
    {
        char *modifier_str = print_type_modifier(iter->element);
        strcat(buf, modifier_str);
        strcat(buf, ".");
        free(modifier_str);
        iter = iter->next;
    }

    if (type.is_base_type)
    {
        char *base_type_str = print_base_type(type.type_data.baseType);
        strcat(buf, base_type_str);
        free(base_type_str);
    }
    else
    {
        strcat(buf, type.type_data.customType);
    }

    return strdup(buf);
}

static char *print_function_type(FunctionTypeData functionType)
{
    char buf[500] = "";
    strcat(buf, "(");

    int i;
    for (i = 0; i < functionType.number_of_args; i++)
    {
        if (i != 0)
            strcat(buf, ", ");

        char *arg_type_str = print_type_data(functionType.argumentTypes[i]);
        strcat(buf, arg_type_str);
        free(arg_type_str);
    }

    strcat(buf, ") -> ");

    char *return_type_str = print_type_data(functionType.returnType);
    strcat(buf, return_type_str);
    free(return_type_str);

    return strndup(buf, 50);
}

static char *print_struct_type(StructTypeData structType)
{
    char buf[50] = "";
    strcat(buf, "struct ");
    strcat(buf, structType.name);

    return strdup(buf);
}

char *print_type(EvalType *type)
{
    switch (type->type)
    {
    case t_variable:
        return print_type_data(type->data.variableTypeData);
    case t_function:
        return print_function_type(type->data.functionTypeData);
    case t_structure:
        return print_struct_type(type->data.structTypeData);
    }
}