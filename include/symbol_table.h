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
#include <stdbool.h>

#define NOT_FOUND (-1)
#define FOUND 0
#define ARG_FOUND (-2)
#define NO_DATA_TYPE 0

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
	index_t next;
	index_t index_to_struct_buffer;
	unsigned long key;
	index_t index_to_dynamic_buff;
	index_t right, left, next_node;
	int type;	//function or variable
	union Values value;
	int data_type;	//(for function it means return value) one of enum data_types
	TFunc_args *args;
	bool definition;
	bool is_declared_now;
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
 * @return 0 on success, INTERNAL_ERROR on malloc or realloc error, NOT_FOUND if function is not declared, ARG_FOUND if argument of given string is already declared
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

/** Gives data type of variable or data type of function return value
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @return returns int value, one of enum data_types
 */
int get_data_type(Resources *resources, index_t index_to_root_node, index_t index_to_string, int type);

/** Gives value of float variable
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param value pointer to float variable to store variable data
 * @param type defferentiates between variable data type and function return value data type (one of enum types_of_tokens)
 * @return returns RETURN_OK on succes, NOT_FOUND if variable is not in symbol_table or INTERNAL_ERROR on error
 */
int load_float_value(Resources *resources, index_t index_to_root_node, index_t index_to_string, float *value);

/** Gives value of integer variable
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param value pointer to integer variable to store variable data
 * @return returns RETURN_OK on succes, NOT_FOUND if variable is not in symbol_table or INTERNAL_ERROR on error
 */
int load_integer_value(Resources *resources, index_t index_to_root_node, index_t index_to_string, int *value);

/** Gives value of string variable
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param value pointer to index_t variable to store variable data
 * @return returns RETURN_OK on succes, NOT_FOUND if variable is not in symbol_table or INTERNAL_ERROR on error
 */
int load_string_value(Resources *resources, index_t index_to_root_node, index_t index_to_string, index_t *value);

/** Gives number of arguments of a function
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param int pointer to variable to store number of function's arguments
 * @return returns RETURN_OK on succes, NOT_FOUND if function is not in symbol_table or INTERNAL_ERROR on error
 */
int load_num_of_args(Resources *resources, index_t index_to_root_node, index_t index_to_string, int *num);

/** Gives number of arguments of a function
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param arg_index integer index to array of function's arguments 
 * @return returns data type of x-th argument (x is value in arg_index), NOT_FOUND if function or argument is not in symbol table or INTERNAL_ERROR on error
 */
int load_arg_data_type(Resources *resources, index_t index_to_root_node, index_t index_to_string, int arg_index);

/** Loads index to identifier and data type of function x-th argument (x is arg_index)
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_func_id index to dynamic buffer (string of identifier)
 * @param arg_index choose number of argument (indexed from 1)
 * @param index_to_arg_id variable to store index to argument's id
 * @param data_type variable to store argument's data type
 * @return returns true if function was defined, false if was not, NOT_FOUND if was not declared or INTERNAL_ERROR on error
 */
int load_arg(Resources *resources, index_t index_to_root_node, index_t index_to_func_id, int arg_index, index_t *index_to_arg_id, int *data_type);

/** Checks if function was definied
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_func_id index to dynamic buffer (string of identifier)
 * @return returns true if function was defined, false if was not, NOT_FOUND if was not declared or INTERNAL_ERROR on error
 */
int check_definition_flag(Resources *resources, index_t index_to_root_node, index_t index_to_func_id);

/** Sets definition flag up
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_func_id index to dynamic buffer (string of identifier)
 * @return returns RETURN_OK on succes, NOT_FOUND if function was not declared or INTERNAL_ERROR on error
 */
int set_definition_flag(Resources *resources, index_t index_to_root_node, index_t index_to_func_id);

/** Checks if function is currently being declared
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_func_id index to dynamic buffer (string of identifier)
 * @return returns true if function is being delared, false if not, INTERNALL_ERROR on error
 */
int check_declaration_status(Resources *resources, index_t index_to_root_node, index_t index_to_func_id);

/** Control variable data type
 * @param resources pointer to structure with buffers
 * @param index_to_root_node index_t variable storing index to dynamic structure buffer
 * @param index_to_string index to dynamic buffer (string of identifier)
 * @param exected_data_type data type to compare with
 * @return returns RETURN_OK on succes, SEMANTIC_ERROR if wrong data types, INTERNAL_ERROR on error, NOT_FOUND if variable was not declared
 */
int check_var_data_types(Resources *resources, index_t index_to_root_node, index_t index_to_string, int expected_data_type);


#endif

