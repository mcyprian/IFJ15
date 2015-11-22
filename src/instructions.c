/**
 * @file instrucrions.c
 * @author Michal Cyprian
 *
 * @section DESCRIPTION
 *
 * 
 * C function for each of instructions to execute it
 */

#include <instructions.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>

int new_instruction(TDynamic_structure_buffer *buff, TInstruction *item, 
                    index_t dest, index_t first, index_t second, int ins) {
    index_t index = ZERO_INDEX;
    new_item(buff, index, item);
    item->first_op = first;
    item->second_op = second;
    item->dest = dest;
    item->ins = ins;
    return RETURN_OK;
}


int mov(Buffers *buffers, TInstruction *instruction)  {
    debug_print("%s\n", "MOV");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, instruction->dest);
    
    reg_dest->i = *access(buffers->data_buff.buffer, int, instruction->first_op);
    return 1;
}

int add_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DATA");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, instruction->dest);
    
    reg_dest->i = *access(buffers->data_buff.buffer, int, instruction->first_op)
                + *access(buffers->data_buff.buffer, int, instruction->second_op);
    return 1;
}

int add_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_REG");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, instruction->dest);
    
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", *access(buffers->data_buff.buffer, int, instruction->second_op));
    reg_dest->i = access(buffers->reg_buff.buffer, TReg, instruction->first_op)->i
                + *access(buffers->data_buff.buffer, int, instruction->second_op);
    debug_print("%s: %d\n", "REGISTER CONTENT", reg_dest->i);
    return 1;
}

int mul_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DATA");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, instruction->dest);
    
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", *access(buffers->data_buff.buffer, int, instruction->second_op));
    reg_dest->i = *access(buffers->data_buff.buffer, int, instruction->first_op)
                * *access(buffers->data_buff.buffer, int, instruction->second_op);
    debug_print("%s: %d\n", "REGISTER CONTENT", reg_dest->i);
    return 1;
}

int halt(Buffers *buffers, TInstruction *instruction) {
    // without efect only to remove warnings
    debug_print("%s\n", "HALT");
    fprintf(stderr, "%s: %lu %d \n", "INTERPRETER TERMINATING", instruction->dest, *(int*)buffers->reg_buff.buffer);
    return -1;
}
