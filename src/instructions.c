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
    item->first_operand = first;
    item->second_operand = second;
    item->dest = dest;
    item->ins = ins;
    return RETURN_OK;
}


int mov(Buffers *buffers, index_t dest, index_t first_op, index_t second_op) {
    debug_print("%s\n", "MOV");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, dest);
    
    reg_dest->i = *access(buffers->data_buff.buffer, int, first_op);
    return 1;
}

int add_data(Buffers *buffers, index_t dest, index_t first_op, index_t second_op) {
    debug_print("%s\n", "ADD_DATA");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, dest);
    
    reg_dest->i = *access(buffers->data_buff.buffer, int, first_op)
                + *access(buffers->data_buff.buffer, int, second_op);
    return 1;
}

int add_reg(Buffers *buffers, index_t dest, index_t first_op, index_t second_op) {
    debug_print("%s\n", "ADD_REG");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, dest);
    
    printf("OP1 CONTENT: %d\n", access(buffers->reg_buff.buffer, TReg, first_op)->i);
    printf("OP2 CONTENT: %d\n", *access(buffers->data_buff.buffer, int, second_op));
    reg_dest->i = access(buffers->reg_buff.buffer, TReg, first_op)->i
                + *access(buffers->data_buff.buffer, int, second_op);
    printf("REGISTER CONTENT: %d\n", reg_dest->i);
    return 1;
}

int mul_data(Buffers *buffers, index_t dest, index_t first_op, index_t second_op) {
    debug_print("%s\n", "MUL_DATA");
    TReg *reg_dest = access(buffers->reg_buff.buffer, TReg, dest);
    
    printf("OP1 CONTENT: %d\n", *access(buffers->data_buff.buffer, int, first_op));
    printf("OP2 CONTENT: %d\n", *access(buffers->data_buff.buffer, int, second_op));
    reg_dest->i = *access(buffers->data_buff.buffer, int, first_op)
                * *access(buffers->data_buff.buffer, int, second_op);
    printf("REGISTER CONTENT: %d\n", reg_dest->i);
    return 1;
}

int halt(Buffers *buffers, index_t dest, index_t first_op, index_t second_op) {
    debug_print("%s\n", "HALT");
    return -1;
}
