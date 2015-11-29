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
#include <datatypes.h>
#include <resources.h>
#include <token.h>
#include <debug.h>

#define NUM_OF_INSTRUCTIONS 45   // TODO set final number


// Converts token enum to number of instrucrion
#define token_to_ins(num, type) \
    ((type) == L_INT ? 2 * (num) : 2 * (num) + 1)

enum instructions
{
    EQ_INT_REG_REG,        // 0 
    EQ_DBL_REG_REG,        // 1
    G_INT_REG_REG,         // 2
    G_DBL_REG_REG,         // 3
    L_INT_REG_REG,         // 4
    L_DBL_REG_REG,         // 5
    GE_INT_REG_REG,        // 6
    GE_DBL_REG_REG,        // 7
    LE_INT_REG_REG,        // 8
    LE_DBL_REG_REG,        // 9
    ADD_INT_REG_REG,       // 10
    ADD_DBL_REG_REG,       // 11
    SUB_INT_REG_REG,       // 12
    SUB_DBL_REG_REG,       // 13
    MUL_INT_REG_REG,       // 14
    MUL_DBL_REG_REG,       // 15
    DIV_INT_REG_REG,       // 16
    DIV_DBL_REG_REG,       // 17
    NE_INT_REG_REG,        // 18
    NE_DBL_REG_REG,        // 19
    MOV_INT_REG,           // 20 
    MOV_INT_CONST,         // 21 
    MOV_DBL_REG,           // 22 
    MOV_DBL_CONST,         // 23 
    CAST_INT_REG,          // 24 
    CAST_INT_CONST,        // 25 
    CAST_DBL_REG,          // 26 
    CAST_DBL_CONST,        // 27  
    PUSH_EMPTY,            // 28 
    PUSH_INT_CONST,        // 29 
    PUSH_DBL_CONST,        // 30   
    PUSH_INDEX_CONST,      // 31   
    PUSH_INT_REG,          // 32   
    PUSH_DBL_REG,          // 33   
    PUSH_INDEX_REG,        // 34   
    POP_EMPTY,             // 35   
    JMP_REG,               // 36   
    JMP_CONST,             // 37   
    JMP_TRUE_REG_REG,      // 38   
    JMP_TRUE_CONST_CONST,  // 39  
    JMP_TRUE_CONST_REG,    // 40 
    JMP_TRUE_REG_CONST,    // 41
    FCE_CALL,              // 42
    FCE_RETURN,            // 43
    HALT                   // 44
};


static inline int new_instruction_reg_reg(TDynamic_structure_buffer *buff, index_t dest, index_t first, index_t second, int ins) {
    TInstruction *item;
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.index = first;
    item->second_op.index = second;
    return RETURN_OK;
}

static inline int new_instruction_reg_int(TDynamic_structure_buffer *buff, index_t dest, index_t first, int second, int ins) {
    TInstruction *item;
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.index = first;
    item->second_op.i = second;
    return RETURN_OK;
}

static inline int new_instruction_int_int(TDynamic_structure_buffer *buff, index_t dest, int first, int second, int ins) {
    TInstruction *item;
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.i = first;
    item->second_op.i = second;
    return RETURN_OK;
}

static inline int new_instruction_reg_dbl(TDynamic_structure_buffer *buff, index_t dest, index_t first, double second, int ins) {
    TInstruction *item;
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");

    item->dest.index = dest;
    item->ins = ins;
    item->first_op.index = first;
    item->second_op.d = second;
    return RETURN_OK;
}

static inline int new_instruction_dbl_dbl(TDynamic_structure_buffer *buff, index_t dest, double first, double second, int ins) {
    TInstruction *item;
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
static inline int mov_int_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_REG");

    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.i);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.i;
   
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int mov_int_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = instruction->first_op.i;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int mov_dbl_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_REG");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.d);
    
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.d;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d);
    return RETURN_OK;
}

static inline int mov_dbl_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d
    = instruction->first_op.d;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d);
    return RETURN_OK;
}

//****************************** ADD ******************************// 
static inline int add_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_REG_REG");
    printf("%lu %lu %lu\n", instruction->dest.index, instruction->first_op.index, instruction->second_op.index);
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    + access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int add_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    + access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d);
    return RETURN_OK;
}

//****************************** SUB ******************************// 
static inline int sub_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    - access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int sub_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    - access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d);
    return RETURN_OK;
}

//****************************** MUL ******************************// 

static inline int mul_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    * access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int mul_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    * access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d);
    return RETURN_OK;
}

//****************************** DIV ******************************// 

static inline int div_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))

        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    / access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int div_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    / access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d);
    return RETURN_OK;
}

//****************************** EQUALS ******************************// 

static inline int eq_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "EQ_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    == access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int eq_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "EQ_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    == access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

//****************************** GREATER ******************************// 

static inline int g_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "G_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    > access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int g_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "G_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    > access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

//****************************** LESS ******************************// 
static inline int l_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "L_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    < access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int l_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "L_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    < access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

//****************************** GREATER OR EQUALS ******************************// 
static inline int ge_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "GE_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    >= access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int ge_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "GE_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    >= access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

//****************************** LESS OR EQUALS ******************************// 
static inline int le_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LE_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    <= access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int le_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LE_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    <= access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

//****************************** NOT EQUALS ******************************// 
static inline int ne_int_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "NE_INT_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i
    != access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int ne_dbl_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "NE_DBL_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d
    != access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

//****************************** CASTING ******************************// 
static inline int cast_int_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CAST_INT_REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d
    = (double)access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d);
    return RETURN_OK;
}

static inline int cast_int_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CAST_INT_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d
    = (double)instruction->first_op.i;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.d);
    return RETURN_OK;
}

static inline int cast_dbl_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CAST_DBL_REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = (int)access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.d;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}

static inline int cast_dbl_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CAST_DBL_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i
    = (int)instruction->first_op.d;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.i);
    return RETURN_OK;
}


//****************************** STACK ****************************//
static inline int push_empty(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH EMPTY");
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    resources->runtime_stack.flags[resources->runtime_stack.next_free++] = 1;
    new_top->value.index = instruction->first_op.index;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int push_int_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH INT CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    new_top->value.i = instruction->first_op.i;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int push_dbl_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH DBL CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    new_top->value.d = instruction->first_op.d;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int push_index_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH INDEX CONST");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    new_top->value.index = instruction->first_op.index;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int push_int_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH INT REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    new_top->value.i = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.i;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int push_dbl_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH DBL REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    new_top->value.d = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.d;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int push_index_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH INDEX REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    TStack_variable *new_top;
    index_t index;

    int iRet = RETURN_OK;
    if ((iRet = get_free_element_index(&resources->runtime_stack, &index)) != RETURN_OK) return iRet;
    if ((iRet = dereference_structure(&resources->runtime_stack, index, 
                                      (void **)&new_top)) != RETURN_OK) return iRet; 
    new_top->value.index = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index;
    new_top->defined = 1;
    return RETURN_OK;
}

static inline int pop_empty(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "POP");
    instruction->dest.index = 0;
    if (resources->runtime_stack.next_free > 1) {
        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined = 0;
        resources->runtime_stack.flags[--resources->runtime_stack.next_free] = 0;
        return RETURN_OK;
    }
    else return -1;
}

//****************************** JUMP ******************************// 
//
// jmp_true_reg_reg    ->     dest from reg   first_operand in reg

static inline int jmp_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined)
        return UNINIT_ERROR;

    debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    
    resources->ip = access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu;

    return RETURN_OK;
}

static inline int jmp_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_CONST");

    debug_print("%s %lu\n", "NEW IP ADRESS", instruction->dest.index - 1lu);
    
    resources->ip = instruction->dest.index - 1lu;

    return RETURN_OK;
}

static inline int jmp_true_reg_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_TRUE_REG_REG");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);

    
    if (access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i) {
        resources->ip = access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu;
        
        debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    }

    return RETURN_OK;
}

static inline int jmp_true_const_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_TRUE_CONST_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);

    if (instruction->first_op.i) {
        resources->ip = instruction->dest.index - 1lu;
        
        debug_print("%s %lu\n", "NEW IP ADRESS", instruction->dest.index - 1lu);
    }

    return RETURN_OK;
}

static inline int jmp_true_const_reg(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_TRUE_CONST_REG");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i);

    
    if (access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index)->value.i) {
        resources->ip = instruction->dest.index +  - 1lu;
        debug_print("%s %lu\n", "NEW IP ADRESS", instruction->dest.index - 1lu);
    }

    return RETURN_OK;
}

static inline int jmp_true_reg_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_TRUE_REG_CONST");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);

    if (instruction->first_op.i) {
        resources->ip = access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu;
        debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    }

    return RETURN_OK;
}

//****************************** FUNCTIONS ******************************// 
// Expecting adress of function in instruction->dest.index
static inline int function_call(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FUNCTION CALL");

    TStack_variable *tmp;                           //    Stack
    push_stack(&resources->runtime_stack, &tmp);    //    ?
    debug_print("%s %lu\n", "RETURNING IP", resources->ip);
    tmp->value.index = resources->ip;               //    ip  ?
    tmp->defined = 1;
    push_stack(&resources->runtime_stack, &tmp);    //    bp  ip  ?

    debug_print("%s %lu\n", "RETURNING BP", resources->bp);
    tmp->value.index = resources->bp;
    tmp->defined = 1;

    debug_print("%s %lu\n", "IP AFTER CALL", instruction->dest.index - 1lu);
    
    resources->ip = instruction->dest.index - 1lu;       // sets ip to adress of function

    debug_print("%s: %ld\n", "NEW VALUE IN BP", instruction->first_op.index);
    resources->bp = resources->runtime_stack.next_free - 1;      // sets bp to stack top

    return RETURN_OK;
}

static inline int function_return(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FUNCTION RETURN");
                                                                             //     Stack
    instruction->dest.index = 0;   // remove warning                                rv  bp  ip
    resources->return_value = access(resources->runtime_stack.buffer, 
                                     TStack_variable, 
                                     resources->runtime_stack.next_free -1)
    ->value.i;
    
    pop_stack(&resources->runtime_stack);                                    //      bp  ip
    resources->bp = access(resources->runtime_stack.buffer,
                           TStack_variable,
                           resources->runtime_stack.next_free -1)
    ->value.index;
    debug_print("%s %lu\n", "RETURNING BP", resources->bp);

    pop_stack(&resources->runtime_stack);                                    //      ip
    resources->ip = access(resources->runtime_stack.buffer,
                           TStack_variable,
                           resources->runtime_stack.next_free -1)
    ->value.index;
    debug_print("%s %lu\n", "RETURNING IP", resources->ip);
    debug_print("%s %lu\n", "IP AFTER RETURN ADRESS", resources->ip);
    return RETURN_OK;   
}


//****************************** HALT ******************************// 

static inline int halt(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "HALT");
    fprintf(stderr, "%s: %lu %d \n", "INTERPRETER TERMINATING", instruction->dest.index + resources->bp, *(int*)resources->runtime_stack.buffer);
    return -1;
}


#endif // !INSTRUCTIONS_H
