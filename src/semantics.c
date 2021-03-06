/**
 * @file sematics.c
 * @author Tomáš Ščavnický <xscavn00@stud.fit.vutbr.cz> Tomáš Sýkora <xsykor25@stud.fit.vutbr.cz> Michal Ďurista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Sematic analysis of IFJ15 language
 */

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
    int x = 0;
    int next_cnt;

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    debug_print("%s%d\n", "ENTER_SCOPE is_definition_scope is: ", tmp->is_definition_scope);
    if (tmp->is_definition_scope == 1){
        tmp->is_definition_scope = 0;
        x = 1;
    }
    next_cnt = tmp->var_cnt;

    add_char(&(resources->string_buff), '$');
    index_t test = save_token(&(resources->string_buff));

    if (x == 1)
	make_root_for_def_scope(resources, &i, currently_analyzed_function, test);
    else
        declare_variable(resources, test, &i, NO_DATA_TYPE);

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), i, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    //indexes above 8 are indexes of functions' definitions
debug_print("%s%lu\n", "ENTER_SCOPE actual i is: ", i);
debug_print("%s%d\n", "ENTER_SCOPE actual x is: ", x);
    if (i > 8 && x != 1){
debug_print("%s\n", "ENTER_SCOPE i'm in cindition if (i > 8 && x != 1) ");
        
debug_print("%s%d\n", "ENTER_SCOPE tmp_next->var_cnt is: ", next_cnt);
        tmp->var_cnt = next_cnt;
    }

    tmp->index_to_struct_buffer = i;
    push(&(resources->struct_buff_trees), &(resources->stack), i);
  
    debug_print("%s\n", "ENTER_SCOPE_RETURN_0");
    return RETURN_OK;
}

int leave_scope(Resources *resources)
{
    debug_print("%s\n", "LEAVE_SCOPE");
    TTree *tmp, *tmp_next;
    index_t r = resources->stack.top;

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), r, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp_next),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

debug_print("%s%d\n", "LEAVE_SCOPE tmp->var_cnt", tmp->var_cnt);
debug_print("%s%d\n", "LEAVE_SCOPE tmp_next->var_cnt", tmp_next->var_cnt);
    for (int i = 0; i < (tmp->var_cnt - tmp_next->var_cnt); i++) {
        catch_internal_error(new_instruction_empty(&(resources->instruction_buffer), POP_EMPTY),
            INTERNAL_ERROR,
            "Failed to generate pop instruction"
            );
    }

    pop(&(resources->struct_buff_trees), &(resources->stack));
    debug_print("%s\n", "LEAVE_SCOPE_RETURN_0");
    return RETURN_OK;
}

int leave_general_scope(Resources *resources) {
    debug_print("%s\n", "LEAVE_GENERAL_SCOPE");
    TTree *tmp;
    
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while(tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    } // after while, tmp is global scope tree

    free_memory(resources, tmp->index_to_struct_buffer);
    pop(&(resources->struct_buff_trees), &(resources->stack));

    debug_print("%s\n", "LEAVE_GENERAL_SCOPE_RETURN_0");
    return RETURN_OK;
}

int is_func_declared(Resources *resources, index_t name_of_func)
{
    debug_print("%s\n", "IS_FUNC_DECLARED");
    TTree *tmp;
    int is_declared = SEMANTIC_ERROR;
    currently_analyzed_function = name_of_func;
    arg_counter = 0;
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while(tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
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
    int iRet;

    if ( (iRet = declaration_test(resources, name_of_func, i, FUNC)) == RETURN_OK) {
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
    int is_main;
    arg_counter = 0;
    index_t index_to_func_table;
    index_t * func_index;
    int type = sem_type_filter(return_type);

    debug_print("%s%s\n", "DECLARE_FUNC, declaring function with name: ", load_token(&(resources->string_buff), index_to_string_buff));

    is_main = strcmp(load_token(&(resources->string_buff), index_to_string_buff), "main");
    if (is_main == 0 && type != L_INT){
	debug_print("%s\n", "DECLARE_FUNC_RETURN_SEMANTIC_ERROR");
	return SEMANTIC_ERROR;
    }

    switch (is_func_declared_withrv(resources, index_to_string_buff, type)) {
        case RETURN_OK:
	    unset_declaration_flag(resources, resources->stack.top, index_to_string_buff);
	    debug_print("%s\n", "DECLARE_FUNC: unsetting declaration flag");
            debug_print("%s\n", "DECLARE_FUNC_RETURN_0");
            return RETURN_OK;
        case NOT_FOUND:
            i = resources->stack.top;
            declare_function(resources, index_to_string_buff, &i, type);

	    set_declaration_flag(resources, i, index_to_string_buff);
	    debug_print("%s\n", "DECLARE_FUNC: setting declaration flag");

            resources->definitions_counter++;
	    if( (save_frame(resources, i, index_to_string_buff, resources->definitions_counter, FUNC)) == NOT_FOUND ){
	        debug_print("%s\n","SAVE_FUNC_INDEX_RETURN_SEMANTIC_ERROR");
		return SEMANTIC_ERROR;
	    } 
	    new_item(&(resources->func_table), index_to_func_table, func_index);

	    if (is_main == 0)
                set_start(resources, index_to_string_buff);
            debug_print("%s\n", "DECLARE_FUNC_RETURN_0");
            return RETURN_OK;
        case SEMANTIC_ERROR:
            debug_print("%s\n", "DECLARE_FUNC_RETURN_SEMANTIC_ERROR");
            return SEMANTIC_ERROR; // odsledovat v synt an pre hlasenie sem. chyby
        default:
            debug_print("%s\n", "DECLARE_FUNC_RETURN_-1");
            return INTERNAL_ERROR;
    }
}

int declare_builtin_funcs(Resources *resources)
{
    debug_print("%s\n", "DECLARE_FUNC");
    index_t string_index, first_arg, second_arg, third_arg;

    // SORT    

    add_str(&(resources->string_buff), "sort");
    currently_analyzed_function = string_index = save_token(&(resources->string_buff));
    declare_func(resources, string_index, L_STRING);
    
    add_str(&(resources->string_buff), "s");
    first_arg = save_token(&(resources->string_buff));
    set_arg(resources, first_arg, L_STRING);
    catch_internal_error(
        check_argc(resources),
        TYPE_ERROR,
        "Wrong argument count."
    );
    set_built_in(resources, currently_analyzed_function);
    set_definition_flag(resources, resources->stack.top, string_index);

    // FIND

    add_str(&(resources->string_buff), "find");
    currently_analyzed_function = string_index = save_token(&(resources->string_buff));
    if (is_func_declared(resources, currently_analyzed_function) == SEMANTIC_ERROR)
	declare_func(resources, string_index, L_INT);
    add_str(&(resources->string_buff), "s");
    first_arg = save_token(&(resources->string_buff));
    set_arg(resources, first_arg, L_STRING);
    add_str(&(resources->string_buff), "search");
    second_arg = save_token(&(resources->string_buff));
    set_arg(resources, second_arg, L_STRING);
    catch_internal_error(
        check_argc(resources),
        TYPE_ERROR,
        "Wrong argument count."
    );
    set_built_in(resources, currently_analyzed_function);
    set_definition_flag(resources, resources->stack.top, string_index);

    // LENGTH

    add_str(&(resources->string_buff), "length");
    currently_analyzed_function = string_index = save_token(&(resources->string_buff));
    if (is_func_declared(resources, currently_analyzed_function) == SEMANTIC_ERROR)
        declare_func(resources, string_index, L_INT);
    add_str(&(resources->string_buff), "s");
    first_arg = save_token(&(resources->string_buff));
    set_arg(resources, first_arg, L_STRING);
    catch_internal_error(
        check_argc(resources),
        TYPE_ERROR,
        "Wrong argument count."
    );
    set_built_in(resources, currently_analyzed_function);
    set_definition_flag(resources, resources->stack.top, string_index);

    // CONCAT

    add_str(&(resources->string_buff), "concat");
    currently_analyzed_function = string_index = save_token(&(resources->string_buff));
    if (is_func_declared(resources, currently_analyzed_function) == SEMANTIC_ERROR)
        declare_func(resources, string_index, L_STRING);
    add_str(&(resources->string_buff), "s1");
    first_arg = save_token(&(resources->string_buff));
    set_arg(resources, first_arg, L_STRING);
    add_str(&(resources->string_buff), "s2");
    second_arg = save_token(&(resources->string_buff));
    set_arg(resources, second_arg, L_STRING);
    catch_internal_error(
        check_argc(resources),
        TYPE_ERROR,
        "Wrong argument count."
    );
    set_built_in(resources, currently_analyzed_function);
    set_definition_flag(resources, resources->stack.top, string_index);

    // SUBSTR

    add_str(&(resources->string_buff), "substr");
    currently_analyzed_function = string_index = save_token(&(resources->string_buff));
    if (is_func_declared(resources, currently_analyzed_function) == SEMANTIC_ERROR)
        declare_func(resources, string_index, L_STRING);
    add_str(&(resources->string_buff), "s");
    first_arg = save_token(&(resources->string_buff));
    set_arg(resources, first_arg, L_STRING);
    add_str(&(resources->string_buff), "i");
    second_arg = save_token(&(resources->string_buff));
    set_arg(resources, second_arg, L_INT);
    add_str(&(resources->string_buff), "n");
    third_arg = save_token(&(resources->string_buff));
    set_arg(resources, third_arg, L_INT);
    catch_internal_error(
        check_argc(resources),
        TYPE_ERROR,
        "Wrong argument count."
    );
    set_built_in(resources, currently_analyzed_function);
    set_definition_flag(resources, resources->stack.top, string_index);

    debug_print("%s\n", "DECLARE_FUNC_RETURN_OK");
    return RETURN_OK;
}



int declare_var(Resources *resources, index_t index_to_string_buff, int data_type)
{
    debug_print("%s\n", "DECLARE_VAR");
debug_print("%s%s\n", "DECLARE_VAR var name: ", load_token(&(resources->string_buff), index_to_string_buff));
    TTree *tmp;
    index_t i = resources->stack.top;
    int is_declared = declaration_test(resources, index_to_string_buff, i, VAR);

    switch (is_declared) {
        case NOT_FOUND: 
            declare_variable(resources, index_to_string_buff, &i, sem_type_filter(data_type));
            catch_internal_error(
                dereference_structure(&(resources->struct_buff_trees), i, (void **)&tmp),
                INTERNAL_ERROR,
                "Failed to dereference structure buffer."
            );
            tmp->var_cnt++;
	    debug_print("%s%d\n", "DECLARE_VAR saved count: ", tmp->var_cnt);
            save_var_index(resources, index_to_string_buff, tmp->var_cnt);
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
    int cmp;


    debug_print("%s%s\n", "CHECK_ARG_DECLARATION name of func: ", load_token(&(resources->string_buff), currently_analyzed_function));
    debug_print("%s%d\n", "CHECK_ARG_DECLARATION argi: ", argi);


    int actual_arg_type, iRet;
    index_t actual_name_of_arg;
    if ((iRet = load_arg(resources, i, currently_analyzed_function, argi, &actual_name_of_arg, &actual_arg_type)) == NOT_FOUND)
    {
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
    }
    
    cmp = strcmp( load_token(&(resources->string_buff), expected_name_of_arg), load_token(&(resources->string_buff), actual_name_of_arg));

    if ((expected_arg_type == actual_arg_type) && (cmp == 0)){
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_OK");
        return RETURN_OK;
    }
    else if (expected_arg_type != actual_arg_type) {
        debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
    }
    else {
	debug_print("%s\n", "CHECK_ARG_DECLARATION_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int set_arg(Resources *resources, index_t name_of_arg, int data_type)
{
    debug_print("%s\n", "SET_ARG");
    index_t i = resources->stack.top;
    int is_main;
    int is_declared_now = -5;
    int iRet;

    if ( (is_main = is_start(resources, currently_analyzed_function)) == true){
	debug_print("%s%s\n", "SET_ARG name of function: ", load_token(&(resources->string_buff), currently_analyzed_function));
        debug_print("%s\n", "SET_ARG_RETURN_2");
        return SEMANTIC_ERROR;
    }

    arg_counter++;
    if( (is_declared_now = check_declaration_status(resources, i, currently_analyzed_function)) == true){
	debug_print("%s%d\n", "SET_ARG is_declared_now: ", is_declared_now);
        add_arg(resources, name_of_arg, sem_type_filter(data_type));
        debug_print("%s\n", "SET_ARG_RETURN_OK");
        return RETURN_OK;
    }
    else {
	debug_print("%s%d\n", "SET_ARG is_declared_now: ", is_declared_now);
        if( (iRet = check_arg_declaration(resources, name_of_arg, sem_type_filter(data_type), arg_counter)) == RETURN_OK){
            debug_print("%s\n", "SET_ARG_RETURN_OK");
            return RETURN_OK;
        }
        else if (iRet == TYPE_ERROR) {
           debug_print("%s\n", "SET_ARG_RETURN_TYPE_ERROR");    
           return TYPE_ERROR;
        }
	else {
	   debug_print("%s\n", "SET_ARG_RETURN_SEMANTIC_ERROR");    
           return SEMANTIC_ERROR;
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
        debug_print("%s\n", "CHECK_ARGC_RETURN_OK");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n", "CHECK_ARGC_RETURN_TYPE_ERROR");
        return TYPE_ERROR;
    }
}

int is_var_declared(Resources *resources, index_t name_of_var) {
    debug_print("%s\n", "IS_VAR_DECLARED");
    TTree *tmp;
    int is_declared = SEMANTIC_ERROR;
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    
    for (int i = resources->stack.length - 1; i > 0; i--) {
        if ((is_declared = declaration_test(resources, name_of_var, tmp->index_to_struct_buffer, VAR)) == 0){ // is declared 0 else 1
            is_declared = RETURN_OK;
            break;
        }
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    }
    
    debug_print("%s%d\n", "IS_VAR_DECLARED", is_declared);
    return is_declared;
}

int check_return_type(Resources *resources, index_t func_name, int expected_data_type) {
    debug_print("%s\n", "CHECK_RETURN_TYPE");
    TTree *tmp;
    int actual_data_type;
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while(tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    } // while; tmp is global scope tree
    actual_data_type = get_data_type(resources, tmp->index_to_struct_buffer, func_name, FUNC);

    debug_print("%s%s\n", "CHECK_RETURN_TYPE name of function: ", load_token(&(resources->string_buff), func_name));

    return (actual_data_type == sem_type_filter(expected_data_type) ? RETURN_OK : TYPE_ERROR);
}

int check_var_type(Resources *resources, index_t var_name, int expected_type)
{
    debug_print("%s\n", "CHECK_VAR_TYPE");
debug_print("%s%d\n", "CHECK_VAR_TYPE expected: ", expected_type);
debug_print("%s%s\n", "CHECK_VAR_TYPE var name: ", load_token(&(resources->string_buff), var_name));
    args_assert(expected_type == 14 || expected_type == 21 || expected_type == 15 || expected_type == 20 || expected_type == 16 || expected_type == 22 || expected_type == 32, INTERNAL_ERROR);
    TTree *tmp;
    int i;

    
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    for (int j = resources->stack.length - 1; j > 0; j--) {
        if ((i = check_var_data_types(resources, tmp->index_to_struct_buffer, var_name, sem_type_filter(expected_type))) == RETURN_OK) {
            debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_OK");
            return RETURN_OK;
        }
        else if (i == L_DOUBLE || i == L_INT){
            debug_print("%s%d\n", "CHECK_VAR_TYPE_RETURN_",i);
            return TYPE_CAST;
        }
        else if (i == SEMANTIC_ERROR){
            debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_OK");
            return TYPE_ERROR;
        }
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    }
    
    debug_print("%s\n", "CHECK_VAR_TYPE_RETURN_4");
    return TYPE_ERROR;
}

int define_func(Resources *resources)
{
    debug_print("%s\n", "DEFINE_FUNC");
    if (check_definition_flag(resources, resources->stack.top, currently_analyzed_function)) {
        debug_print("%s\n", "DEFINE_FUNC_SEMANTIC_ERROR");
        return SEMANTIC_ERROR; // sematicka chyba - dve definicia jednej funkcie
    }

    char *str = load_token(&(resources->string_buff), currently_analyzed_function);
    catch_internal_error(str, NULL, "Failed to load token string.");
    if (strcmp(str,"main") == 0){
        resources->start_main = resources->definitions_counter;
	debug_print("%s%d\n", "DEFINE_FUNC saved main to func table on index: ", resources->definitions_counter);
    }
    save_func_index (resources, currently_analyzed_function, resources->instruction_buffer.next_free);

    int argc;
    int data_type;
    index_t name;
    TTree *tmp;
    index_t general_scope_tree = resources->stack.top;

    set_definition_flag(resources, resources->stack.top, currently_analyzed_function);
    load_num_of_args(resources, resources->stack.top, currently_analyzed_function, &argc);
debug_print("%s%d\n", "DEFINE_FUNC num of args: ", argc);

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    tmp->is_definition_scope = 1;

    enter_scope(resources);
    index_t r = resources->stack.top;

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), r, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    for(int i = 1; i <= argc; i++) {
        load_arg(resources, general_scope_tree, currently_analyzed_function, i, &name, &data_type);
        declare_variable(resources, name, &r, data_type);
        tmp->var_cnt++;
	debug_print("%s%s\n", "DEFINE_FUNC arg name: ", load_token(&(resources->string_buff), name));
	debug_print("%s%d\n", "DEFINE_FUNC saved var count: ", tmp->var_cnt);
        save_var_index(resources, name, tmp->var_cnt);

        // generuj funkciu PUSH
    }

    debug_print("%s\n", "DEFINE_FUNC_RETURN_OK");
    return RETURN_OK;
}
// debug printy prvy token, druhy token, ktory sa pretipuje
int check_tokens(Resources *resources, index_t frst_token, index_t scnd_token)
{
    int frst_token_type = NOT_FOUND;
    int scnd_token_type = NOT_FOUND;
    TToken *token;
    TTree *tmp;

    catch_internal_error(
        dereference_structure(&(resources->struct_buff), frst_token, (void**)&token),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    if (token->original_type == IDENTIFIER){
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
        for (int i = resources->stack.length - 1; i > 0; i--) {
            if ((frst_token_type = get_data_type(resources, tmp->index_to_struct_buffer, token->token_index, VAR)) != NOT_FOUND) // is declared 0 else 1
                break;
            catch_internal_error(
                dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
                INTERNAL_ERROR,
                "Failed to dereference structure buffer."
            );
        }
        if (frst_token_type == NOT_FOUND)
    {
            debug_print("%s\n", "CHECK_TOKENS_SEMANTIC_ERROR");
            return SEMANTIC_ERROR;  //semantic error undeclared variable
        }
    }
    else
        frst_token_type = sem_type_filter(token->original_type);

    
    catch_internal_error(
        dereference_structure(&(resources->struct_buff), scnd_token, (void**)&token),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    if (token->original_type == IDENTIFIER){
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
        for (int i = resources->stack.length - 1; i > 0; i--) {
            if ((scnd_token_type = get_data_type(resources, tmp->index_to_struct_buffer, token->token_index, VAR)) != NOT_FOUND) // is declared 0 else 1
                break;
            catch_internal_error(
                dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
                INTERNAL_ERROR,
                "Failed to dereference structure buffer."
            );
        }
        if (scnd_token_type == NOT_FOUND)
    {
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
        debug_print("%s\n", "CHECK_TOKENS_TYPE_ERROR");
        return TYPE_ERROR;
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
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while(tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
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

    
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    while(tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
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

int get_return_type(Resources *resources, index_t func_name) {
    debug_print("%s\n", "GET_RETURN_TYPE");
    TTree *tmp;
    int actual_data_type;
    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    while(tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    } // while; tmp is global scope tree
    actual_data_type = get_data_type(resources, tmp->index_to_struct_buffer, func_name, FUNC);

    return actual_data_type;
}

int get_var_type(Resources *resources, index_t var_name)
{
    debug_print("%s\n", "GET_VAR_TYPE");
debug_print("%s%s\n", "GET_VAR_TYPE var name: ", load_token(&(resources->string_buff), var_name));
    TTree *tmp;
    int is_declared = SEMANTIC_ERROR;
    int type;

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );
    
    for (int i = resources->stack.length - 1; i > 0; i--) {
        if (declaration_test(resources, var_name, tmp->index_to_struct_buffer, VAR) == 0){ // is declared 0 else 1
            is_declared = RETURN_OK;
            break;
        }
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
    }

    if (is_declared == RETURN_OK){
        type = get_data_type(resources, tmp->index_to_struct_buffer, var_name, VAR);
        debug_print("%s%d\n","GET_VAR_TYPE_RETURN ", type);
        return type;
    }
    else {
        debug_print("%s\n","GET_VAR_TYPE_RETURN_SEMANTIC_ERROR");
    return SEMANTIC_ERROR;
    }
}

int save_func_index(Resources *resources, index_t func_name, index_t index_to_store)
{
    debug_print("%s\n","SAVE_FUNC_INDEX");
    index_t *func_index;
    TTree *tmp;
    index_t index_to_func_table;
    int iret;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree

    if((iret = load_frame(resources, tmp->index_to_struct_buffer, func_name, &index_to_func_table, FUNC)) != RETURN_OK)
	    return iret;

    debug_print("%s%lu\n","SAVE_FUNC_INDEX saving to func_table to index: ", index_to_func_table);
    debug_print("%s%lu\n","SAVE_FUNC_INDEX saving to func_table index: ", index_to_store);
    
    dereference_structure(&(resources->func_table), index_to_func_table, (void **)&func_index);
    *func_index = index_to_store;

    debug_print("%s\n","SAVE_FUNC_INDEX_RETURN_OK");
    return RETURN_OK;
}

int save_var_index(Resources *resources, index_t var_name, index_t index_to_store)
{
    debug_print("%s\n","SAVE_VAR_INDEX");
    TTree *tmp;
    int iret = NOT_FOUND;

    debug_print("%s%s\n","SAVE_VAR_INDEX variable name: ", load_token(&(resources->string_buff), var_name));
    debug_print("%s%lu\n","SAVE_VAR_INDEX index_to_store: ", index_to_store);

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

        for (int i = resources->stack.length - 1; i > 0; i--) {
            if ((iret = save_frame(resources, tmp->index_to_struct_buffer, var_name, index_to_store, VAR)) != NOT_FOUND){ // is declared 0 else 1
                break;
            }

            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
        }

    if( iret != NOT_FOUND ){
        debug_print("%s\n","SAVE_VAR_INDEX_RETURN_OK");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n","SAVE_VAR_INDEX_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int load_func_index(Resources *resources, index_t func_name, index_t *load_index)
{
    debug_print("%s\n","LOAD_FUNC_INDEX");
    TTree *tmp;
    int iret;
    index_t tmp_load_index;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

    while(tmp->next != ZERO_INDEX) {
        dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
    } // after while, tmp is global scope tree

    if( (iret = load_frame(resources, tmp->index_to_struct_buffer, func_name, &tmp_load_index, FUNC)) != NOT_FOUND ){
        debug_print("%s\n","LOAD_FUNC_INDEX_RETURN_OK");
        *load_index = tmp_load_index;
        return RETURN_OK;
    }
    else {
        debug_print("%s\n","LOAD_FUNC_INDEX_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int load_var_index(Resources *resources, index_t var_name, index_t *load_index)
{
    debug_print("%s\n","LOAD_VAR_INDEX");
    TTree *tmp;
    int iret = NOT_FOUND;
    index_t tmp_load_index;

    dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp);

        for (int i = resources->stack.length - 1; i > 0; i--) {
            if ((iret = load_frame(resources, tmp->index_to_struct_buffer, var_name, &tmp_load_index, VAR)) != NOT_FOUND){ // is declared 0 else 1
                break;
            }

            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp);
        }

    if( iret != NOT_FOUND ){
	debug_print("%s%s\n","LOAD_VAR_INDEX var name: ", load_token(&(resources->string_buff), var_name));
	debug_print("%s%lu\n","LOAD_VAR_INDEX loading index: ", tmp_load_index);
        *load_index = tmp_load_index;
        debug_print("%s\n","LOAD_VAR_INDEX_RETURN_OK");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n","LOAD_VAR_INDEX_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int declared_var_cnt(Resources *resources, int *cnt)
{
    debug_print("%s\n", "DECLARED_VAR_CNT");
    TTree *tmp;

    catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

    *cnt = tmp->var_cnt;

    debug_print("%s\n", "DECLARED_VAR_CNT_RETURN_0");
    return RETURN_OK;
}

int check_return_value_type(Resources *resources, int type)
{
	debug_print("%s\n", "CHECK_RETURN_VALUE_TYPE");
	TTree *tmp;
	int actual_data_type;
	index_t func_id = 0;

	catch_internal_error(
        dereference_structure(&(resources->struct_buff_trees), resources->stack.top, (void **)&tmp),
        INTERNAL_ERROR,
        "Failed to dereference structure buffer."
    );

	func_id = tmp->defined_function_id;

    while (tmp->next != ZERO_INDEX) {
        catch_internal_error(
            dereference_structure(&(resources->struct_buff_trees), tmp->next, (void **)&tmp),
            INTERNAL_ERROR,
            "Failed to dereference structure buffer."
        );
	if (!func_id)
	    func_id = tmp->defined_function_id;
    } // while; tmp is global scope tree
    actual_data_type = get_data_type(resources, tmp->index_to_struct_buffer, func_id, FUNC);

    if (actual_data_type == type){
    	debug_print("%s\n", "CHECK_RETURN_VALUE_TYPE_RETURN_OK");
    	return RETURN_OK;
    }
    else if (type == L_STRING || actual_data_type == L_STRING){
    	debug_print("%s\n", "CHECK_RETURN_VALUE_TYPE_RETURN_TYPE_ERROR");
    	return TYPE_ERROR;
    }
    else {
    	debug_print("%s\n", "CHECK_RETURN_VALUE_TYPE_RETURN_TYPE_CAST");
    	return TYPE_CAST;
    }
}
