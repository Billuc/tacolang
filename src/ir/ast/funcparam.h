#if !defined(FUNCPARAM_H__)
#define FUNCPARAM_H__

#include "ir/ast/typedef.h"
#include "ir/eval/type.h"
#include "ir/eval/context.h"
#include "utils/list_utils.h"
#include "utils/error_utils.h"

// A structure to represent the evaluation of a function parameter
// It should contain the parameter's name and its evaluated type
typedef struct functionParameterData
{
    char *name; // Name of the parameter
    Type *type; // Evaluated type of the parameter

    void (*free)(struct functionParameterData *); // Function to free the structure
} FunctionParameterData;

// A structure to represent a function parameter
typedef struct functionParameterElement
{
    char *name;           // Name of the parameter
    TypedefElement *type; // Type of the parameter
    location_t location;  // Location of the parameter in the source code

    void (*free)(struct functionParameterElement *);                                  // Function to free the structure
    FunctionParameterData *(*eval)(struct functionParameterElement *, EvalContext *); // Function to evaluate the function parameter
} FunctionParameterElement;

// A function to create a new function parameter element
FunctionParameterElement *newFunctionParameter(char *name, TypedefElement *type, location_t location);

typedef LinkedList FunctionParameterList; // Alias for LinkedList to represent a list of function parameters

// A function to create a new function parameter list
FunctionParameterList *newFunctionParameterList(void);

#endif // FUNCPARAM_H__
