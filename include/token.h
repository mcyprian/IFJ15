/**
 * @file token.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module token.c, including typedef of general TToken struct.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <datatypes.h>
#include <dynamic_structure_buffer.h>


typedef struct {
    index_t token_index;           // Index to token string in dynamic_buffer
    int token_type;
    index_t token_next;            // Index to next token/expr in dynamic_structure
    int expr_type;
    index_t expr_next;             // Index to next token/expr in dynamic_structure
    TDynamic_structure_buffer *b;
} TToken;

enum token_types
{
	ARROW, // 0
	AUTO, // 1
	CIN, // 2
	CLOSING_BRACKET, // 3
	CLOSING_CURLY_BRACKET, // 4
	CLOSING_SQUARE_BRACKET, // 5
	COLON, // 6
	COMMA, // 7
	COUT, // 8
	DOUBLE, // 9
	ELSE, // 10
	EQUALS, // 11
	EXCLAMATION, // 12
	FOR, // 13
	GREATER, // 14
	IDENTIFIER, // 15
	IF,	// 16
	INT, // 17
	INTEGER, // 18
	LESS, // 19
	MINUS, // 20
	OPENING_BRACKET, // 21
	OPENING_CURLY_BRACKET, // 22
	OPENING_SQUARE_BRACKET, // 23
	OPERATOR, // 24
	RETURN, // 25
	SCIENTIFIC, // 26
	SEMICOLON, // 27
	STRING, // 28
	STRING_LITERAL, // 29
	COMMENT, // 30
	LINE_COMMENT, // 31
	BLOCK_COMMENT, // 32
	BLOCK_COMMENT_END, // 33
	START, // 34
	ERRORT, // 35
	EOFT // 36
};


#endif // !TOKEN_H
