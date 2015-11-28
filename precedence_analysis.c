/**
 * @file precedence_analysis.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Precedence syntax analysis of expresions in IFJ15 language
 */

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "debug.h"
#include "error_macros.h"
#include "precedence_analysis.h"
#include "resources.h"
#include "datatypes.h"
#include "stack.h"
#include "scanner.h"
#include "token.h"
#include "syntax_analysis.h"
#include "semantics.h"

const int precedence_table[NUM_OF_TOKENS][NUM_OF_TOKENS] = 
{//        ==   >   <   >=  <=  +   -   *   /   !=  )   (   $   id  li
 /* == */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R,  S,  S },
 /* >  */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R,  S,  S },  
 /* <  */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R,  S,  S }, 
 /* >= */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R,  S,  S }, 
 /* <= */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R,  S,  S }, 
 /* +  */ { R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  R,  S,  R,  S,  S }, 
 /* -  */ { R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  R,  S,  R,  S,  S }, 
 /* *  */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  R,  S,  S }, 
 /* /  */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  R,  S,  S }, 
 /* != */ { R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R,  S,  S }, 
 /* )  */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  R,  E,  E }, 
 /* (  */ { S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  H,  S,  E,  S,  S }, 
 /* $  */ { S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  E,  S,  E,  S,  S },
 /* id */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  R,  E,  E },
 /* li */ { R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  R,  E,  E }
};

const int short_rules[2] = {IDENTIFIER, L_INT};  // same value for all literals

const int long_rules[11][3] =
{
    {RVALUE, O_EQUALS, RVALUE},
    {RVALUE, O_G, RVALUE},
    {RVALUE, O_L, RVALUE},
    {RVALUE, O_GE, RVALUE},
    {RVALUE, O_LE, RVALUE},
    {RVALUE, O_PLUS, RVALUE},
    {RVALUE, O_MINUS, RVALUE},
    {RVALUE, O_MUL, RVALUE},
    {RVALUE, O_DIV, RVALUE},
    {RVALUE, O_NE, RVALUE},
    {OPENING_BRACKET, RVALUE, CLOSING_BRACKET}
};

long to_int(char *str) {
    debug_print("%s: %s\n", "CONVERTING", str);
    char *tmp;
    long retVal = strtol(str, &tmp, 0);
    if (*tmp == '\0' && retVal != LONG_MAX && retVal != LONG_MIN)
        return retVal;
    else 
        return -1;       // id can't have negative value
}

double to_double(char *str) {
    debug_print("%s: %s\n", "CONVERTING", str);
    char *tmp;
    double retVal = strtod(str, &tmp);
    if (*tmp == '\0')
        return retVal;
    else 
        return -1.0;       // id can't have negative value
}

int get_first_token(TDynamic_structure_buffer *b, TStack *stack, index_t *first) {

    args_assert(b != NULL && stack != NULL && first != NULL, INTERNAL_ERROR);
    
    TToken *tmp = NULL;
    *first = stack->top;

    catch_internal_error(
        dereference_structure(b, *first, (void**)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while (tmp->token_type == RVALUE || tmp->token_type == SHIFT) {
        if (tmp->next == ZERO_INDEX)
            return SYNTAX_ERROR;
        *first = tmp->next;
        catch_internal_error(
            dereference_structure(b, *first, (void**)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    }
    debug_print("%s: %lu\n", "FIRST TOKEN", *first);
    return RETURN_OK;
}

const char* symbols[47];   

#if DEBUG
int print_stack(TDynamic_structure_buffer *b, TStack *stack) {
    symbols[0] = "=="; 
    symbols[1] = ">"; 
    symbols[2] = "<"; 
    symbols[3] = ">="; 
    symbols[4] = "<="; 
    symbols[5] = "+"; 
    symbols[6] = "-"; 
    symbols[7] = "*"; 
    symbols[8] = "/"; 
    symbols[9] = "!="; 
    symbols[10] = ")"; 
    symbols[11] = "("; 
    symbols[12] = "$"; 
    symbols[13] = "id"; 
    symbols[14] = "li"; 
    symbols[45] = "SHIFT"; 
    symbols[46] = "RVALUE"; 

    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;
    catch_internal_error(
        dereference_structure(b, stack->top, (void**)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    fprintf(stderr, "\nStack:\n    %s %d %d %d<-- stack top\n", 
            symbols[type_filter(tmp->token_type)], tmp->status, tmp->original_type, 
            tmp->status);
    
    while (tmp->token_type != END_OF_EXPR) {
        if ((next = tmp->next) == ZERO_INDEX)
            return SYNTAX_ERROR;

        catch_internal_error(
            dereference_structure(b, next, (void**)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer"
        );

        fprintf(stderr, "    %s %d %d %d\n", symbols[tmp->token_type], tmp->status, tmp->original_type, tmp->status);
    }
    return RETURN_OK;
}

#endif

int get_types(TDynamic_structure_buffer *b, TStack *stack, int *values) {
    args_assert(b != NULL && stack != NULL && values != NULL, INTERNAL_ERROR);
    debug_print("%s\n", "GET TYPES");
    
    TToken *tmp = NULL;
    index_t next = ZERO_INDEX;
    int i;
    int t;
    int *s;
    int *f;

    catch_internal_error(
        dereference_structure(b, stack->top, (void**)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    
    for (i = 1; tmp->token_type != SHIFT; i++) {
        if (i > MAX_RULE_LENGTH)
            return SYNTAX_ERROR;
        values[i] = type_filter(tmp->token_type);
        if ((next = tmp->next) == ZERO_INDEX)
            return SYNTAX_ERROR;
        
        catch_internal_error(
            dereference_structure(b, next, (void**)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    }
    values[0] = i - 1;           // store length of rule to index 0
    for (s = (values + 1), f = (values + i - 1); s < f; s++, f--) {
        t = *s;
        *s = *f;
        *f = t;
    }
    return RETURN_OK;
}

int short_reduction(TDynamic_structure_buffer *b, TStack *stack, int original_type, char status) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);

    debug_print("%s\n", "ONE TOKEN REDUCED");

    index_t top_index = stack->top;
    pop(b, stack);
    pop(b, stack);
    push(b, stack, top_index);
    return overwrite_top(b, stack, RVALUE, original_type, status);
}


int long_reduction(Resources *res, TToken **reduced_tokens, TStack *stack, int original_type) {

    debug_print("%s: %d\n", "ORIGINAL_TYPE:", original_type);
    if (original_type == L_INT) {
        int first_val;
        int second_val;
        
        switch (reduced_tokens[0]->status) {
            case L_INT:      // literal
               if ((first_val = to_int(load_token(&res->string_buff, reduced_tokens[0]->token_index))) < 0)
                    return SEMANTIC_ERROR;
                break;
            case IDENTIFIER:
                // TODO get value if first is id
                break;
        }

        switch (reduced_tokens[2]->status) {
            case L_INT: 
                if ((second_val = to_int(load_token(&res->string_buff, reduced_tokens[2]->token_index))) < 0)
                    return SEMANTIC_ERROR;
                break;
           case IDENTIFIER:
                // TODO get value if second is id
                 break;
        }
        // TODO generate instruction
    } else if (original_type == L_DOUBLE) {
        double first_val;
        double second_val;
        
        switch (reduced_tokens[0]->status) {     // literal
            case L_INT:
                if ((first_val = to_double(load_token(&res->string_buff, reduced_tokens[0]->token_index))) < 0.0)
                    return SEMANTIC_ERROR;
                break;
            case IDENTIFIER:
            // TODO get value if first is id
            break;
        }
        switch (reduced_tokens[2]->status) { 
            case L_INT: 
                if((second_val = to_double(load_token(&res->string_buff, reduced_tokens[2]->token_index))) < 0.0)
                    return SEMANTIC_ERROR;
                break;
            case IDENTIFIER:
            // TODO get value if first is id
            break;
        }
        // TODO generate instruction
    } 

    int err = reduce(&res->struct_buff, stack, original_type, RVALUE);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to reduce");
    catch_syntax_error(err, SYNTAX_ERROR, "Failed to reduce", 1);
    return RETURN_OK;
}

int reduce(TDynamic_structure_buffer *b, TStack *stack, int original_type, char status) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    
    TToken *tmp = NULL;

    debug_print("%s\n", "TOP OF THE STACK REDUCED");
    catch_internal_error(
        dereference_structure(b, stack->top, (void**)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while(tmp->token_type != SHIFT) {
        pop(b, stack);
        if (stack->top == ZERO_INDEX)
            return SYNTAX_ERROR;
      
        catch_internal_error(
            dereference_structure(b, stack->top, (void**)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
      
    }
    return overwrite_top(b, stack, RVALUE, original_type, status);
}

int overwrite_top(TDynamic_structure_buffer *b, TStack *stack, int new_type, int original_type, char status) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    debug_print("%s: %s\n", "TOP OVERWRITTEN TO", symbols[new_type]); 
    debug_print("%s: %d\n", "ORIGINAL_TYPE", original_type); 
    TToken *tmp = NULL;
    catch_internal_error(
        dereference_structure(b, stack->top, (void**)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    tmp->original_type = original_type;
    tmp->token_type = new_type;   
    tmp->status = status;   
    return RETURN_OK;
}

int get_rule(Resources *res, TStack *stack) {
    args_assert(res != NULL && stack != NULL, INTERNAL_ERROR);
    
    int values [MAX_RULE_LENGTH + 1];
    int i;
    TToken *reduced_tokens[MAX_RULE_LENGTH];
    int original_type;
    char status;
    int err = get_types(&res->struct_buff, stack, values);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get types of token");
    catch_syntax_error(err, SYNTAX_ERROR, "Failed to get types of token", 1);

#if DEBUG
    char *short_rule_symbol[2];
    short_rule_symbol[0] = "id";
    short_rule_symbol[1] = "li";

    char * long_rule_symbol[11];
    long_rule_symbol[0] = "RVALUE == RVALUE -> RVALUE";
    long_rule_symbol[1] = "RVALUE > RVALUE -> RVALUE";
    long_rule_symbol[2] = "RVALUE < RVALUE -> RVALUE";
    long_rule_symbol[3] = "RVALUE >= RVALUE -> RVALUE";
    long_rule_symbol[4] = "RVALUE <= RVALUE -> RVALUE";
    long_rule_symbol[5] = "RVALUE + RVALUE -> RVALUE";
    long_rule_symbol[6] = "RVALUE - RVALUE -> RVALUE";
    long_rule_symbol[7] = "RVALUE * RVALUE -> RVALUE";
    long_rule_symbol[8] = "RVALUE / RVALUE -> RVALUE";
    long_rule_symbol[9] = "RVALUE != RVALUE -> RVALUE";
    long_rule_symbol[10] = "( RVALUE ) -> RVALUE";
#endif

    if (values[0] == 1) {
        for (i = 0; i < 2; i++) {
            if (short_rules[i] == values[1]) {
#if DEBUG
                debug_print("%s %s %s\n", "APPLIED RULE:", short_rule_symbol[i], "-> RVALUE");
#endif
                catch_internal_error(
                    dereference_structure(&res->struct_buff, stack->top, (void **)&reduced_tokens[0]),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );

                original_type = get_original_type(reduced_tokens[0]);

                if (original_type == IDENTIFIER) {
                    debug_print("%s\n", "TOKEN ID CHECKING TYPE");

                    err = is_var_declared(res, reduced_tokens[0]->token_index);
                    catch_internal_error(err, INTERNAL_ERROR, "Failed to check if var was declared");
                    catch_undefined_error(err, SEMANTIC_ERROR, "Failed to check if var was declared", 1);
                    original_type = get_var_type(res, reduced_tokens[0]->token_index);
                    catch_internal_error(original_type, INTERNAL_ERROR, "Failed to get variable type");
                    catch_undefined_error(original_type, SEMANTIC_ERROR, "Failed to get variable type", 1);
                    debug_print("%s\n", "SETTING STATUS TO IDENTIFIER");
                    status = IDENTIFIER;
                } else  {
                    debug_print("%s\n", "SETTING STATUS TO IDENTIFIER");
                    status = L_INT;
                }

                debug_print("%s: %d\n", "ORIGINAL_TYPE:", original_type);
                catch_internal_error(short_reduction(&res->struct_buff, stack, original_type, status),
                                     INTERNAL_ERROR, "Failed to reduce");
                return RETURN_OK;

            }
        }
        return SYNTAX_ERROR;
    } else if (values[0] == 3) {
        for (i = 0; i < 11; i++) {
            if (!(memcmp(long_rules[i], &values[1], sizeof(long_rules[i])))) {
#if DEBUG
                debug_print("%s %s\n", "APPLIED RULE:", long_rule_symbol[i]);
#endif

                catch_internal_error(
                     dereference_structure(&res->struct_buff, stack->top, (void **)&reduced_tokens[2]),
                     INTERNAL_ERROR,
                     "Failed to dereference structure buffer."
                 );

                catch_internal_error(
                     dereference_structure(&res->struct_buff, reduced_tokens[2]->next, (void **)&reduced_tokens[1]),
                     INTERNAL_ERROR,
                     "Failed to dereference structure buffer."
                 );
                 catch_internal_error(
                     dereference_structure(&res->struct_buff, reduced_tokens[1]->next, (void **)&reduced_tokens[0]),
                     INTERNAL_ERROR,
                     "Failed to dereference structure buffer."
                 );
 
                 if (i == 10)       // rule ( RVALUE )
                     original_type = get_original_type(reduced_tokens[1]);
                 else {

                     err = check_tokens(res, reduced_tokens[1]->next, stack->top);
                     
                     switch(err) {
                         case INTERNAL_ERROR:
                               return INTERNAL_ERROR;
                               break;

                         case SEMANTIC_ERROR:
                             debug_print("%s\n", "SEMANTIC_ERROR");
                             return err;
                             break;
                         
                         case TYPE_ERROR:
                             debug_print("%s\n", "TYPE_ERROR");
                             return err;
                             break;
                        
                         case TYPE_CAST_FIRST:
                             original_type = get_original_type(reduced_tokens[2]);
                             debug_print("%s\n", "TYPE CAST FIRST");
                             break;
                        
                         case TYPE_CAST_SECOND:
                             original_type = get_original_type(reduced_tokens[0]);
                             debug_print("%s\n", "TYPE CAST SECOND");
                             break;
                        
                         case RETURN_OK:
                             original_type = get_original_type(reduced_tokens[0]);
                             debug_print("%s\n", "TYPES OK");
                         default:
                             original_type = get_original_type(reduced_tokens[0]);
                     }

                 }
                 return long_reduction(res, reduced_tokens, stack, original_type);
            }
        }
        return SYNTAX_ERROR;
    } else 
        return SYNTAX_ERROR;

}



int check_expression(Resources *res, TToken **last_token, index_t *last_index) {
    args_assert(res != NULL, INTERNAL_ERROR);

    TToken *input_token = NULL;
    TToken *top_token = NULL;
    TToken *tmp = NULL;
    index_t top_index = ZERO_INDEX;
    index_t input_index = ZERO_INDEX;
    TStack stack;
    int iRet;
    int err;
    int return_type;

    init_stack(&stack);

    new_item(&res->struct_buff, top_index, top_token);
    top_token->token_type = END_OF_EXPR;
    push(&res->struct_buff, &stack, top_index); // $ on top of the stack

    if ((*last_token) != NULL) 
        input_index = *last_index;
    else 
        input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
    
    catch_internal_error(
        dereference_structure(&res->struct_buff, input_index, (void **)&input_token),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    
        
    catch_internal_error(
        dereference_structure(&res->struct_buff, top_index, (void **)&top_token),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    do {
         //print_stack(&res->struct_buff, &stack);
         debug_print("%s %d\n", "TOP", top_token->token_type);
         debug_print("%s %d\n", "INPUT", input_token->token_type);
        
        if (top_token->token_type == IDENTIFIER 
            && input_token->token_type == OPENING_BRACKET) {
            debug_print("%s\n", "FUNCTION CALL IN EXPR");
            
            catch_undefined_error(is_func_declared(res, top_token->token_index),
                                 SEMANTIC_ERROR,
                                 "Function declaration check failed.", 1
            );
            return_type = get_return_type(res, top_token->token_index);
            catch_internal_error(return_type, SYNTAX_ERROR, "Failed to get function return type.");

            dereference_structure(&res->struct_buff, input_index, (void **)last_token);
            iRet = check_syntax(FUNC_CALL, res);
            if (iRet != RETURN_OK) {
                debug_print("%s: %d\n", "RETURN", iRet);
                return iRet;
            } else {
                // Reduction of function call
                err = reduce(&res->struct_buff, &stack, return_type, RVALUE);
                catch_internal_error(err, INTERNAL_ERROR, "Failed to reduce");
                catch_syntax_error(err, SYNTAX_ERROR, "Failed to reduce",1);
                top_index = stack.top;
                catch_syntax_error(
                    get_first_token(&res->struct_buff, &stack, &top_index),
                    INTERNAL_ERROR,
                    "Failed to get first token", 1
                );
                input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
                catch_internal_error(
                    dereference_structure(&res->struct_buff, input_index, (void **)&input_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );
                catch_internal_error(
                    dereference_structure(&res->struct_buff, top_index, (void **)&top_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );
                if (type_filter(top_token->token_type) == END_OF_EXPR &&
                    type_filter(input_token->token_type) == END_OF_EXPR)
                    break;

            }
        }

        switch(precedence_table[type_filter(top_token->token_type)]
                               [type_filter(input_token->token_type)]) {
            case H:
                debug_print("%s\n", "CASE H");
                top_index = input_index;
                push(&res->struct_buff, &stack, top_index);
                input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
                catch_internal_error(
                    dereference_structure(&res->struct_buff, input_index, (void **)&input_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );
                catch_internal_error(
                    dereference_structure(&res->struct_buff, top_index, (void **)&top_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );

                break;

            case S:
                debug_print("%s\n", "CASE S");
                new_item(&res->struct_buff, top_index, top_token);
                catch_internal_error(
                    dereference_structure(&res->struct_buff, stack.top, (void **)&tmp),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );

                top_token->token_type = SHIFT;

                if (tmp->token_type == RVALUE) {
                    index_t rvalue_index = stack.top;
                    pop(&res->struct_buff, &stack);
                    push(&res->struct_buff, &stack, top_index);
                    push(&res->struct_buff, &stack, rvalue_index);

                } else
                    push(&res->struct_buff, &stack, top_index);
                
                catch_internal_error(
                    dereference_structure(&res->struct_buff, input_index, (void **)&input_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );

                top_index = input_index;
                push(&res->struct_buff, &stack, top_index);
                input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
                catch_internal_error(
                    dereference_structure(&res->struct_buff, input_index, (void **)&input_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );
                catch_internal_error(
                    dereference_structure(&res->struct_buff, top_index, (void **)&top_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );
                break;
            
            case R:
                debug_print("%s\n", "CASE R");
                err = get_rule(res, &stack);
                catch_internal_error(err, INTERNAL_ERROR, "Failed to get rule");
                catch_syntax_error(err, SYNTAX_ERROR, "Failed to get rule", 1);
                catch_compatibility_error(err, TYPE_ERROR, "Failed to get rule", 1);
                catch_undefined_error(err, SEMANTIC_ERROR, "Failed to get rule", 1);
                
                top_index = stack.top;
                
                catch_syntax_error(
                    get_first_token(&res->struct_buff, &stack, &top_index),
                    INTERNAL_ERROR,
                    "Failed to get first token", 1
                );
                catch_internal_error(
                    dereference_structure(&res->struct_buff, top_index, (void **)&top_token),
                    INTERNAL_ERROR,
                    "Failed to dereference structure buffer."
                );
                break;
 
            case E:
                debug_print("%s\n", "CASE E");
                if (type_filter(top_token->token_type) == END_OF_EXPR && 
                    type_filter(input_token->token_type) == CLOSING_BRACKET) {
                    catch_internal_error(
                         dereference_structure(&res->struct_buff, input_index, (void **)last_token),
                         INTERNAL_ERROR,
                         "Failed to dereference structure buffer."
                    );
                    
                    catch_internal_error(
                         dereference_structure(&res->struct_buff, stack.top, (void **)&top_token),
                         INTERNAL_ERROR,
                         "Failed to dereference structure buffer."
                    );
                    debug_print("%s: %d\n", "TYPE OF EXPRESSION", top_token->original_type);
                    // send type of expression back to syntax_analysis
                    (*last_token)->original_type = top_token->original_type;
                
                    debug_print("%s: %d\n", "RETURN", RETURN_OK);
                    return RETURN_OK;

                }

                return SYNTAX_ERROR;
            
            default:
                debug_print("%s", "DEFAULT\n");
                return INTERNAL_ERROR;
        }
        // print_stack(&res->struct_buff, &stack);
        // fprintf(stderr, "top %s\n", symbols[top_token->token_type]);
        // fprintf(stderr, "input %s\n", symbols[input_token->token_type]);
                  
    } while (type_filter(top_token->token_type) != END_OF_EXPR || type_filter(input_token->token_type) != END_OF_EXPR);
    
    catch_internal_error(
         dereference_structure(&res->struct_buff, input_index, (void **)last_token),
         INTERNAL_ERROR,
         "Failed to dereference structure buffer."
    );
    
    catch_internal_error(
         dereference_structure(&res->struct_buff, stack.top, (void **)&top_token),
         INTERNAL_ERROR,
         "Failed to dereference structure buffer."
    );
    debug_print("%s: %d\n", "TYPE OF EXPRESSION", top_token->original_type);
    // send type of expression back to syntax_analysis
    (*last_token)->original_type = top_token->original_type;

    debug_print("%s: %d\n", "RETURN", RETURN_OK);
    return RETURN_OK;
}

