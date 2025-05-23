#include "block.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

static void freeBlock(BlockElement *blockElement);
static BlockData *evalBlock(BlockElement *blockElement, EvalContext *context);

BlockElement *newBlock(StatementList *statements, location_t loc) {
  printf("Creating new block\n");

  BlockElement *element = malloc(sizeof(BlockElement));
  element->statements = statements;
  element->location = loc;
  element->free = freeBlock;
  element->eval = evalBlock;
  return element;
}

static void freeBlock(BlockElement *blockElement) {
  if (blockElement == NULL)
    return;

  freeLinkedList(blockElement->statements);
  free(blockElement);
}

static void freeBlockData(BlockData *data) {
  free(data->generated_code);
  free(data);
}

static BlockData *evalBlock(BlockElement *blockElement, EvalContext *context) {
  if (blockElement == NULL)
    return NULL;

  printf("Evaluating block\n");

  BlockData *data = malloc(sizeof(BlockData));
  data->free = freeBlockData;
  data->generated_code = strdup("{\n");

  LinkedListElement *current;
  for (current = blockElement->statements->head; current != NULL;
       current = current->next) {
    StatementElement *statement = (StatementElement *)current->data;
    StatementData *stmt_data = statement->eval(statement, context);
    strappend(&data->generated_code, stmt_data->generated_code);
    stmt_data->free(stmt_data);
  }

  strappend(&data->generated_code, "}\n");

  return data;
}
