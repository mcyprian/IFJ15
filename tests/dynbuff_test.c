#include <stdio.h>
#include <stdlib.h>
#include <dynamic_buffer.h>
#include <check.h>

#include <limits.h>

START_TEST (test_init)
{
	TDynamic_buffer buff;
	ck_assert_int_eq(init_buffer(&buff, -1), 1);
	ck_assert_int_eq(init_buffer(NULL, 100), 1);
	free_buffer(&buff);
	
}
END_TEST


START_TEST (test_add_c)
{
        TDynamic_buffer buff;
        ck_assert_int_eq(init_buffer(&buff, 1), 0);

	ck_assert_int_eq(add_char(NULL, 'a'), 1);
        ck_assert_int_eq(add_char(&buff, '\0'), 1);
	ck_assert_int_eq(add_char(NULL, '\0'), 1);
	
        
	unsigned int cnt = 100000;
	for(unsigned int i = 0; i < cnt ; i++){
		ck_assert_int_eq(add_char(&buff, 'a'), 0);
		ck_assert_int_ge(buff.length, buff.index);
		if(i == 5)ck_assert_str_eq(buff.buffer, "aaaaaa");
	}

	ck_assert_int_eq(buff.index, cnt);
	ck_assert_int_eq(buff.index, strlen(buff.buffer));
	free_buffer(&buff);

}
END_TEST


START_TEST (test_add_str)
{
        TDynamic_buffer buff;
        ck_assert_int_eq(init_buffer(&buff, 1), 0);

	ck_assert_int_eq(add_str(NULL, "abcd"), 1);
	ck_assert_int_eq(add_str(&buff, NULL), 1);
	ck_assert_int_eq(add_str(NULL, "abcd"), 1);
	ck_assert_int_eq(add_str(&buff, "ab\0cd"), 1);

        unsigned int cnt = 100000;
        for(unsigned int i = 0; i < cnt ; i++){
                ck_assert_int_eq(add_str(&buff, "abcd"), 0);
		ck_assert_int_ge(buff.length, buff.index);
                if(i == 4)ck_assert_str_eq(buff.buffer, "abcdabcdabcdabcdabcd");
        }

        ck_assert_int_eq(buff.index, cnt*4);
        ck_assert_int_eq(buff.index, strlen(buff.buffer));
        free_buffer(&buff);

}
END_TEST


Suite * dynbuff_suite(void)
{
	Suite *s;
	TCase *tc_init, *tc_add_c, *tc_add_str;
	s = suite_create("DynamicBuffer");
      
	tc_init = tcase_create("Init");
	tcase_add_test(tc_init, test_init);
	suite_add_tcase(s, tc_init);

	tc_add_c = tcase_create("Add_C");
	tcase_add_test(tc_add_c, test_add_c);
	suite_add_tcase(s, tc_add_c);

	tc_add_str = tcase_create("Add_STR");
	tcase_add_test(tc_add_str, test_add_str);
	suite_add_tcase(s, tc_add_str);	

	return s;
}

int main(void)
 {
	int number_failed;
	Suite *s;
	SRunner *sr;
	s = dynbuff_suite();
	sr = srunner_create(s);
	srunner_set_log (sr, "dynbuff_test.log");
	srunner_run_all(sr, CK_SUBUNIT);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
 }

