#include <stdio.h>
#include <stack.h>
#include <dynamic_structure_buffer.h>
#include <token.h>
#include <error_macros.h>
#include <check.h>

#define NUM 100

START_TEST (test_main) {
    TDynamic_structure_buffer b;
    TStack stack;
    
    init_structure_buffer(&b, NUM, sizeof(TToken));
    init_stack(&stack);
    ck_assert_int_eq(stack.length, 0);
    ck_assert_int_eq(stack.top, ZERO_INDEX);
    TToken *item;
    index_t index;
    int values[NUM];

    ck_assert_int_eq(pop(&b, &stack), ZERO_INDEX);

    for (int i = 0; i < NUM; i++) {
        new_item(&b, index, item);
        item->token_type = i*10;
        push(&b, &stack, index);
    }
    ck_assert_int_eq(stack.length, NUM);
   
    for (int i = 0; i < NUM; i++) {
        dereference_structure(&b, pop(&b, &stack), (void**)(&item));
        ck_assert_int_eq(item->token_type, (NUM * 10 - 10) - 10 * i);
        ck_assert_int_eq(stack.length, NUM - i - 1);
    }
    free_structure_buffer(&b);
}
END_TEST

START_TEST (test_get_types) {
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, NUM, sizeof(TToken));
    TStack stack;
    init_stack(&stack);
    TToken *item;
    index_t index;
    index_t first_index;
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
    ck_assert_int_eq(get_types(&b, &stack, values), INTERNAL_ERROR);

    ck_assert_int_eq(overwrite_top(&b, &stack, SHIFT), RETURN_OK);

    ck_assert_int_eq(get_types(&b, &stack, values), RETURN_OK);
    
    ck_assert_int_eq(values[0], 0);

    for (i = 0; i < 3; i++) {
        new_item(&b, index, item);
        item->token_type = i;
        push(&b, &stack, index);
    
    }
    
    get_types(&b, &stack, values);
    
    for (i = 1; i < 4; i++) 
        ck_assert_int_eq(values[i], 3 - i);

    for (i = 0; i < 2; i++)
        pop(&b, &stack);
    
    overwrite_top(&b, &stack, IDENTIFIER);
    get_types(&b, &stack, values);
    ck_assert_int_eq(values[1], IDENTIFIER);

    free_structure_buffer(&b);
}
END_TEST

Suite * stack_suite(void) {
    Suite *s;
    TCase *tc_main, *tc_types;
    s = suite_create("Stack");

    tc_main = tcase_create("Main");
    tcase_add_test(tc_main, test_main);
    suite_add_tcase(s, tc_main);

    tc_types = tcase_create("Types");
    tcase_add_test(tc_types, test_get_types);
    suite_add_tcase(s, tc_types);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;
    s = stack_suite();
    sr = srunner_create(s);
    srunner_set_log(sr, "stack_test.log");
    srunner_run_all(sr, CK_SUBUNIT);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

