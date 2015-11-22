/**
 * @file sematics.c
 * @author Tomáš Ščavnický <xscavn00@stud.fit.vutbr.cz> Tomáš Sýkora <xsykor25@stud.fit.vutbr.cz> Michal Ďurista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Sematic analysis of IFJ15 language
 */

// TODO: check for internal error, fcia na vypisi zo zasobniku, 

#include <semantics.h>

static index_t currently_analyzed_function = 0;

int enter_scope(Resources *resources)
{
    TTree *tmp;
    index_t i = 0;

    add_char(&(resources->string_buff), '$');
    declare_variable(resources, save_token(&(resources->string_buff)), &i, NO_DATA_TYPE);
    dereference_structure(&(resources->struct_buff_trees), i, (void **)&tmp);
    tmp->index_to_struct_buffer = i;
    push(&(resources->struct_buff_trees), resources->stack, i);
    
    return 0;
}

int leave_scope(Resources *resources)
{
    // TTree *tmp;
    // dereference_structure(&(resources->struct_buff_trees), stack->top, (void **)&tmp);
    
    // free_memory(&resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), resources->stack);

    return 0;
}

int leave_general_scope(Resources *resources) {
    TTree *tmp;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);
    
    free_memory(resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), stack);

    return 0;
}

int declare_func(Resources *resources, index_t index_to_string_buff, int return_type) // TODO: declaration test
{
    currently_analyzed_function = index_to_string_buff;
    index_t i;

    switch (is_func_declared(resources, resources->stack, index_to_string_buff, return_type)) {
        case 1:
            printf("function was declared\n");
            return 0;
        case 0:
            i = resources->stack.top;
            printf("function was not declared\n");
            declare_function(resources, index_to_string_buff, &i, return_type);
            return 1;
        case -1:
            printf("wrong declaration of function\n");
            return -1; // odsledovat v synt an pre hlasenie sem. chyby
        default:
            return -1;
    }
}

int declare_var(Resources *resources, index_t index_to_string_buff, int data_type)
{
    index_t i = resources->stack.top;
    declare_variable(resources, index_to_string_buff, &i, data_type);

    return 0;
}

int add_arg(Resources *resources, index_t name_of_arg, int data_type)
{
    index_t i = resources->stack.top;
    add_func_arg(resources, currently_analyzed_function, i, data_type, name_of_arg);

    return 0;
}

int check_arg_declaration(Resources *resources, index_t expected_name_of_arg, int expected_arg_type, int argi) 
{
    index_t i = resources->stack.top;

    int actual_arg_type;
    index_t actual_name_of_arg;
    load_arg(resources, i, currently_analyzed_function, argi, &actual_name_of_arg, &actual_arg_type);
    
    if ((expected_arg_type == actual_arg_type) && (expected_name_of_arg == actual_name_of_arg))
        printf("arg declared ok\n");
        return 0;
    else
        printf("arg declared wrong\n");
        return 1;

}

int check_argc(Resources *resources, int expected_argc)
{
    index_t i = resources->stack.top;
    int actual_argc;
    load_num_of_args(resources, i, currently_analyzed_function, &actual_argc);
    if (expected_argc == actual_argc)
        printf("argc ok\n");
        return 0;
    else
        printf("argc wrong\n");
        return 1;
}

/**
is_deckared:
function was declared with same return type -> 1
function was declared with different return type -> -1 = sematic error
function was not declared -> 0
*/
int is_func_declared(Resources *resources, index_t name_of_func, int return_type) 
{
    TTree *tmp;
    int is_declared = 0;
    currently_analyzed_function = name_of_func;
    
    printf("%s\n", load_token(resources->string_buff, currently_analyzed_function));
    printf("%s\n", load_token(resources->string_buff, name_of_func));

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree
    if (!declaration_test(resources, name_of_func, tmp->index_to_struct_buffer, FUNC)){ // was declared 0 else 1
        printf("func with this name was declared\n");
        if (return_type == get_data_type(resources, tmp->index_to_struct_buffer, name_of_func, FUNC))
            printf("func with this name and return type was declared\n");
            is_declared = 1;
        else
            printf("wrong return types of func\n");
            is_declared = -1;
    }
    printf("func was not declared\n");
    return is_declared;
}

int is_var_declared(Resources *resources, index_t name_of_var) {
    TTree *tmp;
    int is_declared = 1;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    for (int i = resources->stack.length - 1; i > 0; i--) {
        if (is_declared = declaration_test(resources, name_of_var, tmp->index_to_struct_buffer, VAR)) // is declared 0 else 1
            printf("var was declared\n");
            break;
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }
    printf("var was not declared\n");

    return is_declared; // is declared 0 else 1
}

int check_return_type(Resources *resources, index_t func_name, int expected_data_type) {
    TTree *tmp;
    int actual_data_type;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // while; tmp is global scope tree
    actual_data_type = get_data_type(resources, tmp->index_to_struct_buffer, func_name, FUNC);

    if (actual_data_type == expected_data_type)
        printf("same return types\n");
    else
        printf("wrong return types\n");

    return (actual_data_type == expected_data_type);
}

int check_var_type(Resources *resources, index_t var_name, int expected_type)
{
    TTree *tmp;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);
    for (int i = resources->stack.length - 1; i > 0; i--) {
        if (expected_type == get_data_type(resources, tmp->index_to_struct_buffer, var_name, VAR)) {
            printf("same var types\n");
            return 0;
        }
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }

    printf("wrong var types\n");
    
    return 1;
}

int define_func(Resources *resources)
{
    if (check_definition_flag(resources, resources->stack.top, currently_analyzed_function))
        return -1; // sematicka chyba - dve definicia jednej funkcie

    int argc;
    int data_type;
    index_t name;
    
    set_definition_flag(resources, resources->stack.top, currently_analyzed_function);
    enter_scope(resources, resources->stack);
    
    load_num_of_args(resources, resources->stack.top, currently_analyzed_function, &argc);
    
    for(int i = argc; i > 0; i--) {
        load_arg(resources, i, currently_analyzed_function, i, &name, &data_type);
        declare_variable(resources, name, &currently_analyzed_function, data_type);
    }

    return 0;
}





// int set_value_var(Resources *resources, char *id, int data_type) {

// }
