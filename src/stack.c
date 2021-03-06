/**
 * @file stack.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Abstraction of stack working with next indexes of TNext structs stored in
 * dynamic_structure_buffer.
 * */

#include <stack.h>
#include <token.h>
#include <dynamic_structure_buffer.h>
#include <debug.h>
#include <error_macros.h>

int init_stack(TStack *stack) {
    args_assert(stack != NULL, INTERNAL_ERROR);
    debug_print("%s\n", "STACK INIT");
    stack->top = ZERO_INDEX;
    stack->length = 0;
    return RETURN_OK;
}

int push(TDynamic_structure_buffer *b, TStack *stack, index_t item) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    debug_print("%s %lu\n", "PUSH ", item);
    TNext *tmp = NULL;
    if (stack->top == ZERO_INDEX) 
        stack->top = item;
    else {
        catch_internal_error(dereference_structure(b, item, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");
        
        tmp->next = stack->top;
        stack->top = item;
    }
    stack->length++;
    return RETURN_OK;
}

index_t pop(TDynamic_structure_buffer *b, TStack *stack) {
    args_assert(b != NULL &&  stack != NULL, INTERNAL_ERROR);
    debug_print("%s %lu\n", "POP ", stack->top);
    TNext *tmp = NULL;
    if (stack->top == ZERO_INDEX)
        return ZERO_INDEX;
    index_t old_top = stack->top;
    if (dereference_structure(b, stack->top, (void**)&tmp) ==  INTERNAL_ERROR)
        return ZERO_INDEX;
    stack->top = tmp->next;
    stack->length--;
    return old_top;
}
