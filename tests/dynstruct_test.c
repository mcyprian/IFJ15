#include <stdio.h>
#include <error_macros.h>
#include <dynamic_structure_buffer.h>
#include <check.h>
#include <datatypes.h>
#include <math.h>

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

START_TEST (realloc_test)
{
    TDynamic_structure_buffer b;
    init_structure_buffer(&b, 1, sizeof(TItem));
    
    for (unsigned i = 0; i < sizeof(TItem); i++)
        ck_assert_int_eq(*((char*)(b.buffer) + i), 0);
    ck_assert_int_eq(b.length, 2);
    ck_assert_int_eq(realloc_structure_buffer(&b), RETURN_OK);
    
    for (unsigned i = 0; i < 2 * sizeof(TItem); i++)
        ck_assert_int_eq(*((char*)(b.buffer) + i), 0);
    
    for (int i = 3; i <= 5; i++) {
        ck_assert_int_eq(realloc_structure_buffer(&b), RETURN_OK);
        ck_assert_int_eq(b.length, pow(2, i));
    }
    for (int i = 0; i < 32; i++)
        ck_assert_int_eq(b.flags[i], 0);

    ck_assert_int_eq(realloc_structure_buffer(NULL), INTERNAL_ERROR);
    free_structure_buffer(&b);
}
END_TEST

START_TEST (get_elem_test)
{
    TDynamic_structure_buffer b;
    index_t index;
    index_t length;

    init_structure_buffer(&b, COUNT, sizeof(TItem));
    length = b.length;
 
    ck_assert_int_eq(get_free_element_index(NULL, &index), INTERNAL_ERROR);
    ck_assert_int_eq(get_free_element_index(&b, 0), INTERNAL_ERROR);

    for (int i = 0; i < COUNT; i++) {
        ck_assert_int_eq(b.flags[i + 1], 0);
        ck_assert_int_eq(get_free_element_index(&b, &index), RETURN_OK);
        ck_assert_int_eq(b.flags[index], 1);
        ck_assert_int_eq(index, i + 1);
    }
    ck_assert_int_eq(length, b.length);
    free_structure_buffer(&b);
}
END_TEST

START_TEST (free_elem_test)
{
    TDynamic_structure_buffer b;
    index_t index;

    ck_assert_int_eq(free_element(&b, index), INTERNAL_ERROR);
    init_structure_buffer(&b, COUNT, sizeof(TItem));
    ck_assert_int_eq(free_element(&b, index), INTERNAL_ERROR);

    for (int i = 0; i < COUNT; i++) {
        get_free_element_index(&b, &index);
        ck_assert_int_eq(b.flags[index], 1); 
        ck_assert_int_eq(free_element(&b, index), RETURN_OK);
        ck_assert_int_eq(b.flags[index], 0); 
    }
    index++;
    ck_assert_int_eq(free_element(&b, index), RETURN_OK);
}
END_TEST


Suite * dynbuff_suite(void)
{
	Suite *s;
	TCase *tc_init, *tc_normal, *tc_realloc, *tc_get_elem, *tc_free_elem;
	s = suite_create("DynamicStructureBuffer");
			  
	tc_init = tcase_create("Init");
	tcase_add_test(tc_init, test_init);
	suite_add_tcase(s, tc_init);
	
	tc_normal = tcase_create("NormalUse");
	tcase_add_test(tc_normal, normal_use_test);
	suite_add_tcase(s, tc_normal);

	tc_realloc = tcase_create("Realloc");
	tcase_add_test(tc_realloc, realloc_test);
	suite_add_tcase(s, tc_realloc);

	tc_get_elem = tcase_create("GetElem");
	tcase_add_test(tc_get_elem, get_elem_test);
	suite_add_tcase(s, tc_get_elem);
	
	tc_free_elem = tcase_create("FreeElem");
	tcase_add_test(tc_free_elem, free_elem_test);
	suite_add_tcase(s, tc_free_elem);
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
