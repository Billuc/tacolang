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

static int calls_to_evalExpression = 0;
EvalExpressionData *mock_evalExpression(ExpressionElement *expression, SymbolElement **symbolTable)
{
    EvalType *type = malloc(sizeof(EvalType));
    type->type = t_variable;
    type->data.variableTypeData.is_base_type = true;
    type->data.variableTypeData.type_data.baseType = U32;
    type->data.variableTypeData.modifiers = NULL;

    EvalExpressionData *evalData = malloc(sizeof(EvalExpressionData));
    evalData->type = type;

    calls_to_evalExpression++;
    return evalData;
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
    calls_to_evalExpression = 0;
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

START_TEST(test_evalIntegerValue)
{
    ValueElement *value = newIntegerValue(42);
    SymbolElement *mockSymbolTable = NULL;

    EvalValueData *evalData = value->eval(value, &mockSymbolTable);

    ck_assert_ptr_nonnull(evalData);
    ck_assert_int_eq(evalData->valueType->type, t_variable);
    ck_assert(evalData->valueType->data.variableTypeData.is_base_type);
    ck_assert_int_eq(evalData->valueType->data.variableTypeData.type_data.baseType, U32);

    free(evalData);
    value->free(value);
}
END_TEST

START_TEST(test_evalFloatValue)
{
    ValueElement *value = newFloatValue(3.14f);
    SymbolElement *mockSymbolTable = NULL;

    EvalValueData *evalData = value->eval(value, &mockSymbolTable);

    ck_assert_ptr_nonnull(evalData);
    ck_assert_int_eq(evalData->valueType->type, t_variable);
    ck_assert(evalData->valueType->data.variableTypeData.is_base_type);
    ck_assert_int_eq(evalData->valueType->data.variableTypeData.type_data.baseType, F32);

    free(evalData);
    value->free(value);
}
END_TEST

START_TEST(test_evalExpressionValue)
{
    ExpressionElement *mockExpression = mock_newExpression();
    ValueElement *value = newExpressionValue(mockExpression);
    SymbolElement *mockSymbolTable = NULL;

    EvalValueData *evalData = value->eval(value, &mockSymbolTable);

    ck_assert_ptr_nonnull(evalData);
    ck_assert_int_eq(evalData->valueType->type, t_variable);
    ck_assert(evalData->valueType->data.variableTypeData.is_base_type);
    ck_assert_int_eq(evalData->valueType->data.variableTypeData.type_data.baseType, U32);

    free(evalData);
    value->free(value);
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
    tcase_add_test(tc_value, test_evalIntegerValue);
    tcase_add_test(tc_value, test_evalFloatValue);
    tcase_add_test(tc_value, test_evalExpressionValue);
    suite_add_tcase(s, tc_value);

    return s;
}
