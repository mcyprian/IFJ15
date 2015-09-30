// htab_lookup.c
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#include "htable.h"

htab_listitem * create_htab_listitem(const char * key){

	htab_listitem * new = NULL;

	if ((new = (htab_listitem *)malloc(sizeof(htab_listitem))) == NULL){
		perror("Malloc - htab_listitem");
		return NULL;
	}

	if ((new->key = (char *)malloc((size_t)MAX_WORD_LENGTH)) == NULL){
		perror("Malloc - htab_listitem->key");
		free(new);
		return NULL;
	}
	
	new->data = 0;
	new->next = NULL;

	strncpy(new->key, key, MAX_WORD_LENGTH);

	return new;
}



htab_listitem * htab_lookup(htab_t * tab, const char * key){

	int exist = 0;
	unsigned hash = hash_function(key, tab->htab_size);
	htab_listitem * item = tab->array[hash];

	if (item == NULL){
		if ((item = create_htab_listitem(key)) == NULL)
			return NULL;
		else {
			tab->array[hash] = item;
			return item;
		}
	}

	htab_listitem * iter = item;
	while(iter != NULL){
		if (strcmp(iter->key, key) == 0){	
			exist = 1;
			break;
		}
		item = iter;
		iter = iter->next;
	}
	
	if (exist)return item;

	htab_listitem * new = NULL;
	if ((new = create_htab_listitem(key)) == NULL)
		return NULL;

	item->next = new;
	new->next = NULL;

	return new;
}