#ifndef SYMBOL_TABLE_H__
#define SYMBOL_TABLE_H__

#include "ir/eval/type.h"
#include "utils/list_utils.h"

typedef struct symbolData
{
    char *name;
    Type *type;
} SymbolData;

typedef LinkedList SymbolTable;

SymbolTable *create_SymbolTable();
int compare_SymbolData(void *a, void *b);

#endif