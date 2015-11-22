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

TReg int_type (int i) {
    TReg r; r.i = i; return r;
}

TReg double_type (double d) {
    TReg r; r.d = d; return r;
}

TReg index_t_type (index_t index) {
    TReg r; r.index = index; return r;
}

int new_instruction(TDynamic_structure_buffer *buff, TInstruction *item, 
                    TReg dest, TReg first, TReg second, int ins) {
    index_t index = ZERO_INDEX;
    new_item(buff, index, item);
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

//****************************** MOV ******************************// 
int mov_int_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int mov_int_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int mov_dbl_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int mov_dbl_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** ADD ******************************// 
/*
int add_int_data_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_DATA_DATA");
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = *access(buffers->data_buff.buffer, int, instruction->first_op.index)
    + *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    return 1;
}

int add_int_reg_data(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_DATA");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", *access(buffers->data_buff.buffer, int, instruction->second_op.index));
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + *access(buffers->data_buff.buffer, int, instruction->second_op.index);
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i );
    return 1;
}
int add_int_data_const(Buffers *buffers, TInstruction *instruction) {
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

int add_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int add_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    + instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int add_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i + instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int add_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    + access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int add_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    + instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int add_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d + instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** SUB ******************************// 
int sub_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    - access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int sub_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    - instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int sub_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i - instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int sub_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    - access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int sub_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    - instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int sub_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d - instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** MUL ******************************// 

int mul_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    * access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int mul_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    * instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int mul_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i * instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int mul_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    * access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int mul_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    * instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int mul_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d * instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

//****************************** DIV ******************************// 

int div_int_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_REG_REG");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    / access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->i;
    
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int div_int_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_REG_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->i
    / instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int div_int_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    debug_print("%s: %d\n", "OP2 CONTENT", instruction->second_op.i);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i
    = instruction->first_op.i / instruction->second_op.i;
    
    debug_print("%s: %d\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->i);
    return 1;
}

int div_dbl_reg_reg(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_REG_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    / access(buffers->reg_buff.buffer, TReg, instruction->second_op.index)->d;
    
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int div_dbl_reg_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_REG_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = access(buffers->reg_buff.buffer, TReg, instruction->first_op.index)->d
    / instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}

int div_dbl_const_const(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_CONST_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", instruction->second_op.d);
    
    access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d
    = instruction->first_op.d / instruction->second_op.d;
    
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(buffers->reg_buff.buffer, TReg, instruction->dest.index)->d);
    return 1;
}


//****************************** HALT ******************************// 
int halt(Buffers *buffers, TInstruction *instruction) {
    debug_print("%s\n", "HALT");
    fprintf(stderr, "%s: %lu %d \n", "INTERPRETER TERMINATING", instruction->dest.index, *(int*)buffers->reg_buff.buffer);
    return -1;
}
