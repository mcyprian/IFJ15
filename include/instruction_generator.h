
#include <stdio.h>
#include <stdlib.h>
#include <dynamic_buffer.h>
#include <dynamic_structure_buffer.h>
#include <stack.h>
#include <symbol_table.h>
#include <semantics.h>
#include <debug.h>
#include <datatypes.h>
#include <instructions.h>

#ifndef INS_GENERATOR_H
#define INS_GENERATOR_H

/** Generates instructions for function calls
 * @param resources pointer to structure with buffers
 * @param function_id index to dynamic_buffer to function identifier
 * @return returns RETURN_OK on succes, SEMANTIC_ERROR, INTERNAL_ERROR, SYNTAX_ERROR or LEXICAL_ERROR on error
 */
int generate_function_call(Resources *resouces, index_t function_id);

#endif // ! INS_GENERATOR_H
