/**
 * @file precedence_analysis.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module precedense_analysis.c
 */

#ifndef PRECEDENCE_ANALYSIS_H
#define PRECEDENCE_ANALYSIS_H

#include <dynamic_structure_buffer.h>
#include <stack.h>


enum operations {
    S,    // SHIFT
    R,    // REDUCE
    H,    // ??
    E,    // ERROR
};

#define END_OF_EXPR 14

#define type_filter(type)                                      \
    ((type) < 13 ? (type) : ((type) > 15 ? END_OF_EXPR : L_INT))

#define NUM_OF_TOKENS 15

extern const int precedence_table[NUM_OF_TOKENS][NUM_OF_TOKENS];


/** Checks values of types of items before first SHIFT token
 * @param b pointer to dynamic_structure_buffer
 * @param stack pointer to stack
 * @param values array to write results and number of tokens to index 0
 * @return RETURN_OK on success, INTERNAL_ERROR on error
 */
int get_types(TDynamic_structure_buffer *b, TStack *stack,  int *values);

/** Overwrite token type on top of the stack
 * @param b pointer to dynamic_structure_buffer
 * @param stack pointer to stack
 * @param new_type new token type of stack top
 * @return RETURN_OK on success, INTERNAL_ERROR on error
 */
int overwrite_top(TDynamic_structure_buffer *b, TStack *stack, int new_type);

#endif // !PRECEDENCE_ANALYSIS_H
