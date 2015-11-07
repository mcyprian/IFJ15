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
#include <resources.h>

#define NOT_FOUND 99
#define EMPTY_TREE 99

typedef struct tree {
	unsigned long key;
	index_t token, right, left, next;
}TTree;

/** Finds token in symbol table according to its string identifier.
 * @param resources pointer to structure storing buffers
 * @param index_to_string index to dynamic buffer
 * @param index_to_root_node index of root node to TDynamic_structure_buffer of TTree nodes
 * @param found_token_index pointer to variable storing index of found token to TDynamic_structure_buffer of tokens
 * @return returns RETURN_OK if token was found, NOT_FOUND if was not
 */
int find_symbol(Resources *resources, index_t index_to_string, index_t index_to_root_node, index_t *found_token_index);
/** Adds new token to symbol table
 * @param resources pointer to structure storing buffers
 * @param token_to_store index to TDynamic structure buffer
 * @param index_to_root_node pointer to index_t variable storing index of root node to TDynamic_structure_buffer of TTree nodes 
 * @return returns RETURN_OK or INTERNAL_ERROR
 */
int insert_symbol(Resources *resources, index_t token_to_store, index_t *index_to_root_node);

#endif
