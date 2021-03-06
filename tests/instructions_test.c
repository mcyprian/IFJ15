#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>
#include <instruction_generator.h>
#include <ial.h>
#include <built_functions.h>

#define COUNT 5000

extern int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Resources *resources, TInstruction *instruction);

int main() {

    int iRet = RETURN_OK;
    int instruction_ret = 0;
    Resources resources;
    resources.ip = 9;
    resources.bp = 0;

    if ((iRet = init_structure_buffer(&(resources.runtime_stack), 16, sizeof(TStack_variable))) != RETURN_OK)
        goto DEFAULT;

    if ((iRet = init_buffer(&(resources.string_buff), 25)) != RETURN_OK)
        goto MEM_BUFF;

    if ((iRet = init_structure_buffer(&(resources.instruction_buffer), 128, sizeof(TInstruction))) != RETURN_OK)
        goto STR_BUFF;

    index_t first_string = 1;
    add_str(&resources.string_buff, "dabfec");
    add_char(&resources.string_buff, '\0');

    index_t second_string = ++resources.string_buff.writing_index;
    add_str(&resources.string_buff, "ec");
    printf("first %s second %s\n", load_token(&resources.string_buff, first_string), load_token(&resources.string_buff, second_string));

    debug_print("%s\n", "GENERATING INSTRUCTIONS");
        // int ADD
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 5, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, ADD_INT_MEM_MEM);
//        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
//       new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double ADD
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 5.28, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, ADD_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
/*        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int SUB
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 5, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, SUB_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double SUB
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 5.28, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, SUB_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
    
        // int MUL
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 5, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, MUL_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double MUL
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 5.28, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, MUL_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int DIV
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 5, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, DIV_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double DIV
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 5.28, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, DIV_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int EQ
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, EQ_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double EQ
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.15, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, EQ_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int G
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, G_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double G
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.15, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, G_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int L
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, L_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double L
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.15, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, L_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int GE
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, GE_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double GE
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.15, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, GE_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int LE
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, LE_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double LE
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.15, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, LE_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // int NE
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 0, 0, PUSH_INT_CONST);
        new_instruction_empty(&resources.instruction_buffer, NE_INT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);

        // double NE
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.14, 0, PUSH_DBL_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.15, 0, PUSH_DBL_CONST);
        new_instruction_empty(&resources.instruction_buffer, NE_DBL_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, POP_EMPTY);
*/
        new_instruction_int_int(&resources.instruction_buffer, 0lu, 0, 0lu, FCE_RETURN);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 1, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 2, 0, PUSH_INT_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 4lu, 0lu, 0lu, MOV_TOP_MEM);
        new_instruction_int_int(&resources.instruction_buffer, 0lu, L_INT, 0, SET_TYPE);
        new_instruction_empty(&resources.instruction_buffer,  COUT_MEM_TYPE);
        
	// substr
	new_instruction_mem_mem(&resources.instruction_buffer, 0lu, first_string, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 0lu, 1lu, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 0lu, 3, 0lu, PUSH_INT_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 4lu, 3lu, 2lu, SUBSTR_MEM_MEM);
	new_instruction_empty(&resources.instruction_buffer, COUT_MEM_TYPE);

	//concat
	new_instruction_mem_mem(&resources.instruction_buffer, 0lu, first_string, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 0lu, second_string, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 4lu, 3lu, 2lu, CONCAT_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, COUT_MEM_TYPE);
        
	// length
	new_instruction_mem_mem(&resources.instruction_buffer, 0lu, second_string, 0lu, PUSH_INDEX_CONST);
	new_instruction_mem_mem(&resources.instruction_buffer, 4lu, 3lu, 2lu, LENGTH_MEM);
	new_instruction_empty(&resources.instruction_buffer, COUT_MEM_TYPE);

	// find
	new_instruction_mem_mem(&resources.instruction_buffer, 0lu, first_string, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 0lu, second_string, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 4lu, 3lu, 2lu, FIND_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, COUT_MEM_TYPE);

	// sort
	new_instruction_mem_mem(&resources.instruction_buffer, 0lu, first_string, 0lu, PUSH_INDEX_CONST);
        new_instruction_mem_mem(&resources.instruction_buffer, 4lu, 3lu, 2lu, SORT_MEM);
        new_instruction_empty(&resources.instruction_buffer, COUT_MEM_TYPE);
	

        new_instruction_mem_mem(&resources.instruction_buffer, 0lu, 0lu, 0lu, HALT);
//       new_instruction_empty(&resources.instruction_buffer, EQ_STR_MEM_MEM);
//        new_instruction_empty(&resources.instruction_buffer, G_STR_MEM_MEM);
//        new_instruction_empty(&resources.instruction_buffer, L_STR_MEM_MEM);
        new_instruction_empty(&resources.instruction_buffer, GE_STR_MEM_MEM);
//        new_instruction_empty(&resources.instruction_buffer, LE_STR_MEM_MEM);
//        new_instruction_empty(&resources.instruction_buffer, NE_STR_MEM_MEM);
        new_instruction_mem_mem(&resources.instruction_buffer, 1lu, 0lu, 0lu, FCE_CALL);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 5, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 6, 0, PUSH_INT_CONST);
      
        new_instruction_mem_mem(&resources.instruction_buffer, 0lu, 0lu, 0lu, HALT);
    
    debug_print("%s\n", "INTERPRETING");

    TInstruction *instruction;
    do {
        dereference_structure(&resources.instruction_buffer, resources.ip, (void**)&instruction);
        debug_print("%s: %lu, %s: %d\n", "IP", resources.ip, "INSTRUCTION", instruction->ins);
        instruction_ret = execute_instruction[instruction->ins](&resources, instruction);
        if (instruction_ret == UNINIT_ERROR)
            printf("UNINITIALIZED VALUE\n");

        resources.ip++;
        
        // debug_print("%s: %d\n", "STACK_TOP", access(resources.runtime_stack.buffer, TStack_variable, (resources.runtime_stack.next_free - 1) + resources.bp)->value.i);
        for (index_t i = 1; i < 10; i++) {
            if (i == resources.runtime_stack.next_free - 1 && i == resources.bp) 
                debug_print("%s: %d %s\n", "CONTENT", access(resources.runtime_stack.buffer, TStack_variable, i )->value.i, "<-- STACK TOP  <-- BP");
            else if (i == resources.runtime_stack.next_free -1)
                debug_print("%s: %d %s\n", "CONTENT", access(resources.runtime_stack.buffer, TStack_variable, i )->value.i, "<-- STACK TOP");
            else if (i == resources.bp)
                debug_print("%s: %d %s\n", "CONTENT", access(resources.runtime_stack.buffer, TStack_variable, i )->value.i, "<-- BP");
            else
                debug_print("%s: %d\n", "CONTENT", access(resources.runtime_stack.buffer, TStack_variable, i)->value.i);
        }
        
        
        debug_print("%s: %lu\n", "STACK_NUM", resources.runtime_stack.next_free -1);

    } while (instruction_ret != HALT);

    debug_print("%s\n", "INTERPRETING FINISHED");
    printf("\n");
    for (unsigned i = 1; i < resources.string_buff.writing_index; i++)
        putchar(resources.string_buff.buffer[i]);
    printf("\n");

    free_structure_buffer(&(resources.instruction_buffer));

STR_BUFF:
    free_buffer(&(resources.string_buff));

MEM_BUFF:
    free_structure_buffer(&(resources.runtime_stack));

DEFAULT:
    debug_print("%s: %d\n", "RETURN", iRet);
    return iRet;
}
