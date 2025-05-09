#ifndef SYMBOL_TABLE_H__
#define SYMBOL_TABLE_H__

#include "../eval/type.h"

typedef struct symbolData
{
    char *name;
    EvalType *type;
} SymbolData;

typedef struct symbolElement
{
    SymbolData *symbol;
    struct symbolElement *next;
} SymbolElement;

SymbolElement *createSymbolTable();
SymbolElement *putSymbol(SymbolElement *table, SymbolData *data);
SymbolData *getSymbol(SymbolElement *table, char *name);
void freeSymbolTable(SymbolElement *table);

#endif