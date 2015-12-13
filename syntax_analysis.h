/**
 * project: Implementace interpretu imperativního jazyka IFJ15
 * @file syntax_analysis.h
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module syntax_analysis.c
 */

#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H

#include "datatypes.h"


typedef enum non_terminals {
	GLOBAL = 100,
	PROGRAM ,
	PROGRAM_N,
	BLOCK_STATMENT,
	FUNCTION,
	STATIC_PART,
	TAIL_FUNC,
	DEC_VAR,
	TAIL_VAR,
	ASSIGNMENT,
	FUNC_CALL,
	ARGS,
	ARGS_N,
	PARAMS,
	PARAMS_N,
	TYPE,
	STATEMENT,
	TAIL_STATEMENT,
	CONSTRUCTION,
	IF_ELSE,
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
	CIN_PARAMS_N,
	TEST_EXPR
} NonTerminals;



int check_syntax(int term, Resources * resources);

#endif // !SYNTAX_ANALYSIS_H
