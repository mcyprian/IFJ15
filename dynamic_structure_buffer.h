/**
 * @file dynamic_structure.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for dynamic_structure
 */

 
#ifndef DYNAMIC_STRUCTURE_BUFFER_H
#define DYNAMIC_STRUCTURE_BUFFER_H

#include <stdio.h>
#include "error_macros.h"
#include "datatypes.h"

#define ZERO_INDEX 0

typedef struct {
   size_t size_of_type;
   index_t length;
   index_t next_free;
   void * buffer;
   char * flags;
} TDynamic_structure_buffer;


/** Fast acceess to buffer data
 * @param buffer pointer to TDynamic_structure_buffer
 * @param type type buffer was initialized to
 * @param index index to buffer element
 */
#define access(buffer, type, index)           \
        ((type*)buffer + (index))

/** Makes work with structure buffer easier.
 * @param buff Pointer to TDynamic_structure_buffer
 * @param index index_t variable to store index of new element
 * @param item pointer to store adress of new structure  
 */
#define new_item(buff, index, item)                                                \
    ((get_free_element_index((buff), (&index)),                                    \
      dereference_structure((buff), (index), (void**)(&item)) == INTERNAL_ERROR)?  \
     INTERNAL_ERROR : RETURN_OK)

/** Allocs memory for buffer, sets initial index and length.
 * @param b Pointer to TDynamic_structure_buffer
 * @param initial_length Initial length of buffer
 * @param size_of_type Size of structure to allocate
 * @return Zero on success, INTERNAL_ERROR on malloc error.
 */
int init_structure_buffer(TDynamic_structure_buffer *b, size_t initial_length, size_t size_of_type);

/** Realllocs buffer memory to new size.
 * @param b Pointer to TDynamic_structure_buffer
 * @return Zero on success, INTERNAL_ERROR on realloc error
 */
int realloc_structure_buffer(TDynamic_structure_buffer *b);

/** Free memory block, pointed to by b
 * @param b Pointer to TDynamic_structure_buffer
 */
void free_structure_buffer(TDynamic_structure_buffer *b);

/** Get index to another free structure from buffer
 * @param b Pointer to TDynamic_structure_buffer
 * @param Index Place for saving index of free structure
 * @return Returns INTERNAL_ERROR of RETURN_OK
 */
int get_free_element_index(TDynamic_structure_buffer *b, index_t * index);

/** Get pointer to structure on indexfrom buffer
 * @param b Pointer to TDynamic_structure_buffer
 * @param index Index of structure
 * @param structure Place for saving address of structure
 * @return Returns INTERNAL_ERROR of RETURN_OK
 */
int dereference_structure(TDynamic_structure_buffer *b, index_t  index, void ** structure);

/** Move writing index to next position.
 * @param b pointer to TDynamic_buffer
 * @param index Index of structure
 * @return Return INTERNAL_ERROR of RETURN_OK
 */
int free_element(TDynamic_structure_buffer *b, index_t index);

int push_stack(TDynamic_structure_buffer *b, TStack_variable ** var);
int pop_stack(TDynamic_structure_buffer *b);
#endif // !DYNAMIC_STRUCTURE_H

