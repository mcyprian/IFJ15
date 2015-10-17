/**
 * @file stack.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Abstraction of stack working with next indexes of TToken structs stored in
 * dynamic_structure_buffer.
 * */

#include <stack.h>
#include <token.h>
#include <dynamic_structure_buffer.h>
#include <debug.h>
#include <error_macros.h>

int push(TDynamic_structure_buffer *b, TStack *stack, index_t item) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    TToken *tmp = NULL;
    if (stack->top == ZERO_INDEX) 
        stack->top = item;
    else {
        catch_internal_error(dereference_structure(b, item, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");
        if (tmp->expr_next != ZERO_INDEX)
            tmp->expr_next = stack->top;
        else
            tmp->token_next = stack->top;
        stack->top = item;
    }
    stack->length++;
    return RETURN_OK;
}

index_t pop(TDynamic_structure_buffer *b, TStack *stack) {
    args_assert(b != NULL &&  stack != NULL, NULL);
    TToken *tmp = NULL;
    index_t old_top = stack->top;
    if (dereference_structure(b, stack->top, (void**)&tmp) ==  INTERNAL_ERROR)
        return ZERO_INDEX;
    if (tmp->expr_next != ZERO_INDEX)
        stack->top = tmp->expr_next;
    else
        stack->top = tmp->token_next;
    stack->length--;
    return old_top;
}

int get_types(TDynamic_structure_buffer *b, TStack *stack, int n, int *values) {
    args_assert(b != NULL && stack != NULL && n > 0, NULL);
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR,
            "Failed to dereference structure buffer.");
    for (int i = 0; i < n; i++) {
        if (tmp->expr_next != ZERO_INDEX) {
            values[i] = tmp->expr_type;
            if ((next = tmp->expr_next) == ZERO_INDEX)
                return INTERNAL_ERROR;
        } else {
            values[i] = tmp->token_type;
            if ((next = tmp->token_next) == ZERO_INDEX)
                return INTERNAL_ERROR;
        }
        if (dereference_structure(b, next, (void**)&tmp) ==  INTERNAL_ERROR)
            return INTERNAL_ERROR;
    }
    return RETURN_OK;
}
