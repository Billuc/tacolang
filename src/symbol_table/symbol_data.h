#ifndef SYMBOL_DATA_H__
#define SYMBOL_DATA_H__

#include "types_data.h"

typedef enum
{
    variable,
    function,
    structure,
} SymbolType;

typedef union
{
    TypeData variableTypeData;
    FunctionTypeData functionTypeData;
    StructTypeData structTypeData;
} SymbolTypeData;

typedef struct symbolData
{
    char *name;
    SymbolType symbolType;
    SymbolTypeData symbolTypeData;
} SymbolData;

#endif