/**
 * @file built_functions.c
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Built functions in IFJ2015 language (length, substr, concat)
 */

#include <built_functions.h>

int lenght(TDynamic_buffer *b, index_t index)
{
	return (int)strlen(load_token(b, index));
}

char *substr(TDynamic_buffer *b, index_t index, index_t i, int n)
{	
	char *check_string = load_token(b, index);
	int lenght = (int)strlen(check_string);
	if (i >= (unsigned)lenght)
		return NULL;

	char *string = load_token(b, index + i);
	
	if (n < (int)strlen(string))
		string[n] = '\0';

	return string;
}