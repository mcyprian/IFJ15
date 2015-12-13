/**
 * project: Implementace interpretu imperativního jazyka IFJ15
 * @file built_functions.h
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for built_functions.c module
 */

#include <stdio.h>
#include <stdlib.h>
#include "dynamic_buffer.h"
#include <string.h>

/** Computes the length of given string
 * @param b pointer to TDyanmic buffer
 * @index index returned from save_token (index in the buffer where the string starts)
 * @return length of string
 */
int length(TDynamic_buffer *b, index_t index);

/** Returns substring of given string from index i of n-length
 * @param b pointer to TDyanmic buffer
 * @index index returned from save_token (index in the buffer where the string starts)
 * @index i index where substring starts in the given string
 * @return index where the substring string starts (in buffer b)
 */
index_t substr(TDynamic_buffer *b, index_t index, index_t i, int n);

/** Concats 2 strings
 * @param b pointer to TDyanmic buffer where the first string is 
 * @index string index in the buffer where the first string starts
 * @param c pointer to TDyanmic buffer where the second string is (can be mutual with b)
 * @index string index in the buffer where the second string starts
 * @return Index where the concated string starts (in buffer b)
 */
index_t concat(TDynamic_buffer *b, index_t index1, index_t index2);
