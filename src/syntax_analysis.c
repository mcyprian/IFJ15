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
#include <precedence_analysis.h>
#include <semantics.h>

#include <datatypes.h>
#include <resources.h>


#define get_if_null(resources, index, token)										\
	do {														\
	       if (token == NULL){											\
			index = get_token(resources->source, &(resources->string_buff), &(resources->struct_buff));	\
			dereference_structure(&(resources->struct_buff), index, (void **)&token);			\
			debug_print("type=%d\n", token->token_type);							\
		}													\
	}while(0)													\
								   

int check_syntax(int term, Resources * resources){
	debug_print("%s, term=%d\n", "INIT", term);
	int iRet = RETURN_OK;
	
	static TToken * token = NULL;
	static index_t token_index = ZERO_INDEX;

	int type = 0;
	index_t id = 0;

	get_if_null(resources, token_index, token);
	
	if (token->token_type == ERRORT){
		iRet = LEXICAL_ERROR;
		goto EXIT;
	}

	switch(term){
//**************** GLOBAL **********************//

		case GLOBAL:
			if ((iRet = enter_scope(resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(PROGRAM, resources)) != 0)goto EXIT;
			break;

//**************** PROGRAM **********************//
		case PROGRAM:
			if ((iRet = check_syntax(FUNCTION, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(PROGRAM_N, resources)) != 0)goto EXIT;
			break;


//**************** PROGRAM_N **********************//
		case PROGRAM_N:
			if (token->token_type == EOFT)
				goto EXIT;
			else if (token->token_type >= T_DOUBLE && token->token_type <= T_STRING){
				if ((iRet = check_syntax(PROGRAM, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;	
			break;


//**************** BLOCK_STATMENT **********************//
		case BLOCK_STATMENT:
			if (token->token_type == CLOSING_CURLY_BRACKET){
				if ((iRet = leave_scope(resources)) != 0)goto EXIT;
				goto EXIT;
			}

			if (token->token_type == OPENING_CURLY_BRACKET){
				if ((iRet = check_syntax(OPENING_CURLY_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = enter_scope(resources)) != 0)goto EXIT; 
				if ((iRet = check_syntax(BLOCK_STATMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(CLOSING_CURLY_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = leave_scope(resources)) != 0)goto EXIT;
			}
			else if ((token->token_type >= T_DOUBLE && token->token_type <= T_STRING) ||
				 (token->token_type == AUTO)){
				if ((iRet = check_syntax(DEC_VAR, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT; 
			}
			else if (token->token_type == IDENTIFIER){
				if ((iRet = check_syntax(STATEMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT; 
			}
			else if ((token->token_type >= K_CIN && token->token_type <= K_DO) || 
					(token->token_type >= K_FOR && token->token_type <= K_WHILE)){
				if ((iRet = check_syntax(CONSTRUCTION, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			if ((iRet = check_syntax(BLOCK_STATMENT, resources)) != 0)goto EXIT;
			break;


//**************** FUNCTION **********************//
		case FUNCTION:
			if ((iRet = check_syntax(STATIC_PART, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(TAIL_FUNC, resources)) != 0)goto EXIT;
			break;


//**************** STATIC_PART **********************//
		case STATIC_PART:
			type = token->token_type;
			if ((iRet = check_syntax(TYPE, resources)) != 0)goto EXIT;
			get_if_null(resources, token_index, token);
			id = token->token_index;
			if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
			if ((iRet = declare_func(resources, id, type)) != 0 )goto EXIT;
			if ((iRet = check_syntax(OPENING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(PARAMS, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(CLOSING_BRACKET, resources)) != 0)goto EXIT;
			break;

//**************** TAIL_FUNC **********************//
		case TAIL_FUNC:
			if (token->token_type == SEMICOLON){
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == OPENING_CURLY_BRACKET){
				if ((iRet = check_syntax(OPENING_CURLY_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(BLOCK_STATMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(CLOSING_CURLY_BRACKET, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** TAIL_FUNC **********************//
		case DEC_VAR:
			if (token->token_type >= T_DOUBLE && token->token_type <= T_STRING){
				type = token->token_type;
				if ((iRet = check_syntax(TYPE, resources)) != 0)goto EXIT;
				get_if_null(resources, token_index, token);
				id = token->token_index;
				if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
				if ((iRet = declare_var(resources, id, type)) != 0)goto EXIT;
				if ((iRet = check_syntax(TAIL_VAR, resources)) != 0)goto EXIT;
			}
			else if	(token->token_type == AUTO){
				if ((iRet = check_syntax(AUTO, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(ASSIGNMENT, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR; 
			break;		

//**************** TAIL_VAR **********************//
		case TAIL_VAR:
			if (token->token_type == O_ASSIGN){
				if ((iRet = check_syntax(O_ASSIGN, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			}
			else goto EXIT;
			break;

//**************** STATEMENT **********************//
		case STATEMENT:
			if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(TAIL_STATEMENT, resources)) != 0)goto EXIT;
			break;

//**************** TAIL_STATEMENT **********************//
		case TAIL_STATEMENT:
			if (token->token_type == O_ASSIGN){
				if ((iRet = check_syntax(ASSIGNMENT, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == OPENING_BRACKET){
				if ((iRet = check_syntax(FUNC_CALL, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** ASSIGNMENT ************************//
		case ASSIGNMENT:
			if ((iRet = check_syntax(O_ASSIGN, resources)) != 0)goto EXIT;
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			break;

//**************** FUNC_CALL **********************//
		case FUNC_CALL:
			if ((iRet = check_syntax(OPENING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(ARGS, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(CLOSING_BRACKET, resources)) != 0)goto EXIT;
			break;

//**************** ARGS **********************//
		case ARGS:
			if (token->token_type == CLOSING_BRACKET)goto EXIT;
			else {
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(ARGS_N, resources)) != 0)goto EXIT;
			}
			break;

//**************** ARGS_N **********************//
		case ARGS_N:
			if (token->token_type == CLOSING_BRACKET)goto EXIT;
			else {
				if ((iRet = check_syntax(COMMA, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(ARGS_N, resources)) != 0)goto EXIT;
			}
			break;

//**************** PARAMS **********************//
		case PARAMS:
			if (token->token_type >= T_DOUBLE && token->token_type <= T_STRING){
				if ((iRet = check_syntax(TYPE, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(PARAMS_N, resources)) != 0)goto EXIT;
			}
			else goto EXIT;
			break;

//**************** PARAMS_N **********************//
		case PARAMS_N:
			if (token->token_type == CLOSING_BRACKET)goto EXIT;
			else {
				if ((iRet = check_syntax(COMMA, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(TYPE, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(PARAMS_N, resources)) != 0)goto EXIT;
			}
			break;

//**************** TYPE **********************//
		case TYPE:
			if (!(token->token_type >= T_DOUBLE && token->token_type <= T_STRING)){
				iRet = SYNTAX_ERROR;
				goto EXIT;
			} else {
				token = NULL;
				goto EXIT;
			}
			break;

//**************** CONSTRUCTION **********************//
		case CONSTRUCTION:
			if (token->token_type == K_IF){
				if ((iRet = check_syntax(IF_ELSE, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == K_FOR){
				if ((iRet = check_syntax(FOR_CYCLE, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == K_CIN){
				if ((iRet = check_syntax(CIN, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == K_COUT){
				if ((iRet = check_syntax(COUT, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == K_RETURN){
				if ((iRet = check_syntax(RETURN, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == K_WHILE){
				if ((iRet = check_syntax(WHILE, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == K_DO){
				if ((iRet = check_syntax(DO, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** IF_ELSE **********************//
		case IF_ELSE:
			if (token->token_type == K_IF){
				if ((iRet = check_syntax(K_IF, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(OPENING_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(CLOSING_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(TAIL_IF, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(ELSE, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** TAIL_IF **********************//
		case TAIL_IF:
			if ((token->token_type >= T_DOUBLE && token->token_type <= T_STRING) ||
				 (token->token_type == AUTO)){
				if ((iRet = check_syntax(DEC_VAR, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			}
			else if ((token->token_type >= K_CIN && token->token_type <= K_DO) || 
						(token->token_type >= K_FOR && token->token_type <= K_WHILE)){
				if ((iRet = check_syntax(CONSTRUCTION, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == IDENTIFIER){
				if ((iRet = check_syntax(ASSIGNMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == OPENING_CURLY_BRACKET){
				if ((iRet = check_syntax(OPENING_CURLY_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(BLOCK_STATMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(CLOSING_CURLY_BRACKET, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** ELSE **********************//
		case ELSE:
			if (token->token_type == K_ELSE){
				if ((iRet = check_syntax(K_ELSE, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(TAIL_CONSTR, resources)) != 0)goto EXIT;
			}
			break;

//**************** FOR_CYCLE **********************//
		case FOR_CYCLE:
			if ((iRet = check_syntax(K_FOR, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(OPENING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(FOR_FIRST, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(FOR_SECOND, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(FOR_THIRD, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(CLOSING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(TAIL_CONSTR, resources)) != 0)goto EXIT;
			break;

//**************** FOR_FIRST **********************//
		case FOR_FIRST:
			if ((token->token_type >= T_DOUBLE && token->token_type <= T_STRING) |
				(token->token_type == AUTO)){
				if ((iRet = check_syntax(DEC_VAR, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == IDENTIFIER){
				if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(ASSIGNMENT, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** FOR_SECOND **********************//
		case FOR_SECOND:
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			break;

//**************** FOR_THIRD **********************//
		case FOR_THIRD:
			if ((iRet = check_syntax(IDENTIFIER, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(O_ASSIGN, resources)) != 0)goto EXIT;
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			break;

//**************** WHILE **********************//
		case WHILE:
			if ((iRet = check_syntax(K_WHILE, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(OPENING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			if ((iRet = check_syntax(CLOSING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(TAIL_CONSTR, resources)) != 0)goto EXIT;
			break;

//**************** DO **********************//
		case DO:
			if ((iRet = check_syntax(K_DO, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(OPENING_CURLY_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(BLOCK_STATMENT, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(CLOSING_CURLY_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(K_WHILE, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(OPENING_BRACKET, resources)) != 0)goto EXIT;			
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			if ((iRet = check_syntax(CLOSING_BRACKET, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			break;

//**************** RETURN **********************//
		case RETURN:
			if ((iRet = check_syntax(K_RETURN, resources)) != 0)goto EXIT;
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			break;

//**************** TAIL_CONSTR **********************//
		case TAIL_CONSTR:
			if ((token->token_type >= T_DOUBLE && token->token_type <= T_STRING) || 
				(token->token_type == AUTO)){
				if ((iRet = check_syntax(DEC_VAR, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			}
			else if ((token->token_type >= K_CIN && token->token_type <= K_DO) || 
						(token->token_type >= K_FOR && token->token_type <= K_WHILE)){
				if ((iRet = check_syntax(CONSTRUCTION, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == OPENING_CURLY_BRACKET){
				if ((iRet = check_syntax(OPENING_CURLY_BRACKET, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(BLOCK_STATMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(CLOSING_CURLY_BRACKET, resources)) != 0)goto EXIT;
			}
			else if (token->token_type == IDENTIFIER){
				if ((iRet = check_syntax(ASSIGNMENT, resources)) != 0)goto EXIT;
				if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;


//**************** COUT **********************//
		case COUT:
			if ((iRet = check_syntax(K_COUT, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(COUT_PARAMS, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			break;

//**************** COUT_PARAMS **********************//
		case COUT_PARAMS:
			if (token->token_type == O_LEFT_ARROW){
				if ((iRet = check_syntax(O_LEFT_ARROW, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(COUT_PARAMS_N, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** COUT_PARAMS_N **********************//
		case COUT_PARAMS_N:
			if (token->token_type == O_LEFT_ARROW){
				if ((iRet = check_syntax(O_LEFT_ARROW, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(COUT_PARAMS_N, resources)) != 0)goto EXIT;
			}
			break;

//**************** CIN **********************//
		case CIN:
			if ((iRet = check_syntax(K_CIN, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(CIN_PARAMS, resources)) != 0)goto EXIT;
			if ((iRet = check_syntax(SEMICOLON, resources)) != 0)goto EXIT;
			break;

//**************** CIN_PARAMS **********************//
		case CIN_PARAMS:
			if (token->token_type == O_RIGHT_ARROW){
				if ((iRet = check_syntax(O_RIGHT_ARROW, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(CIN_PARAMS_N, resources)) != 0)goto EXIT;
			}
			else goto SYN_ERR;
			break;

//**************** CIN_PARAMS_N **********************//
		case CIN_PARAMS_N:
			if (token->token_type == O_RIGHT_ARROW){
				if ((iRet = check_syntax(O_RIGHT_ARROW, resources)) != 0)goto EXIT;
				if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
				if ((iRet = check_syntax(CIN_PARAMS_N, resources)) != 0)goto EXIT;
			}
			break;
		
//******************* TERMINALS ************************//

		case IDENTIFIER:
			if (token->token_type != IDENTIFIER){
					iRet = SYNTAX_ERROR;
					goto EXIT;
				} else {
					token = NULL;
					goto EXIT;
				}
			break;

//******************* TEST_EXPR *****************************************//

		case TEST_EXPR:
			if ((iRet = check_expression(resources, &token, &token_index)) != 0)goto EXIT;
			break;

		default:
			if (token->token_type != term){
					iRet = SYNTAX_ERROR;
					goto EXIT;
				} else {
					token = NULL;
					goto EXIT;
				}
			break;
	}

	goto EXIT;

SYN_ERR:
	iRet = SYNTAX_ERROR;

EXIT:
	debug_print("Term=%d, %s: %d\n", term, "RETURN", iRet);
	return iRet;
}
