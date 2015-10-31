/**
 * @file precedence_analysis.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module precedense_analysis.c
 */

#ifndef PRECEDENCE_ANALYSIS_H
#define PRECEDENCE_ANALYSIS_H

enum operations {
    S,    // SHIFT
    R,    // REDUCE
    H,    // ??
    E,    // ERROR
};

#define NUM_OF_TOKENS 14

extern const int precedence_table[NUM_OF_TOKENS][NUM_OF_TOKENS];

#endif // !PRECEDENCE_ANALYSIS_H
