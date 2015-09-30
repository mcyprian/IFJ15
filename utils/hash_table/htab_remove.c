// htab_remove.c
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#include "htable.h"


void htab_remove(htab_t * tab, const char * key){

	int exist = 0;
	unsigned hash = hash_function(key, tab->htab_size);
	htab_listitem * item = tab->array[hash];
	htab_listitem * before = NULL;

	if (item == NULL){
		//fprintf(stderr, "Key - %s %s\n", key, "Doesn't exist");
		return;
	}

	while(item != NULL){
		if (strcmp(item->key, key) == 0){	
			exist = 1;
			break;
		}
		
		before = item;
		item = item->next;

	}

	if (exist){
		if (before == NULL)tab->array[hash] = item->next;
		else before->next = item->next;
		delete_htab_listitem(item);
	}
	//else 
		//fprintf(stderr, "Key - %s %s\n", key, "Doesn't exist");
}