#if !defined(STATEMENT_H__)
#define STATEMENT_H__

#include "assign.h"
#include "declare.h"
#include "../symbol_table/symbol_table.h"

typedef enum
{
    s_declaration,
    s_assignment,
} StatementType;

typedef union
{
    AssignElement *assign;
    DeclareElement *declare;
} StatementData;

typedef struct
{
    StatementType type;
    StatementData data;
} StatementElement;

typedef struct statementLink
{
    StatementElement *element;
    struct statementLink *next;
} StatementLink;

StatementElement *newDeclarationStatement(DeclareElement *declare);
StatementElement *newAssignmentStatement(AssignElement *assign);
void freeStatement(StatementElement *statementEl);
void evalStatement(StatementElement *statementElement, SymbolElement **symbolTable);

StatementLink *addStatement(StatementLink *list, StatementElement *newElement);
void freeStatementList(StatementLink *list);
void evalStatementList(StatementLink *list, SymbolElement **symbolTable);

#endif // STATEMENT_H__
