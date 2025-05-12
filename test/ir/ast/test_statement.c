#include <check.h>
#include <stdlib.h>
#include "ir/ast/statement.h"

/* ------------------------ MOCKS ------------------------ */

int calls_to_freeAssign = 0;
void mock_freeAssign(AssignElement *assign)
{
    calls_to_freeAssign++;
    free(assign);
}

int calls_to_evalAssign = 0;
void mock_evalAssign(AssignElement *assign, SymbolElement **symbolTable)
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
    // No cleanup needed for this test suite
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newAssignmentStatement)
{
    AssignElement *mockAssign = mock_newAssign();
    StatementElement *statement = newAssignmentStatement(mockAssign);

    ck_assert_ptr_nonnull(statement);
    ck_assert_int_eq(statement->type, s_assignment);
    ck_assert_ptr_eq(statement->data.assign, mockAssign);

    free(statement);
    free(mockAssign);
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
    SymbolElement *mockSymbolTable = NULL;
    StatementElement *statement = newAssignmentStatement(mockAssign);

    statement->eval(statement, &mockSymbolTable);

    ck_assert_int_eq(calls_to_evalAssign, 1);

    statement->free(statement);
    freeSymbolTable(mockSymbolTable);
}
END_TEST

START_TEST(test_addStatement)
{
    AssignElement *mockAssign1 = mock_newAssign();
    AssignElement *mockAssign2 = mock_newAssign();
    StatementElement *statement1 = newAssignmentStatement(mockAssign1);
    StatementElement *statement2 = newAssignmentStatement(mockAssign2);

    StatementLink *list = NULL;
    list = addStatement(list, statement1);
    list = addStatement(list, statement2);

    ck_assert_ptr_nonnull(list);
    ck_assert_ptr_eq(list->element, statement2);
    ck_assert_ptr_nonnull(list->next);
    ck_assert_ptr_eq(list->next->element, statement1);

    freeStatementList(list);
}
END_TEST

START_TEST(test_freeStatementList)
{
    AssignElement *mockAssign1 = mock_newAssign();
    AssignElement *mockAssign2 = mock_newAssign();
    StatementElement *statement1 = newAssignmentStatement(mockAssign1);
    StatementElement *statement2 = newAssignmentStatement(mockAssign2);

    StatementLink *list = NULL;
    list = addStatement(list, statement1);
    list = addStatement(list, statement2);

    freeStatementList(list);

    ck_assert_int_eq(calls_to_freeAssign, 2);
    // No direct way to test free, but ensure no segfault occurs
    ck_assert_msg(1, "freeStatementList executed without crashing");
}
END_TEST

START_TEST(test_evalStatementList)
{
    AssignElement *mockAssign1 = mock_newAssign();
    AssignElement *mockAssign2 = mock_newAssign();
    SymbolElement *mockSymbolTable = NULL;

    StatementElement *statement1 = newAssignmentStatement(mockAssign1);
    StatementElement *statement2 = newAssignmentStatement(mockAssign2);

    StatementLink *list = NULL;
    list = addStatement(list, statement1);
    list = addStatement(list, statement2);

    evalStatementList(list, &mockSymbolTable);

    ck_assert_int_eq(calls_to_evalAssign, 2);

    freeStatementList(list);
    freeSymbolTable(mockSymbolTable);
}
END_TEST

Suite *test_statement_suite(void)
{
    Suite *s;
    TCase *tc_statement, *tc_statement_list;

    s = suite_create("Test statement.c");
    tc_statement = tcase_create("Statement");

    tcase_add_checked_fixture(tc_statement, setup, teardown);
    tcase_add_test(tc_statement, test_newAssignmentStatement);
    tcase_add_test(tc_statement, test_freeStatement);
    tcase_add_test(tc_statement, test_evalStatement);
    suite_add_tcase(s, tc_statement);

    tc_statement_list = tcase_create("StatementList");

    tcase_add_checked_fixture(tc_statement, setup, teardown);
    tcase_add_test(tc_statement_list, test_addStatement);
    tcase_add_test(tc_statement_list, test_freeStatementList);
    tcase_add_test(tc_statement_list, test_evalStatementList);
    suite_add_tcase(s, tc_statement_list);

    return s;
}