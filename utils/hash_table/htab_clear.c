// htab_clear.c
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#include "htable.h"


void delete_list(htab_listitem * start){

	htab_listitem * current = start;
	htab_listitem * next = start;
	while(current != NULL){
		next = current->next;
		delete_htab_listitem(current);
		current = next;
	}
}

void htab_clear(htab_t * tab){

	for (unsigned i = 0; i < tab->htab_size; i++){
		if (tab->array[i] != NULL){
			delete_list(tab->array[i]);
		}
	}

}