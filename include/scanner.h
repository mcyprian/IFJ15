/**
 * @file scanner.h
 * @author Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz> Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for scanner.c module
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dynamic_buffer.h>

enum
{
	ARROW,
	AUTO,
	CIN,
	CLOSING_BRACKET,
	CLOSING_CURLY_BRACKET,
	CLOSING_SQUARE_BRACKET,
	COLON,
	COMMA,
	COUT,
	DOUBLE,
	ELSE,
	EQUALS,
	EXCLAMATION,
	FOR,
	GREATER,
	IDENTIFIER,
	IF,
	INT,
	INTEGER,
	LESS,
	OPENING_BRACKET,
	OPENING_CURLY_BRACKET,
	OPENING_SQUARE_BRACKET,
	OPERATOR,
	RETURN,
	SEMICOLON,
	STRING,
	START,
};

typedef struct
{
	int type;
	char *value;
} Token;

int tokenize(FILE *fin, TDynamic_buffer *b);
