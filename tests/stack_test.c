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



Suite * stack_suite(void) {
    Suite *s;
    TCase *tc_main;
    s = suite_create("Stack");

    tc_main = tcase_create("Main");
    tcase_add_test(tc_main, test_main);
    suite_add_tcase(s, tc_main);

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

