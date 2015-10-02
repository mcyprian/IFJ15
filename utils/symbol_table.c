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

enum token_types {IDENTIFIER, OPERATOR, KEYWORD};

typedef struct {
	int token_type;
	char *lexeme_identifier;
}Tlexeme_data;

typedef struct tree {
	Tlexeme_data lexeme;
	int key;
	struct tree *right, *left;
}Ttree;

void Symbol_Table_Init(Ttree *tree) {

	tree = NULL;
}

int Get_Key(char *lexeme_identifier) {

	int i, key;

	for( i=0; lexeme_identifier[i]!='\0'; i++) {
		key += lexeme_identifier[i];
	}

	return key;
}

Ttree* Search_Token(Ttree **tree, char *lexeme_identifier) {

	int key = Get_Key(lexeme_identifier);

	if (*tree == NULL) {
		return NULL;
	}
	else if( key < (*tree)->key) {
		Search_Token(&((*tree)->left), lexeme_identifier);
	}
	else if( key > (*tree)->key) {
		Search_Token(&((*tree)->right), lexeme_identifier);
	}
	else if( key == (*tree)->key) {
		return *tree;
	}
}

int Add_Lexeme(Ttree **tree, char *lexeme_identifier) {

	Ttree *temporary = NULL;
	int key = Get_Key(lexeme_identifier);
	Tlexeme_data lexeme = { .token_type = IDENTIFIER, .lexeme_identifier = lexeme_identifier };

	if(*tree == NULL) {
		temporary = (Ttree *)malloc(sizeof(Ttree));
		if(temporary == NULL) {
			fprintf(stderr, "Malloc error!");
			return -1;
		}

		temporary->left = temporary->right = NULL;
		temporary->lexeme = lexeme;
		temporary->key = key;
		*tree = temporary;

		return key;
	}

	if(key < (*tree)->key) {
		Add_Lexeme(&((*tree)->left), lexeme_identifier);
	}

	else if(key > (*tree)->key) {
		Add_Lexeme(&((*tree)->right), lexeme_identifier);
	}
}

void Del_Symbol_Table(Ttree *tree) {
	if(tree){
		Del_Symbol_Table(tree->right);
		Del_Symbol_Table(tree->left);
		free(tree);
	}
}