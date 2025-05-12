#include <check.h>
#include <stdlib.h>
#include "ir/ast/typedef.h"
#include "ir/ast/modifier.h"

/* ------------------------ MOCKS ------------------------ */

static int calls_to_freeModifierList = 0;
void mock_freeModifierList(ModifierLink *modifiers)
{
    calls_to_freeModifierList++;
    free(modifiers);
}

static int calls_to_evalModifierList = 0;
EvalModifierLinkData *mock_evalModifier(ModifierLink *modifiers, SymbolElement **symbolTable)
{
    calls_to_evalModifierList++;
    return NULL; // TODO
}

ModifierLink *mock_newModifierList(void)
{
    ModifierLink *modifiers = malloc(sizeof(ModifierLink));
    return modifiers;
}

/* ------------------------ FIXTURES ------------------------ */

static void setup(void)
{
    calls_to_freeModifierList = 0;
    calls_to_evalModifierList = 0;
}

static void teardown(void)
{
}

/* ------------------------ TESTS ------------------------ */

START_TEST(test_newTypedef)
{
    ModifierLink *mockModifiers = mock_newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "int");

    ck_assert_ptr_nonnull(typedefEl);
    ck_assert_str_eq(typedefEl->name, "int");
    ck_assert_ptr_eq(typedefEl->modifiers, mockModifiers);

    typedefEl->free(typedefEl);
    ck_assert_int_eq(calls_to_freeModifierList, 1);
}
END_TEST

START_TEST(test_evalTypedef)
{
    ModifierLink *mockModifiers = mock_newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "U8");
    SymbolElement *mockSymbolTable = NULL;

    EvalTypedefData *data = typedefEl->eval(typedefEl, &mockSymbolTable);

    ck_assert_ptr_nonnull(data);
    ck_assert_ptr_eq(data->type->type, t_variable);
    ck_assert(data->type->data.variableTypeData.is_base_type);
    ck_assert_int_eq(data->type->data.variableTypeData.type_data.baseType, U8);
    ck_assert_ptr_null(data->type->data.variableTypeData.modifiers);

    free(data);
    typedefEl->free(typedefEl);
    ck_assert_int_eq(calls_to_freeModifierList, 1);
}
END_TEST

START_TEST(test_evalTypedef_customType)
{
    ModifierLink *mockModifiers = mock_newModifierList();
    TypedefElement *typedefEl = newTypedef(mockModifiers, "CustomType");
    SymbolElement *mockSymbolTable = NULL;

    EvalTypedefData *data = typedefEl->eval(typedefEl, &mockSymbolTable);

    ck_assert_ptr_nonnull(data);
    ck_assert_ptr_eq(data->type->type, t_variable);
    ck_assert(!data->type->data.variableTypeData.is_base_type);
    ck_assert_str_eq(data->type->data.variableTypeData.type_data.customType, "CustomType");
    ck_assert_ptr_null(data->type->data.variableTypeData.modifiers);

    free(data);
    typedefEl->free(typedefEl);
    ck_assert_int_eq(calls_to_freeModifierList, 1);
}
END_TEST

Suite *test_typedef_suite(void)
{
    Suite *s;
    TCase *tc_typedef;

    s = suite_create("Test typedef.c");
    tc_typedef = tcase_create("Typedef");

    tcase_add_checked_fixture(tc_typedef, setup, teardown);
    tcase_add_test(tc_typedef, test_newTypedef);
    tcase_add_test(tc_typedef, test_evalTypedef);
    tcase_add_test(tc_typedef, test_evalTypedef_customType);
    suite_add_tcase(s, tc_typedef);

    return s;
}
