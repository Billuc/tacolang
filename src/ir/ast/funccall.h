#if !defined(FUNCCALL_H__)
#define FUNCCALL_H__

#include "utils/list_utils.h"
#include "ir/eval/context.h"
#include "ir/eval/type.h"
#include "utils/error_utils.h"

// The FunctionArgumentList is a linked list of ValueElements
typedef LinkedList FunctionArgumentList;

typedef struct functionCall
{
    Type *returnType;

    void (*free)(struct functionCall *functionCallData);
} FunctionCall;

typedef struct functionCallElement
{
    char *functionName;
    FunctionArgumentList *arguments;
    location_t location;

    void (*free)(struct functionCallElement *functionCall);
    struct FunctionCall *(*eval)(struct functionCallElement *functionCall, EvalContext *context);
} FunctionCallElement;

FunctionCallElement *newFunctionCall(char *functionName, FunctionArgumentList *arguments, location_t loc);
FunctionArgumentList *newFunctionArgumentList();

#endif // FUNCCALL_H__
