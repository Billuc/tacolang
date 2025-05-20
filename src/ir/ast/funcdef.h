#if !defined(FUNCDEF_H__)
#define FUNCDEF_H__

#include "ir/ast/typedef.h"
#include "ir/ast/block.h"
#include "utils/list_utils.h"
#include "utils/error_utils.h"
#include "ir/ast/funcparam.h"
#include "ir/eval/type.h"
#include "ir/eval/context.h"

typedef struct functionDefinitionData
{
    Type *functionType;

    void (*free)(struct functionDefinitionData *);
} FunctionDefinitionData;

// Define a function definition structure
// This structure contains the function name, parameters, return type and a block element
typedef struct functionDefinition
{
    char *name;                        // Function name
    FunctionParameterList *parameters; // List of parameters (alias: ParameterList)
    TypedefElement *return_type;       // Return type
    BlockElement *block;               // Block element containing the function body
    location_t location;               // Location of the function definition in the source code

    void (*free)(struct functionDefinition *);                                   // Function to free the structure
    FunctionDefinitionData *(*eval)(struct functionDefinition *, EvalContext *); // Function to evaluate the function definition
} FunctionDefinitionElement;

// Function to create a new function definition element
FunctionDefinitionElement *newFunctionDefinition(char *name, FunctionParameterList *parameters, TypedefElement *return_type, BlockElement *block, location_t location);

#endif // FUNCDEF_H__
