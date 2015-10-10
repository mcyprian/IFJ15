#include <stdio.h>
#include <io.h>

int main(int argc, char ** argv){

	debug_print("%s\n", "INIT");
	int iRet = RETURN_OK;
	
	TDynamic_buffer buff;

	if((iRet = init_buffer(&buff, 1000)) != RETURN_OK)
		goto DEFAULT;

	if(argc != 2){
		fprintf(stderr, "%s:%d Incorrect number of arguments\n", __func__, __LINE__);
		iRet = INTERNAL_ERROR;
		goto DEFAULT;
	}

	FILE * f = NULL;
	if((f = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s:%d Cannot open a file: %20s\n", __func__, __LINE__,  argv[1]);
		iRet = INTERNAL_ERROR;
		goto DEFAULT;
	}

	if((iRet = read_from_file(f, &buff)) != RETURN_OK)
		goto CLOSE_FILE;

CLOSE_FILE:
	fclose(f);
	
DEFAULT:
	free_buffer(&buff);
	debug_print("%s: %d\n", "RETURN", iRet );
	return iRet;
}
