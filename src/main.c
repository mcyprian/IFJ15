/**
 * @file main.c
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Module main.c
 */


#include <stdio.h>
#include <debug.h>
#include <error_macros.h>
#include <scanner.h>
#include <datatypes.h>
#include <resources.h>
#include <syntax_analysis.h>
#include <built_functions.h>

int main(int argc, char ** argv){

	debug_print("%s\n", "INIT");
	int iRet = RETURN_OK;

	Resources resources;

	if((iRet = init_buffer(&(resources.string_buff), 1024)) != RETURN_OK)
		goto DEFAULT;

	if ((iRet = init_structure_buffer(&(resources.struct_buff), 256, sizeof(TToken))) != RETURN_OK)
		goto STRING_BUFF;

	if(argc != 2){
		fprintf(stderr, "%s:%d Incorrect number of arguments\n", __func__, __LINE__);
		iRet = INTERNAL_ERROR;
		goto STRUCT_BUFF;
	}

	resources.source = NULL;
	if((resources.source = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s:%d Cannot open a file: %20s\n", __func__, __LINE__,  argv[1]);
		iRet = INTERNAL_ERROR;
		goto STRUCT_BUFF;
	}

	iRet = check_syntax(PROGRAM, &resources);

	fclose(resources.source);

STRUCT_BUFF:
	free_structure_buffer(&(resources.struct_buff));

STRING_BUFF:
	free_buffer(&(resources.string_buff));

DEFAULT:
	debug_print("%s: %d\n", "RETURN", iRet );
	return iRet;
}
