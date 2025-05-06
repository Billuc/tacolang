#ifndef TYPES_DATA_H__
#define TYPES_DATA_H__

#include <stdbool.h>

#define MAX_NB_OF_ARGS 10   // Arbitrarily defined
#define MAX_NB_OF_FIELDS 20 // Arbitrarily defined

typedef enum
{
    U8,      // char
    U16,     // ushort
    U32,     // uint
    U64,     // ulong
    I8,      // ?
    I16,     // short
    I32,     // int
    I64,     // long
    F32,     // float
    F64,     // double
    boolean, // _Bool
    none,    // void
} BaseType;

typedef union
{
    BaseType baseType;
    char *customType;
} Type;

typedef struct
{
    bool is_base_type;
    Type type_data;
    bool mutable;
    bool reference;
    bool array;
    int array_len;
    bool optional;
} TypeData;

typedef struct
{
    TypeData returnType;
    TypeData argumentTypes[MAX_NB_OF_ARGS];
} FunctionTypeData;

typedef struct
{
    TypeData fieldTypes[MAX_NB_OF_FIELDS];
} StructTypeData;

#endif