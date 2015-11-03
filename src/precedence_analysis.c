/**
 * @file precedence_analysis.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Precedence syntax analysis of expresions in IFJ15 language
 */


#include <debug.h>
#include <error_macros.h>
#include <precedence_analysis.h>
#include <stack.h>
#include <token.h>


const int precedence_table[NUM_OF_TOKENS][NUM_OF_TOKENS] = 
{//        ==   >   <   >=  <=  +   -   *   /   !=  )   (   id  li  $
 /* == */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  S,  S,  R },
 /* >  */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  S,  S,  R },  
 /* <  */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  S,  S,  R }, 
 /* >= */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  S,  S,  R }, 
 /* <= */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  S,  S,  R }, 
 /* +  */ { R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  R,  S,  S,  S,  R }, 
 /* -  */ { R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  R,  S,  S,  S,  R }, 
 /* *  */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  S,  S,  R }, 
 /* /  */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  S,  S,  R }, 
 /* != */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  S,  S,  R }, 
 /* )  */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  E,  E,  R }, 
 /* (  */ { S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  H,  S,  S,  S,  E }, 
 /* id */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  E,  E,  R },
 /* li */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  E,  E,  R },
 /* $  */ { S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  E,  S,  S,  S,  E }
};


int get_types(TDynamic_structure_buffer *b, TStack *stack, int *values) {
    args_assert(b != NULL && stack != NULL && values != NULL, INTERNAL_ERROR);
    debug_print("%s\n", "GET TYPES");
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;
    int i;
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");
    
    for (i = 1; tmp->token_type != SHIFT; i++) {
        values[i] = tmp->token_type;
        if ((next = tmp->next) == ZERO_INDEX)
            return INTERNAL_ERROR;
        
        if (dereference_structure(b, next, (void**)&tmp) ==  INTERNAL_ERROR)
            return INTERNAL_ERROR;
    }
    values[0] = i - 1;           // store length of rule to index 0
    return RETURN_OK;
}

int overwrite_top(TDynamic_structure_buffer *b, TStack *stack, int new_type) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    TToken *tmp = NULL;
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");

    tmp->token_type = new_type;    
    return RETURN_OK;
}
