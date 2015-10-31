#include <stdio.h>
#include <stdlib.h>
#include <token.h>
#include <error.h>
#include <precedence_analysis.h>
#include <check.h>


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

Suite * stack_suite(void) {
    Suite *s;
    TCase *tc_main;
    s = suite_create("Precedence_analysis");

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
    srunner_set_log(sr, "pa_test.log");
    srunner_run_all(sr, CK_SUBUNIT);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

