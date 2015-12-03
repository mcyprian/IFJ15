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
#include <dynamic_buffer.h>
#include <datatypes.h>
#include <resources.h>
#include <token.h>
#include <debug.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <ial.h>
#include <built_functions.h>

#define NUM_OF_INSTRUCTIONS 63   // TODO set final number


// Converts token enum to number of instrucrion
#define token_to_ins(num, type) \
    ((type) == L_INT ? 2 * (num) : 2 * (num) + 1)

enum instructions
{
    EQ_INT_MEM_MEM,        // 0 
    EQ_DBL_MEM_MEM,        // 1
    G_INT_MEM_MEM,         // 2
    G_DBL_MEM_MEM,         // 3
    L_INT_MEM_MEM,         // 4
    L_DBL_MEM_MEM,         // 5
    GE_INT_MEM_MEM,        // 6
    GE_DBL_MEM_MEM,        // 7
    LE_INT_MEM_MEM,        // 8
    LE_DBL_MEM_MEM,        // 9
    NE_INT_MEM_MEM,        // 10
    NE_DBL_MEM_MEM,        // 11
    ADD_INT_MEM_MEM,       // 12
    ADD_DBL_MEM_MEM,       // 13
    SUB_INT_MEM_MEM,       // 14
    SUB_DBL_MEM_MEM,       // 15
    MUL_INT_MEM_MEM,       // 16
    MUL_DBL_MEM_MEM,       // 17
    DIV_INT_MEM_MEM,       // 18
    DIV_DBL_MEM_MEM,       // 19
    MOV_TOP_MEM,           // 20
    MOV_INT_MEM,           // 21 
    MOV_INT_CONST,         // 22 
    MOV_DBL_MEM,           // 23 
    MOV_DBL_CONST,         // 24 
    CAST_INT_MEM,          // 25 
    CAST_DBL_MEM,          // 26 
    PUSH_EMPTY,            // 27 
    PUSH_INT_CONST,        // 28 
    PUSH_DBL_CONST,        // 29   
    PUSH_INDEX_CONST,      // 30   
    PUSH_INT_MEM,          // 31   
    PUSH_DBL_MEM,          // 32   
    PUSH_INDEX_MEM,        // 33   
    POP_EMPTY,             // 34   
    JMP_MEM,               // 35   
    JMP_TRUE_MEM,          // 36   
    JMP_FALSE_MEM,         // 37   
    FCE_CALL,              // 38
    FCE_RETURN,            // 39
    CIN_INT,               // 40
    CIN_DOUBLE,            // 41
    CIN_STRING,            // 42
    CONCAT_MEM_MEM,        // 43
    CONCAT_MEM_CONST,      // 44   
    CONCAT_CONST_CONST,    // 45
    SUBSTR_MEM_MEM,        // 46
    LENGTH_MEM,            // 47
    LENGTH_CONST,          // 48
    FIND_MEM_MEM,          // 49
    FIND_MEM_CONST,        // 50
    FIND_CONST_CONST,      // 51
    SORT_MEM,              // 52
    SORT_CONST,            // 53
    COUT_MEM_TYPE,         // 54
    COUT_MEM_INT,          // 55
    COUT_MEM_DBL,          // 56
    COUT_MEM_STRING,       // 57
    COUT_CONST_INT,        // 58
    COUT_CONST_DBL,        // 59
    COUT_CONST_STRING,     // 60
    SET_TYPE,              // 61
    HALT                   // 62
};

static inline int new_instruction_empty(TDynamic_structure_buffer *buff, int ins) {
    debug_print("%s: %d\n", "NEW_INSTRUCTION", ins);
    TInstruction *item;
    index_t index = ZERO_INDEX;
    int err = new_item(buff, index, item);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get new_item");
    
    item->ins = ins;
    return RETURN_OK;
}

static inline int new_instruction_mem_mem(TDynamic_structure_buffer *buff, index_t dest, index_t first, index_t second, int ins) {
    debug_print("%s: %d\n", "NEW_INSTRUCTION", ins);
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

static inline int new_instruction_mem_int(TDynamic_structure_buffer *buff, index_t dest, index_t first, int second, int ins) {
    debug_print("%s: %d\n", "NEW_INSTRUCTION", ins);
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
    debug_print("%s: %d\n", "NEW_INSTRUCTION", ins);
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

static inline int new_instruction_mem_dbl(TDynamic_structure_buffer *buff, index_t dest, index_t first, double second, int ins) {
    debug_print("%s: %d\n", "NEW_INSTRUCTION", ins);
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
    debug_print("%s: %d\n", "NEW_INSTRUCTION", ins);
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
static inline int mov_top_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_TOP_MEM");

    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value;
   
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}


static inline int mov_int_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_MEM");

    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.i);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.i;
   
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int mov_int_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_INT_CONST");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = instruction->first_op.i;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int mov_dbl_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_MEM");
    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.d);
    
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.d;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d);
    return RETURN_OK;
}

static inline int mov_dbl_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MOV_DBL_CONST");
    debug_print("%s: %lf\n", "OP1 CONTENT", instruction->first_op.d);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d
    = instruction->first_op.d;
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d);
    return RETURN_OK;
}

//****************************** ADD ******************************// 
static inline int add_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "ADD_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    + access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int add_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "ADD_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    + access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    return RETURN_OK;
}

//****************************** SUB ******************************// 
static inline int sub_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SUB_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    - access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int sub_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SUB_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    - access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    return RETURN_OK;
}

//****************************** MUL ******************************// 
static inline int mul_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MUL_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    * access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int mul_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "MUL_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    * access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    return RETURN_OK;
}

//****************************** DIV ******************************// 
static inline int div_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "DIV_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;
    if (access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i == 0) 
        return DIV_ZERO_ERROR; 

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    / access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int div_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "DIV_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    if (access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d == 0.000) 
        return DIV_ZERO_ERROR; 

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    / access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    return RETURN_OK;
}

//****************************** EQUALS ******************************// 
static inline int eq_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "EQ_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    == access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int eq_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "EQ_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    == access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

//****************************** GREATER ******************************// 
static inline int g_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "G_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    > access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int g_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "G_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    > access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

//****************************** LESS ******************************// 
static inline int l_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "L_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    < access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int l_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "L_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    < access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

//****************************** GREATER OR EQUALS ******************************// 
static inline int ge_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "GE_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    >= access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int ge_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "GE_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    >= access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

//****************************** LESS OR EQUALS ******************************// 
static inline int le_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LE_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    <= access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int le_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LE_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    <= access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

//****************************** NOT EQUALS ******************************// 
static inline int ne_int_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "NE_INT_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    != access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

static inline int ne_dbl_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "NE_DBL_MEM_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)) 
        return UNINIT_ERROR;

    debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    debug_print("%s: %lf\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
    != access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
    
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    return RETURN_OK;
}

//****************************** CASTING ******************************// 

enum {STACK_TOP, BELOW_TOP};

static inline int cast_int_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CAST_INT_MEM");

    if (instruction->first_op.i == STACK_TOP) {
        if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
            return UNINIT_ERROR;

        debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);

        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d
        = (double)access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i;
        
        access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
        debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
    } else {
        if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined)
            return UNINIT_ERROR;

        debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);

        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d
        = (double)access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i;
        
        access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 2;      // Sets inint flag
        debug_print("%s: %lf\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);
    }

    return RETURN_OK;
}

static inline int cast_dbl_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CAST_DBL_MEM");

    if (instruction->first_op.i == STACK_TOP) {
        if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
            return UNINIT_ERROR;

        debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);

        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i
        = (int)access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d;
        
        access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 1;      // Sets inint flag
        debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    } else {
        if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined)
            return UNINIT_ERROR;

        debug_print("%s: %lf\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d);

        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
        = (int)access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.d;
        
        access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined = 2;      // Sets inint flag
        debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    }

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

static inline int push_int_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH INT MEM");
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

static inline int push_dbl_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH DBL MEM");
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

static inline int push_index_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "PUSH INDEX MEM");
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

//****************************** CIN *******************************// 
static inline int cin_i(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CIN_INT");
    
    int i;
    TStack_variable *tmp;
    push_stack(&resources->runtime_stack, &tmp);
    tmp->defined = 1;

    scanf("%d", &i);
    tmp->value.i = i;
    instruction->dest.i = i;

    debug_print("%s\n", "CIN_INT_RETURN");
    return RETURN_OK;
}

static inline int cin_d(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CIN_DOUBLE");
    
    double d;
    TStack_variable *tmp;
    push_stack(&resources->runtime_stack, &tmp);
    tmp->defined = 1;
    
    scanf("%lf", &d);
    tmp->value.d = d;
    instruction->dest.d = d;

    debug_print("%s\n", "CIN_DOUBLE_RETURN");
    return RETURN_OK;
}

static inline int cin_s(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CIN_STRING");
    
    char c;
    index_t string;
    TStack_variable *tmp;
    push_stack(&resources->runtime_stack, &tmp);
    tmp->defined = 1;
    
    while(isspace((c = getc(stdin))));
    while(!isspace((c = getc(stdin)))) add_char(&(resources->string_buff), c);
    string = save_token(&(resources->string_buff));
    tmp->value.index = string;
    instruction->dest.index = string;

    debug_print("%s\n", "CIN_STRING_RETURN");
    return RETURN_OK;
}

//****************************** COUT ******************************// 
static inline int cout_mem_type(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_MEM_TYPE");
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    instruction->first_op.i = 0;
    switch(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->type) {
        case L_INT:
            debug_print("%s: %d\n", "TYPE INT OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
            printf("%d", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
            break;
        case L_DOUBLE:
            debug_print("%s: %g\n", "TYPE DBL OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
            printf("%lf", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
            break;
        case L_STRING:
            debug_print("%s: %ld\n", "TYPE STRING OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
            printf("%s", load_token(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index));
            break;
    }
    
    pop_stack(&resources->runtime_stack);

    return RETURN_OK;
}


static inline int cout_mem_int(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_MEM_INT");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %d\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.i);
    
    printf("%d", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    
    return RETURN_OK;
}

static inline int cout_mem_dbl(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_MEM_DBL");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %g\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.d);
    
    printf("%f", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.d);
    
    return RETURN_OK;
}

static inline int cout_mem_string(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_MEM_STRING");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    
    printf("%s", load_token(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index));
    
    return RETURN_OK;
}

static inline int cout_const_int(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_CONST_INT");
    debug_print("%s: %d\n", "OP1 CONTENT", instruction->first_op.i);
    resources->return_value = resources->return_value;
   
    printf("%d", instruction->first_op.i);
    
    return RETURN_OK;
}

static inline int cout_const_dbl(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_CONST_DBL");
    debug_print("%s %g\n", "OP1 CONTENT", instruction->first_op.d);
    resources->return_value = resources->return_value;
    
    printf("%f", instruction->first_op.d);
    
    return RETURN_OK;
}

static inline int cout_const_string(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "COUT_CONST_STRING");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);

    printf("%s", load_token(&(resources->string_buff), instruction->first_op.index));
    
    return RETURN_OK;
}

//****************************** CONCAT ****************************//

static inline int concat_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CONCAT_MEM_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined 
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->value.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index
    = concat(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index,
                                        access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->value.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index);
    return RETURN_OK;
}

static inline int concat_mem_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CONCAT_MEM_CONST");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index
    = concat(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index,
    instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index);
    return RETURN_OK;
}

static inline int concat_const_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CONCAT_CONST_CONST");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index
    = concat(&(resources->string_buff), instruction->first_op.index, 
    instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index);
    return RETURN_OK;
}

//****************************** SUBSTR ****************************//

static inline int substr_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SUBSTR_MEM_MEM");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", instruction->second_op.index);
    debug_print("%s: %d\n", "OP3 CONTENT", instruction->dest.i);

    TStack_variable *var;
    push_stack(&(resources->runtime_stack), &var);
    var->value.index = substr(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index,
                                                      access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->value.index,
                                                      access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    var->type = L_STRING;

    var->defined = 1;
    debug_print("%s\n", "END OF SUBSTR_MEM_MEM");
    return RETURN_OK;
}

//****************************** LENGTH ****************************//

static inline int length_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LENGTH_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = length(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int length_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LENGTH_CONST");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = length(&(resources->string_buff), instruction->first_op.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

//****************************** FIND ****************************//

static inline int find_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FIND_MEM_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined 
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->value.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = find(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index,
                                     access(resources->runtime_stack.buffer, TStack_variable, instruction->second_op.index + resources->bp)->value.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int find_mem_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FIND_MEM_CONST");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = find(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index,
    instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

static inline int find_const_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FIND_CONST_CONST");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i
    = find(&(resources->string_buff), instruction->first_op.index, 
    instruction->second_op.index);

    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.i);
    return RETURN_OK;
}

//****************************** SORT ****************************//

static inline int sort_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SORT_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index
    = sort(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, instruction->first_op.index + resources->bp)->value.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index);
    return RETURN_OK;
}

static inline int sort_const(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SORT_CONST");
    debug_print("%s: %ld\n", "OP1 CONTENT", instruction->first_op.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index
    = sort(&(resources->string_buff), instruction->first_op.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index + resources->bp)->value.index);
    return RETURN_OK;
}

//****************************** JUMP ******************************// 
static inline int jmp_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined)
        return UNINIT_ERROR;

    debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    
    resources->ip = access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu;

    return RETURN_OK;
}

static inline int jmp_true_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_TRUE_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined))
        return UNINIT_ERROR;

    if (access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)) {
        resources->ip = access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu;
        
        debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    }
    
    pop_stack(&resources->runtime_stack);

    return RETURN_OK;
}

static inline int jmp_false_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_FALSE_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->defined))
        return UNINIT_ERROR;

    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1))) {
        resources->ip = access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu;
        
        debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    }
    
    pop_stack(&resources->runtime_stack);

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
    resources->return_value = *access(resources->runtime_stack.buffer, 
                                     TStack_variable, 
                                     resources->runtime_stack.next_free -1);
    
    
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
    pop_stack(&resources->runtime_stack);                                    //      ?
    return RETURN_OK;   
}

static inline int set_type(Resources *resources, TInstruction *instruction) {
    debug_print("%s: %d\n", "SETTING TOP TYPE TO", instruction->first_op.i);
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->type = instruction->first_op.i;
    return RETURN_OK;
}
    

//****************************** HALT ******************************// 

static inline int halt(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "HALT");
    fprintf(stderr, "%s: %lu %d \n", "INTERPRETER TERMINATING", instruction->dest.index + resources->bp, *(int*)resources->runtime_stack.buffer);
    return HALT;
}


#endif // !INSTRUCTIONS_H
