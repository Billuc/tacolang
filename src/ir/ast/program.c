#include "program.h"
#include "ir/ast/definition.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

static void freeProgram(ProgramElement *element);
static ProgramData *evalProgram(ProgramElement *element, EvalContext *context);

ProgramElement *newProgram(DefinitionList *definitions, location_t location) {
  printf("Creating new program\n");

  ProgramElement *element = malloc(sizeof(ProgramElement));
  element->definitions = definitions;
  element->location = location;
  element->free = freeProgram;
  element->eval = evalProgram;
  return element;
}

static void freeProgram(ProgramElement *element) {
  freeLinkedList(element->definitions);
  free(element);
}

static void freeProgramData(ProgramData *programData) {
  if (programData == NULL)
    return;

  free(programData->generated_code);
  free(programData);
}

static ProgramData *evalProgram(ProgramElement *element, EvalContext *context) {
  printf("Evaluating program\n");

  ProgramData *program_data = malloc(sizeof(ProgramData));
  program_data->free = freeProgramData;
  program_data->generated_code = strdup("#include <stdint.h>\n");

  LinkedListElement *current;
  for (current = element->definitions->head; current != NULL;
       current = current->next) {
    DefinitionElement *definition = (DefinitionElement *)current->data;

    if (definition != NULL) {
      DefinitionData *def_data = definition->eval(definition, context);
      strappend(&program_data->generated_code, def_data->generated_code);
      def_data->free(def_data);
    }
  }

  return program_data;
}

void eval(ProgramElement *program) {
  EvalContext *context = newEvalContext();
  ProgramData *program_data = program->eval(program, context);
  printf("%s\n", program_data->generated_code);
  context->free(context);
  program->free(program);
}
