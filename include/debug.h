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
            fprintf(stderr, "[DEBUG] (%s:%d:%s) " fmt, __FILE__, __LINE__,      \
                    __func__, __VA_ARGS__);                                    \
    } while(0);
