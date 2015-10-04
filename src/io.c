#include <io.h>

int read_from_file(FILE * f, TDynamic_buffer * buff){
	args_assert(f != NULL && buff != NULL && buff->buffer != NULL, INTERNAL_ERROR);

	char c = 0;
	while((c = fgetc(f)) != EOF)
		if(add_char(buff, c))
			return INTERNAL_ERROR;
	return RETURN_OK;
}
