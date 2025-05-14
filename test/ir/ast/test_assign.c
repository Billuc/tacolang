#include <check.h>
#include <stdlib.h>
#include "ir/ast/assign.h"
#include "ir/ast/variable.h"
#include "ir/ast/value.h"

/* ------------------------ MOCKS ------------------------ */

static int calls_to_freeVariable = 0;
void mock_freeVariable(VariableElement *variable)
{
    calls_to_freeVariable++;
    free(variable);
}

static int calls_to_freeValue = 0;
void mock_freeValue(ValueElement *value)
{
    calls_to_freeValue++;
    free(value);
}

static int calls_to_evalVariable = 0;
VariableData *mock_evalVariable(VariableElement *variable, EvalContext *context)
{
    calls_to_evalVariable++;
    return NULL; // TODO
}

static int calls_to_evalValue = 0;
ValueData *mock_evalValue(ValueElement *value, EvalContext *context)
{
    calls_to_evalValue++;
    return NULL; // TODO
}

VariableElement *mock_newVariable(void)
{
    VariableElement *variable = malloc(sizeof(VariableElement));
    variable->free = mock_freeVariable;
    return variable;
}

ValueElement *mock_newValue(void)
{
    ValueElement *value = malloc(sizeof(ValueElement));
    value->free = mock_freeValue;
    return value;
}

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeVariable = 0;
    calls_to_freeValue = 0;
    calls_to_evalVariable = 0;
    calls_to_evalValue = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newAssign)
{
    VariableElement *mockVar = mock_newVariable();
    ValueElement *mockVal = mock_newValue();
    AssignElement *assign = newAssign(mockVar, mockVal);

    ck_assert_ptr_nonnull(assign);
    ck_assert_ptr_eq(assign->left, mockVar);
    ck_assert_ptr_eq(assign->right, mockVal);

    assign->free(assign);
    ck_assert_int_eq(calls_to_freeVariable, 1);
    ck_assert_int_eq(calls_to_freeValue, 1);
}
END_TEST

START_TEST(test_evalAssign)
{
    VariableElement *mockVar = mock_newVariable();
    ValueElement *mockVal = mock_newValue();
    AssignElement *assign = newAssign(mockVar, mockVal);
    EvalContext *mockContext = NULL;

    assign->eval(assign, mockContext);

    assign->free(assign);
    ck_assert_int_eq(calls_to_freeVariable, 1);
    ck_assert_int_eq(calls_to_freeValue, 1);
}
END_TEST

START_TEST(test_evalAssign_typeCheck)
{
    VariableElement *mockVar = mock_newVariable();
    ValueElement *mockVal = mock_newValue();
    AssignElement *assign = newAssign(mockVar, mockVal);
    EvalContext *mockContext = NULL;

    // TODO
    assign->eval(assign, mockContext);

    assign->free(assign);
    ck_assert_int_eq(calls_to_freeVariable, 1);
    ck_assert_int_eq(calls_to_freeValue, 1);
}
END_TEST

Suite *test_assign_suite(void)
{
    Suite *s;
    TCase *tc_assign;

    s = suite_create("Test assign.c");
    tc_assign = tcase_create("Assign");

    tcase_add_checked_fixture(tc_assign, setup, teardown);
    tcase_add_test(tc_assign, test_newAssign);
    tcase_add_test(tc_assign, test_evalAssign);
    tcase_add_test(tc_assign, test_evalAssign_typeCheck);
    suite_add_tcase(s, tc_assign);

    return s;
}
