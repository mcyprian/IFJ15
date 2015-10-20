#include <stdio.h>
#include <debug.h>
#include <error_macros.h>
#include <scanner.h>

int main(int argc, char ** argv){

	debug_print("%s\n", "INIT");
	int iRet = RETURN_OK;
	

	TDynamic_structure_buffer struct_buff;

	TDynamic_buffer test_buff;

	if((iRet = init_buffer(&test_buff, 1000)) != RETURN_OK)
		goto DEFAULT;

	if ((iRet = init_structure_buffer(&struct_buff, 5, sizeof(TToken))) != RETURN_OK)
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

	for (int i = 0; i < 10; i++) 
	{
		get_token_(f, &test_buff, &struct_buff);
	}

	//tokenize(f, &test_buff, &struct_buff);

	fclose(f);
DEFAULT:
	free_buffer(&test_buff);
	debug_print("%s: %d\n", "RETURN", iRet );
	return iRet;
}
