/**
 * @file symbol_table.c
 * @author Tomas Sykora <xsykor25@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * A symbol table used for storing information about found lexemes, representated 
 * by a binary search tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <error_macros.h>
#include <debug.h>
#include <symbol_table.h>
#include <datatypes.h>
#include <scanner.h>

unsigned long hash_key(char *str) {

	args_assert(str != NULL, INTERNAL_ERROR);

	unsigned long hash = 5381;
	int c;

	while ((c = (*str)++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

Ttree* search_symbol(Ttree **tree, char *lexeme_identifier) {
	
	// args_assert(lexeme_identifier != NULL, INTERNAL_ERROR); 			// nemoze vraciat int

	Ttree *tmp;

	unsigned long key = hash_key(lexeme_identifier), s;

	if (*tree == NULL) {
		return NULL;
	}
	else if( key < (*tree)->key) {
		search_symbol(&((*tree)->left), lexeme_identifier);
	}
	else if( key > (*tree)->key) {
		search_symbol(&((*tree)->right), lexeme_identifier);
	}
	else if( key == (*tree)->key) {
		if((s = strcmp(lexeme_identifier, (*tree)->lexeme.lexeme_identifier)) == 0)
			return *tree;
		else {
			tmp = (*tree)->next;
			while(((s = strcmp(lexeme_identifier, tmp->lexeme.lexeme_identifier)) != 0) && tmp != NULL)
				tmp = tmp->next;
			if(tmp != NULL)
				return tmp;
			else
				return NULL;
		}
	}
	return NULL;

}

int add_symbol(Ttree **tree, char *lexeme_identifier, index_t dynamic_buffer_index) {
	
	args_assert(lexeme_identifier != NULL, INTERNAL_ERROR);

	Ttree *tmp = NULL, *list;
	unsigned long key = hash_key(lexeme_identifier);
	Tlexeme_data lexeme = { .token_data_type = IDENTIFIER, .lexeme_identifier = lexeme_identifier };

	if(*tree == NULL) {
		tmp = (Ttree *)malloc(sizeof(Ttree));
		catch_internal_error(tmp, NULL, "Failed to allocate memory.");
		tmp->left = tmp->right = tmp->next = NULL;
		tmp->lexeme = lexeme;
		tmp->key = key;
		tmp->dynamic_buffer_index = dynamic_buffer_index;
		*tree = tmp;

		return key;
	}

	if(key < (*tree)->key) {
		add_symbol(&((*tree)->left), lexeme_identifier, dynamic_buffer_index);
	}
	else if(key > (*tree)->key) {
		add_symbol(&((*tree)->right), lexeme_identifier, dynamic_buffer_index);
	}
	else if(key == (*tree)->key) {
			list = (*tree)->next;
			while (list!= NULL) {
				list = list->next;
			}
			tmp = (Ttree *)malloc(sizeof(Ttree));
			catch_internal_error(tmp, NULL, "Failed to allocate memory.");
			tmp->next = NULL;
			tmp->lexeme = lexeme;
			tmp->key = key;
			tmp->dynamic_buffer_index = dynamic_buffer_index;
			list = tmp;
	}

	return 0; // zatial vracia 0, len nech nieco vracia a nehadze tie warningy
}

void del_symbol_table(Ttree *tree) {
	if(tree){
		if(tree->next)
			del_list(tree->next);

		del_symbol_table(tree->right);
		del_symbol_table(tree->left);
		free(tree);
	}
}

void del_list(Ttree *list) {
	if(list){
		del_list(list->next);
		free(list);
	}
}
