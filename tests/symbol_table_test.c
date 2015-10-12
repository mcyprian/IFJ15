#include <stdio.h>
#include <stdlib.h>
#include <error_macros.h>
#include <symbol_table.h>
#include <check.h>

#include <limits.h>

START_TEST (test_get_key)
{
	ck_assert_int_eq(get_key(NULL), INTERNAL_ERROR);
	ck_assert_int_eq(get_key("a"), get_key("a"));
	ck_assert_int_eq(get_key("long_variable_identificator123"), 
                     get_key("long_variable_identificator123"));
	ck_assert_int_ne(get_key("net"), get_key("ten"));
	ck_assert_int_ne(get_key("ad"), get_key("bc"));
	
}
END_TEST


Suite * symbol_table_suite(void)
{
	Suite *s;
	TCase *tc_get_key; 
	s = suite_create("SymbolTable");
      
	tc_get_key = tcase_create("Get_Key");
	tcase_add_test(tc_get_key, test_get_key);
	suite_add_tcase(s, tc_get_key);

	return s;
}

int main(void)
 {
	int number_failed;
	Suite *s;
	SRunner *sr;
	s = symbol_table_suite();
	sr = srunner_create(s);
	srunner_set_log (sr, "symbol_table.log");
	srunner_run_all(sr, CK_SUBUNIT);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

