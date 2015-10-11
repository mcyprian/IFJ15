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

void symbol_table_init(Ttree *tree) {

	tree = NULL;
}

int get_key(char *lexeme_identifier) {
	
	args_assert(lexeme_identifier != NULL, INTERNAL_ERROR);

	int i, key = 0;

	for( i=0; lexeme_identifier[i]!='\0'; i++) {
		key += lexeme_identifier[i];
	}

	return key;
}

Ttree* search_token(Ttree **tree, char *lexeme_identifier) {
	
	args_assert(lexeme_identifier != NULL, INTERNAL_ERROR);

	int key = get_key(lexeme_identifier);

	if (*tree == NULL) {
		return NULL;
	}
	else if( key < (*tree)->key) {
		search_token(&((*tree)->left), lexeme_identifier);
	}
	else if( key > (*tree)->key) {
		search_token(&((*tree)->right), lexeme_identifier);
	}
	else if( key == (*tree)->key) {
		return *tree;
	}
}

int add_lexeme(Ttree **tree, char *lexeme_identifier) {
	
	args_assert(lexeme_identifier != NULL, INTERNAL_ERROR);

	Ttree *tmp = NULL;
	int key = get_key(lexeme_identifier);
	Tlexeme_data lexeme = { .token_data_type = IDENTIFIER, .lexeme_identifier = lexeme_identifier };

	if(*tree == NULL) {
		tmp = (Ttree *)malloc(sizeof(Ttree));
		catch_internal_error(tmp, NULL, "Failed to allocate memory.");
		tmp->left = tmp->right = NULL;
		tmp->lexeme = lexeme;
		tmp->key = key;
		*tree = tmp;

		return key;
	}

	if(key < (*tree)->key) {
		add_lexeme(&((*tree)->left), lexeme_identifier);
	}

	else if(key > (*tree)->key) {
		add_lexeme(&((*tree)->right), lexeme_identifier);
	}
}

void del_symbol_table(Ttree *tree) {
	if(tree){
		del_symbol_table(tree->right);
		del_symbol_table(tree->left);
		free(tree);
	}
}
