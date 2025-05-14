#include <check.h>
#include <stdlib.h>
#include "ir/ast/variable.h"
#include "ir/ast/declare.h"

/* ------------------------ MOCKS ------------------------ */

static int calls_to_freeDeclare = 0;
void mock_freeDeclare(DeclareElement *declare)
{
    calls_to_freeDeclare++;
    free(declare);
}

static int calls_to_evalDeclare = 0;
DeclareData *mock_evalDeclare(DeclareElement *declare, EvalContext *context)
{
    calls_to_evalDeclare++;
    return NULL; // TODO
}

DeclareElement *mock_newDeclare(void)
{
    DeclareElement *declare = malloc(sizeof(DeclareElement));
    declare->free = mock_freeDeclare;
    declare->eval = mock_evalDeclare;
    return declare;
}

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeDeclare = 0;
    calls_to_evalDeclare = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newIdentifierVariable)
{
    VariableElement *variable = newIdentifierVariable("testVar");

    ck_assert_ptr_nonnull(variable);
    ck_assert_str_eq(variable->element_data.identifier, "testVar");

    variable->free(variable);
    ck_assert_int_eq(calls_to_freeDeclare, 0); // No dependency on DeclareElement here
}
END_TEST

START_TEST(test_newDeclareVariable)
{
    DeclareElement *mockDeclare = mock_newDeclare();
    VariableElement *variable = newDeclareVariable(mockDeclare);

    ck_assert_ptr_nonnull(variable);
    ck_assert_ptr_eq(variable->element_data.declare, mockDeclare);

    variable->free(variable);
    ck_assert_int_eq(calls_to_freeDeclare, 1);
}
END_TEST

START_TEST(test_evalVariable)
{
    DeclareElement *mockDeclare = mock_newDeclare();
    VariableElement *variable = newDeclareVariable(mockDeclare);
    EvalContext *context = newEvalContext();

    VariableData *data = variable->eval(variable, context);
    ck_assert_int_eq(calls_to_evalDeclare, 1);

    // TODO: Check the data

    free(data);
    variable->free(variable);
}
END_TEST

Suite *test_variable_suite(void)
{
    Suite *s;
    TCase *tc_variable;

    s = suite_create("Test variable.c");
    tc_variable = tcase_create("Variable");

    tcase_add_checked_fixture(tc_variable, setup, teardown);
    tcase_add_test(tc_variable, test_newIdentifierVariable);
    tcase_add_test(tc_variable, test_newDeclareVariable);
    tcase_add_test(tc_variable, test_evalVariable);
    suite_add_tcase(s, tc_variable);

    return s;
}
