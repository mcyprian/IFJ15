/**
 * @file instructions.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <dynamic_structure_buffer.h>

#define access(buffer, type, index)           \
    ((type*)buffer + (index))

typedef struct {
    index_t dest;
    index_t first_op;
    index_t second_op;
    int ins;
} TInstruction;

#define NUM_OF_INSTRUCTIONS 5

enum instructions
{
    MOV,      // 0
    ADD_DATA, // 1
    ADD_REG,  // 2
    MUL_DATA, // 3
    HALT      // 4
};

typedef struct {
    TDynamic_structure_buffer reg_buff;
    TDynamic_structure_buffer data_buff;
    TDynamic_structure_buffer instruction_buff;
} Buffers;

typedef union {
    int i;
    double d;
    index_t index;
} TReg;

int new_instruction(TDynamic_structure_buffer *buff, TInstruction *item, 
                index_t dest, index_t first, index_t second, int ins);

int mov(Buffers * buffers, TInstruction *instruction);

int add_data(Buffers * buffers, TInstruction *instruction);

int add_reg(Buffers * buffers, TInstruction *instruction);

int mul_data(Buffers * buffers, TInstruction *instruction);

int halt(Buffers * buffers, TInstruction *instruction);
#endif // !INSTRUCTIONS_H
