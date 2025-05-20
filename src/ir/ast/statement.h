#if !defined(STATEMENT_H__)
#define STATEMENT_H__

#include "./assign.h"
#include "./declare.h"
#include "./value.h"
#include "ir/eval/context.h"
#include "utils/list_utils.h"
#include "utils/error_utils.h"

typedef enum
{
    s_assignment,
    s_declaration,
    s_value,
} StatementType;

typedef union
{
    AssignElement *assign;
    DeclareElement *declare;
    ValueElement *value;
} Statement;

typedef struct statementElement
{
    StatementType type;
    Statement statement;
    location_t location;

    void (*free)(struct statementElement *);
    void (*eval)(struct statementElement *, EvalContext *);
} StatementElement;

typedef LinkedList StatementList;

StatementElement *newAssignmentStatement(AssignElement *assign, location_t location);
StatementElement *newDeclareStatement(DeclareElement *declare, location_t location);
StatementElement *newValueStatement(ValueElement *value, location_t location);
StatementList *newStatementList();

#endif // STATEMENT_H__
