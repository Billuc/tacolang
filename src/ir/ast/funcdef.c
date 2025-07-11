#include "ir/ast/funcdef.h"
#include "ir/eval/type.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

static void
freeFunctionDefinition(FunctionDefinitionElement *functionDefinition);
static FunctionDefinitionData *
evalFunctionDefinition(FunctionDefinitionElement *functionDefinition,
                       EvalContext *context);

FunctionDefinitionElement *
newFunctionDefinition(char *name, FunctionParameterList *parameters,
                      TypedefElement *return_type, BlockElement *block,
                      location_t location) {
  printf("Creating new function definition: %s\n", name);

  FunctionDefinitionElement *element =
      malloc(sizeof(FunctionDefinitionElement));
  element->name = name;
  if (parameters == NULL) {
    parameters = newFunctionParameterList();
  }
  element->parameters = parameters;
  element->return_type = return_type;
  element->block = block;
  element->location = location;
  element->free = freeFunctionDefinition;
  element->eval = evalFunctionDefinition;
  return element;
}

static void
freeFunctionDefinition(FunctionDefinitionElement *functionDefinition) {
  free(functionDefinition->name);
  freeLinkedList(functionDefinition->parameters);
  functionDefinition->return_type->free(functionDefinition->return_type);
  functionDefinition->block->free(functionDefinition->block);
  free(functionDefinition);
}

static void
freeFunctionDefinitionData(FunctionDefinitionData *functionDefinitionData) {
  if (functionDefinitionData == NULL)
    return;

  free(functionDefinitionData->generated_code);
  free_type(functionDefinitionData->functionType);
  free(functionDefinitionData);
}

static FunctionDefinitionData *
evalFunctionDefinition(FunctionDefinitionElement *functionDefinition,
                       EvalContext *context) {
  if (functionDefinition == NULL)
    return NULL;

  printf("Evaluating function definition: %s\n", functionDefinition->name);

  FunctionDefinitionData *functionData = malloc(sizeof(FunctionDefinitionData));
  functionData->free = freeFunctionDefinitionData;
  functionData->functionType = malloc(sizeof(Type));
  functionData->functionType->type_type = t_function;
  FunctionType *functionType =
      &functionData->functionType->type_data.function_type;
  functionType->number_of_args = 0;

  unshift(context->symbolTableStack, create_SymbolTable());

  char *function_params_code = strdup("");
  LinkedListElement *current;
  for (current = functionDefinition->parameters->head; current != NULL;
       current = current->next) {
    FunctionParameterElement *parameter =
        (FunctionParameterElement *)current->data;
    FunctionParameterData *parameterData = parameter->eval(parameter, context);

    functionType->number_of_args++;
    if (functionType->number_of_args > MAX_NB_OF_ARGS) {
      print_error(functionDefinition->location,
                  "Function '%s' has too many parameters",
                  functionDefinition->name);
      functionData->free(functionData);
      free(function_params_code);
      return NULL;
    }
    copy_simpleType(
        &parameterData->type->type_data.variable_type,
        &functionType->argument_types[functionType->number_of_args - 1]);

    SymbolData *symbol = malloc(sizeof(SymbolData));
    symbol->name = strdup(parameter->name);
    symbol->type = copy_type(parameterData->type);
    putSymbol(context, symbol);

    if (current != functionDefinition->parameters->head) {
      strappend(&function_params_code, ", ");
    }
    strappend(&function_params_code, parameterData->generated_code);

    parameterData->free(parameterData);
  }

  TypedefData *returnTypeData = functionDefinition->return_type->eval(
      functionDefinition->return_type, context);
  copy_simpleType(&returnTypeData->type->type_data.variable_type,
                  &functionType->return_type);

  BlockData *block_data =
      functionDefinition->block->eval(functionDefinition->block, context);

  SymbolTable *table = shift(context->symbolTableStack);
  freeLinkedList(table);

  SymbolData *fnSymbol = malloc(sizeof(SymbolData));
  fnSymbol->name = strdup(functionDefinition->name);
  fnSymbol->type = copy_type(functionData->functionType);
  putSymbol(context, fnSymbol);

  functionData->generated_code = strdup(returnTypeData->generated_code);
  strappend(&functionData->generated_code, " ");
  strappend(&functionData->generated_code, functionDefinition->name);
  strappend(&functionData->generated_code, "(");
  strappend(&functionData->generated_code, function_params_code);
  strappend(&functionData->generated_code, ") ");
  strappend(&functionData->generated_code, block_data->generated_code);

  returnTypeData->free(returnTypeData);
  block_data->free(block_data);

  return functionData;
}
