#include <stdlib.h>
#include "statement.h"

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
    }

    free(element);
}

void evalStatement(StatementElement *element, SymbolElement **symbolTable)
{
    switch (element->type)
    {
    case s_assignment:
        evalAssign(element->data.assign, symbolTable);
        break;
    }
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
    SymbolElement *table;
    if (symbolTable == NULL)
    {
        table = createSymbolTable();
        symbolTable = &table;
    }

    // Reverse the list
    StatementLink *reversedList = NULL;
    StatementLink *iter = list;
    while (iter != NULL)
    {
        StatementLink *new = malloc(sizeof(StatementLink));
        new->element = iter->element;
        new->next = reversedList;
        reversedList = new;
        iter = iter->next;
    }

    // Iterate through the list and evaluate each statement
    iter = reversedList;
    while (iter != NULL)
    {
        evalStatement(iter->element, symbolTable);
        iter = iter->next;
    }

    // Free the symbol table
    if (symbolTable != NULL)
    {
        freeSymbolTable(*symbolTable);
        // free(symbolTable); // Segfault here
        symbolTable = NULL; // Set to NULL to avoid dangling pointer
    }
}