/**
 * project: Implementace interpretu imperativního jazyka IFJ15
 * @file ial.c
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Built functions in IFJ2015 language (find)and a symbol table 
 * storing indexes to tokens in dynamic_structure_buffer.
 */

#include "ial.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "error_macros.h"
#include "debug.h"
#include "datatypes.h"
#include "resources.h"
#include "dynamic_buffer.h"
#include "semantics.h"

int hash_key(char *str, unsigned long *hash) {

    args_assert(str != NULL, INTERNAL_ERROR);

    int c;

    *hash = 5381;

    while ((c = *str++))
        *hash = (((*hash) << 5) + *hash) + c;

    return RETURN_OK;
}


int make_root_for_def_scope(Resources *resources, index_t *index_to_root_node, index_t index_to_func_id, index_t index_to_string){

    args_assert(resources != NULL, INTERNAL_ERROR);

    index_t new_node_index;
    TTree *new_node;
    unsigned long key;
    char *str = NULL;

    new_item(&(resources->struct_buff_trees), new_node_index, new_node);

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    hash_key(str, &key);

        *index_to_root_node = new_node_index;
        new_node->right = new_node->left = new_node->next_node = ZERO_INDEX;
        new_node->key = key;
        new_node->data_type = NO_DATA_TYPE;
        new_node->type = VAR; 
        new_node->args = NULL;
        new_node->index_to_dynamic_buff = index_to_string;
        new_node->definition = false;
	new_node->var_cnt = 0;
	new_node->is_definition_scope = 0;
	new_node->defined_function_id = index_to_func_id;

	return RETURN_OK;
}

int create_variable_node(index_t index_to_node, index_t new_node_index, unsigned long key, TDynamic_structure_buffer *struct_buff_nodes, index_t index_to_dynamic_buffer, int data_type){

    args_assert(struct_buff_nodes != NULL, INTERNAL_ERROR);

    TTree *new_node, *actual_node;

    dereference_structure(struct_buff_nodes, index_to_node, (void**)&actual_node);

    if (key > actual_node->key){
        if (actual_node->right == ZERO_INDEX){
            dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
            actual_node->right = new_node_index;
            new_node->left = new_node->right = new_node->next_node = ZERO_INDEX;
            new_node->key = key;
            new_node->data_type = data_type;
            new_node->type = VAR;
            new_node->args = NULL;
            new_node->index_to_dynamic_buff = index_to_dynamic_buffer;
            new_node->definition = false;
	    new_node->var_cnt = 0;
	    new_node->is_definition_scope = 0;
	    new_node->defined_function_id = 0;
        }
        else
            create_variable_node(actual_node->right, new_node_index, key, struct_buff_nodes, index_to_dynamic_buffer, data_type);
    }
    else if (key < actual_node->key){
        if (actual_node->left == ZERO_INDEX){
            dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
            actual_node->left = new_node_index;
            new_node->left = new_node->right = new_node->next_node = ZERO_INDEX;
            new_node->key = key;
            new_node->data_type = data_type;
            new_node->type = VAR;
            new_node->args = NULL;
            new_node->index_to_dynamic_buff = index_to_dynamic_buffer;
            new_node->definition = false;
	    new_node->var_cnt = 0;
	    new_node->is_definition_scope = 0;
	    new_node->defined_function_id = 0;
        }
        else
            create_variable_node(actual_node->left, new_node_index, key, struct_buff_nodes, index_to_dynamic_buffer, data_type);
    }
    else {  //key == actual_node->key  --- hash colision
        while(actual_node->next_node != ZERO_INDEX)
        {
            dereference_structure(struct_buff_nodes, actual_node->next_node, (void**)&actual_node);
        }
        actual_node->next_node = new_node_index;
        dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
        new_node->right = new_node->left = new_node->next_node = ZERO_INDEX;
        new_node->key = key;
        new_node->data_type = data_type;
        new_node->type = VAR;
        new_node->args = NULL;
        new_node->index_to_dynamic_buff = index_to_dynamic_buffer;
        new_node->definition = false;
	new_node->var_cnt = 0;
	new_node->is_definition_scope = 0;
	new_node->defined_function_id = 0;
    }
    return RETURN_OK;
}


int declare_variable(Resources *resources, index_t index_to_string, index_t *index_to_root_node, int data_type){

    args_assert(resources != NULL && index_to_string > 0, INTERNAL_ERROR);

    index_t new_node_index;
    TTree *new_node;
    unsigned long key;
    char *str = NULL;

    new_item(&(resources->struct_buff_trees), new_node_index, new_node);

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");
    hash_key(str, &key);

    if (*index_to_root_node == ZERO_INDEX){
        *index_to_root_node = new_node_index;
        new_node->right = new_node->left = new_node->next_node = ZERO_INDEX;
        new_node->key = key;
        new_node->data_type = data_type;
        new_node->type = VAR; 
        new_node->args = NULL;
        new_node->index_to_dynamic_buff = index_to_string;
        new_node->definition = false;
	new_node->var_cnt = 0;
	new_node->is_definition_scope = 0;
	new_node->defined_function_id = 0;
    }
    else
        create_variable_node(*index_to_root_node, new_node_index, key, &(resources->struct_buff_trees), index_to_string, data_type);
    
    return RETURN_OK;
}


int create_function_node(index_t index_to_node, index_t new_node_index, unsigned long key, TDynamic_structure_buffer *struct_buff_nodes, index_t index_to_dynamic_buffer, int ret_val){

    args_assert(struct_buff_nodes != NULL && index_to_dynamic_buffer > 0, INTERNAL_ERROR);

    TTree *new_node, *actual_node;

    dereference_structure(struct_buff_nodes, index_to_node, (void**)&actual_node);

    if (key > actual_node->key){
        if (actual_node->right == ZERO_INDEX){
            dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
            actual_node->right = new_node_index;
            new_node->left = new_node->right = new_node->next_node = ZERO_INDEX;
            new_node->key = key;
            new_node->data_type = ret_val;
            new_node->type = FUNC;
            new_node->args = NULL;
            new_node->value.i = 0;
            new_node->index_to_dynamic_buff = index_to_dynamic_buffer;
            new_node->definition = false;
	    new_node->var_cnt = 0;
	    new_node->is_definition_scope = 0;
	    new_node->is_declared_now = 0;
	    new_node->start = false;
	    new_node->defined_function_id = 0;
        }
        else
            create_function_node(actual_node->right, new_node_index, key, struct_buff_nodes, index_to_dynamic_buffer, ret_val);
    }
    else if (key < actual_node->key){
        if (actual_node->left == ZERO_INDEX){
            dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
            actual_node->left = new_node_index;
            new_node->left = new_node->right = new_node->next_node = ZERO_INDEX;
            new_node->key = key;
            new_node->data_type = ret_val;
            new_node->type = FUNC;
            new_node->args = NULL;
            new_node->value.i = 0;
            new_node->index_to_dynamic_buff = index_to_dynamic_buffer;
            new_node->definition = false;
	    new_node->var_cnt = 0;
	    new_node->is_definition_scope = 0;
	    new_node->is_declared_now = 0;
	    new_node->start = false;
	    new_node->defined_function_id = 0;
        }
        else
            create_function_node(actual_node->left, new_node_index, key, struct_buff_nodes, index_to_dynamic_buffer, ret_val);
    }
    else {  //key == actual_node->key  --- hash colision
        while(actual_node->next_node != ZERO_INDEX)
        {
            dereference_structure(struct_buff_nodes, actual_node->next_node, (void**)&actual_node);
        }
        actual_node->next_node = new_node_index;
        dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
        new_node->right = new_node->left = new_node->next_node = ZERO_INDEX;
        new_node->key = key;
        new_node->data_type = ret_val;
        new_node->type = FUNC;
        new_node->args = NULL;
        new_node->value.i = 0;
        new_node->index_to_dynamic_buff = index_to_dynamic_buffer;
        new_node->definition = false;
	new_node->var_cnt = 0;
	new_node->is_definition_scope = 0;
	new_node->is_declared_now = 0;
	new_node->start = false;
	new_node->defined_function_id = 0;
    }
    return RETURN_OK;
}


int declare_function(Resources *resources, index_t index_to_string, index_t *index_to_root_node, int ret_val){

    args_assert(resources != NULL && index_to_string > 0, INTERNAL_ERROR);

    index_t new_node_index;
    TTree *new_node;
    unsigned long key;
    char *str = NULL;

    new_item(&(resources->struct_buff_trees), new_node_index, new_node);

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");
    hash_key(str, &key);

    if (*index_to_root_node == ZERO_INDEX){
        *index_to_root_node = new_node_index;
        new_node->right = new_node->left = new_node->next_node = ZERO_INDEX;
        new_node->key = key;
        new_node->data_type = ret_val;
        new_node->type = FUNC; 
        new_node->args = NULL;
        new_node->value.i = 0;
        new_node->index_to_dynamic_buff = index_to_string;
        new_node->definition = false;
	new_node->var_cnt = 0;
	new_node->is_definition_scope = 0;
	new_node->is_declared_now = 0;
	new_node->start = false;
	new_node->defined_function_id = 0;
    }
    else
        create_function_node(*index_to_root_node, new_node_index, key, &(resources->struct_buff_trees), index_to_string, ret_val);
    
    return RETURN_OK;
}


int iterate_through_tree(Resources *resources, char *str, index_t actual_node_index, index_t *found_node_index, int type_of_node){

    args_assert(resources != NULL && str != NULL, INTERNAL_ERROR);

    TTree *actual_node;
    bool found = false;
    unsigned long key;
    int v = NOT_FOUND;

    hash_key(str, &key);
    dereference_structure(&(resources->struct_buff_trees), actual_node_index, (void**)&actual_node);

    if(actual_node_index == ZERO_INDEX)
        return NOT_FOUND;
    else if(key > actual_node->key)
    {
        v = iterate_through_tree(resources, str, actual_node->right, found_node_index, type_of_node);
    }
    else if(key < actual_node->key)
    {
        v = iterate_through_tree(resources, str, actual_node->left, found_node_index, type_of_node);
    }
    else if(key == actual_node->key)
    {
        if(strcmp(str, load_token(&(resources->string_buff), actual_node->index_to_dynamic_buff)) == 0 && actual_node->type == type_of_node)
        {

            *found_node_index = actual_node_index;
            return FOUND;
        }
        else //hash colision
        {
            while(actual_node->next_node != ZERO_INDEX && found == false)
            {
                actual_node_index = actual_node->next_node;
                dereference_structure(&(resources->struct_buff_trees), actual_node_index, (void**)&actual_node);
                if (strcmp(str, load_token(&(resources->string_buff), actual_node->index_to_dynamic_buff)) == 0 && actual_node->type == type_of_node)
                    found = true;
            }
            if(found == true)
            {
                *found_node_index = actual_node_index;
                return FOUND;
            }
            else
                return NOT_FOUND;
        }
    }
    
    if (v == FOUND)
        return FOUND;
    else
        return NOT_FOUND;
}





int set_int_value(Resources *resources, index_t index_to_string, index_t index_to_root_node, int value){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX && index_to_string > 0, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, VAR);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->value.i = value;
        return RETURN_OK;
    }
    else
        return NOT_FOUND;
}



int check_func_args_id(Resources *resources, index_t index_to_root_node, char *string_of_func_id, index_t index_to_arg_id, index_t *found_node_index){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    int found;
    char *str_arg;
    TTree *found_node;
    bool exist = false;

    found = iterate_through_tree(resources, string_of_func_id, index_to_root_node, found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), *found_node_index, (void**)&found_node);
        str_arg = load_token(&(resources->string_buff), index_to_arg_id);
        catch_internal_error(str_arg, NULL, "Failed to load token string.");
        for ( int i = 0; i < found_node->value.i; i++){
            if ( (strcmp(str_arg, load_token(&(resources->string_buff), found_node->args[i].index_to_string_identifier))) == 0){
                exist = true;
                break;
            }
        }
        if (exist)
            return ARG_FOUND;
        else
            return RETURN_OK;
    }
    else
        return NOT_FOUND;
}


int add_func_arg(Resources *resources, index_t index_to_string, index_t index_to_root_node, int data_type, index_t index_to_dynamic_buff){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX && index_to_string > 0, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    TTree *found_node;
    TFunc_args *tmp, arg;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    int i = check_func_args_id(resources, index_to_root_node, str, index_to_dynamic_buff, &found_node_index);
    if (i == ARG_FOUND)
        return ARG_FOUND;
    else if (i == NOT_FOUND)
        return NOT_FOUND;

    arg.data_type = data_type;
    arg.index_to_string_identifier = index_to_dynamic_buff;

    dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
    tmp = realloc(found_node->args, sizeof(TFunc_args)*(found_node->value.i + 1));
    catch_internal_error(tmp, NULL, "Failed to realloc memory.");
    found_node->args = tmp;

    found_node->args[found_node->value.i] = arg;
    found_node->value.i += 1;

    return RETURN_OK;
}


int declaration_test(Resources *resources, index_t index_to_string, index_t index_to_root_node, int type){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    int found;
    index_t found_node_index;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, type);

    if (found == FOUND)
        return RETURN_OK;
    else
        return NOT_FOUND;
}


void free_memory(Resources *resources, index_t index_to_root_node){

    TTree *node;

    dereference_structure(&(resources->struct_buff_trees), index_to_root_node, (void**)&node);
    if (node->args != NULL)
        free(node->args);
    if (node->right != ZERO_INDEX)
        free_memory(resources, node->right);
    if (node->left != ZERO_INDEX)
        free_memory(resources, node->left);
    if (node->next_node != ZERO_INDEX)
        free_memory(resources, node->next_node);
}


int get_data_type(Resources *resources, index_t index_to_root_node, index_t index_to_string, int type){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");


    if (type == VAR)
        found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, VAR);
    else
        found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        return found_node->data_type;
    }
    else
        return NOT_FOUND;
}


int check_var_data_types(Resources *resources, index_t index_to_root_node, index_t index_to_string, int expected_data_type){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, VAR);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if ( found_node->data_type == expected_data_type )
            return RETURN_OK;
        else if ( found_node->data_type == AUTO ){
            found_node->data_type = expected_data_type;
            return RETURN_OK;
        }
	else if (expected_data_type != L_STRING){
		return found_node->data_type;
	}
        else
            return SEMANTIC_ERROR;
    }
    else
        return NOT_FOUND;
}




int load_integer_value(Resources *resources, index_t index_to_root_node, index_t index_to_string, int *value){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX && value != NULL, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, VAR);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        *value = found_node->value.i;
        return RETURN_OK;
    }
    else
        return NOT_FOUND;
}


int load_num_of_args(Resources *resources, index_t index_to_root_node, index_t index_to_string, int *num){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        *num = found_node->value.i;
        return RETURN_OK;
    }
    else
        return NOT_FOUND;
}


int load_arg_data_type(Resources *resources, index_t index_to_root_node, index_t index_to_string, int arg_index){

    args_assert(resources != NULL && index_to_string != ZERO_INDEX && index_to_root_node != ZERO_INDEX && arg_index > 0, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if (found_node->value.i >= arg_index)
            return found_node->args[arg_index-1].data_type;
        else
            return NOT_FOUND;
    }
    else
        return NOT_FOUND;
}


int load_arg(Resources *resources, index_t index_to_root_node, index_t index_to_func_id, int arg_index, index_t *index_to_arg_id, int *data_type){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_func_id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if (found_node->value.i >= arg_index){
            *index_to_arg_id = found_node->args[arg_index-1].index_to_string_identifier;
            *data_type = found_node->args[arg_index-1].data_type;
            return RETURN_OK;
        }
        else
            return NOT_FOUND;
    }
    else
        return NOT_FOUND;
}


int check_definition_flag(Resources *resources, index_t index_to_root_node, index_t index_to_func_id){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_func_id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if (found_node->definition == true)
            return true;
        else
            return false;
    }
    else
        return NOT_FOUND;
}


int set_definition_flag(Resources *resources, index_t index_to_root_node, index_t index_to_func_id){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_func_id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->definition = true;
        return RETURN_OK;
    }
    else
        return NOT_FOUND;

}


int check_declaration_status(Resources *resources, index_t index_to_root_node, index_t index_to_func_id){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_func_id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if (found_node->is_declared_now)
            return true;
        else
            return false;
    }
    else
        return NOT_FOUND;
}


int save_frame(Resources *resources, index_t index_to_root_node, index_t id, index_t index_to_store, int type){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, type);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->frame_index = index_to_store;
        return RETURN_OK;
    }
    else
        return NOT_FOUND;
}


int load_frame(Resources *resources, index_t index_to_root_node, index_t id, index_t *load_index, int type){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, type);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        *load_index = found_node->frame_index;
        return RETURN_OK;
    }
    else
        return NOT_FOUND;
}

int set_built_in(Resources *resources, index_t func_name)
{
    debug_print("%s\n","SET_BUILT_IN");
    args_assert(resources != NULL && func_name != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), func_name);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, 1, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->built_in = true;
        debug_print("%s\n","SET_BUILT_IN_RETURN_OK");
        return RETURN_OK;
    }
    else{
        debug_print("%s\n","SET_BUILT_IN_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int set_start(Resources *resources, index_t func_name)
{
    debug_print("%s\n","SET_START");
    args_assert(resources != NULL && func_name != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), func_name);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, 1, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->start = true;
        debug_print("%s\n","SET_START_RETURN_OK");
        return RETURN_OK;
    }
    else {
        debug_print("%s\n","SET_START_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int is_built_in(Resources *resources, index_t func_name)
{
    debug_print("%s\n","IS_BUILT_IN");
    args_assert(resources != NULL && func_name != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), func_name);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, 1, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if (found_node->built_in) {
            debug_print("%s\n","IS_BUILT_IN_RETURN_true");
            return true;
        }
        else {
            debug_print("%s\n","IS_BUILT_IN_RETURN_false");
            return false;
        }
    }
    else {
        debug_print("%s\n","IS_BUILT_IN_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int is_start(Resources *resources, index_t func_name)
{
    debug_print("%s\n","IS_START");
    args_assert(resources != NULL && func_name != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), func_name);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, 1, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        if (found_node->start) {
            debug_print("%s\n","IS_START_RETURN_true");
            return true;
        }
        else {
            debug_print("%s\n","IS_START_RETURN_false");
            return false;
        }
    }
    else {
        debug_print("%s\n","IS_START_RETURN_SEMANTIC_ERROR");
        return SEMANTIC_ERROR;
    }
}

int set_declaration_flag(Resources *resources, index_t index_to_root_node, index_t index_to_func_id){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_func_id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->is_declared_now = 1;
            return RETURN_OK;
    }
    else
        return NOT_FOUND;
}

int unset_declaration_flag(Resources *resources, index_t index_to_root_node, index_t index_to_func_id){

    args_assert(resources != NULL && index_to_root_node != ZERO_INDEX, INTERNAL_ERROR);

    char *str;
    index_t found_node_index;
    int found;
    TTree *found_node;

    str = load_token(&(resources->string_buff), index_to_func_id);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, &found_node_index, FUNC);

    if (found == FOUND){
        dereference_structure(&(resources->struct_buff_trees), found_node_index, (void**)&found_node);
        found_node->is_declared_now = 0;
            return RETURN_OK;
    }
    else
        return NOT_FOUND;
}

int str_length(char *string)
{
	int i = 0;
	while (*string)
	{
		string++;
		i++;
	}

	return i;
}

int find(TDynamic_buffer *b, index_t string, index_t substring)
{
	/* creating fail */

	char *search = load_token(b, substring);
	char *s = load_token(b, string);

	int search_lenght = str_length(search);

	int k,r;
    int i;
	if (search_lenght == 0)
		return 0;


	int *fail = malloc((search_lenght + 1) * sizeof(int));
	if (!fail)
	 	return -2;	// return INTERNAL_ERROR
    
    for (i = 0; i < search_lenght; i++)
        fail[i] = 0;

	fail[0] = 0;
	for (k = 1; k < search_lenght; k++)
	{
		r = fail[k-1];
		while (r > 0 && search[r-1] != search[k])
			r = fail[r-1];

		fail[k+1] = r + 1;
	}

	/* find */
	int tInd = 1, pInd = 1;
	int s_lenght = str_length(s);

	while (tInd <= s_lenght && pInd <= search_lenght)
	{
		if (pInd == 0 || s[tInd-1] == search[pInd-1])
		{
			tInd++;
			pInd++;
		}
		else
			pInd = fail[pInd - 1];
	}

	free(fail);
	fail = NULL;

	if (pInd > search_lenght)
		return tInd - search_lenght - 1;
	else
		return -1;
}

index_t sort(TDynamic_buffer *b, index_t string)
{
	save_token(b);	// whatever is in b, puts '\0' after that
	HeapSort(load_token(b, string));

	return string;
}

void SiftDown(char *a, int left, int right)
{
	int i,j,tmp;
	bool cont;
	i = left;
	j = 2 * i;
	tmp = a[i];
	cont = (j <= right);

	while (cont)
	{
		if (j < right)
		{
			if (a[j] < a[j + 1])
				j++;
		}

		if (tmp >= a[j])
			cont = false;
		else
		{
			a[i] = a[j];
			i = j;
			j = 2 * i;
			cont = (j <= right);
		}
	}

	a[i] = tmp;
}

void HeapSort(char *a)
{
	int i,left,right,tmp;
	int n = (int)strlen(a) - 1;

	left = n / 2 ;
	right = n ;
	for (i = left; i >= 0; i--)
		SiftDown(a, i, right);

	for (right = n ; right >= 1; right--)
	{
		tmp = a[0];
		a[0] = a[right];
		a[right] = tmp;
		SiftDown(a, 0, right - 1);
	}
}

