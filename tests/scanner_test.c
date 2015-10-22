#include <stdio.h>
#include <debug.h>
#include <error_macros.h>
#include <scanner.h>
#include <token.h>

int main(int argc, char ** argv){

	debug_print("%s\n", "INIT");
	int iRet = RETURN_OK;
	

	TDynamic_structure_buffer struct_buff;

	TDynamic_buffer test_buff;

    index_t index;
    TToken *token;

	if((iRet = init_buffer(&test_buff, 1000)) != RETURN_OK)
		goto DEFAULT;

	if ((iRet = init_structure_buffer(&struct_buff, 5, sizeof(TToken))) != RETURN_OK)
		goto TEST_BUFF;

	if(argc != 2){
		fprintf(stderr, "%s:%d Incorrect number of arguments\n", __func__, __LINE__);
		iRet = INTERNAL_ERROR;
		goto STRUCT_BUFF;
	}

	FILE * f = NULL;
	if((f = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s:%d Cannot open a file: %20s\n", __func__, __LINE__,  argv[1]);
		iRet = INTERNAL_ERROR;
		goto STRUCT_BUFF;
	}

	do {
		index = get_token(f, &test_buff, &struct_buff);
        if (dereference_structure(&struct_buff, index, (void**)&token) == INTERNAL_ERROR) {
            iRet = INTERNAL_ERROR;
            goto STRUCT_BUFF;
        }
        printf("%s %d\n", load_token(&test_buff, token->token_index), token->token_type);

	} while(token->token_type != EOFT);
	fclose(f);

STRUCT_BUFF:
	free_structure_buffer(&struct_buff);

TEST_BUFF:
	free_buffer(&test_buff);

DEFAULT:
	debug_print("%s: %d\n", "RETURN", iRet );
	return iRet;
}
