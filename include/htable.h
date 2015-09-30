// htable.h
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#ifndef HTABLE_H
#define HTABLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define MAX_WORD_LENGTH 127

struct item{
	char * key;
	long long int data;
	struct item * next;
};

typedef struct item htab_listitem;

typedef struct table{
	unsigned htab_size;
	htab_listitem * array[];
}htab_t;

unsigned int hash_function(const char *str, unsigned htab_size);

htab_t * htab_init(unsigned size);
void htab_clear(htab_t * tab);
void htab_remove(htab_t * tab, const char * key);
void htab_foreach(htab_t * tab, void (*f)(htab_listitem * item));
htab_listitem * htab_lookup(htab_t * tab, const char * key);
void htab_statistics(htab_t * tab);


static inline void delete_htab_listitem(htab_listitem * item){
	free(item->key);
	free(item);
}

static inline void htab_free(htab_t * tab){
	htab_clear(tab);
	free(tab);
}

#endif