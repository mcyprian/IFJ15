/**
 * F_EXPR 
 * @file 

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
#include <resources.h>
#include <datatypes.h>
#include <stack.h>
#include <scanner.h>
#include <token.h>
#include <syntax_analysis.h>


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
    printf("\nStack:\n    %s  <-- stack top\n", symbols[type_filter(tmp->token_type)]);
    while (tmp->token_type != END_OF_EXPR) {
        if ((next = tmp->next) == ZERO_INDEX)
            return SYNTAX_ERROR;

        catch_internal_error(
            dereference_structure(b, next, (void**)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer"
        );

        printf("    %s\n", symbols[tmp->token_type]);
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

int overwrite_top(TDynamic_structure_buffer *b, TStack *stack, int new_type) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    debug_print("%s: %s\n", "TOP OVERWRITTEN TO", symbols[new_type]); 
    
    TToken *tmp = NULL;
    catch_internal_error(
        dereference_structure(b, stack->top, (void**)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    // TODO set to fce return type when reducing fce call
    tmp->original_type = tmp->token_type;
    tmp->token_type = new_type;    
    return RETURN_OK;
}

int reduce(TDynamic_structure_buffer *b, TStack *stack) {
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
    return overwrite_top(b, stack, RVALUE);
}

int get_rule(TDynamic_structure_buffer *b, TStack *stack) {
    args_assert(b != NULL && stack != NULL, INTERNAL_ERROR);
    
    int values [MAX_RULE_LENGTH + 1];
    int i;
    int err = get_types(b, stack, values);
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get types of token");
    catch_syntax_error(err, SYNTAX_ERROR, "Failed to get types of token", 1);

#if DEBUG
    char *short_rule_symbol[2];
    short_rule_symbol[0] = "id";    // TODO remove after debug
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
                printf("Applied rule: %s -> RVALUE\n", short_rule_symbol[i]);
                debug_print("%s %s %s\n", "APPLIED RULE:", short_rule_symbol[i], "-> RVALUE\n");
#endif
                return RETURN_OK;
            }
        }
        return SYNTAX_ERROR;
    } else if (values[0] == 3) {
        for (i = 0; i < 11; i++) {
           if (!(memcmp(long_rules[i], &values[1], sizeof(long_rules[i])))) {
#if DEBUG
             //   printf("Applied rule: %s\n", long_rule_symbol[i]);
                debug_print("%s %s\n", "APPLIED RULE:", long_rule_symbol[i]);
#endif
                return RETURN_OK;
            }
        }
        return SYNTAX_ERROR;
    }
    else 
        return SYNTAX_ERROR;
}

int check_expression(Resources *res, TToken **last_token, index_t *last_index) {
    args_assert(res != NULL, INTERNAL_ERROR);

    TToken *input_token = NULL;
    TToken *top_token = NULL;
    TToken *tmp = NULL;
    index_t top_index = ZERO_INDEX;
    index_t input_index = ZERO_INDEX;
    int iRet;
    TStack stack;
    int err;

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
        print_stack(&res->struct_buff, &stack);
        printf("top %d\n", top_token->token_type);
        printf("input %d\n", input_token->token_type);
        
        if (top_token->token_type == IDENTIFIER 
            && input_token->token_type == OPENING_BRACKET) {
            printf("FUNCTION CALL IN EXPR\n");
            debug_print("%s\n", "FUNCTION CALL IN EXPR\n");
            dereference_structure(&res->struct_buff, input_index, (void **)last_token);
            iRet = check_syntax(FUNC_CALL, res);
            if (iRet != RETURN_OK) {
                debug_print("%s: %d\n", "RETURN", iRet);
                return iRet;
            } else {
                // Reduction of function call
                err = reduce(&res->struct_buff, &stack);
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
                if (tmp->token_type == RVALUE) {
                   catch_internal_error(
                       overwrite_top(&res->struct_buff, &stack, SHIFT),
                       INTERNAL_ERROR,
                       "Failed to overwrite_top"
                   );
                    top_token->token_type = RVALUE;
                } else
                    top_token->token_type = SHIFT;
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
                err = get_rule(&res->struct_buff, &stack);
                catch_internal_error(err, INTERNAL_ERROR, "Failed to get rule");
                catch_syntax_error(err, SYNTAX_ERROR, "Failed to get rule", 1);
                err = reduce(&res->struct_buff, &stack);
                catch_internal_error(err, INTERNAL_ERROR, "Failed to reduce");
                catch_syntax_error(err, SYNTAX_ERROR, "Failed to reduce",1);
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
                    dereference_structure(&res->struct_buff, input_index, (void **)last_token);
                    return RETURN_OK;
                }

                return SYNTAX_ERROR;
            
            default:
                debug_print("%s", "DEFAULT\n");
                return INTERNAL_ERROR;
        }
       // print_stack(&res->struct_buff, &stack);
       // printf("top %s\n", symbols[top_token->token_type]);
       // printf("input %s\n", symbols[input_token->token_type]);
                  
    } while (type_filter(top_token->token_type) != END_OF_EXPR || type_filter(input_token->token_type) != END_OF_EXPR);
    dereference_structure(&res->struct_buff, input_index, (void **)last_token);
    debug_print("%s: %d\n", "RETURN", RETURN_OK);
    return RETURN_OK;
}


