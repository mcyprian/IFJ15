/**
 * @file ial.h
 * @author Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for ial.c module
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dynamic_buffer.h>
#include <string.h>

/** Finds the first occurrence of a string search (in buffer c) in a string s (in buffer b)
 * @param b pointer to TDyanmic buffer where string s is 
 * @index string index in the buffer where the string "string" starts
 * @index string index in the buffer where the string "search" starts
 * @return The first occurence of the "search" on success, 0 if "search" is empty, -1 if "search" is not found, -2 on error
 */
int find(TDynamic_buffer *b, index_t string, index_t substring);

/** Sorts characters in the given string according to their values in ASCII table
 * @param b pointer to TDyanmic buffer where string s is 
 * @param string index in the buffer where the string is
 * @return index to string where sorted string is
 */
index_t sort(TDynamic_buffer *b, index_t string);

/** Algorithm for reconstructing the heap
 * @param a pointer to the given string
 * @param left left element
 * @param right right element
 */
void SiftDown(char *a, int left, int right);

/** Sorts characters in the given string using HeapSort algorithm
 * @param a pointer to the given string
 */
void HeapSort(char *a);

/** Computes the length of given string
 * @param string pointer to the given string
 * @return the length of given string
 */
int str_length(char *string);