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
 * @param struct_buff_nodes pointer to TDynamic_structure_buffer of TTree nodes
 * @param b pointer to dynamic buffer storing string indetifiers of tokens
 * @param index_to_string index to dynamic buffer 
 * @return index to dynamic structure buffer or NOT_FOUND if searched string was not found
 */
index_t find_symbol(TDynamic_structure_buffer *struct_buff_tokens, TDynamic_structure_buffer *struct_buff_nodes, TDynamic_buff *b, index_t index_to_string);

/** Adds new token to symbol table
 * @param struct_buff_tokens pointer to TDynamic_structure_buffer of tokens
 * @param token_to_store index to TDynamic structure buffer
 * @param struct_buff_nodes pointer to TDynamic_structure_buffer of TTree nodes
 * @param b pointer to dynamic buffer storing string indetifiers of tokens 
 * @return returns RETURN_OK or INTERNAL_ERROR
 */
int add_symbol(TDynamic_structure_buffer *struct_buff_tokens, index_t token_to_store, TDynamic_structure_buffer *struct_buff_nodes, TDynamic_buff *b);

#endif
