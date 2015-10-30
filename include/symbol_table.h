/**
 * @file symbol_table.h
 * @author Tomas Sykora <xsykor25@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for symbol_table.c module
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <datatypes.h>

#define NOT_FOUND 99
#define EMPTY_TREE 99

typedef struct tree {
	unsigned long key;
	index_t token, right, left, next;
}TTree;

/** Finds token in symbol table according to its string identifier.
 * @param struct_buff_tokens pointer to TDynamic_structure_buffer of tokens
 * @param b pointer to dynamic buffer storing string indetifiers of tokens
 * @param index_to_string index to dynamic buffer
 * @param struct_buff_nodes pointer to TDynamic_structure_buffer of TTree nodes
 * @param index_to_root_node index of root node to TDynamic_structure_buffer of TTree nodes
 * @param found_token_index pointer to variable storing index of found token to TDynamic_structure_buffer of tokens
 * @return returns RETURN_OK if token was found, NOT_FOUND if was not
 */
int find_symbol(TDynamic_structure_buffer *struct_buff_tokens, TDynamic_buffer *b, index_t index_to_string, TDynamic_structure_buffer *struct_buff_nodes, index_t index_to_root_node, index_t *found_token_index);
/** Adds new token to symbol table
 * @param struct_buff_tokens pointer to TDynamic_structure_buffer of tokens
 * @param token_to_store index to TDynamic structure buffer
 * @param struct_buff_nodes pointer to TDynamic_structure_buffer of TTree nodes
 * @param index_to_root_node pointer to index_t variable storing index of root node to TDynamic_structure_buffer of TTree nodes
 * @param b pointer to dynamic buffer storing string indetifiers of tokens 
 * @return returns RETURN_OK or INTERNAL_ERROR
 */
int insert_symbol(TDynamic_structure_buffer *struct_buff_tokens, index_t token_to_store, TDynamic_structure_buffer *struct_buff_nodes, index_t *index_to_root_node, TDynamic_buffer *b);

#endif
