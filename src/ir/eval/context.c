#include "context.h"
#include <stdlib.h>

static void freeEvalContext(EvalContext *context);

EvalContext *newEvalContext()
{
    EvalContext *context = malloc(sizeof(EvalContext));
    context->free = freeEvalContext;
    context->symbolTableStack = newLinkedList(freeLinkedList);
    SymbolTable *globalSymbolTable = create_SymbolTable();
    push(context->symbolTableStack, globalSymbolTable);
    return context;
}

static void freeEvalContext(EvalContext *context)
{
    freeLinkedList(context->symbolTableStack);
    free(context);
}

SymbolData *findSymbol(EvalContext *context, char *name)
{
    LinkedListElement *current;
    SymbolData to_find = {.name = name};

    for (current = context->symbolTableStack->head; current != NULL; current = current->next)
    {
        SymbolTable *symbolTable = (SymbolTable *)current->data;
        SymbolData *data = (SymbolData *)find(symbolTable, &to_find, compare_SymbolData);

        if (data != NULL)
            return data;
    }

    return NULL;
}

void putSymbol(EvalContext *context, SymbolData *symbol)
{
    SymbolTable *symbolTable = (SymbolTable *)context->symbolTableStack->head->data;
    push(symbolTable, symbol);
}