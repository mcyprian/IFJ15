/**
 * @file precedence_analysis.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Precedence syntax analysis of expresions in IFJ15 language
 */


#include <string.h>
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

const int short_rules[2][1] =
{
    {IDENTIFIER},
    {L_INT}                // same value for all literals
};

const int long_rules[11][3] =
{
    {RVALUE, O_EQUALS, RVALUE},
    {RVALUE, O_GE, RVALUE},
    {RVALUE, O_LE, RVALUE},
    {RVALUE, O_GE, RVALUE},
    {RVALUE, O_LE, RVALUE},
    {RVALUE, O_PLUS, RVALUE},
    {RVALUE, O_MINUS, RVALUE},
    {RVALUE, O_MUL, RVALUE},
    {RVALUE, O_DIV, RVALUE},
    {RVALUE, O_NE, RVALUE},
    {OPENING_BRACKET, RVALUE, CLOSING_BRACKET}
};


int get_types(TDynamic_structure_buffer *b, TStack *stack, int *values) {
    args_assert(b != NULL && stack != NULL && values != NULL, INTERNAL_ERROR);
    debug_print("%s\n", "GET TYPES");
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;
    int i;
    int t;
    int *s;
    int *f;

    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");
    
    for (i = 1; tmp->token_type != SHIFT; i++) {
        if (i > MAX_RULE_LENGTH)
            return SYNTAX_ERROR;
        values[i] = tmp->token_type;
        if ((next = tmp->next) == ZERO_INDEX)
            return INTERNAL_ERROR;
        
        if (dereference_structure(b, next, (void**)&tmp) ==  INTERNAL_ERROR)
            return INTERNAL_ERROR;
    }
    values[0] = i - 1;           // store length of rule to index 0
    for (s = (values + 1), f = (values + i - 1); s < f; s++, f--) {
        t = *s;
        *s = *f;
        *f = t;
    }
    return RETURN_OK;
}

int overwrite_top(TDynamic_structure_buffer *b, TStack *stack, int new_type) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    TToken *tmp = NULL;
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");
    tmp->token_type = new_type;    
    return RETURN_OK;
}

int reduce(TDynamic_structure_buffer *b, TStack *stack) {
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;

    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");


    while(tmp->token_type != SHIFT) {
        pop(b, stack);
        if (stack->top == ZERO_INDEX)
            return SYNTAX_ERROR;
      
        catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");
      
    }
    return overwrite_top(b, stack, RVALUE);
}

int get_rule(TDynamic_structure_buffer *b, TStack *stack) {
    int values [MAX_RULE_LENGTH + 1];
    int i;
    int err = get_types(b, stack, values);
    if (err != RETURN_OK)
        return err;

    printf("Get rule start value length == %d\n", values[0]);
    if (values[0] == 1) {
        for (i = 0; i < 3; i++) {
            if (short_rules[i][0] == values[1])
                printf("used rule %d -> RVALUE\n", short_rules[i][0]);
            return RETURN_OK;
        }
        return SYNTAX_ERROR;
    } else if (values[0] == 3) {
        for (i = 0; i < 11; i++) {
           if (!(memcmp(long_rules[i], &values[1], sizeof(long_rules[i])))) {
                printf("used rule %d \n", i);
                return RETURN_OK;
            }
        }
        return SYNTAX_ERROR;
    }
    else 
        return INTERNAL_ERROR;
}
    
