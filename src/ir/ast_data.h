#ifndef AST_DATA_H__
#define AST_DATA_H__

typedef struct
{
} NoData;

typedef struct
{
    char *name;
} AssignData;

typedef struct
{
    char *name;
    char *type;
} DeclareData;

typedef struct
{
    char *operation;
} ExpressionData;

typedef struct
{
    int value;
} IntegerData;

typedef struct
{
    char *modifier;
} ModifierData;

typedef struct
{
    char *typename;
} TypeDefData;

#endif