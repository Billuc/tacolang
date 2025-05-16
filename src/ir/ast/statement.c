#include <stdlib.h>
#include "statement.h"

static void freeStatement(StatementElement *element);
static void evalStatement(StatementElement *element, EvalContext *context);

StatementElement *newAssignmentStatement(AssignElement *assign)
{
    StatementElement *element = malloc(sizeof(StatementElement));
    Statement data = {.assign = assign};
    element->statement = data;
    element->type = s_assignment;
    element->free = freeStatement;
    element->eval = evalStatement;
    return element;
}

StatementElement *newDeclareStatement(DeclareElement *declare)
{
    StatementElement *element = malloc(sizeof(StatementElement));
    Statement data = {.declare = declare};
    element->statement = data;
    element->type = s_declaration;
    element->free = freeStatement;
    element->eval = evalStatement;
    return element;
}

static void freeStatement(StatementElement *element)
{
    switch (element->type)
    {
    case s_assignment:
        element->statement.assign->free(element->statement.assign);
        break;
    case s_declaration:
        element->statement.declare->free(element->statement.declare);
        break;
    }

    free(element);
}

static void evalStatement(StatementElement *element, EvalContext *context)
{
    switch (element->type)
    {
    case s_assignment:
        element->statement.assign->eval(element->statement.assign, context);
        break;
    case s_declaration:
        element->statement.declare->eval(element->statement.declare, context);
        break;
    }
}

StatementList *newStatementList()
{
    return newLinkedList(freeStatement);
}
