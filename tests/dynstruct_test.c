#include <stdio.h>
#include <error_macros.h>
#include <dynamic_structure_buffer.h>
#include <check.h>

typedef struct {
    int data;
    long next;
} TItem;

#define COUNT 10000

int main() {
    
	TDynamic_structure_buffer b;
	init_structure_buffer(&b, 1, sizeof(TItem));
	unsigned long index = 0;
	unsigned long items[COUNT];
	unsigned long datas[COUNT];
	TItem *item = NULL;

	for(unsigned long i = 0 ; i < COUNT ; i++){
		get_free_element_index(&b, &index);
		dereference_structure(&b, index, &item);
		item->data = i*100;
		items[i] = index;
		datas[i] = i*100;
	}
    
	for (unsigned long i = 0 ; i < COUNT ; i++) {
		dereference_structure(&b, items[i], &item);
	}
	free_structure_buffer(&b);
	return 0;
}

