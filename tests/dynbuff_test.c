#include <stdio.h>
#include <stdlib.h>
#include <error_macros.h>
#include <dynamic_buffer.h>
#include <check.h>

#include <limits.h>

START_TEST (test_init)
{
	TDynamic_buffer buff;
	ck_assert_int_eq(init_buffer(&buff, 0), INTERNAL_ERROR);
	ck_assert_int_eq(init_buffer(NULL, 100), INTERNAL_ERROR);
	ck_assert_int_eq(init_buffer(NULL, -10000), INTERNAL_ERROR);
	
	ck_assert_int_eq(init_buffer(&buff, 100), 0);
	free_buffer(&buff);
	
}
END_TEST


START_TEST (test_add_c)
{
        TDynamic_buffer buff;
        ck_assert_int_eq(init_buffer(&buff, 1), 0);

	ck_assert_int_eq(add_char(NULL, 'a'), INTERNAL_ERROR);
        ck_assert_int_eq(add_char(&buff, '\0'), INTERNAL_ERROR);
	ck_assert_int_eq(add_char(NULL, '\0'), INTERNAL_ERROR);
	
        
	unsigned int cnt = 100000;
	for(unsigned int i = 0; i < cnt ; i++){
		ck_assert_int_eq(add_char(&buff, 'a'), 0);
		ck_assert_int_ge(buff.length, buff.writing_index);
		if(i == 5)ck_assert_str_eq(buff.buffer, "aaaaaa");
	}

	ck_assert_int_eq(buff.writing_index, cnt);
	ck_assert_int_eq(buff.writing_index, strlen(buff.buffer));
	free_buffer(&buff);

}
END_TEST


START_TEST (test_add_str)
{
        TDynamic_buffer buff;
        ck_assert_int_eq(init_buffer(&buff, 1), 0);

	ck_assert_int_eq(add_str(NULL, "abcd"), INTERNAL_ERROR);
	ck_assert_int_eq(add_str(&buff, NULL), INTERNAL_ERROR);
	ck_assert_int_eq(add_str(NULL, "abcd"), INTERNAL_ERROR);
	ck_assert_int_eq(add_str(&buff, "ab\0cd"), 0);

        unsigned int cnt = 100000;
        for(unsigned int i = 0; i < cnt ; i++){
                ck_assert_int_eq(add_str(&buff, "abcd"), 0);
		ck_assert_int_ge(buff.length, buff.writing_index);
                if(i == 4)ck_assert_str_eq(buff.buffer, "ababcdabcdabcdabcdabcd");
        }

        ck_assert_int_eq(buff.writing_index, cnt*4+2);
        ck_assert_int_eq(buff.writing_index, strlen(buff.buffer));
        free_buffer(&buff);

	ck_assert_int_eq(init_buffer(&buff, 1), 0);
	ck_assert_int_eq(add_str(&buff, "abcdefgh"), 0);
	ck_assert_int_eq(buff.length, 10);
	free_buffer(&buff);


}
END_TEST

START_TEST (test_empty)
 {
	TDynamic_buffer buff;
	ck_assert_int_eq(init_buffer(&buff, 1), 0); 
	ck_assert_int_eq(add_str(&buff, "12345678"), 0);
	
	ck_assert_int_eq(empty_buffer(NULL), INTERNAL_ERROR);
	ck_assert_int_eq(empty_buffer(&buff), 0);
	ck_assert_str_eq(buff.buffer, "");

	free_buffer(&buff);
}
END_TEST

START_TEST (test_read) //consult 
{
	TDynamic_buffer buff;
	ck_assert_int_eq(init_buffer(&buff, 1), 0);
	ck_assert_int_eq(add_str(&buff, "abcdefgh"), 0);

	ck_assert_ptr_eq(read_buffer(NULL), NULL);
	ck_assert_str_eq(read_buffer(&buff), buff.buffer);
	
	ck_assert_ptr_eq(get_str(NULL, 1), NULL);
	ck_assert_ptr_eq(get_str(&buff, -1), NULL);
	ck_assert_ptr_eq(get_str(NULL, -1), NULL);
	ck_assert_ptr_eq(get_str(&buff, 1), &buff.buffer[0]);
	ck_assert_ptr_eq(get_str(&buff, 0), NULL);
	ck_assert_ptr_eq(get_str(&buff, 2), &buff.buffer[1]);
	ck_assert_str_eq(get_str(&buff, 1), &buff.buffer[3]);

	free_buffer(&buff);	
}
END_TEST

START_TEST (test_token)
{
	TDynamic_buffer buff;
	ck_assert_int_eq(init_buffer(&buff, 1), 0);

	char * ptr = NULL;

	for(int i = 0 ; i < 10 ; i++)
		ck_assert_int_eq(add_char(&buff, 'a'), 0);	
	ck_assert_ptr_eq(ptr = get_token(&buff, save_token(&buff)), read_buffer(&buff));

	for(int i = 0 ; i < 10 ; i++)
		ck_assert_int_eq(add_char(&buff, 'b'), 0);
	ck_assert_int_eq(*get_token(&buff, save_token(&buff)), 'b');

	ck_assert_str_eq(get_str(&buff, 11), "aaaaaaaaaa");
	ck_assert_str_eq(get_str(&buff, 11), "bbbbbbbbbb");

	free_buffer(&buff);
}
END_TEST

Suite * dynbuff_suite(void)
{
	Suite *s;
	TCase *tc_init, *tc_add_c, *tc_add_str, *tc_empty, *tc_read, *tc_save;
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

	tc_empty = tcase_create("EMPTY");
	tcase_add_test(tc_empty, test_empty);
	suite_add_tcase(s, tc_empty);

	tc_read = tcase_create("READ");
	tcase_add_test(tc_read, test_read);
	suite_add_tcase(s, tc_read);

	tc_save = tcase_create("SAVE");
	tcase_add_test(tc_save, test_token);
	suite_add_tcase(s, tc_save);

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

