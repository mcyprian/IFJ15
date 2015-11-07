/**
 * @file symbol_table.c
 * @author Tomas Sykora <xsykor25@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * A symbol table storing indexes to tokens in dynamic_structure_buffer.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <error_macros.h>
#include <debug.h>
#include <symbol_table.h>
#include <datatypes.h>
#include <resources.h>
#include <token.h>

int hash_key(char *str, unsigned long *hash) {

    args_assert(str != NULL, INTERNAL_ERROR);

    int c;

    *hash = 5381;

    while ((c = (*str)++))
        *hash = (((*hash) << 5) + *hash) + c;

    return RETURN_OK;
}

int create_node(index_t index_to_node, index_t new_node_index, unsigned long key, TDynamic_structure_buffer *struct_buff_nodes, index_t token_to_store){

    args_assert(struct_buff_nodes != NULL && token_to_store > 0, INTERNAL_ERROR);

    TTree *new_node, *actual_node;

    dereference_structure(struct_buff_nodes, index_to_node, (void**)&actual_node);

    if (key > actual_node->key){
        if (actual_node->right == ZERO_INDEX){
            dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
            actual_node->right = new_node_index;
            new_node->left = new_node->right = new_node->next = ZERO_INDEX;
            new_node->key = key;
            new_node->token = token_to_store;
        }
        else
            create_node(actual_node->right, new_node_index, key, struct_buff_nodes, token_to_store);
    }
    else if (key < actual_node->key){
        if (actual_node->left == ZERO_INDEX){
            dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
            actual_node->left = new_node_index;
            new_node->left = new_node->right = new_node->next = ZERO_INDEX;
            new_node->key = key;
            new_node->token = token_to_store;
        }
        else
            create_node(actual_node->left, new_node_index, key, struct_buff_nodes, token_to_store);
    }
    else {  //key == actual_node->key  --- hash colision
        while(actual_node->next != ZERO_INDEX)
        {
            dereference_structure(struct_buff_nodes, actual_node->next, (void**)&actual_node);
        }
        actual_node->next = new_node_index;
        dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
        new_node->right = new_node->left = new_node->next = ZERO_INDEX;
        new_node->key = key;
        new_node->token = token_to_store;
    }
    return RETURN_OK;
}

int insert_symbol(Resources *resources, index_t token_to_store, index_t *index_to_root_node){

    args_assert(resources != NULL && token_to_store > 0, INTERNAL_ERROR);

    index_t new_node_index;
    TTree *new_node;
    TToken *token;
    unsigned long key;
    char *str = NULL;

    new_item(&(resources->struct_buff_nodes), new_node_index, new_node);

    dereference_structure(&(resources->struct_buff), token_to_store, (void**)&token);
    str = load_token(&(resources->string_buff), token->token_index);
    catch_internal_error(str, NULL, "Failed to load token string.");
    hash_key(str, &key);

    if (*index_to_root_node == ZERO_INDEX){
        *index_to_root_node = new_node_index;
        new_node->right = new_node->left = new_node->next = ZERO_INDEX;
        new_node->key = key;
        new_node->token = token_to_store;
    }
    else
        create_node(*index_to_root_node, new_node_index, key, &(resources->struct_buff_nodes), token_to_store);
    
    return RETURN_OK;
}

int iterate_through_tree(Resources *resources, char *str, index_t actual_node_index, index_t *found_token_index){

    args_assert(resources != NULL && str != NULL, INTERNAL_ERROR);

    TTree *actual_node;
    TToken *token;
    bool found = false;
    unsigned long key;
    int v;

    hash_key(str, &key);
    dereference_structure(&(resources->struct_buff_nodes), actual_node_index, (void**)&actual_node);

    if(actual_node_index == ZERO_INDEX)
        return NOT_FOUND;
    else if(key > actual_node->key)
    {
        v = iterate_through_tree(resources, str, actual_node->right, found_token_index);
    }
    else if(key < actual_node->key)
    {
        v = iterate_through_tree(resources, str, actual_node->left, found_token_index);
    }
    else if(key == actual_node->key)
    {
        dereference_structure(&(resources->struct_buff), actual_node->token, (void**)&token);
        if(strcmp(str, load_token(&(resources->string_buff), token->token_index)) == 0)
        {
            *found_token_index = actual_node->token;
            return RETURN_OK;
        }
        else //hash colision
        {
            while(actual_node->next != ZERO_INDEX && found == false)
            {
                actual_node_index = actual_node->next;
                dereference_structure(&(resources->struct_buff_nodes), actual_node_index, (void**)&actual_node);
                dereference_structure(&(resources->struct_buff), actual_node->token, (void**)&token);
                if (strcmp(str, load_token(&(resources->string_buff), token->token_index)) == 0)
                    found = true;
            }
            if(found == true)
            {
                *found_token_index = actual_node->token;
                return RETURN_OK;
            }
            else
                return NOT_FOUND;
        }
    }
    
    if (v == RETURN_OK)
        return RETURN_OK;
    else
        return NOT_FOUND;
}

int find_symbol(Resources *resources, index_t index_to_string, index_t index_to_root_node, index_t *found_token_index){

    args_assert(resources != NULL, INTERNAL_ERROR);

    char *str;
    int found;

    str = load_token(&(resources->string_buff), index_to_string);
    catch_internal_error(str, NULL, "Failed to load token string.");

    found = iterate_through_tree(resources, str, index_to_root_node, found_token_index);

    if (found == RETURN_OK)
        return RETURN_OK;
    else
        return NOT_FOUND;
}
