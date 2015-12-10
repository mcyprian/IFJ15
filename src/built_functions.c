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
    for (unsigned j = 0; j < b->writing_index; j++)
        putchar(b->buffer[j]);

	save_token(b);	// whatever is in b, puts '\0' after that

	char *check_string = load_token(b, index);
	int lenght = (int)strlen(check_string);
	if (i >= (unsigned)lenght)
		return 0;

	char *new_string = load_token(b, index + i);
	index_t temporary_w_index = b->writing_index;
	if (add_str_index(b, index + i) == INTERNAL_ERROR)
		return 0;

	int new_string_length = (int)strlen(load_token(b, index + i));
	char *rewrite_string = load_token(b, temporary_w_index);
	if (n < new_string_length)
		rewrite_string[n] = '\0';

	index_t return_w_index = b->writing_index;
	if (add_str_index(b, temporary_w_index) == INTERNAL_ERROR)
		return 0;

	//index_t return_index = save_token(b);
    for (unsigned i = 0; i < b->writing_index; i++)
        putchar(b->buffer[i]);
    
    putchar('\n');
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

	return index;
}
