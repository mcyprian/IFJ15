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
#if DEBUG
    const char* instrucion_symbols[64];
    instrucion_symbols[0] = "EQ_INT_MEM_MEM";
    instrucion_symbols[1] = "EQ_DBL_MEM_MEM";
    instrucion_symbols[2] = "G_INT_MEM_MEM";
    instrucion_symbols[3] = "G_DBL_MEM_MEM";
    instrucion_symbols[4] = "L_INT_MEM_MEM";
    instrucion_symbols[5] = "L_DBL_MEM_MEM";
    instrucion_symbols[6] = "GE_INT_MEM_MEM";
    instrucion_symbols[7] = "GE_DBL_MEM_MEM";
    instrucion_symbols[8] = "LE_INT_MEM_MEM";
    instrucion_symbols[9] = "LE_DBL_MEM_MEM";
    instrucion_symbols[10] = "NE_INT_MEM_MEM";
    instrucion_symbols[11] = "NE_DBL_MEM_MEM";
    instrucion_symbols[12] = "ADD_INT_MEM_MEM";
    instrucion_symbols[13] = "ADD_DBL_MEM_MEM";
    instrucion_symbols[14] = "SUB_INT_MEM_MEM";
    instrucion_symbols[15] = "SUB_DBL_MEM_MEM";
    instrucion_symbols[16] = "MUL_INT_MEM_MEM";
    instrucion_symbols[17] = "MUL_DBL_MEM_MEM";
    instrucion_symbols[18] = "DIV_INT_MEM_MEM";
    instrucion_symbols[19] = "DIV_DBL_MEM_MEM";
    instrucion_symbols[20] = "EQ_STR_MEM_MEM";
    instrucion_symbols[21] = "G_STR_MEM_MEM";
    instrucion_symbols[22] = "L_STR_MEM_MEM";
    instrucion_symbols[23] = "GE_STR_MEM_MEM";
    instrucion_symbols[24] = "LE_STR_MEM_MEM";
    instrucion_symbols[25] = "NE_STR_MEM_MEM";
    instrucion_symbols[26] = "MOV_TOP_MEM";
    instrucion_symbols[27] = "MOV_INT_MEM";
    instrucion_symbols[28] = "MOV_INT_CONST";
    instrucion_symbols[29] = "MOV_DBL_MEM";
    instrucion_symbols[30] = "MOV_DBL_CONST";
    instrucion_symbols[31] = "CAST_INT_MEM";
    instrucion_symbols[32] = "CAST_DBL_MEM";
    instrucion_symbols[33] = "PUSH_EMPTY";
    instrucion_symbols[34] = "PUSH_INT_CONST";
    instrucion_symbols[35] = "PUSH_DBL_CONST";
    instrucion_symbols[36] = "PUSH_INDEX_CONST";
    instrucion_symbols[37] = "PUSH_INT_MEM";
    instrucion_symbols[38] = "PUSH_DBL_MEM";
    instrucion_symbols[39] = "PUSH_INDEX_MEM";
    instrucion_symbols[40] = "POP_EMPTY";
    instrucion_symbols[41] = "JMP_MEM";
    instrucion_symbols[42] = "JMP_TRUE_MEM";
    instrucion_symbols[43] = "JMP_FALSE_MEM";
    instrucion_symbols[44] = "FCE_CALL";
    instrucion_symbols[45] = "FCE_RETURN";
    instrucion_symbols[46] = "CIN_INT";
    instrucion_symbols[47] = "CIN_DOUBLE";
    instrucion_symbols[48] = "CIN_STRING";
    instrucion_symbols[49] = "CONCAT_MEM_MEM";
    instrucion_symbols[50] = "SUBSTR_MEM_MEM";
    instrucion_symbols[51] = "LENGTH_MEM";
    instrucion_symbols[52] = "FIND_MEM_MEM";
    instrucion_symbols[53] = "SORT_MEM";  
    instrucion_symbols[54] = "COUT_MEM_TYPE"; 
    instrucion_symbols[55] = "NO_RETURN";
    instrucion_symbols[56] = "SET TYPE";
    instrucion_symbols[57] = "HALT";

	debug_print("%s\n", "CREATING VISUALIZATION OF INSTRUCTION BUFFER");
    FILE *fp;
    int ins_num;
    if ((fp = fopen("instruction_buffer_content", "w")) == NULL) {
        debug_print("%s\n", "Failed to open instruction buffer debug file");
    } else {
        fprintf(fp, "INDEX    INSTRUCTION\n");
        for(index_t tmp_ip = 1 ; tmp_ip < resources->instruction_buffer.next_free ; tmp_ip++){
			ins_num = access(resources->instruction_buffer.buffer, TInstruction, tmp_ip)->ins;
            fprintf(fp, " %lu      %s\n", tmp_ip, instrucion_symbols[ins_num]);
        }
        
        fclose(fp);
    }
#endif

	debug_print("%s\n", "INIT_INTERPRET");
	args_assert(resources != NULL, INTERNAL_ERROR);
	int iRet = RETURN_OK;
    register int instruction_ret;

	if (resources->start_main == ZERO_INDEX){
		iRet = RUNTIME_ERROR;
		goto DEFAULT;
	}

	resources->ip = 1;
	access(resources->instruction_buffer.buffer, TInstruction, 1)->dest.index = 2;
	access(resources->instruction_buffer.buffer, TInstruction, 2)->dest.index = resources->start_main;

	debug_print("%s :%lu\n", "START POINT", resources->start_main);

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
    } while (instruction_ret == RETURN_OK);

    iRet = (instruction_ret == HALT ? RETURN_OK : instruction_ret);
	

DEFAULT:
	debug_print("%s %d\n", "INTERPRET_RETURN", iRet);
	return iRet;
}
