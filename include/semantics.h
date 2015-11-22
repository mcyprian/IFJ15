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

/** Enters a block of code
 * @param resources pointer to structure with buffers
 * @return 
 */
int enter_scope(Resources *resources);

/** Leaves a block of code
 * @param resources pointer to structure with buffers
 * @return 
 */
int leave_scope(Resources *resources);

/** Executed in the end of analysis
 * @param resources pointer to structure with buffers
 * @return 
 */
int leave_general_scope(Resources *resources);

/** Declares a function in a table
 * @param resources pointer to structure with buffers
 * @param index_to_string_buff index to string buffer where name of the function is
 * @param return_type data type of returned value of the function
 * @return 
 */
int declare_func(Resources *resources, index_t index_to_string_buff, int return_type);

/** Declares variable in a table
 * @param resources pointer to structure with buffers
 * @param index_to_string_buff index to string buffer where name of the variable is
 * @param data_type data type of the variable
 * @return 
 */
int declare_var(Resources *resources, index_t index_to_string_buff, int data_type);

/** Adds argument to a function
 * @param resources pointer to structure with buffers
 * @param name_of_func index to buffer where name of the function is
 * @param name_of_arg index to buffer where name of the argument is
 * @param data_type data type of the argument
 * @return 
 */
int add_arg(Resources *resources, index_t name_of_arg, int data_type);

int check_arg_declaration(Resources *resources, index_t expected_name_of_arg, int expected_arg_type, int argi);

int check_argc(Resources *resources, int expected_argc);

/** Checks if function is declared
 * @param resources pointer to structure with buffers
 * @param name_of_func index to buffer where name of the function is
 * @return 
 */
int is_func_declared(Resources *resources, index_t name_of_func, int return_type);

/** Checks if variable is declared
 * @param resources pointer to structure with buffers
 * @param name_of_var index to buffer where name of the variable is
 * @return 
 */
int is_var_declared(Resources *resources, index_t name_of_var);

/** Checks the return data type of a function
 * @param resources pointer to structure with buffers
 * @param func_name index to buffer where name of the function is
 * @param expected_data_type expected data type
 * @return 
 */
int check_return_type(Resources *resources, index_t func_name, int expected_data_type);

/** Checks the data type of a variable
 * @param resources pointer to structure with buffers
 * @param var_name index to buffer where name of the variable is
 * @param expected_type expected data type
 * @return 
 */
int check_var_type(Resources *resources, index_t var_name, int expected_type);

int define_func(Resources *resources);