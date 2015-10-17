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

#include <dynamic_structure_buffer.h>

typedef unsigned long index_t;

typedef struct {
    index_t token_index;           // Index to token string in dynamic_buffer
    int token_type;
    index_t token_next;            // Index to next token/expr in dynamic_structure
    int expr_type;
    index_t expr_next;             // Index to next token/expr in dynamic_structure
    TDynamic_structure_buffer *b;
} TToken;


#define new_item(buff, index, item)                                                \
    ((get_free_element_index((buff), (&index)),                                    \
      dereference_structure((buff), (index), (void**)(&item)) == INTERNAL_ERROR)?  \
     INTERNAL_ERROR : RETURN_OK)

#endif // !TOKEN_H
