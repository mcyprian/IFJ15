#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>

#define COUNT 5000

int main() {

    int iRet = RETURN_OK;
    int instruction_ret = 0;
    Buffers buffers;
    index_t ip = 1;
    TInstruction *instruction;

    if ((iRet = init_structure_buffer(&(buffers.reg_buff), 16, sizeof(TReg))) != RETURN_OK)
        goto DEFAULT;

    if ((iRet = init_structure_buffer(&(buffers.data_buff), 128, sizeof(int))) != RETURN_OK)
        goto REG_BUFF;

    if ((iRet = init_structure_buffer(&(buffers.instruction_buff), 128, sizeof(TInstruction))) != RETURN_OK)
        goto DATA_BUFF;

    debug_print("%s\n", "FILLING ARRAY OF FUNCTIONS");
    int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Buffers *buffers, TInstruction *instruction) = { 
        mov_int_reg,
        add_int_reg_const,
        mov_dbl_reg,
        mov_dbl_const,
        add_int_reg_reg,
        add_int_reg_const,
        add_int_const_const,
        add_dbl_reg_reg,
        add_dbl_reg_const,
        add_dbl_const_const,
        sub_int_reg_reg,
        sub_int_reg_const,
        sub_int_const_const,
        sub_dbl_reg_reg,
        sub_dbl_reg_const,
        sub_dbl_const_const,
        mul_int_reg_reg,
        mul_int_reg_const,
        mul_int_const_const,
        mul_dbl_reg_reg,
        mul_dbl_reg_const,
        mul_dbl_const_const,
        div_int_reg_reg,
        div_int_reg_const,
        div_int_const_const,
        div_dbl_reg_reg,
        div_dbl_reg_const,
        div_dbl_const_const,
        eq_int_reg_reg,
        eq_int_reg_const,
        eq_int_const_const,
        eq_dbl_reg_reg,
        eq_dbl_reg_const,
        eq_dbl_const_const,
        g_int_reg_reg,
        g_int_reg_const,
        g_int_const_const,
        g_dbl_reg_reg,
        g_dbl_reg_const,
        g_dbl_const_const,
        l_int_reg_reg,
        l_int_reg_const,
        l_int_const_const,
        l_dbl_reg_reg,
        l_dbl_reg_const,
        l_dbl_const_const,
        ge_int_reg_reg,
        ge_int_reg_const,
        ge_int_const_const,
        ge_dbl_reg_reg,
        ge_dbl_reg_const,
        ge_dbl_const_const,
        le_int_reg_reg,
        le_int_reg_const,
        le_int_const_const,
        le_dbl_reg_reg,
        le_dbl_reg_const,
        le_dbl_const_const,
        ne_int_reg_reg,
        ne_int_reg_const,
        ne_int_const_const,
        ne_dbl_reg_reg,
        ne_dbl_reg_const,
        ne_dbl_const_const,
        halt
    };
    debug_print("%s\n", "GENERATING INSTRUCTIONS");
   
    
    for (int i = 0; i < COUNT; i++) {
        // int ADD
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(33), ADD_INT_CONST_CONST);
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 2, MUL_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 3, ADD_INT_REG_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 1lu, 2lu, 9, ADD_INT_REG_CONST);
        // double ADD
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.3, 33.2, ADD_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.0, 2.0, MUL_DBL_CONST_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 3.0, ADD_DBL_REG_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 1lu, 2lu, 9.0, ADD_DBL_REG_CONST);
     
        // int SUB
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 33, SUB_INT_CONST_CONST);
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 2, MUL_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 3, SUB_INT_REG_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 1lu, 2lu, 9, SUB_INT_REG_CONST);
        // double SUB
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.3, 33.2, SUB_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.0, 2.0, MUL_DBL_CONST_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 3.0, SUB_DBL_REG_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 1lu, 2lu, 9.0, SUB_DBL_REG_CONST);
     
        // int MUL
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 33, MUL_INT_CONST_CONST);
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 2, MUL_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 3, MUL_INT_REG_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 1lu, 2lu, 9, MUL_INT_REG_CONST);
        // double MUL
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.3, 33.2, MUL_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.0, 2.0, MUL_DBL_CONST_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 3.0, MUL_DBL_REG_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 1lu, 2lu, 9.0, MUL_DBL_REG_CONST);
    
        // int DIV
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 33, DIV_INT_CONST_CONST);
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 5, 2, DIV_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 3, DIV_INT_REG_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 1lu, 2lu, 9, DIV_INT_REG_CONST);
        // double DIV
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.3, 33.2, DIV_DBL_CONST_CONST);
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 5.0, 2.0, DIV_DBL_CONST_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 3.0, DIV_DBL_REG_CONST);
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 1lu, 2lu, 9.0, DIV_DBL_REG_CONST);
     
        // MOV 
        new_instruction_int_int(&buffers.instruction_buff, instruction, 2lu, 3, 0, MOV_INT_CONST);
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 2lu, 3.0, 0, MOV_DBL_CONST);

        // EQUALS
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 0, 0, EQ_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 0, EQ_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 3.14, 3.14, EQ_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 0.0, EQ_DBL_CONST_CONST);
 
        // GREATER
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 0, 0, G_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 0, G_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 3.14, 3.15, G_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 0.0, G_DBL_CONST_CONST);
        
        // LESS
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 0, 0, L_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 0, L_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 3.14, 3.15, L_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 0.0, L_DBL_CONST_CONST);
        
        // GREATER OR GEUALS
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 0, 0, GE_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 0, GE_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 3.14, 3.15, GE_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 0.0, GE_DBL_CONST_CONST);
       
        // LESS OR LEUALS
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 0, 0, LE_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 0, LE_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 3.14, 3.15, LE_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 0.0, LE_DBL_CONST_CONST);
        
        // NOT NEUALS
        new_instruction_int_int(&buffers.instruction_buff, instruction, 1lu, 0, 0, NE_INT_CONST_CONST);
        new_instruction_reg_int(&buffers.instruction_buff, instruction, 2lu, 1lu, 0, NE_INT_CONST_CONST);
        
        new_instruction_dbl_dbl(&buffers.instruction_buff, instruction, 1lu, 3.14, 3.15, NE_DBL_CONST_CONST);
        
        new_instruction_reg_dbl(&buffers.instruction_buff, instruction, 2lu, 1lu, 0.0, NE_DBL_CONST_CONST);
    } 
        new_instruction_reg_reg(&buffers.instruction_buff, instruction, 0lu, 0lu, 0lu, HALT);
    
    debug_print("%s\n", "INTERPRETING");
    do {
        dereference_structure(&buffers.instruction_buff, ip, (void**)&instruction);
        ip++;
        debug_print("%s: %lu, %s: %d\n", "IP", ip, "INSTRUCTION", instruction->ins);
    
        instruction_ret = execute_instruction[instruction->ins](&buffers, instruction);
    
    
        debug_print("%s: %d\n", "REGISTER1", access(buffers.reg_buff.buffer, TReg, 1lu)->i);
        debug_print("%s: %d\n", "REGISTER2:", access(buffers.reg_buff.buffer, TReg, 2lu)->i);

        debug_print("%s: %d\n", "INSTRUCT RET", instruction_ret);
    } while (instruction_ret != -1);

    debug_print("%s\n", "INTERPRETING FINISHED");
    debug_print("%s: %d\n", "REGISTER1",  access(buffers.reg_buff.buffer, TReg, 1lu)->i);
    debug_print("%s: %d\n", "REGISTER2",  access(buffers.reg_buff.buffer, TReg, 2lu)->i);

    free_structure_buffer(&(buffers.instruction_buff));

DATA_BUFF:
    free_structure_buffer(&(buffers.data_buff));

REG_BUFF:
    free_structure_buffer(&(buffers.reg_buff));

DEFAULT:
    debug_print("%s: %d\n", "RETURN", iRet);
    return iRet;
}
