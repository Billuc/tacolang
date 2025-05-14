#if !defined(PROGRAM_H__)
#define PROGRAM_H__

#include "ir/ast/statement.h"
#include "ir/eval/context.h"

typedef struct programElement
{
    StatementList *statements;

    void (*free)(struct programElement *);
    void (*eval)(struct programElement *, EvalContext *);
} ProgramElement;

ProgramElement *newProgram(StatementList *statements);
void eval(ProgramElement *program);

#endif // PROGRAM_H__
