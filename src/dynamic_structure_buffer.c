/**
 * @file dynamic_structure.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 * 
 * @section DESCRIPTION
 *  
 * Header file for dynamic_structure
 **/


#include <stdlib.h>
#include <string.h>
#include <dynamic_structure_buffer.h>
#include <error_macros.h>
#include <debug.h>

int init_structure_buffer(TDynamic_structure_buffer *b, size_t initial_length, size_t size_of_type){
	args_assert(b != NULL && initial_length > 0, INTERNAL_ERROR);
	debug_print("%s\n", "STRUCT BUFFER INIT");

	initial_length++;

	b->buffer = calloc(initial_length,  size_of_type);
	catch_internal_error(b->buffer, NULL, "Failed to allocate memory for buffer.1");

	b->flags = calloc(initial_length, 1);
	catch_internal_error(b->flags, NULL, "Failed to allocate memory for buffer.2");
	
	b->next_free = 1;
	b->length = initial_length;
	b->size_of_type = size_of_type;

	return RETURN_OK;
}


int realloc_structure_buffer(TDynamic_structure_buffer *b){
	args_assert(b != NULL, INTERNAL_ERROR);


	char *tmp = NULL;
	tmp = realloc(b->buffer, b->length * b->size_of_type* 2);
	catch_internal_error(tmp, NULL, "Failed to realloc memory for buffer.");
	b->buffer = tmp;

	tmp = NULL;
	tmp = realloc(b->flags, b->length * 2);
	catch_internal_error(tmp, NULL, "Failed to realloc memory for buffer.");
	b->flags = tmp;
	debug_print("%s %lu\n", "REALLOCATED TO", b->length * 2);

	memset((char *)b->buffer + b->length * b->size_of_type, 0, b->length * b->size_of_type);
	memset(b->flags + b->length, 0, b->length);
	b->length *= 2;

	return RETURN_OK;
}

void free_structure_buffer(TDynamic_structure_buffer *b){
    debug_print("%s\n", "STRUCT BUFFER FREE");
	if(b != NULL){
		free(b->buffer);
		free(b->flags);
	}
}

int get_free_element_index(TDynamic_structure_buffer *b, index_t * index){
	args_assert(b != NULL && index != NULL, INTERNAL_ERROR);

	if(b->next_free >= b->length)
		catch_internal_error(realloc_structure_buffer(b), INTERNAL_ERROR, "Failed to realloc buffer.");

	b->flags[b->next_free] = 1;
	debug_print("%s %lu\n", "GET ELEMENT", b->next_free);
	*index = b->next_free;
	
	do {
		b->next_free++;
	} while(b->next_free < b->length && b->flags[b->next_free]) ;

	return RETURN_OK;	
}

int dereference_structure(TDynamic_structure_buffer *b, index_t  index, void ** structure){
	args_assert(b != NULL && structure != NULL, INTERNAL_ERROR);

	catch_internal_error(index >= b->length, 1, "Bad index.");

	*structure = (char *)b->buffer + index * b->size_of_type;
	
	return RETURN_OK;
}

int free_element(TDynamic_structure_buffer *b, index_t index){
	args_assert(b != NULL, INTERNAL_ERROR);	

	catch_internal_error(index >= b->length, 1, "Bad index.");
    debug_print("%s %lu\n", "FREE ELEMENT", index);

	if(!b->flags[index])return RETURN_OK;

	memset((char *)b->buffer + index * b->size_of_type, 0, b->size_of_type);
	b->flags[index] = 0;
	
	if(index < b->next_free)b->next_free = index;
	
	return RETURN_OK;
}

int push_stack(TDynamic_structure_buffer *b, TStack_variable ** var){
	args_assert(b != NULL, INTERNAL_ERROR);

	index_t index;
	int iRet = RETURN_OK;		
	if ((iRet = get_free_element_index(b, &index)) != RETURN_OK)return iRet;
	if ((iRet = dereference_structure(b, index, (void **)var)) != RETURN_OK)return iRet;
	b->flags[b->next_free++] = 1;
	return RETURN_OK;
}

int pop_stack(TDynamic_structure_buffer *b){
	args_assert(b != NULL, INTERNAL_ERROR);
	
	if (b->next_free > 1){
		b->flags[--b->next_free] = 0;
		return RETURN_OK;	
	}
	else return -1;
}
