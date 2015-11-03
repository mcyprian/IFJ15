#include <stdio.h>
#include <stdlib.h>
#include <token.h>
#include <stack.h>
#include <error.h>
#include <precedence_analysis.h>
#include <check.h>

#define NUM 100

START_TEST (test_main) {
   ck_assert_int_eq(precedence_table[type_filter(IDENTIFIER)][type_filter(IDENTIFIER)], 3);
   ck_assert_int_eq(precedence_table[type_filter(OPENING_BRACKET)][type_filter(CLOSING_BRACKET)], 2);

   ck_assert_int_eq(type_filter(O_EQUALS), O_EQUALS);
   ck_assert_int_eq(type_filter(IDENTIFIER), IDENTIFIER);
   ck_assert_int_eq(type_filter(L_INT), L_INT);
   ck_assert_int_eq(type_filter(L_DOUBLE), L_INT);
   ck_assert_int_eq(type_filter(L_STRING), L_INT);
   ck_assert_int_eq(type_filter(T_DOUBLE), END_OF_EXPR);
   ck_assert_int_eq(type_filter(K_ELSE), END_OF_EXPR);

}
END_TEST

START_TEST (test_get_types) {
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, NUM, sizeof(TToken));
    TStack stack;
    init_stack(&stack);
    TToken *item;
    index_t index;
    int values[NUM];
    int i;
    
    for (int i = 0; i < 5; i++) {
        new_item(&b, index, item);
        item->token_type = i;
        push(&b, &stack, index);
    
    }
    ck_assert_int_eq(get_types(NULL, &stack, values), INTERNAL_ERROR);
    ck_assert_int_eq(get_types(&b, NULL, values), INTERNAL_ERROR);
    ck_assert_int_eq(get_types(&b, &stack, NULL), INTERNAL_ERROR);
    ck_assert_int_eq(get_types(&b, &stack, values), SYNTAX_ERROR);

    ck_assert_int_eq(overwrite_top(&b, &stack, SHIFT), RETURN_OK);

    ck_assert_int_eq(get_types(&b, &stack, values), RETURN_OK);
    
    ck_assert_int_eq(values[0], 0);

    for (i = 0; i < 3; i++) {
        new_item(&b, index, item);
        item->token_type = i;
        push(&b, &stack, index);
    
    }
    
    get_types(&b, &stack, values);
    
    for (i = 0; i < 4; i++) 
        ck_assert_int_eq(values[i], 3 - i);

    for (i = 0; i < 2; i++)
        pop(&b, &stack);
    
    overwrite_top(&b, &stack, IDENTIFIER);
    get_types(&b, &stack, values);
    ck_assert_int_eq(values[1], IDENTIFIER);

    free_structure_buffer(&b);
}
END_TEST

START_TEST (test_rules) {
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, NUM, sizeof(TToken));
    TStack stack;
    init_stack(&stack);
    TToken *item;
    index_t index;
    int i;
    int tokens_one[] = {SHIFT, IDENTIFIER};
    int tokens_two[] = {SHIFT, RVALUE, O_GE, RVALUE};
    int tokens_three[] = {SHIFT, OPENING_BRACKET, RVALUE, CLOSING_BRACKET};

    for (i = 0; i < 2; i++) {
        new_item(&b, index, item);
        item->token_type = tokens_one[i];
        push(&b, &stack, index);
    }
    get_rule(&b, &stack);
    reduce(&b, &stack);

    for (i = 0; i < 4; i++) {
        new_item(&b, index, item);
        item->token_type = tokens_two[i];
        push(&b, &stack, index);
    }
    get_rule(&b, &stack);
    reduce(&b, &stack);

    for (i = 0; i < 4; i++) {
        new_item(&b, index, item);
        item->token_type = tokens_three[i];
        push(&b, &stack, index);
    }
    get_rule(&b, &stack);
    reduce(&b, &stack);
 
    free_structure_buffer(&b);
}
END_TEST
 
Suite * stack_suite(void) {
    Suite *s;
    TCase *tc_main, *tc_types, *tc_rules;
    s = suite_create("Precedence_analysis");

    tc_main = tcase_create("Main");
    tcase_add_test(tc_main, test_main);
    suite_add_tcase(s, tc_main);

    tc_types = tcase_create("Types");
    tcase_add_test(tc_types, test_get_types);
    suite_add_tcase(s, tc_types);

    tc_rules = tcase_create("Rules");
    tcase_add_test(tc_rules, test_rules);
    suite_add_tcase(s, tc_rules);
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = stack_suite();
    sr = srunner_create(s);
    srunner_set_log(sr, "pa_test.log");
    srunner_run_all(sr, CK_SUBUNIT);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

