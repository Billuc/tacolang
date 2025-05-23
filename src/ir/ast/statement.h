#if !defined(STATEMENT_H__)
#define STATEMENT_H__

#include "./assign.h"
#include "./declare.h"
#include "./value.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"
#include "utils/list_utils.h"

typedef enum {
  s_assignment,
  s_declaration,
  s_value,
} StatementType;

typedef union {
  AssignElement *assign;
  DeclareElement *declare;
  ValueElement *value;
} Statement;

typedef struct statementData {
  char *generated_code; // Generated code for the statement

  void (*free)(struct statementData *);
} StatementData;

typedef struct statementElement {
  StatementType type;
  Statement statement;
  location_t location;

  void (*free)(struct statementElement *);
  StatementData *(*eval)(struct statementElement *, EvalContext *);
} StatementElement;

typedef LinkedList StatementList;

StatementElement *newAssignmentStatement(AssignElement *assign,
                                         location_t location);
StatementElement *newDeclareStatement(DeclareElement *declare,
                                      location_t location);
StatementElement *newValueStatement(ValueElement *value, location_t location);
StatementList *newStatementList();

#endif // STATEMENT_H__
