/**
 * @file error_macros.h
 * @author Michal Cyprian <xcypri01.stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Collection of error and warning macros to check functions return codes
 * and handle specific situations
 */

#ifndef ERROR_MACROS_H
#define ERROR_MACROS_H

#include <stdlib.h>
#include <stdio.h>

enum {LEXICAL_ERROR = 1, SYNTAX_ERROR, SEMANTIC_ERROR, TYPE_ERROR, GET_TYPE_ERROR,
      OTHER_SEM_ERROR, INPUT_ERROR, UNINIT_ERROR, DIV_ZERO_ERROR, RUNTIME_ERROR,
      INTERNAL_ERROR = 99};

/** Prints warning message to stderr
 * @param var vatiable to be checked
 * @param value value indicating unexpected situation
 * @param msg warning message
 */
#define catch_warning(var, value, msg)                               \
    do {                                                             \
        if ((var) == (value)) {                                      \
                fprintf(stderr, "[WARNING] (%s:%s:%d) " msg "\n",    \
                        __FILE__, __func__, __LINE__);               \
        }                                                            \
    } while (0)

/** Lexical analysis error, wrong structure of lexem.
 * @param var variable to be checked
 * @param value value indicating error
 * @param msg error message
 * @param v set macro to be verbal
 * @return one if error occures
 */
#define catch_lexical_error(var, value, msg, v)                        \
    do {                                                               \
        if ((var) == (value)) {                                        \
            if(v)                                                      \
                fprintf(stderr, "[LEXICAL ERROR] (%s:%s:%d) " msg "\n",\
                        __FILE__, __func__, __LINE__);                 \
                return LEXICAL_ERROR;                                  \
        }                                                              \
    } while (0)

/** Syntax analysis error.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 * @return two if error occures
 */
#define catch_syntax_error(var, value, msg, v)                         \
    do {                                                               \
        if ((var) == (value)) {                                        \
            if(v)                                                      \
                fprintf(stderr, "[SYNTAX ERROR] (%s:%s:%d) " msg "\n", \
                        __FILE__, __func__, __LINE__);                 \
                return SYNTAX_ERROR;                                   \
        }                                                              \
    } while (0)

/** Undefined function/variable error.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 * @return three if error occures
 */
#define catch_undefined_error(var, value, msg, v)                        \
    do {                                                                 \
        if ((var) == (value)) {                                          \
            if(v)                                                        \
                fprintf(stderr, "[SEMANTIC ERROR] (%s:%s:%d) " msg "\n", \
                        __FILE__, __func__, __LINE__);                   \
                return SEMANTIC_ERROR`;                                  \
        }                                                                \
    } while (0)


/** Type compatibility error.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 * @return four if error occures
 */
#define catch_compatibility_error(var, value, msg, v)                    \
    do {                                                                 \
        if ((var) == (value)) {                                          \
            if(v)                                                        \
                fprintf(stderr, "[SEMANTIC ERROR] (%s:%s:%d) " msg "\n", \
                        __FILE__, __func__, __LINE__);                   \
                return TYPE_ERROR;                                       \
        }                                                                \
    } while (0)

/** Data type selection error.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 * @return five if error occures
 */
#define catch_type_error(var, value, msg, v)                             \
    do {                                                                 \
        if ((var) == (value)) {                                          \
            if(v)                                                        \
                fprintf(stderr, "[TYPE ERROR] (%s:%s:%d) " msg "\n",     \
                        __FILE__, __func__, __LINE__);                   \
                return GET_TYPE_ERROR;                                   \
        }                                                                \
    } while (0)

/** Other semantic errors.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 * @return six if error occures
 */
#define catch_semantic_error(var, value, msg, v)                         \
    do {                                                                 \
        if ((var) == (value)) {                                          \
            if(v)                                                        \
                fprintf(stderr, "[SEMANTIC ERROR] (%s:%s:%d) " msg "\n", \
                        __FILE__, __func__, __LINE__);                   \
                return OTHER_SEM_ERROR;                                  \
        }                                                                \
    } while (0)

/** Input reading errors.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 *
 * TODO free memory before exit
 */
#define catch_input_error(var, value, msg)                              \
    do {                                                                \
        if ((var) == (value)) {                                         \
                fprintf(stderr, "[RUNTIME ERROR] (%s:%s:%d) " msg "\n", \
                        __FILE__, __func__, __LINE__);                  \
                exit (INPUT_ERROR);                                     \
        }                                                               \
    } while (0)

/** Working with uninitialized variable.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 *
 * TODO free memory before exit
 */
#define catch_uninicialized_error(var, value, msg)                      \
    do {                                                                \
        if ((var) == (value)) {                                         \
                fprintf(stderr, "[UNINIT ERROR] (%s:%s:%d) " msg "\n",  \
                        __FILE__, __func__, __LINE__);                  \
                exit(UNINIT_ERROR);                                     \
        }                                                               \
    } while (0)

/** Devision by zero.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 *
 * TODO free memory before exit
 */
#define catch_div_zero_error(var, value, msg)                           \
    do {                                                                \
        if ((var) == (value)) {                                         \
                fprintf(stderr, "[DIV ERROR] (%s:%s:%d) " msg "\n",     \
                        __FILE__, __func__, __LINE__);                  \
                exit(DIV_ZERO_ERROR);                                   \
        }                                                               \
    } while (0)

/** Other runtime errors.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 * @return ten if error occures
 */
#define catch_runtime_error(var, value, msg, v)                         \
    do {                                                                \
        if ((var) == (value)) {                                         \
            if(v)                                                       \
                fprintf(stderr, "[RUNTIME ERROR] (%s:%s:%d) " msg "\n"  \
                        __FILE__, __func__, __LINE__);                  \
                return RUNTIME_ERROR;                                   \
        }                                                               \
    } while (0)

/** Internal error in interpret.
 * @param var variable to be checked
 * @param value value indicating  error
 * @param msg error message
 * @param v set macro to be verbal
 *
 * TODO free memory before exit
 */
#define catch_internal_error(var, value, msg)                           \
    do {                                                                \
        if ((var) == (value)) {                                         \
            fprintf(stderr, "[INTERNAL ERROR] (%s:%s:%d) " msg "\n",    \
                    __FILE__, __func__, __LINE__);                      \
            exit(INTERNAL_ERROR);                                       \
        }                                                               \
    } while (0)

/** Checks if condition is met.
 * @param condition condition to be checked
 * @return ret_val on assertion failed
 */
#define args_assert(condition, ret_val)                                        \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, "[ASSERTION FAILED] (%s:%s:%d) Unexpected values"  \
                    " of function arguments.\n",__FILE__, __func__, __LINE__); \
             return ret_val;                                                   \
        }                                                                      \
    } while(0)
            
#endif // !ERROR_MACROS_H
