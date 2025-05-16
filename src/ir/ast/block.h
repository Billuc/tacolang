#if !defined(BLOCK_H__)
#define BLOCK_H__

#include "ir/ast/statement.h"
#include "ir/eval/context.h"

typedef struct blockElement
{
    StatementList *statements;

    void (*free)(struct blockElement *);
    void (*eval)(struct blockElement *, EvalContext *);
} BlockElement;

BlockElement *newBlock(StatementList *statements);

#endif // BLOCK_H__
