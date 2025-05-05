#include "./symbol_table.h"
#include <stdlib.h>
#include <string.h>

SymbolElement* createTable() {
    return NULL;
}

SymbolElement* put(SymbolElement* table, SymbolData* data) {
    SymbolElement* newElement = malloc(sizeof(SymbolElement));
    newElement->symbol = data;
    newElement->next = table;
    return newElement;
}

SymbolData* get(SymbolElement* table, char* name) {
    SymbolElement* iterator;
    
    for (iterator = table; iterator != NULL; iterator = iterator->next) {
        if (!strcmp(iterator->symbol->name, name)) {
            return iterator->symbol;
        }
    }

    return NULL;
}