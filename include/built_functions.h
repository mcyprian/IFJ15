/**
 * @file built_functions.h
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for built_functions.c module
 */

#include <stdio.h>
#include <stdlib.h>
#include <dynamic_buffer.h>
#include <string.h>

/** Computes the length of given string
 * @param b pointer to TDyanmic buffer
 * @index index returned from save_token (index in the buffer where the string starts)
 * @return length of string
 */
int lenght(TDynamic_buffer *b, index_t index);

/** Returns substring of given string from index i of n-length
 * @param b pointer to TDyanmic buffer
 * @index index returned from save_token (index in the buffer where the string starts)
 * @index i index where substring starts in the given string
 * @return substring on success, NULL on error
 */
char *substr(TDynamic_buffer *b, index_t index, index_t i, int n);