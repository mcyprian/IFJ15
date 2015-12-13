/**
 * project: Implementace interpretu imperativního jazyka IFJ15
 * @file instructions.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file including definitions of inline instruction functions
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "dynamic_structure_buffer.h"
#include "dynamic_buffer.h"
#include "datatypes.h"
#include "resources.h"
#include "token.h"
#include "debug.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ial.h"
#include "built_functions.h"

#define NUM_OF_INSTRUCTIONS 59   // TODO set final number


// Converts token enum to number of instrucrion
#define token_to_ins(num, type) \
  ((type) == L_STRING ? 20 + (num) : ((type) == L_INT ? 2 * (num) : 2 * (num) + 1))

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
    EQ_STR_MEM_MEM,        // 20
    G_STR_MEM_MEM,         // 21
    L_STR_MEM_MEM,         // 22
    GE_STR_MEM_MEM,        // 23
    LE_STR_MEM_MEM,        // 24
    NE_STR_MEM_MEM,        // 25
    MOV_TOP_MEM,           // 26
    MOV_INT_MEM,           // 27 
    MOV_INT_CONST,         // 28 
    MOV_DBL_MEM,           // 29 
    MOV_DBL_CONST,         // 30 
    CAST_INT_MEM,          // 31 
    CAST_DBL_MEM,          // 32 
    PUSH_EMPTY,            // 33 
    PUSH_INT_CONST,        // 34 
    PUSH_DBL_CONST,        // 35   
    PUSH_INDEX_CONST,      // 36   
    PUSH_INT_MEM,          // 37   
    PUSH_DBL_MEM,          // 38   
    PUSH_INDEX_MEM,        // 39   
    POP_EMPTY,             // 40   
    JMP_MEM,               // 41   
    JMP_TRUE_MEM,          // 42   
    JMP_FALSE_MEM,         // 43
    JMP_FUNC,  		   // 44
    FCE_CALL,              // 45
    FCE_RETURN,            // 46
    CIN_INT,               // 47
    CIN_DOUBLE,            // 48
    CIN_STRING,            // 49
    CONCAT_MEM_MEM,        // 50
    SUBSTR_MEM_MEM,        // 51
    LENGTH_MEM,            // 52
    FIND_MEM_MEM,          // 53
    SORT_MEM,              // 54
    COUT_MEM_TYPE,         // 55
    NO_RETURN,             // 56
    SET_TYPE,              // 57
    HALT                   // 58
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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

static inline int eq_str_mem_mem(Resources *resources, TInstruction *instruction) { 
    debug_print("%s\n", "EQ_STR_MEM_MEM"); 
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = (strcmp(load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index),
              load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index))
    == 0);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
   
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

static inline int g_str_mem_mem(Resources *resources, TInstruction *instruction) { 
    debug_print("%s\n", "G_STR_MEM_MEM"); 
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = (strcmp(load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index),
              load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index))
    > 0);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
   
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

static inline int l_str_mem_mem(Resources *resources, TInstruction *instruction) { 
    debug_print("%s\n", "L_STR_MEM_MEM"); 
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = (strcmp(load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index),
              load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index))
    < 0);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
   
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

static inline int ge_str_mem_mem(Resources *resources, TInstruction *instruction) { 
    debug_print("%s\n", "GE_STR_MEM_MEM"); 
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = (strcmp(load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index),
              load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index))
    >= 0);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
   
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

static inline int le_str_mem_mem(Resources *resources, TInstruction *instruction) { 
    debug_print("%s\n", "LE_STR_MEM_MEM"); 
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = (strcmp(load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index),
              load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index))
    <= 0);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
   
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

static inline int ne_str_mem_mem(Resources *resources, TInstruction *instruction) { 
    debug_print("%s\n", "NE_STR_MEM_MEM"); 
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = (strcmp(load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index),
              load_token(&resources->string_buff, access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index))
    != 0);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "DEST CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
   
    pop_stack(&resources->runtime_stack); 

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
    new_top->value.index = instruction->first_op.index;
    new_top->defined = 0;
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
    return RETURN_OK;
}

//****************************** CIN *******************************// 
static inline int cin_i(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CIN_INT");
    
    int i;
    TStack_variable *tmp;
    push_stack(&resources->runtime_stack, &tmp);
    tmp->defined = 1;

    if (scanf("%d", &i) != 1) return INPUT_ERROR;
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
    
    if (scanf("%lf", &d) != 1) return INPUT_ERROR;
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
            printf("%g", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.d);
            break;
        case L_STRING:
            debug_print("%s: %ld\n", "TYPE STRING OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
            printf("%s", load_token(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index));
            break;
    }
    
    pop_stack(&resources->runtime_stack);

    return RETURN_OK;
}

//****************************** CONCAT ****************************//

static inline int concat_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "CONCAT_MEM_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined 
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index
    = concat(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index,
                                        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

//****************************** SUBSTR ****************************//

static inline int substr_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SUBSTR_MEM_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->defined 
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->value.index);
    debug_print("%s: %d\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    debug_print("%s: %d\n", "OP3 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);

    instruction->dest.index = 0;

    // TStack_variable *var;
    // push_stack(&(resources->runtime_stack), &var);
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->value.index
    = substr(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->value.index,
                                        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i,
                                        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    // var->type = L_STRING;

    // var->defined = 1;
	access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->type = L_STRING;      // Sets inint flag
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 3)->value.index);

    pop_stack(&resources->runtime_stack); 
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

//****************************** LENGTH ****************************//

static inline int length_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "LENGTH_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i
    = length(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i);
    
    return RETURN_OK;
}

//****************************** FIND ****************************//

static inline int find_mem_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FIND_MEM_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined 
        && access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index);
    debug_print("%s: %ld\n", "OP2 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);

    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i
    = find(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.index,
                                     access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);

    if (access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i == -2)
	    return INTERNAL_ERROR;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->defined = 1;      // Sets inint flag
    debug_print("%s: %d\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 2)->value.i);
    
    pop_stack(&resources->runtime_stack); 

    return RETURN_OK;
}

//****************************** SORT ****************************//

static inline int sort_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "SORT_MEM");
    if (!access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined)
        return UNINIT_ERROR;

    debug_print("%s: %ld\n", "OP1 CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    instruction->dest.index = 0;

    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index
    = sort(&(resources->string_buff), access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined = 1;      // Sets inint flag
    debug_print("%s: %ld\n", "REGISTER CONTENT", access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.index);
    
    return RETURN_OK;
}

//****************************** JUMP ******************************// 
static inline int jmp_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_MEM");

    debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    
    resources->ip = instruction->dest.index - 1lu;

    return RETURN_OK;
}

static inline int jmp_func(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_FUNC");

    index_t *jmp_index;
    int iRet;

    if ((iRet = dereference_structure(&(resources->func_table), instruction->dest.index, (void **)&jmp_index)) != RETURN_OK)
	return iRet;
    
    debug_print("instruction->dest.index: %lu, jmp_index: %lu\n", instruction->dest.index, *jmp_index);
    
    if (*jmp_index == 0){
        return SEMANTIC_ERROR;
    }
    resources->ip = *jmp_index - 1lu;
    resources->bp = resources->tmp_bp;

    debug_print("%s %lu\n", "NEW IP ADRESS", resources->ip);

    return RETURN_OK;
}


static inline int jmp_true_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_TRUE_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    if (access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i) {
        resources->ip = instruction->dest.index - 1lu;
        
        debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    }
    
    pop_stack(&resources->runtime_stack);

    return RETURN_OK;
}

static inline int jmp_false_mem(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "JMP_FALSE_MEM");
    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined))
        return UNINIT_ERROR;

    if (!(access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->value.i)) {
        resources->ip = instruction->dest.index - 1lu;
        
        debug_print("%s %lu\n", "NEW IP ADRESS", access(resources->runtime_stack.buffer, TStack_variable, instruction->dest.index)->value.index - 1lu);
    }
    
    pop_stack(&resources->runtime_stack);

    return RETURN_OK;
}


//****************************** FUNCTIONS ******************************// 
// Expecting adress of ources->instruction_buffer.buffer, TInstruction, (resources->instruction_buffer.next_free - 1))unction in instruction->dest.index
static inline int function_call(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FUNCTION CALL");

    TStack_variable *tmp;                           //    Stack
    push_stack(&resources->runtime_stack, &tmp);    //    ?
    debug_print("%s %lu\n", "RETURNING IP", instruction->dest.index);
    tmp->value.index = instruction->dest.index;               //    ip  ?
    tmp->defined = 1;
    push_stack(&resources->runtime_stack, &tmp);    //    bp  ip  ?

    debug_print("%s %lu\n", "RETURNING BP", resources->bp);
    tmp->value.index = resources->bp;
    tmp->defined = 1;

    debug_print("%s: %ld\n", "NEW VALUE IN TMP BP", instruction->first_op.index);
    resources->tmp_bp = resources->runtime_stack.next_free - 1;      // sets bp to stack top

    return RETURN_OK;
}

static inline int function_return(Resources *resources, TInstruction *instruction) {
    debug_print("%s\n", "FUNCTION RETURN");
                                                                                //   Stack
                                                                                //   rv  loc1 loc2 ... locN  bp  ip
    resources->return_value = *access(resources->runtime_stack.buffer, 
                                     TStack_variable, 
                                     resources->runtime_stack.next_free -1);
    
    
    pop_stack(&resources->runtime_stack);                                       //  loc1 loc2 ... locN  bp  ip

    int i;
    for (i = 0; i < instruction->first_op.i; i++) {
        access(resources->runtime_stack.buffer, TStack_variable, resources->runtime_stack.next_free - 1)->defined = 0;
        if (resources->runtime_stack.next_free > 1)
            resources->runtime_stack.flags[--resources->runtime_stack.next_free] = 0;
    }                                                                           //   bp   ip

    resources->bp = access(resources->runtime_stack.buffer,
                           TStack_variable,
                           resources->runtime_stack.next_free -1)
    ->value.index;
    debug_print("%s %lu\n", "RETURNING BP", resources->bp);

    pop_stack(&resources->runtime_stack);                                       //   ip
    resources->ip = access(resources->runtime_stack.buffer,
                           TStack_variable,
                           resources->runtime_stack.next_free -1)
    ->value.index;
    debug_print("%s %lu\n", "RETURNING IP", resources->ip);
    debug_print("%s %lu\n", "IP AFTER RETURN ADRESS", resources->ip);
    pop_stack(&resources->runtime_stack);                                       //   ?

    TStack_variable *tmp;                        
    push_stack(&resources->runtime_stack, &tmp);
    *tmp = resources->return_value;
    return RETURN_OK;   
}

static inline int no_return(Resources *resources, TInstruction *instruction) {
    instruction->first_op.i = 0;
    resources->return_value = resources->return_value;
    return RUNTIME_ERROR;
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
    instruction->dest.index = resources->bp;
    return HALT;
}


#endif // !INSTRUCTIONS_H
