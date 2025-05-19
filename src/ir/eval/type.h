#if !defined(TYPE_H__)
#define TYPE_H__

#include <stdbool.h>
#include "type_modifier.h"

#define MAX_NB_OF_ARGS 10   // Arbitrarily defined
#define MAX_NB_OF_FIELDS 20 // Arbitrarily defined

typedef enum
{
    U8,      // unsigned char
    U16,     // unsigned short
    U32,     // unsigned long
    U64,     // unsigned long long
    I8,      // signed char
    I16,     // signed short
    I32,     // signed long
    I64,     // signed long long
    F32,     // float
    F64,     // double
    Boolean, // _Bool
    None,    // void
} BaseType;

struct baseTypeData
{
    BaseType baseType;
    char *name;
};

extern const struct baseTypeData BASE_TYPES[]; // Defined in type.c
extern const int BASE_TYPES_LEN;               // Defined in type.c

typedef union
{
    BaseType base_type;
    char *custom_type;
} SimpleTypeData;

typedef struct
{
    bool is_base_type;
    SimpleTypeData type_data;
    TypeModifierList *modifiers;
} SimpleType;

typedef struct
{
    SimpleType return_type;
    short number_of_args;
    SimpleType argument_types[MAX_NB_OF_ARGS];
} FunctionType;

typedef struct
{
    SimpleType field_type;
    char *field_name;
} StructField;

typedef struct
{
    char *struct_name;
    short number_of_fields;
    StructField fields[MAX_NB_OF_FIELDS];
} StructType;

typedef enum
{
    t_variable,
    t_function,
    t_structure,
} TypeType;

typedef union
{
    SimpleType variable_type;
    FunctionType function_type;
    StructType struct_type;
} TypeData;

typedef struct type
{
    TypeType type_type;
    TypeData type_data;
} Type;

int compare_simpleType(SimpleType data1, SimpleType data2);
int compare_functionType(FunctionType function1, FunctionType function2);
int compare_structType(StructType struct1, StructType struct2);
int compare_type(Type *type1, Type *type2);

char *print_type(Type *type);
char *print_simpleType(SimpleType type);
void free_simpleType(SimpleType *simpleType);
void free_type(Type *type);
Type *copy_type(Type *toCopy);

#endif // TYPE_H__
