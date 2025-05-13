#if !defined(CONTEXT_H__)
#define CONTEXT_H__

#include "utils/list_utils.h"
#include "ir/eval/symbol_table.h"

typedef struct evalContext
{
    LinkedList *symbolTableStack;

} EvalContext;

SymbolData *findSymbol(EvalContext *context, char *name);

#endif // CONTEXT_H__
