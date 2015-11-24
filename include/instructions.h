/**
 * @file instructions.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file including definitions of inline instruction functions
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <dynamic_structure_buffer.h>
#include <debug.h>

typedef union {    // TODO move to datatypes
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

#define NUM_OF_INSTRUCTIONS 65   // TODO set final number

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
    EQ_INT_REG_REG,        // 28
    EQ_INT_REG_CONST,      // 29
    EQ_INT_CONST_CONST,    // 30
    EQ_DBL_REG_REG,        // 31
    EQ_DBL_REG_CONST,      // 32
    EQ_DBL_CONST_CONST,    // 33
    G_INT_REG_REG,         // 34
    G_INT_REG_CONST,       // 35
    G_INT_CONST_CONST,     // 36
    G_DBL_REG_REG,         // 37
    G_DBL_REG_CONST,       // 38
    G_DBL_CONST_CONST,     // 39
    L_INT_REG_REG,         // 40
    L_INT_REG_CONST,       // 41
    L_INT_CONST_CONST,     // 42
    L_DBL_REG_REG,         // 43
    L_DBL_REG_CONST,       // 44
    L_DBL_CONST_CONST,     // 45
    GE_INT_REG_REG,        // 46
    GE_INT_REG_CONST,      // 47
    GE_INT_CONST_CONST,    // 48
    GE_DBL_REG_REG,        // 49
    GE_DBL_REG_CONST,      // 50
    GE_DBL_CONST_CONST,    // 51
    EG_INT_REG_REG,        // 52
    LE_INT_REG_CONST,      // 53
    LE_INT_CONST_CONST,    // 54
    LE_DBL_REG_REG,        // 55
    LE_DBL_REG_CONST,      // 56
    LE_DBL_CONST_CONST,    // 57
    NE_INT_REG_REG,        // 58
    NE_INT_REG_CONST,      // 59
    NE_INT_CONST_CONST,    // 60
    NE_DBL_REG_REG,        // 61
    NE_DBL_REG_CONST,      // 62
    NE_DBL_CONST_CONST,    // 63
 
    HALT                   // 64
};

typedef struct {
    TDynamic_structure_buffer reg_buff;
    TDynamic_structure_buffer data_buff;   // TODO remove data_buff ?
    TDynamic_structure_buffer instruction_buff;
} Buffers;

static inline TReg int_type (int i) {
        TReg r; r.i = i; return r;
}

static inline TReg double_type (double d) {
        TReg r; r.d = d; return r;
}

static inline TReg index_t_type (index_t index) {
        TReg r; r.index = index; return r;
}


static int new_instruction(TDynamic_structure_buffer *buff, TInstruction *item, 
                    TReg dest, TReg first, TReg second, int ins) {
    
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest.index;
    item->ins = ins;

    switch (ins) {
        case MOV_INT_REG:
        case MOV_DBL_REG:
        case ADD_INT_REG_REG:
        case SUB_DBL_REG_REG:
        case MUL_INT_REG_REG:
        case DIV_INT_REG_REG:
        case HALT:
            item->first_op.index = first.index;
            item->second_op.index = second.index;
            break;
        
        case ADD_INT_REG_CONST:
        case SUB_INT_REG_CONST:
        case MUL_INT_REG_CONST:
        case DIV_INT_REG_CONST:
            item->first_op.index = first.index;
            item->second_op.i = second.i;
            break;
        
        case MOV_INT_CONST:
        case ADD_INT_CONST_CONST:
        case SUB_INT_CONST_CONST:
        case MUL_INT_CONST_CONST:
        case DIV_INT_CONST_CONST:
            item->first_op.i = first.i;
            item->second_op.i = second.i;
            break;
        
        case ADD_DBL_REG_CONST:
        case SUB_DBL_REG_CONST:
        case MUL_DBL_REG_CONST:
        case DIV_DBL_REG_CONST:
            item->first_op.index = first.index;
            item->second_op.d = second.d;
            break;
        
        case MOV_DBL_CONST:
        case ADD_DBL_CONST_CONST:
        case SUB_DBL_CONST_CONST:
        case MUL_DBL_CONST_CONST:
        case DIV_DBL_CONST_CONST:
            item->first_op.d = first.d;
            item->second_op.d = second.d;

        default:
            return INTERNAL_ERROR;

    }
    return RETURN_OK;
}


static inline int new_instruction_reg_reg(TDynamic_structure_buffer *buff, TInstruction *item, 
                            index_t dest, index_t first, index_t second, int ins) {
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.index = first;
    item->second_op.index = second;
    return RETURN_OK;
}

static inline int new_instruction_reg_int(TDynamic_structure_buffer *buff, TInstruction *item, 
                            index_t dest, index_t first, int second, int ins) {
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.index = first;
    item->second_op.i = second;
    return RETURN_OK;
}

static inline int new_instruction_int_int(TDynamic_structure_buffer *buff, TInstruction *item, 
                            index_t dest, int first, int second, int ins) {
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.i = first;
    item->second_op.i = second;
    return RETURN_OK;
}

static inline int new_instruction_reg_dbl(TDynamic_structure_buffer *buff, TInstruction *item, 
                            index_t dest, index_t first, double second, int ins) {
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.index = first;
    item->second_op.d = second;
    return RETURN_OK;
}

static inline int new_instruction_dbl_dbl(TDynamic_structure_buffer *buff, TInstruction *item, 
                            index_t dest, double first, double second, int ins) {
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.d = first;
    item->second_op.d = second;
    return RETURN_OK;
}

//****************************** MOV ******************************// 
static inline int mov_int_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int mov_int_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int mov_dbl_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int mov_dbl_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** ADD ******************************// 
/*
static inline int add_int_data_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_DATA_DATA");
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = *access(buffers->data_buff.buffer, int, instruction->first_op.index)
    + *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    return 1;
}

static inline int add_int_reg_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_DATA");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", *access(buffers->data_buff.buffer, int, instruction->second_op.index));
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i );
    return 1;
}
static inline int add_int_data_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_DATA_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", *access(buffers->data_buff.buffer, int, instruction->first_op.index));
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
  
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = *access(buffers->data_buff.buffer, int, instruction->first_op.index)
    + instruction->second_op.i;

    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}
*/

static inline int add_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int add_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int add_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i + instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int add_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    + access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int add_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    + instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int add_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d + instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** SUB ******************************// 
static inline int sub_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    - access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int sub_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    - instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int sub_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i - instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int sub_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    - access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int sub_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    - instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int sub_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d - instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** MUL ******************************// 

static inline int mul_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    * access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int mul_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    * instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int mul_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i * instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int mul_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    * access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int mul_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    * instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int mul_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d * instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** DIV ******************************// 

static inline int div_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    / access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int div_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    / instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int div_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i / instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int div_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    / access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int div_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    / instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

static inline int div_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d / instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** EQUALS ******************************// 

static inline int eq_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "EQ_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    == access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int eq_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "EQ_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    == instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int eq_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "EQ_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i == instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int eq_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "EQ_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    == access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int eq_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "EQ_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    == instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int eq_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "EQ_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.d == instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

//****************************** GREATER ******************************// 

static inline int g_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "G_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    > access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int g_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "G_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    > instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int g_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "G_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i > instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int g_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "G_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    > access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int g_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "G_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    > instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int g_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "G_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.d > instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

//****************************** LESS ******************************// 

static inline int l_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "L_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    < access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int l_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "L_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    < instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int l_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "L_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i < instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int l_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "L_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    < access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int l_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "L_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    < instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int l_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "L_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.d < instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

//****************************** GREATER OR EQUALS ******************************// 

static inline int ge_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "GE_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    >= access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ge_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "GE_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    >= instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ge_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "GE_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i >= instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ge_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "GE_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    >= access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ge_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "GE_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    >= instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ge_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "GE_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.d >= instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

//****************************** LESS OR EQUALS ******************************// 

static inline int le_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "LE_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    <= access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int le_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "LE_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    <= instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int le_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "LE_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i <= instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int le_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "LE_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    <= access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int le_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "LE_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    <= instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int le_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "LE_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.d <= instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

//****************************** NOT EQUALS ******************************// 

static inline int ne_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "NE_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    != access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ne_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "NE_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    != instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ne_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "NE_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i != instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ne_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "NE_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    != access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ne_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "NE_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    != instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

static inline int ne_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "NE_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.d != instruction->second_op.d;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

//****************************** HALT ******************************// 

static inline int halt(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "HALT");
    fprintf(stderr, "%s: %lu %d \n", "INTERPRETER TERMINATING", instruction->dest.index, *(int*)buffers->reg_buff.buffer);
    return -1;
}
#endif // !INSTRUCTIONS_H
