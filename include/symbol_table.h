/**
 * @file symbol_table.h
 * @author Tomas Sykora <xsykor25@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for symbol_table.c module
 */

 enum token_types {IDENTIFIER, OPERATOR, KEYWORD};

typedef struct {
	int token_data_type;
	char *lexeme_identifier;
}Tlexeme_data;

typedef struct tree {
	Tlexeme_data lexeme;
	int key;
	struct tree *right, *left;
}Ttree;

/** Initializes symbol table.
 * @param tree pointer to Ttree (structure of binary tree's node)
 */
void symbol_table_init(Ttree *tree);

/** Searches exact lexeme in symbol table according to its identifier.
 * @param tree pointer to a symbol table
 * @param lexeme_identifier string identifier of searched lexeme
 * @return pointer to found object if searched token was found, NULL if was not
 */
Ttree* search_token(Ttree **tree, char *lexeme_identifier);

/** Adds new lexeme to symbol table
 * @param tree pointer to a symbol table
 * @param lexeme_identifier string identifier of searched lexeme
 * @return key to binary search tree, or -1 on malloc failure
 */
int add_lexeme(Ttree **tree, char *lexeme_identifier);

/** Delete symbol table and dealloc memory
 * @param tree pointer to a symbol table
 */
void del_symbol_table(Ttree *tree);
