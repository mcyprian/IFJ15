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

