#include <check.h>
#include <stdlib.h>
#include "ir/ast/value.h"
#include "ir/ast/expression.h"

/* ------------------------ MOCKS ------------------------ */

static int calls_to_freeExpression = 0;
void mock_freeExpression(ExpressionElement *expression)
{
    calls_to_freeExpression++;
    free(expression);
}

ExpressionElement *mock_newExpression(void)
{
    ExpressionElement *expression = malloc(sizeof(ExpressionElement));
    expression->free = mock_freeExpression;
    return expression;
}

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeExpression = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newExpressionValue)
{
    ExpressionElement *mockExpression = mock_newExpression();
    ValueElement *value = newExpressionValue(mockExpression);

    ck_assert_ptr_nonnull(value);
    ck_assert_ptr_eq(value->value.expression, mockExpression);

    value->free(value);
    ck_assert_int_eq(calls_to_freeExpression, 1);
}
END_TEST

START_TEST(test_newIntegerValue)
{
    ValueElement *value = newIntegerValue(42);

    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(value->type, v_integer);
    ck_assert_int_eq(value->value.integer, 42);

    value->free(value);
    ck_assert_int_eq(calls_to_freeExpression, 0);
}
END_TEST

START_TEST(test_newFloatValue)
{
    ValueElement *value = newFloatValue(3.14f);

    ck_assert_ptr_nonnull(value);
    ck_assert_int_eq(value->type, v_floating);
    ck_assert_float_eq_tol(value->value.floating, 3.14f, 0.0001f);

    value->free(value);
    ck_assert_int_eq(calls_to_freeExpression, 0);
}
END_TEST

Suite *test_value_suite(void)
{
    Suite *s;
    TCase *tc_value;

    s = suite_create("Test value.c");
    tc_value = tcase_create("Value");

    tcase_add_checked_fixture(tc_value, setup, teardown);
    tcase_add_test(tc_value, test_newExpressionValue);
    tcase_add_test(tc_value, test_newIntegerValue);
    tcase_add_test(tc_value, test_newFloatValue);
    suite_add_tcase(s, tc_value);

    return s;
}
