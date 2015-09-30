// htab_statistics.c
// Řešení IJC-DU2, příklad b), 26.4.2015
// Autor: Radovan Sroka, FIT
// Přeloženo: gcc 4.8.2

#include "htable.h"

void htab_statistics(htab_t * tab){

	htab_listitem * current = NULL;

	double sum = 0;

	unsigned long long min = ULLONG_MAX;
	unsigned long long max = 0;

	unsigned null_counter = 0;

	for (unsigned i = 0; i < tab->htab_size; i++){
		if (tab->array[i] != NULL){
			current = tab->array[i];
			unsigned long long counter = 0;
			while(current != NULL){
				counter++;
				current = current->next;
			}
			if (counter < min)min = counter;
			if (counter > max)max = counter;

			sum += (double)counter;
		}
		else
			null_counter++;
	}	

	printf("sum = %lf, average = %lf, minimum = %lld, max = %lld, null = %u\n",sum, sum / tab->htab_size, min, max, null_counter);

}