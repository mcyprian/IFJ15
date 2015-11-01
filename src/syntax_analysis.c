/**
 * @file syntax_analysis.c
 * @author Radovan Sroka <xsroka00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Module syntax_analysis.c
 */

#include <syntax_analysis.h>
#include <token.h>
#include <dynamic_structure_buffer.h>
#include <dynamic_buffer.h>
#include <scanner.h>
#include <debug.h>
#include <error_macros.h>

#include <datatypes.h>
#include <resources.h>

#define isUsed(x) x ? 1 : 0
#define use(x) x = NULL;

int check_syntax(NonTerminals term, Resources * resources){
	debug_print("%s, term=%d\n", "INIT", term);
	int iRet = RETURN_OK;

	static TToken * token = NULL;
	static index_t token_index;

	if (term == PROGRAM){
		token_index = ZERO_INDEX;
		token = NULL;
	}

	if (isUsed(token)){
		token_index = get_token(resources->source, &(resources->string_buff), &(resources->struct_buff));
		dereference_structure(&(resources->struct_buff), token_index, (void **)token);
	}

	switch(term){
		case PROGRAM:
			if ((iRet = check_syntax(FUNC, resources)) != 0)goto DEFAULT;
			if ((iRet = check_syntax(PROGRAM_N, resources)) != 0)goto DEFAULT;
			break;
		case PROGRAM_N:
			if ((iRet = check_syntax(FUNC, resources)) != 0)goto DEFAULT;
			break;
		case BLOCK_STATMENT:
			break;
		case FUNC:
			break;
		case STATIC_PART:
			break;
		case TAIL_FUNC:
			break;
		case DEC_VAR:
			break;
		case ASSIGNMENT:
			break;
		case FUNC_CALL:
			break;
		case ARGS:
			break;
		case ARGS_N:
			break;
		case PARAMS:
			break;
		case PARAMS_N:
			break;
		case TYPE:
			break;
		case STATMENT:
			break;
		case TAIL_STATMENT:
			break;
		case CONSTRUCTION:
			break;
		case IF_ELSE:
			break;
		case TAIL_IF:
			break;
		case ELSE:
			break;
		case FOR_CYCLE:
			break;
		case FOR_FIRST:
			break;
		case FOR_SECOND:
			break;
		case FOR_THIRD:
			break;
		case WHILE:
			break;
		case DO:
			break;
		case RETURN:
			break;
		case TAIL_CONSTR:
			break;
		case COUT:
			break;
		case COUT_PARAMS:
			break;
		case COUT_PARAMS_N:
			break;
		case CIN:
			break;
		case CIN_PARAMS:
			break;
		case CIN_PARAMS_N:
			break;

	}


DEFAULT:
	debug_print("Term=%d, %s: %d\n", term, "RETURN", iRet);
	return iRet;
}