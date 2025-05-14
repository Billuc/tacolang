#include <check.h>
#include <stdlib.h>
#include "ir/ast/typedef.h"
#include "ir/ast/modifier.h"

/* ------------------------ MOCKS ------------------------ */

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newTypedef)
{
    ModifierList *mockModifiers = newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "int");

    ck_assert_ptr_nonnull(typedefEl);
    ck_assert_str_eq(typedefEl->name, "int");
    ck_assert_ptr_eq(typedefEl->modifiers, mockModifiers);

    typedefEl->free(typedefEl);
}
END_TEST

START_TEST(test_evalTypedef)
{
    ModifierList *mockModifiers = newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "U8");
    EvalContext *context = newEvalContext();

    TypedefData *data = typedefEl->eval(typedefEl, context);

    ck_assert_ptr_nonnull(data);
    ck_assert_ptr_eq(data->type->type_type, t_variable);
    ck_assert(data->type->type_data.variable_type.is_base_type);
    ck_assert_int_eq(data->type->type_data.variable_type.type_data.base_type, U8);
    ck_assert_ptr_null(data->type->type_data.variable_type.modifiers);

    data->free(data);
    typedefEl->free(typedefEl);
}
END_TEST

START_TEST(test_evalTypedef_customType)
{
    ModifierList *mockModifiers = newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "CustomType");
    EvalContext *context = newEvalContext();

    TypedefData *data = typedefEl->eval(typedefEl, context);

    ck_assert_ptr_nonnull(data);
    ck_assert_ptr_eq(data->type->type_type, t_variable);
    ck_assert(!data->type->type_data.variable_type.is_base_type);
    ck_assert_str_eq(data->type->type_data.variable_type.type_data.custom_type, "CustomType");
    ck_assert_ptr_null(data->type->type_data.variable_type.modifiers);

    data->free(data);
    typedefEl->free(typedefEl);
}
END_TEST

Suite *test_typedef_suite(void)
{
    Suite *s;
    TCase *tc_typedef;

    s = suite_create("Test typedef.c");
    tc_typedef = tcase_create("Core");

    tcase_add_checked_fixture(tc_typedef, setup, teardown);
    tcase_add_test(tc_typedef, test_newTypedef);
    tcase_add_test(tc_typedef, test_evalTypedef);
    tcase_add_test(tc_typedef, test_evalTypedef_customType);
    suite_add_tcase(s, tc_typedef);

    return s;
}
