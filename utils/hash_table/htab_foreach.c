// htab_foreach.c
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#include "htable.h"


void htab_foreach(htab_t * tab, void (*f)(htab_listitem * item)){

	htab_listitem * current = NULL;

	for (unsigned i = 0; i < tab->htab_size; i++){
		if (tab->array[i] != NULL){
			current = tab->array[i];
			while(current != NULL){
				f(current);
				current = current->next;
			}
		}
	}

}