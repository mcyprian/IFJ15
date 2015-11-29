#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>
#include <instruction_generator.h>

#define COUNT 5000

extern int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Resources *resources, TInstruction *instruction);

int main() {

    int iRet = RETURN_OK;
    int instruction_ret = 0;
    Resources resources;
    resources.ip = 6;
    resources.bp = 0;

    if ((iRet = init_structure_buffer(&(resources.runtime_stack), 16, sizeof(TStack_variable))) != RETURN_OK)
        goto DEFAULT;


    if ((iRet = init_structure_buffer(&(resources.instruction_buffer), 128, sizeof(TInstruction))) != RETURN_OK)
        goto REG_BUFF;

   debug_print("%s\n", "GENERATING INSTRUCTIONS");
   
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 6, 0, PUSH_INT_CONST);
        new_instruction_reg_reg(&resources.instruction_buffer, 
                                resources.runtime_stack.next_free - 2,
                                resources.runtime_stack.next_free - 2,
                                resources.runtime_stack.next_free - 1,
                                ADD_INT_REG_REG);

        new_instruction_int_int(&resources.instruction_buffer, 0lu, 0, 0, POP_EMPTY);
        // int ADD
/*        new_instruction(&resources.instruction_buffer, index_t_type(1lu), int_type(5), int_type(33), ADD_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 3, ADD_INT_REG_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 1lu, 2lu, 9, ADD_INT_REG_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, PUSH_INT);
        // double ADD
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.3, 33.2, ADD_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.0, 2.0, MUL_DBL_CONST_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 3.0, ADD_DBL_REG_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 1lu, 2lu, 9.0, ADD_DBL_REG_CONST);
*/      
        new_instruction_reg_reg(&resources.instruction_buffer, 0lu, 0lu, 0lu, FCE_RETURN);
 /*    
        // int SUB
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 5, 33, SUB_INT_CONST_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 5, 2, MUL_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 3, SUB_INT_REG_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 1lu, 2lu, 9, SUB_INT_REG_CONST);
        // double SUB
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.3, 33.2, SUB_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.0, 2.0, MUL_DBL_CONST_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 3.0, SUB_DBL_REG_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 1lu, 2lu, 9.0, SUB_DBL_REG_CONST);
     
        // int MUL
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 5, 33, MUL_INT_CONST_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 5, 2, MUL_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 3, MUL_INT_REG_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 1lu, 2lu, 9, MUL_INT_REG_CONST);
        // double MUL
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.3, 33.2, MUL_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.0, 2.0, MUL_DBL_CONST_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 3.0, MUL_DBL_REG_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 1lu, 2lu, 9.0, MUL_DBL_REG_CONST);
    
        // int DIV
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 5, 33, DIV_INT_CONST_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 5, 2, DIV_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 3, DIV_INT_REG_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 1lu, 2lu, 9, DIV_INT_REG_CONST);
        // double DIV
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.3, 33.2, DIV_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 5.0, 2.0, DIV_DBL_CONST_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 3.0, DIV_DBL_REG_CONST);
        new_instruction_reg_dbl(&resources.instruction_buffer, 1lu, 2lu, 9.0, DIV_DBL_REG_CONST);
     
        // MOV 
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 3, 0, MOV_INT_CONST);
        new_instruction_dbl_dbl(&resources.instruction_buffer, 2lu, 3.0, 0, MOV_DBL_CONST);

        // EQUALS
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 0, 0, EQ_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 0, EQ_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 3.14, 3.14, EQ_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 0.0, EQ_DBL_CONST_CONST);
 
        // GREATER
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 0, 0, G_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 0, G_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 3.14, 3.15, G_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 0.0, G_DBL_CONST_CONST);
        
        // LESS
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 0, 0, L_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 0, L_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 3.14, 3.15, L_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 0.0, L_DBL_CONST_CONST);
        
        // GREATER OR EUALS
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 0, 0, GE_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 0, GE_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 3.14, 3.15, GE_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 0.0, GE_DBL_CONST_CONST);
       
        // LESS OR EUALS
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 0, 0, LE_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 0, LE_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 3.14, 3.15, LE_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 0.0, LE_DBL_CONST_CONST);
        
        // NOT EUALS
        new_instruction_int_int(&resources.instruction_buffer, 1lu, 0, 0, NE_INT_CONST_CONST);
        new_instruction_reg_int(&resources.instruction_buffer, 2lu, 1lu, 0, NE_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&resources.instruction_buffer, 1lu, 3.14, 3.15, NE_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&resources.instruction_buffer, 2lu, 1lu, 0.0, NE_DBL_CONST_CONST);
        // CAST
*/
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 1, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 2, 0, PUSH_INT_CONST);
        new_instruction_reg_reg(&resources.instruction_buffer, 1lu, 0lu, 0lu, FCE_CALL);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 5, 0, PUSH_INT_CONST);
        new_instruction_int_int(&resources.instruction_buffer, 2lu, 6, 0, PUSH_INT_CONST);
        
        
        new_instruction_reg_reg(&resources.instruction_buffer, 0lu, 0lu, 0lu, HALT);
    
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
  //      debug_print("%s: %d\n", "REGISTER1", access(resources.runtime_stack.buffer, TStack_variable, 1lu)->value.i);
  //      debug_print("%s: %d\n", "REGISTER2", access(resources.runtime_stack.buffer, TStack_variable, 2lu)->value.i);

    } while (instruction_ret != -1);

    debug_print("%s\n", "INTERPRETING FINISHED");

    free_structure_buffer(&(resources.instruction_buffer));


REG_BUFF:
    free_structure_buffer(&(resources.runtime_stack));

DEFAULT:
    debug_print("%s: %d\n", "RETURN", iRet);
    return iRet;
}
