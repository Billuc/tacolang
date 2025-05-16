#include <check.h>
#include <stdlib.h>
#include "ir/ast/assign.h"
#include "ir/ast/variable.h"
#include "ir/ast/value.h"

extern int number_of_errors;
extern char **error_messages;

/* ------------------------ MOCKS ------------------------ */

#include "mocks/mock_variable.h"
#include "mocks/mock_value.h"

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
    VariableElement *mockVar = mock_newVariable(mock_DeclareVariable);
    ValueElement *mockVal = mock_newValue(mock_FloatValue);
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
    VariableElement *mockVar = mock_newVariable(mock_DeclareVariable);
    ValueElement *mockVal = mock_newValue(mock_IntValue);
    AssignElement *assign = newAssign(mockVar, mockVal);
    EvalContext *mockContext = NULL;

    assign->eval(assign, mockContext);

    ck_assert_int_eq(number_of_errors, 0);
    ck_assert_int_eq(calls_to_evalVariable, 1);
    ck_assert_int_eq(calls_to_evalValue, 1);

    assign->free(assign);
    ck_assert_int_eq(calls_to_freeVariable, 1);
    ck_assert_int_eq(calls_to_freeValue, 1);
}
END_TEST

START_TEST(test_evalAssign_typeCheck)
{
    VariableElement *mockVar = mock_newVariable(mock_DeclareVariable);
    ValueElement *mockVal = mock_newValue(mock_FloatValue);
    AssignElement *assign = newAssign(mockVar, mockVal);
    EvalContext *mockContext = NULL;

    assign->eval(assign, mockContext);

    ck_assert_int_eq(number_of_errors, 1);
    ck_assert_int_eq(calls_to_evalVariable, 1);
    ck_assert_int_eq(calls_to_evalValue, 1);

    assign->free(assign);
    ck_assert_int_eq(calls_to_freeVariable, 1);
    ck_assert_int_eq(calls_to_freeValue, 1);
}
END_TEST

START_TEST(test_evalAssign_nonAssignableVariable)
{
    VariableElement *mockVar = mock_newVariable(mock_IdentifierVariable);
    ValueElement *mockVal = mock_newValue(mock_FloatValue);
    AssignElement *assign = newAssign(mockVar, mockVal);
    EvalContext *mockContext = NULL;

    assign->eval(assign, mockContext);

    ck_assert_int_eq(number_of_errors, 1);
    ck_assert_int_eq(calls_to_evalVariable, 1);
    ck_assert_int_eq(calls_to_evalValue, 1);

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
    tcase_add_test(tc_assign, test_evalAssign_nonAssignableVariable);
    suite_add_tcase(s, tc_assign);

    return s;
}
