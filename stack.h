/**
 * project: Implementace interpretu imperativního jazyka IFJ15
 * @file stack.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @secition DESCRIPTION
 *
 * Header file of module stack.c
 */

#ifndef STACK_H
#define STACK_H

#include "datatypes.h"
#include "dynamic_structure_buffer.h"

struct next{
    index_t next;
};

struct stack{
    index_t top;
    unsigned long length;
};

/** Initialize stack
 * @param stack to be inicialized
 * @return RETURN_OK on success, INTERNAL_ERROR on error
 */
int init_stack(TStack *stack);

/** Pushes TToken on given index to the top of stack
 * @param b pointer to dynamic_structure_buffer
 * @param stack item is going to be pushed into
 * @param item index of item in structure buffer
 * @return RETURN_OK on success, INTERNAL_ERROR on error.
 */
int push(TDynamic_structure_buffer *b, TStack *stack, index_t item);


/** Pops item from the top of the stack
 * @param b pointer to dynamic_structure_buffer
 * @param stack pointer to stack
 * @return index to item on top of the stack on success ZERO_INDEX on error.
 */
index_t pop(TDynamic_structure_buffer *b, TStack *stack);

#endif // !STACK_H
