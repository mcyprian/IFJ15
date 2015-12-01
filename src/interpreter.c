/**
 * @file interpret.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 * 
 * @section DESCRIPTION
 *  
 **/

#include <datatypes.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>
#include <instructions.h>

extern int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Resources *resources, TInstruction *instruction);

int run_program(Resources * resources){

	debug_print("%s\n", "INIT_INTERPRET");
	args_assert(resources != NULL, INTERNAL_ERROR);
	int iRet = RETURN_OK;
    register int instruction_ret;

	if (resources->start_main == ZERO_INDEX){
		iRet = RUNTIME_ERROR;
		goto DEFAULT;
	}

	resources->ip = resources->start_main;
	TInstruction * instruction;
	do {
        dereference_structure(&(resources->instruction_buffer), resources->ip, (void**)&instruction);
        debug_print("%s: %lu, %s: %d\n", "IP", resources->ip, "INSTRUCTION", instruction->ins);
    
        instruction_ret = execute_instruction[instruction->ins](resources, instruction);
    
        debug_print("%s: %d\n", "INSTRUCT RET", instruction_ret);
        resources->ip++;

#if DEBUG
        for (index_t i = 1; i < 10; i++) {
                if (i == resources->runtime_stack.next_free - 1 && i == resources->bp) 
                    debug_print("%s: %d %lf %s\n", "CONTENT", access(resources->runtime_stack.buffer, TStack_variable, i )->value.i, 
                                                              access(resources->runtime_stack.buffer, TStack_variable, i )->value.d, "<-- STACK TOP  <-- BP");
                else if (i == resources->runtime_stack.next_free -1) 
                    debug_print("%s: %d %lf %s\n", "CONTENT", access(resources->runtime_stack.buffer, TStack_variable, i )->value.i,
                                                              access(resources->runtime_stack.buffer, TStack_variable, i )->value.d, "<-- STACK TOP");
                else if (i == resources->bp)
                    debug_print("%s: %d %lf %s\n", "CONTENT", access(resources->runtime_stack.buffer, TStack_variable, i )->value.i,
                                                              access(resources->runtime_stack.buffer, TStack_variable, i )->value.d, "<-- BP");
                else
                    debug_print("%s: %d %lf \n", "CONTENT", access(resources->runtime_stack.buffer, TStack_variable, i)->value.i,
                                                            access(resources->runtime_stack.buffer, TStack_variable, i )->value.d);
        }
#endif
    // TODO catch UNINIT_ERROR, DIV_ZERO_ERROR
    } while (instruction_ret == RETURN_OK);

    iRet = (instruction_ret == HALT ? RETURN_OK : instruction_ret);
	

DEFAULT:
	debug_print("%s %d\n", "INTERPRET_RETURN", iRet);
	return iRet;
}
