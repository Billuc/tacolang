#include "block.h"
#include <stdlib.h>

extern void yyerror(char *s);
static void freeBlock(BlockElement *blockElement);
static void evalBlock(BlockElement *blockElement, EvalContext *context);

BlockElement *newBlock(StatementList *statements)
{
    BlockElement *element = malloc(sizeof(BlockElement));
    element->statements = statements;
    element->free = freeBlock;
    element->eval = evalBlock;
    return element;
}

static void freeBlock(BlockElement *blockElement)
{
    if (blockElement == NULL)
        return;

    freeLinkedList(blockElement->statements);
    free(blockElement);
}

static void evalBlock(BlockElement *blockElement, EvalContext *context)
{
    if (blockElement == NULL)
        return;

    LinkedListElement *current = blockElement->statements->head;
    for (current = blockElement->statements->head; current != NULL; current = current->next)
    {
        StatementElement *statement = (StatementElement *)current->data;
        statement->eval(statement, context);
    }
}