#include "statement.h"
#include "utils/str_utils.h"
#include <stdio.h>
#include <stdlib.h>

static void freeStatement(StatementElement *element);
static StatementData *evalStatement(StatementElement *element,
                                    EvalContext *context);

StatementElement *newAssignmentStatement(AssignElement *assign,
                                         location_t location) {
  printf("Creating new assignment statement\n");

  StatementElement *element = malloc(sizeof(StatementElement));
  Statement data = {.assign = assign};
  element->statement = data;
  element->type = s_assignment;
  element->location = location;
  element->free = freeStatement;
  element->eval = evalStatement;
  return element;
}

StatementElement *newDeclareStatement(DeclareElement *declare,
                                      location_t location) {
  printf("Creating new declare statement\n");

  StatementElement *element = malloc(sizeof(StatementElement));
  Statement data = {.declare = declare};
  element->statement = data;
  element->type = s_declaration;
  element->location = location;
  element->free = freeStatement;
  element->eval = evalStatement;
  return element;
}

StatementElement *newValueStatement(ValueElement *value, location_t location) {
  printf("Creating new value statement\n");

  StatementElement *element = malloc(sizeof(StatementElement));
  Statement data = {.value = value};
  element->statement = data;
  element->type = s_value;
  element->location = location;
  element->free = freeStatement;
  element->eval = evalStatement;
  return element;
}

StatementElement *newCommentStatement(char *comment, location_t location) {
  printf("Creating new comment statement\n");

  StatementElement *element = malloc(sizeof(StatementElement));
  Statement data = {.comment = strdup(comment)};
  element->statement = data;
  element->type = s_comment;
  element->location = location;
  element->free = freeStatement;
  element->eval = evalStatement; // Comments don't evaluate to code
  return element;
}

static void freeStatement(StatementElement *element) {
  switch (element->type) {
  case s_assignment:
    element->statement.assign->free(element->statement.assign);
    break;
  case s_declaration:
    element->statement.declare->free(element->statement.declare);
    break;
  case s_value:
    element->statement.value->free(element->statement.value);
    break;
  case s_comment:
    free(element->statement.comment);
    break;
  }

  free(element);
}

static void freeStatementData(StatementData *data) {
  free(data->generated_code);
  free(data);
}

static StatementData *evalStatement(StatementElement *element,
                                    EvalContext *context) {
  printf("Evaluating statement\n");

  StatementData *data = malloc(sizeof(StatementData));
  data->free = freeStatementData;
  data->generated_code = strdup("");

  switch (element->type) {
  case s_assignment: {
    Assign *assign_data =
        element->statement.assign->eval(element->statement.assign, context);
    strappend(&data->generated_code, assign_data->generated_code);
    assign_data->free(assign_data);
    break;
  }
  case s_declaration: {
    DeclareData *declare_data =
        element->statement.declare->eval(element->statement.declare, context);
    strappend(&data->generated_code, declare_data->generated_code);
    declare_data->free(declare_data);
    break;
  }
  case s_value: {
    ValueData *value_data =
        element->statement.value->eval(element->statement.value, context);
    if (!value_data) {
      break;
    }
    strappend(&data->generated_code, value_data->generated_code);
    value_data->free(value_data);
    break;
  }
  case s_comment: {
    strappend(&data->generated_code, "// ");
    strappend(&data->generated_code, element->statement.comment);
    break;
  }
  }

  strappend(&data->generated_code, ";\n");
  return data;
}

StatementList *newStatementList() { return newLinkedList(freeStatement); }
