/**
 * @file syntax_analysis.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module syntax_analysis.c
 */

#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H

#include <datatypes.h>


typedef enum non_terminals {
	PROGRAM,
	PROGRAM_N,
	BLOCK_STATMENT,
	FUNC,
	STATIC_PART,
	TAIL_FUNC,
	DEC_VAR,
	ASSIGNMENT,
	FUNC_CALL,
	ARGS,
	ARGS_N,
	PARAMS,
	PARAMS_N,
	TYPE,
	STATMENT,
	TAIL_STATMENT,
	CONSTRUCTION,
	IF_ELSE,
	TAIL_IF,
	ELSE,
	FOR_CYCLE,
	FOR_FIRST,
	FOR_SECOND,
	FOR_THIRD,
	WHILE,
	DO,
	RETURN,
	TAIL_CONSTR,
	COUT,
	COUT_PARAMS,
	COUT_PARAMS_N,
	CIN,
	CIN_PARAMS,
	CIN_PARAMS_N
} NonTerminals;



int check_syntax(NonTerminals term, Resources * resources);

#endif // !SYNTAX_ANALYSIS_H
