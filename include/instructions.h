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

#define NUM_OF_INSTRUCTIONS 29

enum instructions
{
    MOV_INT_REG,           // 0
    MOV_INT_CONST,         // 1
    MOV_DBL_REG,           // 2
    MOV_DBL_CONST,         // 3
    ADD_INT_REG_REG,       // 4
    ADD_INT_REG_CONST,     // 5
    ADD_INT_CONST_CONST,   // 6
    ADD_DBL_REG_REG,       // 7
    ADD_DBL_REG_CONST,     // 8
    ADD_DBL_CONST_CONST,   // 9
    SUB_INT_REG_REG,       // 10
    SUB_INT_REG_CONST,     // 11
    SUB_INT_CONST_CONST,   // 12
    SUB_DBL_REG_REG,       // 13
    SUB_DBL_REG_CONST,     // 14
    SUB_DBL_CONST_CONST,   // 15
    MUL_INT_REG_REG,       // 16
    MUL_INT_REG_CONST,     // 17
    MUL_INT_CONST_CONST,   // 18
    MUL_DBL_REG_REG,       // 19
    MUL_DBL_REG_CONST,     // 20
    MUL_DBL_CONST_CONST,   // 21
    DIV_INT_REG_REG,       // 22
    DIV_INT_REG_CONST,     // 23
    DIV_INT_CONST_CONST,   // 24
    DIV_DBL_REG_REG,       // 25
    DIV_DBL_REG_CONST,     // 26
    DIV_DBL_CONST_CONST,   // 27
 
    HALT                   // 28
};

typedef struct {
    TDynamic_structure_buffer reg_buff;
    TDynamic_structure_buffer data_buff;   // TODO remove data_buff ?
    TDynamic_structure_buffer instruction_buff;
} Buffers;

TReg int_type (int i);
TReg double_type (double d);
TReg index_t_type (index_t index);

int new_instruction(TDynamic_structure_buffer *buff, TInstruction *item, 
                TReg dest, TReg first, TReg second, int ins);

int mov_int_reg(Buffers *buffers, TInstruction *instruction);

int mov_int_const(Buffers *buffers, TInstruction *instruction);

int mov_dbl_reg(Buffers *buffers, TInstruction *instruction);

int mov_dbl_const(Buffers *buffers, TInstruction *instruction);
/*
int add_int_data_data(Buffers *buffers, TInstruction *instruction);

int add_int_reg_data(Buffers *buffers, TInstruction *instruction);

int add_int_data_const(Buffers *buffers, TInstruction *instruction);
*/

int add_int_reg_reg(Buffers *buffers, TInstruction *instruction);

int add_int_reg_const(Buffers *buffers, TInstruction *instruction);

int add_int_const_const(Buffers *buffers, TInstruction *instruction);

int add_dbl_reg_reg(Buffers *buffers, TInstruction *instruction);

int add_dbl_reg_const(Buffers *buffers, TInstruction *instruction);

int add_dbl_const_const(Buffers *buffers, TInstruction *instruction);

int sub_int_reg_reg(Buffers *buffers, TInstruction *instruction);

int sub_int_reg_const(Buffers *buffers, TInstruction *instruction);

int sub_int_const_const(Buffers *buffers, TInstruction *instruction);

int sub_dbl_reg_reg(Buffers *buffers, TInstruction *instruction);

int sub_dbl_reg_const(Buffers *buffers, TInstruction *instruction);

int sub_dbl_const_const(Buffers *buffers, TInstruction *instruction);

int mul_int_reg_reg(Buffers *buffers, TInstruction *instruction);

int mul_int_reg_const(Buffers *buffers, TInstruction *instruction);

int mul_int_const_const(Buffers *buffers, TInstruction *instruction);

int mul_dbl_reg_reg(Buffers *buffers, TInstruction *instruction);

int mul_dbl_reg_const(Buffers *buffers, TInstruction *instruction);

int mul_dbl_const_const(Buffers *buffers, TInstruction *instruction);

int div_int_reg_reg(Buffers *buffers, TInstruction *instruction);

int div_int_reg_const(Buffers *buffers, TInstruction *instruction);

int div_int_const_const(Buffers *buffers, TInstruction *instruction);

int div_dbl_reg_reg(Buffers *buffers, TInstruction *instruction);

int div_dbl_reg_const(Buffers *buffers, TInstruction *instruction);

int div_dbl_const_const(Buffers *buffers, TInstruction *instruction);


int halt(Buffers * buffers, TInstruction *instruction);
#endif // !INSTRUCTIONS_H
