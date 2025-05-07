#include "./symbol_table.h"
#include <stdlib.h>
#include <string.h>

// TODO : improve symbol table by using a hash table instead of a linked list

SymbolElement *createSymbolTable()
{
    return NULL;
}

SymbolElement *putSymbol(SymbolElement *table, SymbolData *data)
{
    SymbolElement *newElement = malloc(sizeof(SymbolElement));
    newElement->symbol = data;
    newElement->next = table;
    return newElement;
}

SymbolData *getSymbol(SymbolElement *table, char *name)
{
    SymbolElement *iterator;

    for (iterator = table; iterator != NULL; iterator = iterator->next)
    {
        if (!strcmp(iterator->symbol->name, name))
        {
            return iterator->symbol;
        }
    }

    return NULL;
}

void freeSymbolTable(SymbolElement *table)
{
    SymbolElement *iter = table;
    SymbolElement *next = NULL;

    while (iter != NULL)
    {
        free(iter->symbol->name);
        free(iter->symbol);
        next = iter->next;
        free(iter);
        iter = next;
    }
}

SymbolData *createSymbol(char *name, char *type)
{
}