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
#include <syntax_analysis.h>
#include <built_functions.h>
#include <ial.h>
#include <symbol_table.h>
#include <stack.h>
#include <datatypes.h>
#include <interpreter.h>
#include <resources.h>

int main(int argc, char ** argv){

	debug_print("%s\n", "INIT");
	int iRet = RETURN_OK;

	Resources resources;

	if(argc != 2){
		fprintf(stderr, "%s:%d Incorrect number of arguments\n", __func__, __LINE__);
		iRet = INTERNAL_ERROR;
 		goto DEFAULT;
 	}


	if((iRet = init_buffer(&(resources.string_buff), 1024)) != RETURN_OK)
		goto DEFAULT;

	if ((iRet = init_structure_buffer(&(resources.struct_buff), 256, sizeof(TToken))) != RETURN_OK)
		goto STRING_BUFF;

	if ((iRet = init_structure_buffer(&(resources.struct_buff_trees), 256, sizeof(TTree))) != RETURN_OK)
		goto STRUCT_BUFF;

	if ((iRet = init_structure_buffer(&(resources.runtime_stack), 256, sizeof(TStack_variable))) != RETURN_OK)
		goto TREE_BUFF;

	init_stack(&(resources.stack));

	resources.source = NULL;
	if((resources.source = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s:%d Cannot open a file: %20s\n", __func__, __LINE__,  argv[1]);
		iRet = INTERNAL_ERROR;
		goto STACK;
	}

	if ((iRet = check_syntax(GLOBAL, &resources)) != 0)goto FREE;
	if ((iRet = run_program(&resources)) != 0)goto FREE;

FREE:
	fclose(resources.source);

STACK:
	free_structure_buffer(&(resources.runtime_stack));

TREE_BUFF:
    free_structure_buffer(&(resources.struct_buff_trees));

STRUCT_BUFF:
	free_structure_buffer(&(resources.struct_buff));

STRING_BUFF:
	free_buffer(&(resources.string_buff));

DEFAULT:
	debug_print("%s: %d\n", "RETURN", iRet );
	return iRet;
}
