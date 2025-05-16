#include <check.h>
#include <stdlib.h>
#include "ir/ast/declare.h"
#include "ir/ast/typedef.h"

/* ------------------------ MOCKS ------------------------ */

#include "./mocks/mock_typedef.h"

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeTypedef = 0;
    calls_to_evalTypedef = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newDeclare)
{
    TypedefElement *mockType = mock_newTypedef();
    DeclareElement *declare = newDeclare("testVar", mockType);

    ck_assert_ptr_nonnull(declare);
    ck_assert_str_eq(declare->name, "testVar");
    ck_assert_ptr_eq(declare->type, mockType);

    declare->free(declare);
    ck_assert_int_eq(calls_to_freeTypedef, 1);
}
END_TEST

START_TEST(test_evalDeclare)
{
    TypedefElement *mockType = mock_newTypedef();
    DeclareElement *declare = newDeclare("testVar", mockType);
    EvalContext *context = newEvalContext();

    DeclareData *data = declare->eval(declare, context);
    ck_assert_int_eq(calls_to_evalTypedef, 1);
    ck_assert_int_eq(context->symbolTableStack->size, 1);
    SymbolTable *localTable = (SymbolTable *)(context->symbolTableStack->head->data);
    ck_assert_int_eq(localTable->size, 1);
    SymbolData *symbol = (SymbolData *)(localTable->head->data);
    ck_assert_str_eq(symbol->name, "testVar");
    ck_assert_int_eq(compare_type(symbol->type, mockType), 0);

    free(data);
    declare->free(declare);
}
END_TEST

Suite *test_declare_suite(void)
{
    Suite *s;
    TCase *tc_declare;

    s = suite_create("Test declare.c");
    tc_declare = tcase_create("Declare");

    tcase_add_checked_fixture(tc_declare, setup, teardown);
    tcase_add_test(tc_declare, test_newDeclare);
    tcase_add_test(tc_declare, test_evalDeclare);
    suite_add_tcase(s, tc_declare);

    return s;
}
