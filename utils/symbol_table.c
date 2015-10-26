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
#include <dynamic_buffer.h>
#include <dynamic_structure_buffer.h>
#include <token.h>



int hash_key(char *str, unsigned long *hash) {

	args_assert(str != NULL, INTERNAL_ERROR);

	int c;

	*hash = 5381;

	while ((c = (*str)++))
		*hash = (((*hash) << 5) + *hash) + c;

	return RETURN_OK;
}

int add_symbol(TDynamic_structure_buffer *struct_buff_tokens, index_t token_to_store, TDynamic_structure_buffer *struct_buff_nodes, TDynamic_buffer *b){

	args_assert(struct_buff_tokens != NULL && token_to_store > 0 && struct_buff_nodes != NULL && b!= NULL, INTERNAL_ERROR);

	index_t new_node_index;
	TTree *new_node;
	TToken *token;
	unsigned long key;
	char *str = NULL;

	new_item(struct_buff_nodes, new_node_index, new_node);

	if(new_node_index == 1){
		new_node->left = new_node->right = new_node->next = ZERO_INDEX;
		dereference_structure(struct_buff_tokens, token_to_store, (void**)&token);
		str = load_token(b, token->token_index);
		catch_internal_error(str, NULL, "Failed to load token string.");
		hash_key(str, &key);
		new_node->key = key;
		new_node->token = token_to_store;
	} else 
	{
		create_node(str, token_to_store, struct_buff_nodes, 1, b, new_node_index);
	}

	return RETURN_OK;
}

int create_node(char *str, index_t token_to_store, TDynamic_structure_buffer *struct_buff_nodes, index_t actual_node_index, TDynamic_buffer *b, index_t new_node_index){

	args_assert(str != NULL && token_to_store > 0 && struct_buff_nodes != NULL && actual_node_index > 0 && b != NULL && new_node_index > 0, INTERNAL_ERROR);

	TTree *node, *new_node;
	unsigned long key;

	dereference_structure(struct_buff_nodes, actual_node_index, (void**)&node);
	hash_key(str, &key);

	if(key > node->key){
		if(node->right == ZERO_INDEX){
			dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
			node->right = new_node_index;
			new_node->left = new_node->right = new_node->next = ZERO_INDEX;
			new_node->key = key;
			new_node->token = token_to_store;
		}
		else
		{
			actual_node_index = node->right;
			create_node(str, token_to_store, struct_buff_nodes, actual_node_index, b, new_node_index);
		}
	}
	else if(key < node->key){
		if(node->left == ZERO_INDEX){
			dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
			node->left = new_node_index;
			new_node->right = new_node->left = new_node->next = ZERO_INDEX;
			new_node->key = key;
			new_node->token = token_to_store;
		}
		else
		{
			actual_node_index = node->left;
			create_node(str, token_to_store, struct_buff_nodes, actual_node_index, b, new_node_index);
		}
	}
	else if(key == node->key){
		while(node->next != ZERO_INDEX)
		{
			dereference_structure(struct_buff_nodes, node->next, (void**)&node);
		}
		node->next = new_node_index;
		dereference_structure(struct_buff_nodes, new_node_index, (void**)&new_node);
		new_node->right = new_node->left = new_node->next = ZERO_INDEX;
		new_node->key = key;
		new_node->token = token_to_store;
	}
	return RETURN_OK;
}

index_t find_symbol(TDynamic_structure_buffer *struct_buff_tokens, TDynamic_structure_buffer *struct_buff_nodes, TDynamic_buffer *b, index_t index_to_string){

	args_assert(struct_buff_tokens != NULL && struct_buff_nodes != NULL && b != NULL, INTERNAL_ERROR);

	index_t found_token_index;
    int found;

	if(struct_buff_nodes->next_free == 1)
		return EMPTY_TREE;
	else 
	{
		found = iterate_through_tree(struct_buff_tokens, b, index_to_string, struct_buff_nodes, 1, &found_token_index);
	}
	if(found == 0)
	{
		return found_token_index;
	}
	else
	{
		return NOT_FOUND;
	}
}

int iterate_through_tree(TDynamic_structure_buffer *struct_buff_tokens, TDynamic_buffer *b, index_t index_to_string, TDynamic_structure_buffer *struct_buff_nodes, index_t actual_node_index, index_t *found_token_index){

	args_assert(struct_buff_tokens != NULL && b != NULL && struct_buff_nodes != NULL, INTERNAL_ERROR);

	TTree *actual_node;
	TToken *token;
	unsigned long key;
	char *str;
	bool found = false;

	dereference_structure(struct_buff_nodes, actual_node_index, (void**)&actual_node);

	str = load_token(b, index_to_string);
	hash_key(str, &key);

	if(actual_node_index == ZERO_INDEX)
		return NOT_FOUND;
	else if(key > actual_node->key)
	{
		iterate_through_tree(struct_buff_tokens, b, index_to_string, struct_buff_nodes, actual_node->right, found_token_index);
	}
	else if(key < actual_node->key)
	{
		iterate_through_tree(struct_buff_tokens, b, index_to_string, struct_buff_nodes, actual_node->left, found_token_index);
	}
	else if(key == actual_node->key)
	{
		dereference_structure(struct_buff_tokens, actual_node->token, (void**)&token);
		if(strcmp(str, load_token(b, token->token_index)) == 0)
		{
			*found_token_index = actual_node->token;
			return RETURN_OK;
		}
		else
		{
			while(actual_node->next != ZERO_INDEX && found == false)
			{
				actual_node_index = actual_node->next;
				dereference_structure(struct_buff_nodes, actual_node_index, (void**)&actual_node);
				dereference_structure(struct_buff_nodes, actual_node->token, (void**)&token);
				if (strcmp(str, load_token(b, token->token_index)) == 0)
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
	return RETURN_OK;
}
