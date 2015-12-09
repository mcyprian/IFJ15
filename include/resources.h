/**
 * @file resources.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of incommon main.c syntax_analysis.c
 */

#ifndef RESOURCES_H
#define RESOURCES_H
#include <resources.h>
#include <dynamic_buffer.h>
#include <dynamic_structure_buffer.h>
#include <datatypes.h>
#include <stack.h>

union value{ 
	int i;
	double d;
	index_t index;
};

struct stack_variable{
	int type;
	int defined;
	TValue value;
};

struct res{
	FILE * source;
	TStack stack;
	TDynamic_buffer string_buff;
	TDynamic_structure_buffer struct_buff;
	TDynamic_structure_buffer struct_buff_trees;
	TDynamic_structure_buffer instruction_buffer;
	TDynamic_structure_buffer runtime_stack;
	TDynamic_structure_buffer func_table;
	index_t start_main;
	index_t ip;
	index_t bp;
	struct stack_variable return_value;
	int definitions_counter;
};

struct instruction{
    TValue dest;
    TValue first_op;
    TValue second_op;
    int ins;
};

#endif //!RESOURCES_H
