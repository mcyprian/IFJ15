/**
 * @file debug.h
 * @author Michal Cyprian <xcypri01.stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Macro to print debug messages
 */

#ifdef DEBUG
#define DEBUG_ON 1
#else
#define DEBUG_ON 0
#endif

/** Prints debug message to stderr
 * @param fmt formatted string
 * @param ... variable args
 */
#define debug_print(fmt, ...)                                                  \
    do {                                                                       \
        if (DEBUG_ON)                                                          \
            fprintf(stderr, "[DEBUG] (%s:%d:%s) " fmt, __FILE__, __LINE__,     \
                    __func__, __VA_ARGS__);                                    \
    } while(0)

/** Checks if condition is met.
 * @param condition condition to be checked
 * @return ret_val on assertion failed
 */
#define args_assert(condition, ret_val)                                        \
    do {                                                                       \
        if (!(condition)) {                                                    \
        fprintf(stderr, "[ASSERTION FAILED] (%s:%s:%d) Unexpected values"      \
        " of function arguments.\n",__FILE__, __func__, __LINE__);             \
        return ret_val;                                                        \
        }                                                                      \
    } while(0)

