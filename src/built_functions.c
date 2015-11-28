/**
 * @file built_functions.c
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Built functions in IFJ2015 language (length, substr, concat)
 */

#include <built_functions.h>
#include <ial.h>

int length(TDynamic_buffer *b, index_t index)
{
	return (int)strlen(load_token(b, index));
}

index_t substr(TDynamic_buffer *b, index_t index, index_t i, int n)
{
	save_token(b);	// whatever is in b, puts '\0' after that
	char *check_string = load_token(b, index);
	//printf("%s\n", check_string);
	int lenght = str_length(check_string);
	if (i >= (unsigned)lenght)
		return 0;

	char *string = load_token(b, index + i);
	
	if (n < str_length(string))
		string[n] = '\0';

	if (add_str(b, string) == INTERNAL_ERROR)
		return 0;

	index_t return_index = save_token(b);

	return return_index;
}

index_t concat(TDynamic_buffer *b, index_t index1, index_t index2)
{
	char *string1 = load_token(b, index1);
	char *string2 = load_token(b, index2);

	save_token(b);	// whatever is in b, puts '\0' after that

	if (add_str(b, string1) == INTERNAL_ERROR)
		return 0;

	if (add_str(b, string2) == INTERNAL_ERROR)
		return 0;

	index_t index = save_token(b);

	return index;
}
