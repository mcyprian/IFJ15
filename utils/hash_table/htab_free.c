#include "htable.h"


void htab_free(htab_t * tab){

	htab_clear(tab);

	free(tab);

}