/**
 * @file scanner.h
 * @author Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz> Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for scanner.c module
 */

#include <dynamic_structure_buffer.h>
#include <token.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dynamic_buffer.h>
#include <debug.h>
#include <error_macros.h>


//presun tento enum do token.h

enum
{
	ARROW, //0
	AUTO, //1
	CIN, //2
	CLOSING_BRACKET, //3
	CLOSING_CURLY_BRACKET, //4
	CLOSING_SQUARE_BRACKET, //5
	COLON, //6
	COMMA, //7
	COUT, //8
	DOUBLE, //9
	ELSE, //10
	EQUALS, //11
	EXCLAMATION, //12
	FOR, //13
	GREATER, //14
	IDENTIFIER, //15
	IF,	//16
	INT, //17
	INTEGER, //18
	LESS, //19
	OPENING_BRACKET, //20
	OPENING_CURLY_BRACKET, //21
	OPENING_SQUARE_BRACKET, //22
	OPERATOR, //23
	RETURN, //24
	SEMICOLON, //25
	STRING, //26
	START, //27
	STRING_LITERAL //28
};

typedef struct
{
	int type;
	char *value;
} Token;

index_t get_token_(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer);

//int tokenize(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buff);
