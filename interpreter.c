/**
 * @file interpret.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 * 
 * @section DESCRIPTION
 *  
 **/

#include "datatypes.h"
#include "dynamic_structure_buffer.h"
#include "error_macros.h"
#include "debug.h"
#include "instructions.h"

extern int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Resources *resources, TInstruction *instruction);

int run_program(Resources * resources){

	debug_print("%s\n", "INIT_INTERPRET");
	args_assert(resources != NULL, INTERNAL_ERROR);
	int iRet = RETURN_OK;

	if (resources->start_main == ZERO_INDEX){
		iRet = RUNTIME_ERROR;
		goto DEFAULT;
	}

	resources->ip = resources->start_main;
	TInstruction * instruction;
	register int instruction_ret;
	do {
        dereference_structure(&(resources->instruction_buffer), resources->ip, (void**)&instruction);
        debug_print("%s: %lu, %s: %d\n", "IP", resources->ip, "INSTRUCTION", instruction->ins);
    
        instruction_ret = execute_instruction[instruction->ins](resources, instruction);
    
        debug_print("%s: %d\n", "INSTRUCT RET", instruction_ret);
        resources->ip++;
    } while (instruction_ret != -1);
	

DEFAULT:
	debug_print("%s %d\n", "INTERPRET_RETURN", iRet);
	return iRet;
}
