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
                    index_t dest, index_t first, index_t second, 
                    int ins, int type) {
    index_t index = ZERO_INDEX;
    new_item(buff, index, item);
    item->dest.index = dest;
    item->first_op.index = first;
    item->ins = ins;

    switch (type) {
        case BBB:
            item->second_op.index = second;
            break;
        case BBC:
            item->second_op.i = second;
            break;
        default:
            return INTERNAL_ERROR;

    }
    return RETURN_OK;
}


int mov(Buffers *buffers, TInstruction *instruction)  {
    debug_print("%s\n", "MOV");
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = *access(buffers->data_buff.buffer, int, instruction->first_op.index);
    return 1;
}

// reg int_buff int_buff
int add_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DATA");
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = *access(buffers->data_buff.buffer, int, instruction->first_op.index)
    + *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    return 1;
}

// reg reg int_buff
int add_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", *access(buffers->data_buff.buffer, int, instruction->second_op.index));
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i );
    return 1;
}

// reg reg const
int add_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);

    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int mul_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DATA");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", *access(buffers->data_buff.buffer, int, instruction->second_op.index));
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = *access(buffers->data_buff.buffer, int, instruction->first_op.index)
    * *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int halt(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "HALT");
    fprintf(stderr, "%s: %lu %d \n", "INTERPRETER TERMINATING", instruction->dest.index, *(int*)buffers->reg_buff.buffer);
    return -1;
}
