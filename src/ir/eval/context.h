#if !defined(CONTEXT_H__)
#define CONTEXT_H__

#include "utils/list_utils.h"
#include "ir/eval/symbol_table.h"

typedef struct evalContext
{
    LinkedList *symbolTableStack;
    // Generated code

    void (*free)(struct evalContext *);
} EvalContext;

EvalContext *newEvalContext();
SymbolData *findSymbol(EvalContext *context, char *name);
void putSymbol(EvalContext *context, SymbolData *symbol);

#endif // CONTEXT_H__
