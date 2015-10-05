#include <stdio.h>
#include <io.h>

int main(int argc, char ** argv){

	TDynamic_buffer buff;
	int iRet = RETURN_OK;

	if((iRet = init_buffer(&buff, 1000)) != RETURN_OK)
		goto DEFAULT;

	if(argc != 2){
		fprintf(stderr, "Incorrect number of arguments\n");
		iRet = INTERNAL_ERROR;
		goto DEFAULT;
	}

	FILE * f = NULL;
	if((f = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "Cannot open a file: %20s\n", argv[1]);
		iRet = INTERNAL_ERROR;
		goto DEFAULT;
	}

	if((iRet = read_from_file(f, &buff)) != RETURN_OK)
		goto DEFAULT;
	
DEFAULT:
	fclose(f);
	free_buffer(&buff);
	return iRet;
}