/**
 * @file debug.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Macro to print debug messages
 */

#define DEBUG 1

#if DEBUG
/** Prints debug message to stderr
 * @param fmt formatted string
 * @param ... variable args
 */
#define debug_print(fmt, ...)                                                  \
    do {                                                                       \
	fprintf(stderr, "[DEBUG_PRINT] (%s:%d:%s) " fmt, __FILE__, __LINE__,         \
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

#else 

#define debug_printf(fmt, ...) do {}while(0)
#define args_assert(condition, ret_val) do {}while(0)

#endif // !DEBUG

