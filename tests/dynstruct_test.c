#include <stdio.h>
#include <error_macros.h>
#include <dynamic_structure_buffer.h>
#include <check.h>
#include <datatypes.h>

typedef struct {
    int data;
    long next;
} TItem;

#define COUNT 100000

START_TEST (test_init)
{
	TDynamic_structure_buffer b; 
	ck_assert_int_eq(init_structure_buffer(NULL, 0, -5), INTERNAL_ERROR);
	ck_assert_int_eq(init_structure_buffer(NULL, 10, 4), INTERNAL_ERROR);
	ck_assert_int_eq(init_structure_buffer(&b, 0, 4), INTERNAL_ERROR);
	ck_assert_int_eq(init_structure_buffer(&b, -5, 4), INTERNAL_ERROR);
	ck_assert_int_eq(init_structure_buffer(&b, 10, -5), INTERNAL_ERROR);
	ck_assert_int_eq(init_structure_buffer(NULL, 0, -5), INTERNAL_ERROR);
}
END_TEST

START_TEST (normal_use_test)
{
	TDynamic_structure_buffer b;
	ck_assert_int_eq(init_structure_buffer(&b, 1, sizeof(TItem)), RETURN_OK);
	index_t index = 0;
	index_t items[COUNT];
	index_t datas[COUNT];
	TItem *item = NULL;

	for(index_t i = 0 ; i < COUNT ; i++){
		ck_assert_int_eq(get_free_element_index(&b, &index), RETURN_OK);
		ck_assert_int_eq(dereference_structure(&b, index, (void **)&item), RETURN_OK);
		item->data = i*100;
		items[i] = index;
		datas[i] = i*100;
	}
    
	for (index_t i = 0 ; i < COUNT ; i++) {
		ck_assert_int_eq(dereference_structure(&b, items[i], (void **)&item), RETURN_OK);
		ck_assert_int_eq(datas[i], item->data);
	}
	free_structure_buffer(&b);
}
END_TEST

Suite * dynbuff_suite(void)
{
	Suite *s;
	TCase *tc_init, *tc_normal;
	s = suite_create("DynamicStructureBuffer");
			  
	tc_init = tcase_create("Init");
	tcase_add_test(tc_init, test_init);
	suite_add_tcase(s, tc_init);
	
	tc_normal = tcase_create("NormalUse");
	tcase_add_test(tc_normal, normal_use_test);
	suite_add_tcase(s, tc_normal);

	return s;
}

int main () 
{	
	int number_failed;
	Suite *s;
	SRunner *sr;
	s = dynbuff_suite();
	sr = srunner_create(s);
	srunner_set_log (sr, "dynstruct_test.log");
	srunner_run_all(sr, CK_SUBUNIT);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
