#include <stdio.h>
#include <stack.h>
#include <dynamic_structure_buffer.h>
#include <token.h>
#include <error_macros.h>
#include <check.h>

#define NUM 100

START_TEST (test_main) {
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, NUM, sizeof(TToken));
    TStack stack;
    init_stack(&stack);
    ck_assert_int_eq(stack.length, 0);
    ck_assert_int_eq(stack.top, ZERO_INDEX);
    TToken *item;
    index_t index;
    int values[NUM];

    for (int i = 0; i < NUM; i++) {
        new_item(&b, index, item);
        item->token_type = i*10;
        push(&b, &stack, index);
    }
    ck_assert_int_eq(stack.length, NUM);
   
    get_types(&b, &stack, NUM, values);

    for (int i = 0; i < NUM; i++)
        ck_assert_int_eq(values[i], (NUM * 10 - 10) - 10 * i);

    for (int i = 0; i < NUM; i++) {
        dereference_structure(&b, pop(&b, &stack), (void**)(&item));
        ck_assert_int_eq(item->token_type, (NUM * 10 - 10) - 10 * i);
        ck_assert_int_eq(stack.length, NUM - i - 1);
    }
    free_structure_buffer(&b);
}
END_TEST

START_TEST (test_expr) {
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, NUM, sizeof(TToken));
    TStack stack;
    init_stack(&stack);
    TToken *item;
    index_t index;
    index_t first_index;
    int values[NUM];
    
    for (int i = 0; i < 3; i++) {
        new_item(&b, index, item);
        item->token_type = i*10;
        push(&b, &stack, index);
        if (i == 0)
            first_index = index;

    }
    item->expr_next = first_index;
    item->expr_type = 33;

    get_types(&b, &stack, 2, values);
    ck_assert_int_eq(values[0], 33);
    ck_assert_int_eq(values[1], 0);

    new_item(&b, index, item);
    item->token_type = 9;
    push(&b, &stack, index);

    get_types(&b, &stack, 2, values);
    ck_assert_int_eq(values[0], 9);
    ck_assert_int_eq(values[1], 33);

    item->expr_next = first_index;
    item->expr_type = 9;
    get_types(&b, &stack, 2, values);
    ck_assert_int_eq(values[0], 9);
    ck_assert_int_eq(values[1], 0);

    pop(&b, &stack);
    ck_assert_int_eq(get_types(&b, &stack, 2, values), INTERNAL_ERROR);
    ck_assert_int_eq(values[0], 0);

    free_structure_buffer(&b);
}
END_TEST

Suite * stack_suite(void) {
    Suite *s;
    TCase *tc_main, *tc_expr;
    s = suite_create("Stack");

    tc_main = tcase_create("Main");
    tcase_add_test(tc_main, test_main);
    suite_add_tcase(s, tc_main);

    tc_expr = tcase_create("Expr");
    tcase_add_test(tc_expr, test_expr);
    suite_add_tcase(s, tc_expr);

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

