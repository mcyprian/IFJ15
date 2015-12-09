/**
 * @file built_functions.c
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Built functions in IFJ2015 language (length, substr, concat)
 */

#include <built_functions.h>

int length(TDynamic_buffer *b, index_t index)
{
	return (int)strlen(load_token(b, index));
}

index_t substr(TDynamic_buffer *b, index_t index, index_t i, int n)
{
    printf("\n");
    printf("hladam v stringu na indexe %ld, od pozicie %ld, %d znakov\n", index, i, n);
    printf("Vypis buffer: ");
    for (unsigned j = 0; j < b->writing_index; j++)
        putchar(b->buffer[j]);
    
    printf("\nkoniec vypisu \n");

	save_token(b);	// whatever is in b, puts '\0' after that

	char *check_string = load_token(b, index);
    printf("Toto je string v ktorom hladam: %s\n", load_token(b, index));
	int lenght = (int)strlen(check_string);
	if (i >= (unsigned)lenght)
		return 0;

	char *new_string = load_token(b, index + i);
    printf("toto je string od noveho indexu i (%ld - index + i): %s\n",index+i, new_string);
	index_t temporary_w_index = b->writing_index;
	if (add_str_index(b, index + i) == INTERNAL_ERROR)
		return 0;

	printf("Toto je novy index, kde je rewrite string: %ld \n", temporary_w_index);
	int new_string_length = (int)strlen(load_token(b, index + i));
	char *rewrite_string = load_token(b, temporary_w_index);
    printf("v tomto retazci chceme prepisovat: %s \n", rewrite_string);
	if (n < new_string_length)
	{
		printf("SUBSTR: som v IFE\n");
		rewrite_string[n] = '\0';
	}
	index_t return_w_index = b->writing_index;
	if (add_str_index(b, temporary_w_index) == INTERNAL_ERROR)
		return 0;

	printf("toto je return index: %ld \n", return_w_index);
	printf("toto je posledny writing index bufferu: %ld \n", b->writing_index);
	//index_t return_index = save_token(b);
    printf("toto som ulozil na koniec bufferu, nas novy substr: %s\n", load_token(b, return_w_index));
    printf("\n\n\nnovy vypis bufferu: ");
    for (unsigned i = 0; i < b->writing_index; i++)
        putchar(b->buffer[i]);
    
    putchar('\n');
    printf("koniec noveho vypisu\n\n");
	//return return_w_index;
	return temporary_w_index;
}

index_t concat(TDynamic_buffer *b, index_t index1, index_t index2)
{
	//char *string1 = load_token(b, index1);
	//char *string2 = load_token(b, index2);

	save_token(b);	// whatever is in b, puts '\0' after that

	if (add_str_index(b, index1) == INTERNAL_ERROR)
		return 0;

	if (add_str_index(b, index2) == INTERNAL_ERROR)
		return 0;

	index_t index = save_token(b);

	printf("\nconcat string: %s\n", load_token(b, index));
	return index;
}
