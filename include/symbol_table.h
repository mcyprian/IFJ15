/**
 * @file symbol_table.h
 * @author Tomas Sykora <xsykor25@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for symbol_table.c module
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <datatypes.h>
#include <resources.h>

#define NOT_FOUND 1
#define FOUND 0

enum data_types{
	INTEGER,
	FLOAT,
	STRING
};

enum types_of_tokens{ 
	VAR, 
	FUNC 
};

typedef union Values {
	int i;	//value of integer variable or number of arguments of function
	float f;	//value of float variable
	index_t index_to_dynamic_buff;	//index to buffer with value of string variable
}TValues;

typedef struct func_args {
	int data_type;
	index_t index_to_string_identifier;
}TFunc_args;

typedef struct tree {
	unsigned long key;
	index_t index_to_dynamic_buff;
	index_t right, left, next;
	int type;	//function or variable
	union Values value;
	int data_type;	//(for function it means return value) one of enum data_types
	TFunc_args *args;
}TTree;


/** Inserts new node of variable to symbol talbe tree
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node pointer to index_t variable storing index to dynamic structure buffer
 * @param data_type data type of new variable
 * @return 0 on success, INTERNAL_ERROR on error
 */
int declare_variable(Resources *resources, index_t index_to_string, index_t *index_to_root_node, int data_type);

/** Sets value to float variable
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node pointer to index_t variable storing index to dynamic structure buffer
 * @param value value of variable to be stored in symbol table
 * @return 0 on success, INTERNAL_ERROR on error
 */
int set_float_value(Resources *resources, index_t index_to_string, index_t index_to_root_node, float value);

/** Sets value to integer variable
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param value value of variable to be stored in symbol table
 * @return 0 on success, NOT_FOUND if varieble is not in symbol table, INTERNAL_ERROR on error
 */
int set_int_value(Resources *resources, index_t index_to_string, index_t index_to_root_node, int value);

/** Sets value to string variable
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_dynamic_buff index to string stored in dynamic buffer
 * @return 0 on success, NOT_FOUND if varieble is not in symbol table, INTERNAL_ERROR on error
 */
int set_string_value(Resources *resources, index_t index_to_string, index_t index_to_root_node, index_t index_to_dynamic_buff);

/** Inserts new node of function to symbol talbe tree
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param ret_val data type of return value of the function
 * @return 0 on success, NOT_FOUND if varieble is not in symbol table, INTERNAL_ERROR on error
 */
int declare_function(Resources *resources, index_t index_to_string, index_t *index_to_root_node, int ret_val);

/** Adds another argument to declared function in symbol table
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node pointer to index_t variable storing index to dynamic structure buffer
 * @param data_type data type of the argument
 * @param index_to_dynamic_buff index to string of argument's identifier in dynamic buffer
 * @return 0 on success, INTERNAL_ERROR on malloc or realloc error
 */
int add_func_arg(Resources *resources, index_t index_to_string, index_t index_to_root_node, int data_type, index_t index_to_dynamic_buff);

/** Finds out wether the function or variable was declared before
 * @param resources pointer to structure with buffers
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param type differentiates between function and variable (use enum types_of_tokens)
 * @return 0 if declared, 1 if not declared, INTERNAL_ERROR on error
 */
int declaration_test(Resources *resources, index_t index_to_string, index_t index_to_root_node, int type);

/** Deallocs memory used to store funtion arguments
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 */
void free_memory(Resources *resources, index_t index_to_root_node);

/** Load value of variable stored in symbol table
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param value pointer to TValues union to store value of variable
 * @return 0 on succes, INTERNAL_ERROR on failure
 */
int load_variable_value(Resources *resources, index_t index_to_root_node, index_t index_to_string, TValues *value);

/** Gives data type of variable or data type of function return value
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param type_of_node int value to differentiate between function and variable, one of enum types_of_tokens
 * @return returns int value, one of enum data_types
 */
int get_data_type(Resources *resources, index_t index_to_root_node, index_t index_to_string, int type_of_node);

/** Gives value of float variable
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @return returns float value stored in symbol table
 */
float load_float_value(Resources *resources, index_t index_to_root_node, index_t index_to_string);

/** Gives value of integer variable
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @return returns integer value stored in symbol table
 */
int load_integer_value(Resources *resources, index_t index_to_root_node, index_t index_to_string);

/** Gives value of string variable
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @return returns index to dynamic buffer where string value is stored
 */
index_t load_string_value(Resources *resources, index_t index_to_root_node, index_t index_to_string);

/** Gives number of arguments of a function
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @return returns number of function's arguments
 */
int load_num_of_args(Resources *resources, index_t index_to_root_node, index_t index_to_string);

/** Gives number of arguments of a function
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param arg_index integer index to array of function's arguments 
 * @return returns data type of x-th argument (x is value in arg_index)
 */
int load_arg_data_type(Resources *resources, index_t index_to_root_node, index_t index_to_string, int arg_index);

#endif

