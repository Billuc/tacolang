#include <check.h>
#include <stdlib.h>
#include "ir/ast/expression.h"

/* ------------------------ MOCKS ------------------------ */

// Add mock implementations for functions in expression.c if needed

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    // Initialize any global state or variables
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newExpression)
{
    ExpressionElement *expression = newExpression("+");

    ck_assert_ptr_nonnull(expression);
    ck_assert_str_eq(expression->operation, "+");

    expression->free(expression);
}
END_TEST

START_TEST(test_evalExpression)
{
    ExpressionElement *expression = newExpression("+");
    EvalContext *evalContext = newEvalContext();

    ExpressionData *data = expression->eval(expression, evalContext);

    // TODO : Check the data

    free(data);
    expression->free(expression);
}
END_TEST

Suite *test_expression_suite(void)
{
    Suite *s;
    TCase *tc_expression;

    s = suite_create("Test expression.c");
    tc_expression = tcase_create("Expression");

    tcase_add_checked_fixture(tc_expression, setup, teardown);
    tcase_add_test(tc_expression, test_newExpression);
    tcase_add_test(tc_expression, test_evalExpression);
    suite_add_tcase(s, tc_expression);

    return s;
}
