#include <stdlib.h>
#include "program.h"

static void freeProgram(ProgramElement *element);
static void evalProgram(ProgramElement *element, EvalContext *context);

ProgramElement *newProgram(StatementList *statements)
{
    ProgramElement *element = malloc(sizeof(ProgramElement));
    element->statements = statements;
    element->free = freeProgram;
    element->eval = evalProgram;
    return element;
}

static void freeProgram(ProgramElement *element)
{
    freeLinkedList(element->statements);
    free(element);
}

static void evalProgram(ProgramElement *element, EvalContext *context)
{
    LinkedListElement *current;

    for (current = element->statements->head; current != NULL; current = current->next)
    {
        StatementElement *statement = (StatementElement *)current->data;
        statement->eval(statement, context);
    }
}

void eval(ProgramElement *program)
{
    EvalContext *context = newEvalContext();
    program->eval(program, context);
    context->free(context);
    program->free(program);
}