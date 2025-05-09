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
    BaseType baseType;
    char *customType;
} Type;

typedef struct
{
    bool is_base_type;
    Type type_data;
    TypeModifierLink *modifiers;
} TypeData;

typedef struct
{
    TypeData returnType;
    short number_of_args;
    TypeData argumentTypes[MAX_NB_OF_ARGS];
} FunctionTypeData;

typedef struct
{
    TypeData type;
    char *name;
} StructField;

typedef struct
{
    char *name;
    short number_of_fields;
    StructField fields[MAX_NB_OF_FIELDS];
} StructTypeData;

typedef enum
{
    t_variable,
    t_function,
    t_structure,
} TypeType;

typedef union
{
    TypeData variableTypeData;
    FunctionTypeData functionTypeData;
    StructTypeData structTypeData;
} EvalTypeData;

typedef struct
{
    TypeType type;
    EvalTypeData data;
} EvalType;

bool type_data_equals(TypeData typeData1, TypeData typeData2);
bool function_type_data_equals(FunctionTypeData typeData1, FunctionTypeData typeData2);
bool struct_type_data_equals(StructTypeData typeData1, StructTypeData typeData2);
bool eval_type_equals(EvalType *type1, EvalType *type2);

char *print_type(EvalType *type);

#endif // TYPE_H__
