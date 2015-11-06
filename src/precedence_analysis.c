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
#include <resources.h>
#include <datatypes.h>
#include <stack.h>
#include <scanner.h>
#include <token.h>


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

const int short_rules[2][1] =
{
    {IDENTIFIER},
    {L_INT}                // same value for all literals
};

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

const char* symbols[47];



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
    catch_internal_error(dereference_structure(b, stack->top, (void**)&tmp), INTERNAL_ERROR, "Failed to dereference structure buffer.");

    printf("\nStack:\n    %s  <-- stack top\n", symbols[tmp->token_type]);
    while (tmp->token_type != END_OF_EXPR) {
        if ((next = tmp->next) == ZERO_INDEX)
            return SYNTAX_ERROR;

        if (dereference_structure(b, next, (void**)&tmp) ==  INTERNAL_ERROR)
            return INTERNAL_ERROR;

        printf("    %s\n", symbols[tmp->token_type]);
    }
}
        
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
            return SYNTAX_ERROR;
        
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
    catch_internal_error(err, INTERNAL_ERROR, "Failed to get types of token");
    catch_syntax_error(err, SYNTAX_ERROR, "Failed to get types of token", 1);

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

    if (values[0] == 1) {
        for (i = 0; i < 3; i++) {
            if (short_rules[i][0] == values[1])
                printf("Applied rule: %s -> RVALUE\n", short_rule_symbol[i]);
            return RETURN_OK;
        }
        return SYNTAX_ERROR;
    } else if (values[0] == 3) {
        for (i = 0; i < 11; i++) {
           if (!(memcmp(long_rules[i], &values[1], sizeof(long_rules[i])))) {
                printf("Applied rule: %s\n", long_rule_symbol[i]);
                return RETURN_OK;
            }
        }
        return SYNTAX_ERROR;
    }
    else 
        return INTERNAL_ERROR;
}

int check_expression(Resources *res) {
    TToken *input_token = NULL;
    TToken *top_token = NULL;
    index_t top_index = ZERO_INDEX;
    index_t input_index = ZERO_INDEX;
    TStack stack;
    int err;

    init_stack(&stack);

    new_item(&res->struct_buff, top_index, top_token);
    top_token->token_type = END_OF_EXPR;
    push(&res->struct_buff, &stack, top_index); // $ on top of the stack

    input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
    dereference_structure(&res->struct_buff, input_index, (void **)&input_token);
    dereference_structure(&res->struct_buff, top_index, (void **)&top_token);

    do {
        print_stack(&res->struct_buff, &stack);
        printf("top %s\n", symbols[top_token->token_type]);
        printf("input %s\n", symbols[input_token->token_type]);

        switch(precedence_table[type_filter(top_token->token_type)]
                               [type_filter(input_token->token_type)]) {
            case H:
                printf("Case H\n");
                top_index = input_index;
                push(&res->struct_buff, &stack, top_index);
                input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
                dereference_structure(&res->struct_buff, input_index, (void **)&input_token);
                dereference_structure(&res->struct_buff, top_index, (void **)&top_token);
                break;

            case S:
                printf("Case S\n");
                new_item(&res->struct_buff, top_index, top_token);
                top_token->token_type = SHIFT;
                push(&res->struct_buff, &stack, top_index);

                dereference_structure(&res->struct_buff, input_index, (void **)&input_token);
                top_index = input_index;
                push(&res->struct_buff, &stack, top_index);
                input_index = get_token(res->source, &res->string_buff, &res->struct_buff);
                dereference_structure(&res->struct_buff, input_index, (void **)&input_token);
                dereference_structure(&res->struct_buff, top_index, (void **)&top_token);
                break;
            
            case R:
                printf("Case R\n");
                err = get_rule(&res->struct_buff, &stack);
                catch_internal_error(err, INTERNAL_ERROR, "Failed to get rule");
                catch_syntax_error(err, SYNTAX_ERROR, "Failed to get rule", 1);
                err = reduce(&res->struct_buff, &stack);
                catch_internal_error(err, INTERNAL_ERROR, "Failed to reduce");
                catch_syntax_error(err, SYNTAX_ERROR, "Failed to reduce",1);
                break;
            
            case E:
                printf("Case E\n");
                return SYNTAX_ERROR;
            
            default:
                return INTERNAL_ERROR;
        }
        print_stack(&res->struct_buff, &stack);
        printf("top %s\n", symbols[top_token->token_type]);
        printf("input %s\n", symbols[input_token->token_type]);
    } while (type_filter(top_token->token_type) != END_OF_EXPR && type_filter(input_token->token_type) != END_OF_EXPR);

    return RETURN_OK;
}
