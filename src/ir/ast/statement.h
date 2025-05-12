#if !defined(STATEMENT_H__)
#define STATEMENT_H__

#include "assign.h"
#include "declare.h"
#include "common.h"
#include "../symbol_table/symbol_table.h"

typedef enum
{
    s_assignment,
} StatementType;

typedef union
{
    AssignElement *assign;
} StatementData;

typedef struct statementElement
{
    StatementType type;
    StatementData data;

    FREE_FUNC(struct statementElement, free);
    EVAL_FUNC(struct statementElement, void, eval);
} StatementElement;

typedef struct statementLink
{
    StatementElement *element;
    struct statementLink *next;
} StatementLink;

StatementElement *newAssignmentStatement(AssignElement *assign);
// void freeStatement(StatementElement *statementEl);
// void evalStatement(StatementElement *statementElement, SymbolElement **symbolTable);

StatementLink *addStatement(StatementLink *list, StatementElement *newElement);
void freeStatementList(StatementLink *list);
void evalStatementList(StatementLink *list, SymbolElement **symbolTable);

#endif // STATEMENT_H__
