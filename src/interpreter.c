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
    const char* instrucion_symbols[60];
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
    instrucion_symbols[10] = "ADD_INT_MEM_MEM";
    instrucion_symbols[11] = "ADD_DBL_MEM_MEM";
    instrucion_symbols[12] = "SUB_INT_MEM_MEM";
    instrucion_symbols[13] = "SUB_DBL_MEM_MEM";
    instrucion_symbols[14] = "MUL_INT_MEM_MEM";
    instrucion_symbols[15] = "MUL_DBL_MEM_MEM";
    instrucion_symbols[16] = "DIV_INT_MEM_MEM";
    instrucion_symbols[17] = "DIV_DBL_MEM_MEM";
    instrucion_symbols[18] = "NE_INT_MEM_MEM";
    instrucion_symbols[19] = "NE_DBL_MEM_MEM";
    instrucion_symbols[20] = "MOV_TOP_MEM";
    instrucion_symbols[21] = "MOV_INT_MEM";
    instrucion_symbols[22] = "MOV_INT_CONST";
    instrucion_symbols[23] = "MOV_DBL_MEM";
    instrucion_symbols[24] = "MOV_DBL_CONST";
    instrucion_symbols[25] = "CAST_INT_MEM";
    instrucion_symbols[26] = "CAST_DBL_MEM";
    instrucion_symbols[27] = "PUSH_EMPTY";
    instrucion_symbols[28] = "PUSH_INT_CONST";
    instrucion_symbols[29] = "PUSH_DBL_CONST";
    instrucion_symbols[30] = "PUSH_INDEX_CONST";
    instrucion_symbols[31] = "PUSH_INT_MEM";
    instrucion_symbols[32] = "PUSH_DBL_MEM";
    instrucion_symbols[33] = "PUSH_INDEX_MEM";
    instrucion_symbols[34] = "POP_EMPTY";
    instrucion_symbols[35] = "JMP_MEM";
    instrucion_symbols[36] = "JMP_TRUE_MEM";
    instrucion_symbols[37] = "JMP_FALSE_MEM";
    instrucion_symbols[38] = "FCE_CALL";
    instrucion_symbols[39] = "FCE_RETURN";
    instrucion_symbols[40] = "CIN_INT";
    instrucion_symbols[41] = "CIN_DOUBLE";
    instrucion_symbols[42] = "CIN_STRING";
    instrucion_symbols[43] = "CONCAT_MEM_MEM";
    instrucion_symbols[44] = "CONCAT_MEM_CONST";
    instrucion_symbols[45] = "CONCAT_CONST_CONST";
    instrucion_symbols[46] = "SUBSTR_MEM_MEM";
    instrucion_symbols[47] = "LENGTH_MEM";
    instrucion_symbols[48] = "LENGTH_CONST";
    instrucion_symbols[49] = "FIND_MEM_MEM";
    instrucion_symbols[50] = "FIND_MEM_CONST";
    instrucion_symbols[51] = "FIND_CONST_CONST"; 
    instrucion_symbols[52] = "SORT_MEM";  
    instrucion_symbols[53] = "SORT_CONST"; 
    instrucion_symbols[54] = "COUT_MEM_INT"; 
    instrucion_symbols[55] = "COUT_MEM_DBL";
    instrucion_symbols[56] = "COUT_MEM_STRING";
    instrucion_symbols[57] = "COUT_CONST_INT";
    instrucion_symbols[58] = "COUT_CONST_DBL";
    instrucion_symbols[59] = "COUT_CONST_STRING";
    instrucion_symbols[60] = "HALT";

	debug_print("%s\n", "CREATING VISUALIZATION OF INSTRUCTION BUFFER");
    FILE *fp;
    int tmp_ip = 1;
    int ins_num;
    if ((fp = fopen("instruction_buffer_content", "w")) == NULL) {
        debug_print("%s\n", "Failed to open instruction buffer debug file");
    } else {
        fprintf(fp, "INDEX    INSTRUCTION\n");
        while((ins_num = access(resources->instruction_buffer.buffer, TInstruction, tmp_ip)->ins) != HALT)
            fprintf(fp, " %d      %s\n", tmp_ip++, instrucion_symbols[ins_num]);
        
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
    } while (instruction_ret == RETURN_OK);

    iRet = (instruction_ret == HALT ? RETURN_OK : instruction_ret);
	

DEFAULT:
	debug_print("%s %d\n", "INTERPRET_RETURN", iRet);
	return iRet;
}
