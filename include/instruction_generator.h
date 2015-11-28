
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

int generate_function_call(Resources *resouces, index_t function_id);

#endif // ! INS_GENERATOR_H
