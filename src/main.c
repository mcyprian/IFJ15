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
#include <instructions.h>

#define ALLOC_SIZE 1024

int main(int argc, char ** argv){

	debug_print("%s\n", "INIT");
	int iRet = RETURN_OK;

	Resources resources;
	resources.start_main = ZERO_INDEX;
	resources.bp = 1;
	
	if(argc != 2){
		fprintf(stderr, "%s:%d Incorrect number of arguments\n", __func__, __LINE__);
		iRet = INTERNAL_ERROR;
 		goto DEFAULT;
 	}


	if((iRet = init_buffer(&(resources.string_buff), ALLOC_SIZE)) != RETURN_OK)
		goto DEFAULT;

	if ((iRet = init_structure_buffer(&(resources.struct_buff), ALLOC_SIZE, sizeof(TToken))) != RETURN_OK)
		goto STRING_BUFF;

	if ((iRet = init_structure_buffer(&(resources.struct_buff_trees), ALLOC_SIZE, sizeof(TTree))) != RETURN_OK)
		goto STRUCT_BUFF;

	if ((iRet = init_structure_buffer(&(resources.runtime_stack), ALLOC_SIZE, sizeof(TStack_variable))) != RETURN_OK)
		goto TREE_BUFF;

	if ((iRet = init_structure_buffer(&(resources.instruction_buffer), ALLOC_SIZE, sizeof(TInstruction))) != RETURN_OK)
		goto RUN_STACK;

	if ((iRet = init_stack(&(resources.stack))) != RETURN_OK)
		goto INS_BUFF;

	resources.source = NULL;
	if((resources.source = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "%s:%d Cannot open a file: %20s\n", __func__, __LINE__,  argv[1]);
		iRet = INTERNAL_ERROR;
		goto INS_BUFF;
	}

	if ((iRet = new_instruction_mem_mem(&(resources.instruction_buffer), 0, 0, 0, FCE_CALL)) != 0)goto FREE;
	if ((iRet = new_instruction_mem_mem(&(resources.instruction_buffer), 0, 0, 0, JMP_MEM )) != 0)goto FREE;
	if ((iRet = new_instruction_empty(&(resources.instruction_buffer), HALT)) != 0)goto FREE;
	if ((iRet = check_syntax(GLOBAL, &resources)) != 0)goto FREE;
	if ((iRet = new_instruction_empty(&(resources.instruction_buffer), HALT)) != 0)goto FREE;
	if ((iRet = run_program(&resources)) != 0)goto FREE;

FREE:
	fclose(resources.source);

INS_BUFF:
	free_structure_buffer(&(resources.instruction_buffer));

RUN_STACK:
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
