#if !defined(STATEMENT_H__)
#define STATEMENT_H__

#include "./assign.h"
#include "./declare.h"
#include "ir/eval/context.h"
#include "utils/list_utils.h"

typedef enum
{
    s_assignment,
} StatementType;

typedef union
{
    AssignElement *assign;
} Statement;

typedef struct statementElement
{
    StatementType type;
    Statement statement;

    void (*free)(struct statementElement *);
    void (*eval)(struct statementElement *, EvalContext *);
} StatementElement;

typedef LinkedList StatementList;

StatementElement *newAssignmentStatement(AssignElement *assign);
StatementList *newStatementList();

#endif // STATEMENT_H__
