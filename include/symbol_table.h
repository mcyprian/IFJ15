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

// enum token_types {IDENTIFIER, OPERATOR, KEYWORD};

typedef struct {
	int token_data_type;
	char *lexeme_identifier;
}Tlexeme_data;

typedef struct tree {
	Tlexeme_data lexeme;
	unsigned long key;
	index_t dynamic_buffer_index;
	struct tree *right, *left, *next;
}Ttree;

/**
*/
unsigned long hash_key(char *str);

/** Searches exact lexeme in symbol table according to its identifier.
 * @param tree pointer to a symbol table
 * @param lexeme_identifier string identifier of searched lexeme
 * @return pointer to found object if searched token was found, NULL if it was not
 */
Ttree* search_symbol(Ttree **tree, char *lexeme_identifier);

/** Adds new lexeme to symbol table
 * @param tree pointer to a symbol table
 * @param lexeme_identifier string identifier of searched lexeme
 * @return key to binary search tree, or -1 on malloc failure
 */
int add_symbol(Ttree **tree, char *lexeme_identifier, index_t dynamic_buffer_index);

/** Delete symbol table and dealloc memory
 * @param tree pointer to a symbol table
 */
void del_symbol_table(Ttree *tree);

/** Delete linked list and dealloc memory
 * @param tree pointer to a list node
 */
void del_list(Ttree *list);


#endif
