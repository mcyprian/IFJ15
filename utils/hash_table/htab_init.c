// htab_init.c
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#include "htable.h"

htab_t * htab_init(unsigned size){

	htab_t * tab = NULL;

	if ((tab = (htab_t *)malloc(sizeof(htab_t) + sizeof(htab_listitem*) * size)) == NULL){
		perror("Malloc - hashtable");
		return NULL;
	}
	for (unsigned i = 0; i < size; i++)tab->array[i] = NULL;

	tab->htab_size = size;

	return tab;
}