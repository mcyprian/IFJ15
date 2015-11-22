/**
 * @file sematics.c
 * @author Tomáš Ščavnický <xscavn00@stud.fit.vutbr.cz> Tomáš Sýkora <xsykor25@stud.fit.vutbr.cz> Michal Ďurista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Sematic analysis of IFJ15 language
 */

// TODO: check for internal error

#include <sematics.h>

static index_t currently_analyzed_function = 0;


int enter_scope(Resources *resources, TStack *stack)
{
    TTree *tmp;
    index_t i = 0;

    add_char(&(resources->string_buff), '$');
    declare_variable(resources, save_token(&(resources->string_buff)), &i, NO_DATA_TYPE);
    dereference_structure(&(resources->struct_buff_trees), i, (void **)&tmp);
    tmp->index_to_struct_buffer = i;
    push(&(resources->struct_buff_trees), stack, i);
    
    return 0;
}

int leave_scope(Resources *resources, TStack *stack)
{
    // TTree *tmp;
    // dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);
    
    // free_memory(&resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), stack);

    return 0;
}

int leave_general_scope(Resources *resources, TStack *stack) {
    TTree *tmp;
    dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);
    
    free_memory(resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), stack);

    return 0;
}

int declare_func(Resources *resources, TStack *stack, index_t index_to_string_buff, int return_type) // TODO: declaration test
{
    currently_analyzed_function = index_to_string_buff;
    index_t i;

    switch (is_func_declared(resources, stack, index_to_string_buff, return_type)) {
        case 1:
            printf("function was declared\n");
            return 0;
        case 0:
            i = stack->top;
            declare_function(resources, index_to_string_buff, &i, return_type);
            return 1;
        case -1:
            return -1; // odsledovat v synt an pre hlasenie sem. chyby
        default:
            return -1;
    }
}

int declare_var(Resources *resources, TStack *stack, index_t index_to_string_buff, int data_type)
{
    index_t i = stack->top;
    declare_variable(resources, index_to_string_buff, &i, data_type);

    return 0;
}

int add_arg(Resources *resources, TStack *stack, index_t name_of_arg, int data_type)
{
    index_t i = stack->top;
    add_func_arg(resources, currently_analyzed_function, i, data_type, name_of_arg);

    return 0;
}

int check_arg_declaration(Resources *resources, TStack *stack, index_t expected_name_of_arg, int expected_arg_type, int argi) 
{
    index_t i = stack->top;

    int actual_arg_type;
    index_t actual_name_of_arg;
    load_arg(resources, i, currently_analyzed_function, argi, &actual_name_of_arg, &actual_arg_type);
    
    if ((expected_arg_type == actual_arg_type) && (expected_name_of_arg == actual_name_of_arg))
        return 0;
    else
        return 1;

}

int check_argc(Resources *resources, TStack *stack, int expected_argc)
{
    index_t i = stack->top;
    int actual_argc;
    load_num_of_args(resources, i, currently_analyzed_function, &actual_argc);
    if (expected_argc == actual_argc)
        return 0;
    else
        return 1;
}

/**
is_deckared:
function was declared with same return type -> 1
function was declared with different return type -> -1 = sematic error
function was not declared -> 0
*/
int is_func_declared(Resources *resources, TStack *stack, index_t name_of_func, int return_type) 
{
    TTree *tmp;
    int is_declared = 0;
    currently_analyzed_function = name_of_func;
    dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree
    if (!declaration_test(resources, name_of_func, tmp->index_to_struct_buffer, FUNC)){ // was declared 0 else 1
        if (return_type == get_data_type(resources, tmp->index_to_struct_buffer, name_of_func, FUNC))
            is_declared = 1;
        else
            is_declared = -1;
    }

    return is_declared;
}

int is_var_declared(Resources *resources, TStack *stack, index_t name_of_var) {
    TTree *tmp;
    int is_declared = 1;
    dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);

    for (int i = stack->length - 1; i > 0; i--) {
        if (is_declared = declaration_test(resources, name_of_var, tmp->index_to_struct_buffer, VAR)) // is declared 0 else 1
            break;
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }

    return is_declared; // is declared 0 else 1
}

int check_return_type(Resources *resources, TStack *stack, index_t func_name, int expected_data_type) {
    TTree *tmp;
    int actual_data_type;
    dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // while; tmp is global scope tree
    actual_data_type = get_data_type(resources, tmp->index_to_struct_buffer, func_name, FUNC);

    return (actual_data_type == expected_data_type);
}

int check_var_type(Resources *resources, TStack *stack, index_t var_name, int expected_type)
{
    TTree *tmp;

    dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);
    for (int i = stack->length - 1; i > 0; i--) {
        if (expected_type == get_data_type(resources, tmp->index_to_struct_buffer, var_name, VAR))
            return 0;
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }

    return 1;
}

int define_func(Resources *resources, TStack *stack)
{
    if (check_definition_flag(resources, stack->top, currently_analyzed_function))
        return -1; // sematicka chyba - dve definicia jednej funkcie

    int argc;
    int data_type;
    index_t name;
    
    set_definition_flag(resources, stack->top, currently_analyzed_function);
    enter_scope(resources, stack);
    
    load_num_of_args(resources, stack->top, currently_analyzed_function, &argc);
    
    for(int i = argc; i > 0; i--) {
        load_arg(resources, i, currently_analyzed_function, i, &name, &data_type);
        declare_variable(resources, name, &currently_analyzed_function, data_type);
    }

    return 0;
}





// int set_value_var(Resources *resources, char *id, int data_type) {

// }
