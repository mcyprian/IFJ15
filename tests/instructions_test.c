#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>

#define COUNT 100000

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

    int (*execute_instruction[NUM_OF_INSTRUCTIONS])(Buffers *buffers, TInstruction *instruction);

    debug_print("%s\n", "FILLING ARRAY OF FUNCTIONS");
    execute_instruction[MOV_INT_REG] = mov_int_reg;
    execute_instruction[MOV_INT_CONST] = add_int_reg_const;
    execute_instruction[MOV_DBL_REG] = mov_dbl_reg;
    execute_instruction[MOV_DBL_CONST] = mov_dbl_const;
    execute_instruction[ADD_INT_REG_REG] = add_int_reg_reg;
    execute_instruction[ADD_INT_REG_CONST] = add_int_reg_const;
    execute_instruction[ADD_INT_CONST_CONST] = add_int_const_const;
    execute_instruction[ADD_DBL_REG_REG] = add_dbl_reg_reg;
    execute_instruction[ADD_DBL_REG_CONST] = add_dbl_reg_const;
    execute_instruction[ADD_DBL_CONST_CONST] = add_dbl_const_const;
    execute_instruction[SUB_INT_REG_REG] = sub_int_reg_reg;
    execute_instruction[SUB_INT_REG_CONST] = sub_int_reg_const;
    execute_instruction[SUB_INT_CONST_CONST] = sub_int_const_const;
    execute_instruction[SUB_DBL_REG_REG] = sub_dbl_reg_reg;
    execute_instruction[SUB_DBL_REG_CONST] = sub_dbl_reg_const;
    execute_instruction[SUB_DBL_CONST_CONST] = sub_dbl_const_const;
    execute_instruction[MUL_INT_REG_REG] = mul_int_reg_reg;
    execute_instruction[MUL_INT_REG_CONST] = mul_int_reg_const;
    execute_instruction[MUL_INT_CONST_CONST] = mul_int_const_const;
    execute_instruction[MUL_DBL_REG_REG] = mul_dbl_reg_reg;
    execute_instruction[MUL_DBL_REG_CONST] = mul_dbl_reg_const;
    execute_instruction[MUL_DBL_CONST_CONST] = mul_dbl_const_const;
    execute_instruction[DIV_INT_REG_REG] = div_int_reg_reg;
    execute_instruction[DIV_INT_REG_CONST] = div_int_reg_const;
    execute_instruction[DIV_INT_CONST_CONST] = div_int_const_const;
    execute_instruction[DIV_DBL_REG_REG] = div_dbl_reg_reg;
    execute_instruction[DIV_DBL_REG_CONST] = div_dbl_reg_const;
    execute_instruction[DIV_DBL_CONST_CONST] = div_dbl_const_const;
 
    execute_instruction[HALT] = halt;
/*
    debug_print("%s\n", "FILLING INT BUFFER");
    // set int values in data_buff
    *((int*)buffers.data_buff.buffer + 1) = 3;
    *((int*)buffers.data_buff.buffer + 2) = 2;
    *((int*)buffers.data_buff.buffer + 3) = 5;
*/
    debug_print("%s\n", "GENERATING INSTRUCTIONS");
    for (int i = 0; i < COUNT; i++) {
        // int ADD
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(33), ADD_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(2), MUL_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), int_type(3), ADD_INT_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), int_type(9), ADD_INT_REG_CONST);
        // double ADD
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.3), double_type(33.2), ADD_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.0), double_type(2.0), MUL_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), double_type(3.0), ADD_DBL_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), double_type(9.0), ADD_DBL_REG_CONST);
     
        // int SUB
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(33), SUB_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(2), MUL_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), int_type(3), SUB_INT_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), int_type(9), SUB_INT_REG_CONST);
        // double SUB
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.3), double_type(33.2), SUB_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.0), double_type(2.0), MUL_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), double_type(3.0), SUB_DBL_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), double_type(9.0), SUB_DBL_REG_CONST);
     
        // int MUL
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(33), MUL_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(2), MUL_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), int_type(3), MUL_INT_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), int_type(9), MUL_INT_REG_CONST);
        // double MUL
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.3), double_type(33.2), MUL_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.0), double_type(2.0), MUL_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), double_type(3.0), MUL_DBL_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), double_type(9.0), MUL_DBL_REG_CONST);
    
        // int DIV
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(33), DIV_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), int_type(5), int_type(2), DIV_INT_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), int_type(3), DIV_INT_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), int_type(9), DIV_INT_REG_CONST);
        // double DIV
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.3), double_type(33.2), DIV_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), double_type(5.0), double_type(2.0), DIV_DBL_CONST_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), index_t_type(1lu), double_type(3.0), DIV_DBL_REG_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(1lu), index_t_type(2lu), double_type(9.0), DIV_DBL_REG_CONST);
     
        // MOV 
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), int_type(3), int_type(0), MOV_INT_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(2lu), double_type(3.0), double_type(0), MOV_DBL_CONST);
        new_instruction(&buffers.instruction_buff, instruction, index_t_type(0lu), index_t_type(0lu), index_t_type(0lu), HALT);
    }
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
