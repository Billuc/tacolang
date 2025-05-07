#include <stdlib.h>
#include "statement.h"

StatementElement *newDeclarationStatement(DeclareElement *declare)
{
    StatementElement *element = malloc(sizeof(StatementElement));
    StatementData data = {.declare = declare};
    element->data = data;
    element->type = s_declaration;
    return element;
}

StatementElement *newAssignmentStatement(AssignElement *assign)
{
    StatementElement *element = malloc(sizeof(StatementElement));
    StatementData data = {.assign = assign};
    element->data = data;
    element->type = s_assignment;
    return element;
}

void freeStatement(StatementElement *element)
{
    switch (element->type)
    {
    case s_assignment:
        freeAssign(element->data.assign);
        break;
    case s_declaration:
        freeDeclare(element->data.declare);
        break;
    }

    free(element);
}

void evalStatement(StatementElement *element, SymbolElement **symbolTable)
{
    // TODO
}

StatementLink *addStatement(StatementLink *list, StatementElement *newElement)
{
    StatementLink *newHead = malloc(sizeof(StatementLink));
    newHead->element = newElement;
    newHead->next = list;
    return newHead;
}

void freeStatementList(StatementLink *list)
{
    StatementLink *iter = list;

    while (iter != NULL)
    {
        freeStatement(iter->element);
        StatementLink *next = iter->next;
        free(iter);
        iter = next;
    }
}

void evalStatementList(StatementLink *list, SymbolElement **symbolTable)
{
    // TODO
}