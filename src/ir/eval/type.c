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

int compare_simpleType(SimpleType data1, SimpleType data2)
{
    int modifier_compare = compare(data1.modifiers, data2.modifiers, compare_typeModifier);

    if (modifier_compare != 0)
        return modifier_compare;

    if (data1.is_base_type != data2.is_base_type)
        return (int)data1.is_base_type - (int)data2.is_base_type;

    if (data1.is_base_type)
        return data1.type_data.base_type - data2.type_data.base_type;
    else
        return strcmp(data1.type_data.custom_type, data2.type_data.custom_type);
}

int compare_functionType(FunctionType function1, FunctionType function2)
{
    if (function1.number_of_args != function2.number_of_args)
        return function1.number_of_args - function2.number_of_args;

    int i;
    for (i = 0; i < function1.number_of_args; i++)
    {
        int type_compare = compare_simpleType(function1.argument_types[i], function2.argument_types[i]);

        if (type_compare != 0)
            return type_compare;
    }

    return compare_simpleType(function1.return_type, function2.return_type);
}

int compare_structType(StructType struct1, StructType struct2)
{
    if (struct1.number_of_fields != struct2.number_of_fields)
        return struct1.number_of_fields - struct2.number_of_fields;

    int name_compare = strcmp(struct1.struct_name, struct2.struct_name);
    if (name_compare != 0)
        return name_compare;

    int i;
    for (i = 0; i < struct1.number_of_fields; i++)
    {
        int type_compare = compare_simpleType(struct1.fields[i].field_type, struct2.fields[i].field_type);
        if (type_compare != 0)
            return type_compare;

        int field_name_compare = strcmp(struct1.fields[i].field_name, struct2.fields[i].field_name);
        if (field_name_compare != 0)
            return field_name_compare;
    }

    return 0;
}

int compare_type(Type *type1, Type *type2)
{
    if (type1->type_type != type2->type_type)
        return type1->type_type - type2->type_type;

    switch (type1->type_type)
    {
    case t_variable:
        return compare_simpleType(type1->type_data.variable_type, type2->type_data.variable_type);
    case t_function:
        return compare_functionType(type1->type_data.function_type, type2->type_data.function_type);
    case t_structure:
        return compare_structType(type1->type_data.struct_type, type2->type_data.struct_type);
    default:
        return 1;
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
    default:
        return strdup("Unknown");
    }
}

static char *print_simpleType(SimpleType type)
{
    char buf[50] = "";

    LinkedListElement *iter = type.modifiers->head;
    for (iter = type.modifiers->head; iter != NULL; iter = iter->next)
    {
        TypeModifier *modifier = (TypeModifier *)iter->data;
        char *modifier_str = print_typeModifier(*modifier);
        strcat(buf, modifier_str);
        strcat(buf, ".");
        free(modifier_str);
    }

    if (type.is_base_type)
    {
        char *base_type_str = print_baseType(type.type_data.base_type);
        strcat(buf, base_type_str);
        free(base_type_str);
    }
    else
    {
        strcat(buf, type.type_data.custom_type);
    }

    return strdup(buf);
}

static char *print_functionType(FunctionType functionType)
{
    char buf[500] = "";
    strcat(buf, "(");

    int i;
    for (i = 0; i < functionType.number_of_args; i++)
    {
        if (i != 0)
            strcat(buf, ", ");

        char *arg_type_str = print_simpleType(functionType.argument_types[i]);
        strcat(buf, arg_type_str);
        free(arg_type_str);
    }

    strcat(buf, ") -> ");

    char *return_type_str = print_simpleType(functionType.return_type);
    strcat(buf, return_type_str);
    free(return_type_str);

    return strndup(buf, 50);
}

static char *print_structType(StructType structType)
{
    char buf[50] = "";
    strcat(buf, "struct ");
    strcat(buf, structType.struct_name);

    return strdup(buf);
}

char *print_type(Type *type)
{
    switch (type->type_type)
    {
    case t_variable:
        return print_type_data(type->type_data.variable_type);
    case t_function:
        return print_function_type(type->type_data.function_type);
    case t_structure:
        return print_struct_type(type->type_data.struct_type);
    default:
        return strdup("Unknown");
    }
}

void free_simpleType(SimpleType *simpleType)
{
    if (simpleType == NULL)
        return;

    if (!simpleType->is_base_type)
        free(simpleType->type_data.custom_type);

    freeLinkedList(simpleType->modifiers);
}

void free_type(Type *type)
{
    if (type == NULL)
        return;

    if (type->type_type == t_variable)
    {
        free_simpleType(&type->type_data.variable_type);
    }
    else if (type->type_type == t_function)
    {
        int i;
        for (i = 0; i < type->type_data.function_type.number_of_args; i++)
        {
            free_simpleType(&type->type_data.function_type.argument_types[i]);
        }

        free_simpleType(&type->type_data.function_type.return_type);
    }
    else if (type->type_type == t_structure)
    {
        free(type->type_data.struct_type.struct_name);
        int i;
        for (i = 0; i < type->type_data.struct_type.number_of_fields; i++)
        {
            free(type->type_data.struct_type.fields[i].field_name);
            free_simpleType(&type->type_data.struct_type.fields[i].field_type);
        }
    }
    free(type);
}