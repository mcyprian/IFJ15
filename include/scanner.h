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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dynamic_buffer.h>
#include <debug.h>
#include <error_macros.h>


typedef struct
{
	int type;
	char *value;
} Token;


/** Scans for next token.
 *  @param fin input text stream/file
 *  @param buffer pointer to TDynamic_buffer
 *  @param struct_buffer pointer do TDynamic_structure_buffer
 *  @return index of TToken in TDynamic_structure_buffer
 */
index_t get_token(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer);

/** Checks if the given string belongs to reserved words
 * @param identifier given string
 * @return type of the reserved word or IDENTIFIER if it is not a reserved word
 */
int reservedWord(char *identifier);