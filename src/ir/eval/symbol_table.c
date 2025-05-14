#include "./symbol_table.h"
#include <stdlib.h>
#include <string.h>

// TODO : improve symbol table by using a hash table instead of a linked list

static void free_SymbolData(SymbolData *data)
{
    free(data->name);
    free_type(data->type);
    free(data);
}

SymbolTable *create_SymbolTable()
{
    return newLinkedList(free_SymbolData);
}

int compare_SymbolData(void *a, void *b)
{
    SymbolData *data1 = (SymbolData *)a;
    SymbolData *data2 = (SymbolData *)b;

    return strcmp(data1->name, data2->name);
}