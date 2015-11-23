/**
 * @file sematics.c
 * @author Tomáš Ščavnický <xscavn00@stud.fit.vutbr.cz> Tomáš Sýkora <xsykor25@stud.fit.vutbr.cz> Michal Ďurista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Sematic analysis of IFJ15 language
 */

// TODO: check for internal error, fcia na vypisi zo zasobniku, 
#include <datatypes.h>
#include <semantics.h>
#include <stack.h>
#include <debug.h>


static index_t currently_analyzed_function = 0;
static int arg_counter;

int enter_scope(Resources *resources)
{
    debug_print("%s\n", "ENTER_SCOPE");
    TTree *tmp;
    index_t i = 0;

    add_char(&(resources->string_buff), '$');
    printf("1\n");
    index_t test = save_token(&(resources->string_buff));
    printf("%s\n", load_token(&(resources->string_buff), test));
    declare_variable(resources, test, &i, NO_DATA_TYPE);
    printf("2\n");
    dereference_structure(&(resources->struct_buff_trees), i, (void **)&tmp);
    printf("3\n");
    tmp->index_to_struct_buffer = i;
    printf("4\n");
    push(&(resources->struct_buff_trees), &(resources->stack), i);
    
    debug_print("%s\n", "ENTER_SCOPE_RETURN_0");
    return 0;
}

int leave_scope(Resources *resources)
{
    debug_print("%s\n", "LEAVE_SCOPE");
    pop(&(resources->struct_buff_trees), &(resources->stack));

    debug_print("%s\n", "LEAVE_SCOPE_RETURN_0");
    return 0;
}

int leave_general_scope(Resources *resources) {
    debug_print("%s\n", "LEAVE_GENERAL_SCOPE");
    TTree *tmp;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);
    
    free_memory(resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), &(resources->stack));

    debug_print("%s\n", "LEAVE_GENERAL_SCOPE_RETURN_0");
    return 0;
}

int is_func_declared(Resources *resources, index_t name_of_func)
{
    debug_print("%s\n", "IS_FUNC_DECLARED_WITHRV");
    TTree *tmp;
    int is_declared = -1;
    currently_analyzed_function = name_of_func;
    arg_counter = 0;
    
    printf("%s\n", load_token(&(resources->string_buff), currently_analyzed_function));
    printf("%s\n", load_token(&(resources->string_buff), name_of_func));

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree
    if (!declaration_test(resources, name_of_func, tmp->index_to_struct_buffer, FUNC)){ // was declared 0 else -1
        printf("func with this name was declared\n");
        is_declared = 0;
    }
    
    debug_print("%s%d\n", "IS_FUNC_DECLARED_RETURN_", is_declared);
    return is_declared;
}


/**
is_deckared:
function was declared with same return type -> 1
function was declared with different return type -> -1 = sematic error
function was not declared -> 0
*/
int is_func_declared_withrv(Resources *resources, index_t name_of_func, int return_type) 
{
    debug_print("%s\n", "IS_FUNC_DECLARED_WITHRV");
    int is_declared = 0;
    currently_analyzed_function = name_of_func;
    index_t i = resources->stack.top;
    
    printf("%s\n", load_token(&(resources->string_buff), currently_analyzed_function));
    printf("%s\n", load_token(&(resources->string_buff), name_of_func));

    if (!declaration_test(resources, name_of_func, i, FUNC)){ // was declared 0 else 1
        printf("func with this name was declared\n");
        if (return_type == get_data_type(resources, resources->stack.top, name_of_func, FUNC)){
            printf("func with this name and return type was declared\n");
            is_declared = 1;
        }
        else {
            printf("wrong return types of func\n");
            is_declared = -1;
        }
    }
    printf("func was not declared\n");
    debug_print("%s%d\n", "IS_FUNC_DECLARED_WITHRV_RETURN_", is_declared);
    return is_declared;
}

int declare_func(Resources *resources, index_t index_to_string_buff, int return_type) // TODO: declaration test
{
    debug_print("%s\n", "DECLARE_FUNC");
    currently_analyzed_function = index_to_string_buff;
    index_t i;
    arg_counter = 0;

    switch (is_func_declared_withrv(resources, index_to_string_buff, sem_type_filter(return_type))) {
        case 1:
            printf("function was declared\n");
            debug_print("%s\n", "DECLARE_FUNC_RETURN_0");
            return 0;
        case 0:
            i = resources->stack.top;
            printf("function was not declared\n");
            declare_function(resources, index_to_string_buff, &i, sem_type_filter(return_type));
            debug_print("%s\n", "DECLARE_FUNC_RETURN_0");
            return 0;
        case -1:
            printf("wrong declaration of function\n");
            debug_print("%s\n", "DECLARE_FUNC_RETURN_1");
            return 1; // odsledovat v synt an pre hlasenie sem. chyby
        default:
            debug_print("%s\n", "DECLARE_FUNC_RETURN_-1");
            return -1;
    }
}

int declare_var(Resources *resources, index_t index_to_string_buff, int data_type)
{
    debug_print("%s\n", "DECLARE_VAR");
    index_t i = resources->stack.top;

    int is_declared = declaration_test(resources, index_to_string_buff, i, sem_type_filter(data_type));

    if ( is_declared == NOT_FOUND){
        declare_variable(resources, index_to_string_buff, &i, sem_type_filter(data_type));
        debug_print("%s\n", "DECLARE_VAR_RETURN_0");
        return 0;
    }
    else if ( is_declared == 0)
        debug_print("%s\n", "DECLARE_VAR_RETURN_1");
        return 1;  //semantic error double declaration
    debug_print("%s\n", "DECLARE_VAR_RETURN_INTERNALL_ERROR");
    return INTERNAL_ERROR;
}

int add_arg(Resources *resources, index_t name_of_arg, int data_type)
{
    debug_print("%s\n", "ADD_ARG");
    index_t i = resources->stack.top;
    add_func_arg(resources, currently_analyzed_function, i, data_type, name_of_arg);

    debug_print("%s\n", "ADD_ARG_RETURN_0");
    return 0;
}

int check_arg_declaration(Resources *resources, index_t expected_name_of_arg, int expected_arg_type, int argi) 
{
    debug_print("%s\n", "CHECK_ARG_DECLARATION");
    index_t i = resources->stack.top;

    int actual_arg_type, iret;
    index_t actual_name_of_arg;
    if ((iret = load_arg(resources, i, currently_analyzed_function, argi, &actual_name_of_arg, &actual_arg_type)) == NOT_FOUND)
    {
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_1");
        return 1;
    }
    
    if ((expected_arg_type == actual_arg_type) && (expected_name_of_arg == actual_name_of_arg)){
        printf("arg declared ok\n");
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_0");
        return 0;
    }
    else {
        printf("arg declared wrong\n");
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_1");
        return 1;
    }
}

int set_arg(Resources *resources, index_t name_of_arg, int data_type)
{
    debug_print("%s\n", "SET_ARG");
    index_t i = resources->stack.top;

    arg_counter++;
    if(!check_declaration_status(resources, i, currently_analyzed_function)){
        add_arg(resources, name_of_arg, sem_type_filter(data_type));
        debug_print("%s\n", "SET_ARG_RETURN_OK");
        return RETURN_OK;
    }
    else {
        if(check_arg_declaration(resources, name_of_arg, sem_type_filter(data_type), arg_counter)){
            debug_print("%s\n", "SET_ARG_RETURN_OK");
            return RETURN_OK;
        }
        else{
            debug_print("%s\n", "SET_ARG_RETURN_1");    
           return 1;   //semantic error wrong argument declaration
        }
    }
}

int check_argc(Resources *resources)
{
    debug_print("%s\n", "CHECK_ARGC");
    index_t i = resources->stack.top;
    int actual_argc;

    load_num_of_args(resources, i, currently_analyzed_function, &actual_argc);
    if (arg_counter == actual_argc){
        printf("argc ok\n");
        debug_print("%s\n", "CHECK_ARGC_RETURN_0");
        return 0;
    }
    else {
        printf("argc wrong\n");
        debug_print("%s\n", "CHECK_ARGC_RETURN_1");
        return 1;
    }
}

int is_var_declared(Resources *resources, index_t name_of_var) {
    debug_print("%s\n", "IS_VAR_DECLARED");
    TTree *tmp;
    int is_declared = 1;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    for (int i = resources->stack.length - 1; i > 0; i--) {
        if ((is_declared = declaration_test(resources, name_of_var, tmp->index_to_struct_buffer, VAR)) == 0){ // is declared 0 else 1
            printf("var was declared\n");
            break;
        }
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }
    
    debug_print("%s%d\n", "IS_VAR_DECLARED", is_declared);
    return is_declared; // is declared 0 else 1
}

int check_return_type(Resources *resources, index_t func_name, int expected_data_type) {
    debug_print("%s\n", "CHECK_RETURN_TYPE");
    TTree *tmp;
    int actual_data_type;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // while; tmp is global scope tree
    actual_data_type = get_data_type(resources, tmp->index_to_struct_buffer, func_name, FUNC);

    if (actual_data_type == sem_type_filter(expected_data_type))
        printf("same return types\n");
    else
        printf("wrong return types\n");

    return (actual_data_type == sem_type_filter(expected_data_type));
}

int check_var_type(Resources *resources, index_t var_name, int expected_type)
{
    debug_print("%s\n", "CHECK_VAR_TYPE");
    TTree *tmp;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);
    for (int i = resources->stack.length - 1; i > 0; i--) {
        if (sem_type_filter(expected_type) == get_data_type(resources, tmp->index_to_struct_buffer, var_name, VAR)) {
            printf("same var types\n");
            debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_0");
            return 0;
        }
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }

    printf("wrong var types\n");
    
    debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_1");
    return 1;
}

int define_func(Resources *resources)
{
    debug_print("%s\n", "DEFINE_FUNC");
    if (check_definition_flag(resources, resources->stack.top, currently_analyzed_function))
        return -1; // sematicka chyba - dve definicia jednej funkcie

    int argc;
    int data_type;
    index_t name;
    
    set_definition_flag(resources, resources->stack.top, currently_analyzed_function);
    enter_scope(resources);
    
    load_num_of_args(resources, resources->stack.top, currently_analyzed_function, &argc);
    
    for(int i = argc; i > 0; i--) {
        load_arg(resources, i, currently_analyzed_function, i, &name, &data_type);
        declare_variable(resources, name, &currently_analyzed_function, data_type);
    }

    debug_print("%s\n", "DEFINE_FUNC_RETURN_0");
    return 0;
}





// int set_value_var(Resources *resources, char *id, int data_type) {

// }
