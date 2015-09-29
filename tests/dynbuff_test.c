#include <stdio.h>
#include <stdlib.h>
#include <dynamic_buffer.h>
#include <check.h>

START_TEST (test_null)
{
	char * str = NULL;
	fail_if(!str, "str is NULL");	
}
END_TEST

Suite * dynbuff_suite(void)
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("DynamicBuffer");
      
	/* Core test case */
	tc_core = tcase_create("Core_");

	tcase_add_test(tc_core, test_null);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
 {
	int number_failed;
	Suite *s;
	SRunner *sr;
	s = dynbuff_suite();
	sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

