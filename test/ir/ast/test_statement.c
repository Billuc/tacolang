#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include "ir/ast/statement.h"

/* ------------------------ MOCKS ------------------------ */

static int calls_to_freeAssign = 0;
void mock_freeAssign(AssignElement *assign)
{
    calls_to_freeAssign++;
    free(assign);
}

static int calls_to_evalAssign = 0;
void mock_evalAssign(AssignElement *assign, EvalContext *context)
{
    calls_to_evalAssign++;
    return;
}

AssignElement *mock_newAssign(void)
{
    AssignElement *assign = malloc(sizeof(AssignElement));
    assign->free = mock_freeAssign;
    assign->eval = mock_evalAssign;
    return assign;
}

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeAssign = 0;
    calls_to_evalAssign = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newAssignmentStatement)
{
    AssignElement *mockAssign = mock_newAssign();
    StatementElement *statement = newAssignmentStatement(mockAssign);

    ck_assert_ptr_nonnull(statement);
    ck_assert_int_eq(statement->type, s_assignment);
    ck_assert_ptr_eq(statement->statement.assign, mockAssign);

    statement->free(statement);
}
END_TEST

START_TEST(test_freeStatement)
{
    AssignElement *mockAssign = mock_newAssign();
    StatementElement *statement = newAssignmentStatement(mockAssign);

    statement->free(statement);

    ck_assert_int_eq(calls_to_freeAssign, 1);
    // No direct way to test free, but ensure no segfault occurs
    ck_assert_msg(1, "freeStatement executed without crashing");
}
END_TEST

START_TEST(test_evalStatement)
{
    AssignElement *mockAssign = mock_newAssign();
    EvalContext *context = newEvalContext();
    StatementElement *statement = newAssignmentStatement(mockAssign);

    statement->eval(statement, context);

    ck_assert_int_eq(calls_to_evalAssign, 1);

    statement->free(statement);
    context->free(context);
}
END_TEST

Suite *test_statement_suite(void)
{
    Suite *s;
    TCase *tc_statement, *tc_statement_list;

    s = suite_create("Test statement.c");
    tc_statement = tcase_create("Core");

    tcase_add_checked_fixture(tc_statement, setup, teardown);
    tcase_add_test(tc_statement, test_newAssignmentStatement);
    tcase_add_test(tc_statement, test_freeStatement);
    tcase_add_test(tc_statement, test_evalStatement);
    suite_add_tcase(s, tc_statement);

    return s;
}