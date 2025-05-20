#if !defined(PROGRAM_H__)
#define PROGRAM_H__

#include "ir/ast/statement.h"
#include "ir/eval/context.h"
#include "utils/error_utils.h"

typedef struct programElement
{
    StatementList *statements;
    location_t location;

    void (*free)(struct programElement *);
    void (*eval)(struct programElement *, EvalContext *);
} ProgramElement;

ProgramElement *newProgram(StatementList *statements, location_t location);
void eval(ProgramElement *program);

#endif // PROGRAM_H__
