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

index_t get_token_(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer);

int reservedWord(char *identifier);

//int tokenize(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buff);
