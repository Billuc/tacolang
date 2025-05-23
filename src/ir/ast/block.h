#if !defined(BLOCK_H__)
#define BLOCK_H__

#include "ir/ast/statement.h"
#include "ir/eval/context.h"

typedef struct blockData {
  char *generated_code; // Generated code for the block

  void (*free)(struct blockData *);
} BlockData;

typedef struct blockElement {
  StatementList *statements;
  location_t location;

  void (*free)(struct blockElement *);
  BlockData *(*eval)(struct blockElement *, EvalContext *);
} BlockElement;

BlockElement *newBlock(StatementList *statements, location_t loc);

#endif // BLOCK_H__
