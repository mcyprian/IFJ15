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
    args_assert(b != NULL &&  stack != NULL, INTERNAL_ERROR);
    debug_print("%s %lu\n", "POP ", stack->top);
    TToken *tmp = NULL;
    if (stack->top == ZERO_INDEX)
        return ZERO_INDEX;
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
    args_assert(b != NULL && stack != NULL && n > 0, INTERNAL_ERROR);
    debug_print("%s %d\n", "GET TYPES OF ", n);
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

int reduce(TDynamic_structure_buffer *b, TStack *stack, int n, int expr_type) {
    args_assert(b != NULL && stack != NULL && n > 0, INTERNAL_ERROR);
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR,
            "Failed to dereference structure buffer.");
    for (int i = 0; i < n; i++) {
        if (tmp->expr_next != ZERO_INDEX) {
            if ((next = tmp->expr_next) == ZERO_INDEX)
                return INTERNAL_ERROR;
        } else {
            if ((next = tmp->token_next) == ZERO_INDEX)
                return INTERNAL_ERROR;
        }
        if (dereference_structure(b, next, (void**)&tmp) ==  INTERNAL_ERROR)
            return INTERNAL_ERROR;
    }                                        // next holds index to n + 1 element on stack
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR,
            "Failed to dereference structure buffer.");
    tmp->expr_next = next;  // TODO in case expr->anything we lost information aobut previous expr.
    tmp->expr_type = expr_type;
    return RETURN_OK;
}

