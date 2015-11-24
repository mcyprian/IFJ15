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


static index_t currently_analyzed_function;
static int arg_counter;

int enter_scope(Resources *resources)
{
    debug_print("%s\n", "ENTER_SCOPE");
    TTree *tmp;
    index_t i = 0;

    add_char(&(resources->string_buff), '$');
    index_t test = save_token(&(resources->string_buff));
    declare_variable(resources, test, &i, NO_DATA_TYPE);
    dereference_structure(&(resources->struct_buff_trees), i, (void **)&tmp);
    tmp->index_to_struct_buffer = i;
    push(&(resources->struct_buff_trees), &(resources->stack), i);
    
    debug_print("%s\n", "ENTER_SCOPE_RETURN_0");
    return RETURN_OK;
}

int leave_scope(Resources *resources)
{
    debug_print("%s\n", "LEAVE_SCOPE");
    pop(&(resources->struct_buff_trees), &(resources->stack));
    debug_print("%s\n", "LEAVE_SCOPE_RETURN_0");
    return RETURN_OK;
}

int leave_general_scope(Resources *resources) {
    debug_print("%s\n", "LEAVE_GENERAL_SCOPE");
    TTree *tmp;
    
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);
    free_memory(resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), &(resources->stack));

    debug_print("%s\n", "LEAVE_GENERAL_SCOPE_RETURN_0");
    return RETURN_OK;
}

int is_func_declared(Resources *resources, index_t name_of_func)
{
    debug_print("%s\n", "IS_FUNC_DECLARED_WITHRV");
    TTree *tmp;
    int is_declared = NOT_FOUND;
    currently_analyzed_function = name_of_func;
    arg_counter = 0;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree
    if (!declaration_test(resources, name_of_func, tmp->index_to_struct_buffer, FUNC)) // was declared 0 else -1
        is_declared = RETURN_OK;
    
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
    int is_declared = NOT_FOUND;
    currently_analyzed_function = name_of_func;
    index_t i = resources->stack.top;

    if (!declaration_test(resources, name_of_func, i, FUNC)) {
        if (return_type == get_data_type(resources, resources->stack.top, name_of_func, FUNC))
            is_declared = RETURN_OK;
        else
            is_declared = SEMANTIC_ERROR;
    }

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
        case RETURN_OK:
            debug_print("%s\n", "DECLARE_FUNC_RETURN_0");
            return RETURN_OK;
        case NOT_FOUND:
            i = resources->stack.top;
            declare_function(resources, index_to_string_buff, &i, sem_type_filter(return_type));
            debug_print("%s\n", "DECLARE_FUNC_RETURN_0");
            return RETURN_OK;
        case SEMANTIC_ERROR:
            debug_print("%s\n", "DECLARE_FUNC_RETURN_1");
            return SEMANTIC_ERROR; // odsledovat v synt an pre hlasenie sem. chyby
        default:
            debug_print("%s\n", "DECLARE_FUNC_RETURN_-1");
            return INTERNAL_ERROR;
    }
}

int declare_var(Resources *resources, index_t index_to_string_buff, int data_type)
{
    debug_print("%s\n", "DECLARE_VAR");
    index_t i = resources->stack.top;
    int is_declared = declaration_test(resources, index_to_string_buff, i, VAR);

    switch (is_declared) {
        case NOT_FOUND: 
            declare_variable(resources, index_to_string_buff, &i, sem_type_filter(data_type));
            debug_print("%s\n", "DECLARE_VAR_RETURN_0");
            return RETURN_OK;
        case FOUND:
            debug_print("%s\n", "DECLARE_VAR_RETURN_1");
            return SEMANTIC_ERROR;  //semantic error double declaration
        default:
            debug_print("%s\n", "DECLARE_VAR_RETURN_INTERNALL_ERROR");
            return INTERNAL_ERROR;
    }
}

int add_arg(Resources *resources, index_t name_of_arg, int data_type)
{
    debug_print("%s\n", "ADD_ARG");
    index_t i = resources->stack.top;
    add_func_arg(resources, currently_analyzed_function, i, data_type, name_of_arg);
    debug_print("%s\n", "ADD_ARG_RETURN_0");
    return RETURN_OK;
}

int check_arg_declaration(Resources *resources, index_t expected_name_of_arg, int expected_arg_type, int argi) 
{
    debug_print("%s\n", "CHECK_ARG_DECLARATION");
    index_t i = resources->stack.top;

    int actual_arg_type, iRet;
    index_t actual_name_of_arg;
    if ((iRet = load_arg(resources, i, currently_analyzed_function, argi, &actual_name_of_arg, &actual_arg_type)) == NOT_FOUND)
    {
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
    }
    
    if ((expected_arg_type == actual_arg_type) && (expected_name_of_arg == actual_name_of_arg)){
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_OK");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
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
            debug_print("%s\n", "SET_ARG_RETURN_TYPE_ERROR");    
           return TYPE_ERROR;
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
        debug_print("%s\n", "CHECK_ARGC_RETURN_0");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n", "CHECK_ARGC_RETURN_1");
        return TYPE_ERROR;
    }
}

int is_var_declared(Resources *resources, index_t name_of_var) {
    debug_print("%s\n", "IS_VAR_DECLARED");
    TTree *tmp;
    int is_declared = SEMANTIC_ERROR;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    for (int i = resources->stack.length - 1; i > 0; i--) {
        if ((is_declared = declaration_test(resources, name_of_var, tmp->index_to_struct_buffer, VAR)) == 0){ // is declared 0 else 1
            is_declared = RETURN_OK;
            break;
        }
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }
    
    debug_print("%s%d\n", "IS_VAR_DECLARED", is_declared);
    return is_declared;
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

    return (actual_data_type == sem_type_filter(expected_data_type) ? RETURN_OK : TYPE_ERROR);
}

int check_var_type(Resources *resources, index_t var_name, int expected_type)
{
    debug_print("%s\n", "CHECK_VAR_TYPE");
    TTree *tmp;
    int i;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);
    for (int j = resources->stack.length - 1; j > 0; j--) {
        if ((i = check_var_data_types(resources, tmp->index_to_struct_buffer, var_name, sem_type_filter(expected_type))) == RETURN_OK) {
            debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_OK");
            return RETURN_OK;
        }
	else if (i == L_DOUBLE || i == L_INT){
            debug_print("%s%d\n", "CHECK_VAR_TYPE_RETURN_",i);
	    return i;
	} 
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    }
    
    debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_1");
    return TYPE_ERROR;
}

int define_func(Resources *resources)
{
    debug_print("%s\n", "DEFINE_FUNC");
    if (check_definition_flag(resources, resources->stack.top, currently_analyzed_function)) {
        debug_print("%s\n", "DEFINE_FUNC_SEMANTIC_ERROR");
        return SEMANTIC_ERROR; // sematicka chyba - dve definicia jednej funkcie
    }

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

    debug_print("%s\n", "DEFINE_FUNC_RETURN_OK");
    return RETURN_OK;
}
// debug printy prvy token, druhy token, ktory sa pretipuje
int check_tokens(Resources *resources, index_t frst_token, index_t scnd_token)
{
    int frst_token_type, scnd_token_type;
    TToken *token;
    TTree *tmp;
    int is_declared = SEMANTIC_ERROR;

    dereference_structure(&(resources->struct_buff), frst_token, (void**)&token);
    if (token->original_type == IDENTIFIER){

        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

        for (int i = resources->stack.length - 1; i > 0; i--) {
            if ((is_declared = declaration_test(resources, token->token_index, tmp->index_to_struct_buffer, VAR)) == RETURN_OK) // is declared 0 else 1
                break;
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
        }
        if (is_declared == RETURN_OK)
            frst_token_type = get_data_type(resources, tmp->index_to_struct_buffer, token->token_index, VAR);
        else {
            debug_print("%s\n", "CHECK_TOKENS_SEMANTIC_ERROR");
            return SEMANTIC_ERROR;  //semantic error undeclared variable
        }
    }
    else
        frst_token_type = sem_type_filter(token->original_type);

    dereference_structure(&(resources->struct_buff), scnd_token, (void**)&token);
    if (token->original_type == IDENTIFIER){

        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

        for (int i = resources->stack.length - 1; i > 0; i--) {
            if ((is_declared = declaration_test(resources, token->token_index, tmp->index_to_struct_buffer, VAR)) == RETURN_OK) // is declared 0 else 1
                break;
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
        }
        if (is_declared == 0)
            scnd_token_type = get_data_type(resources, tmp->index_to_struct_buffer, token->token_index, VAR);
        else {
            debug_print("%s\n", "CHECK_TOKENS_SEMANTIC_ERROR");
            return SEMANTIC_ERROR;  //semantic error undeclared variable
        }
    }
    else {
        scnd_token_type = sem_type_filter(token->original_type);
    }

    if (frst_token_type == scnd_token_type) {
        debug_print("%s\n", "CHECK_TOKENS_RETURN_OK");
        return RETURN_OK;
    }
    else if ((frst_token_type == L_STRING) || (scnd_token_type == L_STRING)) {
        debug_print("%s\n", "CHECK_TOKENS_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
    else if (frst_token_type == L_DOUBLE)
        return TYPE_CAST_SECOND;  //pretypuj druhy token
    else
        return TYPE_CAST_FIRST;  //pretypuj prvy token
}

int check_arg_type(Resources *resources, int type)
{
    debug_print("%s\n", "CHECK_ARG_TYPE");
    TTree *tmp;
    int declared_type;
    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree

    arg_counter++;
    declared_type = load_arg_data_type(resources, tmp->index_to_struct_buffer, currently_analyzed_function, arg_counter);
    if (type == declared_type){
        debug_print("%s\n", "CHECK_ARG_TYPE_RETURN_OK");
        return RETURN_OK;
    }
    else if (type == L_STRING || declared_type == L_STRING){
        debug_print("%s\n", "CHECK_ARG_TYPE_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
       } 
    else {
        debug_print("%s\n", "CHECK_ARG_TYPE_RETURN_TYPE_CAST"); 
        return TYPE_CAST;   //pretypuj
    }
}

int check_argc_function_call(Resources *resources)
{
    debug_print("%s\n", "CHECK_ARGC_FUNCTION_CALL");
    TTree *tmp;
    int actual_argc;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree

    load_num_of_args(resources, tmp->index_to_struct_buffer, currently_analyzed_function, &actual_argc);
    if (arg_counter == actual_argc){
        debug_print("%s\n", "CHECK_ARGC_FUNCTION_CALL_RETURN_OK");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n", "CHECK_ARGC_FUNCTION_CALL_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
    }
}



// int set_value_var(Resources *resources, char *id, int data_type) {

// }
