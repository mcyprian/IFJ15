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

typedef union {
    int i;
    double d;
    index_t index;
} TReg;

typedef struct {
    TReg dest;
    TReg first_op;
    TReg second_op;
    int ins;
} TInstruction;

#define NUM_OF_INSTRUCTIONS 6

enum instructions
{
    MOV,       // 0
    ADD_DATA,  // 1
    ADD_REG,   // 2
    ADD_CONST, // 3
    MUL_DATA,  // 4
    HALT       // 5
};

enum instruction_type {
         //  dest   first_op  second_op
    BBB, // buffer   buffer    buffer
    BBC  // buffer   buffer    const
};

typedef struct {
    TDynamic_structure_buffer reg_buff;
    TDynamic_structure_buffer data_buff;
    TDynamic_structure_buffer instruction_buff;
} Buffers;


int new_instruction(TDynamic_structure_buffer *buff, TInstruction *item, 
                index_t dest, index_t first, index_t second, 
                int ins, int type);

int mov(Buffers * buffers, TInstruction *instruction);

int add_data(Buffers * buffers, TInstruction *instruction);

int add_reg(Buffers * buffers, TInstruction *instruction);

int add_const(Buffers *buffers, TInstruction *instruction);

int mul_data(Buffers * buffers, TInstruction *instruction);

int halt(Buffers * buffers, TInstruction *instruction);
#endif // !INSTRUCTIONS_H
