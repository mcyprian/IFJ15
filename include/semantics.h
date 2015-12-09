/**
 * @file sematics.h
 * @author Tomáš Ščavnický <xscavn00@stud.fit.vutbr.cz> Tomáš Sýkora <xsykor25@stud.fit.vutbr.cz> Michal Ďurista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file for sematics.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <dynamic_buffer.h>
#include <dynamic_structure_buffer.h>
#include <stack.h>
#include <symbol_table.h>
#include <instructions.h>

#define sem_type_filter(type)      \
    ( (type) == 14 || (type) == 21 ? 14 : ((type) == 15 || (type) == 20 ? 15 :((type) == 16 || (type) == 22 ? 16 :((type) == 32 ? 32 : 0 ))))

#define TYPE_CAST_FIRST 11
#define TYPE_CAST_SECOND 22
#define TYPE_CAST 11

/** Enters a block of code
 * @param resources pointer to structure with buffers
 * @return RETURN_OK
 */
int enter_scope(Resources *resources);

/** Leaves a block of code
 * @param resources pointer to structure with buffers
 * @return RETURN_OK
 */
int leave_scope(Resources *resources);

/** Executed in the end of analysis
 * @param resources pointer to structure with buffers
 * @return RETURN_OK
 */
int leave_general_scope(Resources *resources);

/** Checks if function is declared
 * @param resources pointer to structure with buffers
 * @param name_of_func index to buffer where name of the function is
 * @return RETURN_OK on succes, else NOT_FOUND
 */
int is_func_declared(Resources *resources, index_t name_of_func);

/** Checks if the function is declared, if yes, wheter it is declared with the return_type
 * @param resources pointer to structure with buffers
 * @param name_of_func index to buffer where name of the function is
 * @param return_type data type of returned value of the function
 * @return NOT_FOUND if function is not declared, SEMANTIC_ERROR if it is declared with different return_type, RETURN_OK on success
 */
int is_func_declared_withrv(Resources *resources, index_t name_of_func, int return_type);

/** Declares a function in a table
 * @param resources pointer to structure with buffers
 * @param index_to_string_buff index to string buffer where name of the function is
 * @param return_type data type of returned value of the function
 * @return RETURN_OK on success, else SEMANTIC_ERROR on error, INTERNAL_ERROR on internal error
 */
int declare_func(Resources *resources, index_t index_to_string_buff, int return_type);

/** Declared built in funcitons sort, find, concat, substr, length
 * @param resources pointer to structure with buffers
 * @return RETURN_OK on success, else SEMANTIC_ERROR or TYPE_ERROR on error, INTERNAL_ERROR on internal error
 */
int declare_builtin_funcs(Resources *resources);

/** Declares variable in a table
 * @param resources pointer to structure with buffers
 * @param index_to_string_buff index to string buffer where name of the variable is
 * @param data_type data type of the variable
 * @return RETURN_OK on success, else SEMANTIC_ERROR on error, INTERNAL_ERROR on internal error
 */
int declare_var(Resources *resources, index_t index_to_string_buff, int data_type);

/** Adds argument to a function
 * @param resources pointer to structure with buffers
 * @param name_of_arg index to string buffer where the name of the argument is
 * @param data_type data type of the argument
 * @return RETURN_OK
 */
int add_arg(Resources *resources, index_t name_of_arg, int data_type);

/** Checks if the argument is declared
 * @param resources pointer to structure with buffers
 * @param expected_name_of_arg index to buffer where name of the argument is
 * @param expected_arg_type expected data type of the argument
 * @param argi index of the argument in a function
 * @return RETURN_OK on success, else TYPE_ERROR
 */
int check_arg_declaration(Resources *resources, index_t expected_name_of_arg, int expected_arg_type, int argi);

/** Checks if the argument is declared, if not, sets the argument with data_type
 * @param name_of_arg index to buffer where name of the argument is
 * @param data_type data type of the argument
 * @return RETURN_OK on success, else TYPE_ERROR 
 */
int set_arg(Resources *resources, index_t name_of_arg, int data_type);

/** Checks the count of arguments
 * @param resources pointer to structure with buffers
 * @return RETURN_OK on success, else TYPE_ERROR 
 */
int check_argc(Resources *resources);

/** Checks if variable is declared
 * @param resources pointer to structure with buffers
 * @param name_of_var index to buffer where name of the variable is
 * @return RETURN_OK on success, SEMANTIC_ERROR on error
 */
int is_var_declared(Resources *resources, index_t name_of_var);

/** Checks the return data type of a function
 * @param resources pointer to structure with buffers
 * @param func_name index to buffer where name of the function is
 * @param expected_data_type expected data type
 * @return RETURN_OK on success, TYPE_ERROR on error
 */
int check_return_type(Resources *resources, index_t func_name, int expected_data_type);

/** Checks the data type of a variable
 * @param resources pointer to structure with buffers
 * @param var_name index to buffer where name of the variable is
 * @param expected_type expected data type
 * @return RETURN_OK on success, L_DOUBLE or L_INT if casting is needed, TYPE_ERROR on error
 */
int check_var_type(Resources *resources, index_t var_name, int expected_type);

/** Defines a new function
 * @param resources pointer to structure with buffers
 * @return RETURN_OK on success, SEMANTIC_ERROR if it was already defined
 */
int define_func(Resources *resources);

/** Compares two token data types
 * @param resources pointer to structure with buffers
 * @param frst_token index to structure buffer with tokens
 * @param scnd_token index to structure buffer with tokens
 * @return RETURN_OK if same data types, TYPE_CAST_FIRST if first token must be changed to double, TYPE_CAST_SECOND if second, SEMANTIC_ERROR on semantic error
 */
int check_tokens(Resources *resources, index_t frst_token, index_t scnd_token);

/** Checks tha data type of argument
 * @param resources pointer to structure with buffers
 * @param type expected data type
 * @return RETURN_OK on success, TYPE_ERROR on error, TYPE_CAST if casting is needed
 */
int check_arg_type(Resources *resources, int type);

/** Checks the count of arguments after calling the function
 * @param resources pointer to structure with buffers
 * @return RETURN_OK on success, TYPE_ERROR on error
 */
int check_argc_function_call(Resources *resources);

int get_return_type(Resources *resources, index_t func_name);

int get_var_type(Resources *resources, index_t var_name);

int save_func_index(Resources *resources, index_t index_to_store);

int save_var_index(Resources *resources, index_t var_name, index_t index_to_store);

int load_func_index(Resources *resources, index_t func_name, index_t *load_index);

int load_var_index(Resources *resources, index_t var_name, index_t *load_index);

int declared_var_cnt(Resources *resources, int *cnt);

int check_return_value_type(Resources *resources, int type);
