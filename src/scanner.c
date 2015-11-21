/**
 * @file scanner.c
 * @author Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz> Michal Durista <xduris04@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Scanner used for tokenizing IFJ15 language.
 */

#include <scanner.h>
#include <token.h>

#define L_STRING_BACKSLASH 123
#define SEPARATOR c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == '=' || c == '!' || c == ';' || c == ',' || c == '(' || c == ')' || isspace(c)


int reservedWord(char *identifier)
{
	switch(identifier[0])
	{
		case 'a':
			if (strcmp(identifier, "auto") == 0)
			{	
				return AUTO;
			}

		case 'c':
			if (strcmp(identifier, "cin") == 0)
			{	
				return K_CIN;
			}
			else if (strcmp(identifier, "cout") == 0)
			{
				return K_COUT;
			}
				
		case 'd':
			if (strcmp(identifier, "double") == 0)
			{	
				return T_DOUBLE;
			} 
			else if (strcmp(identifier, "do") == 0)
			{
				return K_DO;
			}

		case 'e':
			if (strcmp(identifier, "else") == 0)
			{	
				return K_ELSE;
			}

		case 'f':
			if (strcmp(identifier, "for") == 0)
			{	
				return K_FOR;
			}

		case 'i':
			if (strcmp(identifier, "if") == 0)
			{	
				return K_IF;
			}
			else if (strcmp(identifier, "int") == 0)
			{	
				return T_INT;
			}
				
		case 'r':
			if (strcmp(identifier, "return") == 0)
			{	
				return K_RETURN;
			}

		case 's':
			if (strcmp(identifier, "string") == 0)
			{	
				return T_STRING;
			}

		case 'w':
			if (strcmp(identifier, "while") == 0) 
			{
				return K_WHILE;
			}
	} // switch

	return IDENTIFIER;
} // reservedWord


index_t get_token(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer) {
	args_assert(buffer != NULL && struct_buffer != NULL, INTERNAL_ERROR);
	debug_print("%s\n", "GET_TOKEN");


	int c;
	int previous = 0;
	int state = START;
	bool read = true;
	bool signed_scientific = false;
	bool wrong_identifier = false;
	bool is_number_after_sign = false;
	bool is_number_after_dot = false;
	index_t index = 0;
	TToken *token;

	new_item(struct_buffer, index, token);

	while (1) {
		if (read)
		{	
			c = fgetc(fin);

			if (feof(fin))
			{
				//new_item(struct_buffer, index, token);
				token->token_index = save_token(buffer);
				token->token_type = EOFT;
				// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);

				return index;
			}
		}
		else
		{
			c = previous;
			read = true;
		}

		switch (state) 
		{
			case START:
				//new_item(struct_buffer, index, token); // pridat overenie, ci to vyslo

				if ((c > 64 && c < 91) || (c > 96 && c < 123) || c == '_') // A-Z || a-z || _
				{
					state = IDENTIFIER;
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					
				}
				else if (c > 47 && c < 58) // 0-9
				{
					state = L_INT;
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
				}
				else  
				{
					switch (c)
					{
						case '+':
							token->token_index = save_token(buffer);
							token->token_type = O_PLUS;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case '-':
							token->token_index = save_token(buffer);
							token->token_type = O_MINUS;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case '/':
							state = COMMENT;
							break;

						case '*':
							token->token_index = save_token(buffer);
							token->token_type = O_MUL;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case ';':
							token->token_index = save_token(buffer);
							token->token_type = SEMICOLON;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case ',':
							token->token_index = save_token(buffer);
							token->token_type = COMMA;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case '(':
							token->token_index = save_token(buffer);
							token->token_type = OPENING_BRACKET;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case ')':
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_BRACKET;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case '{':
							token->token_index = save_token(buffer);
							token->token_type = OPENING_CURLY_BRACKET;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case '}':
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_CURLY_BRACKET;
							// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;

						case '=':
							state = O_EQUALS;
							break;

						case '<':
							state = O_L;
							break;

						case '>':
							state = O_G;
							break;

						case '!':
							state = UO_EXCLAMATION;
							break;
						
						case '"':
							state = L_STRING;
							break;

						default:
							if (!isspace(c))
							{
								catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
								token->token_index = save_token(buffer);
								token->token_type = ERRORT;
								// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
								return index;
							}

							break;
					} // switch(c)
				} // else
				break;

			case COMMENT:
				if (c == '/')  // //
				{
					state = LINE_COMMENT;
				}
				else if (c == '*')  // /*
				{
					state = BLOCK_COMMENT;
				}
				else // /
				{
					ungetc(c, fin);
					state = START;
					token->token_index = save_token(buffer);
					token->token_type = O_DIV;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					return index;
				}

				break;

			case LINE_COMMENT:
				if (c == '\n')
					state = START;

				break;

			case BLOCK_COMMENT:
				if (c == '*')
					state = BLOCK_COMMENT_END;

				break;

			case BLOCK_COMMENT_END:
				if (c == '/')
					state = START;
				else
					state = BLOCK_COMMENT;

				break;

			case O_EQUALS:
				if (c == '=') // ==
				{
					token->token_index = save_token(buffer);
					token->token_type = O_EQUALS;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else // =
				{
					ungetc(c, fin);
					state = START;
					token->token_index = save_token(buffer);
					token->token_type = O_ASSIGN;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					return index;
					break;
				}

			case O_G:
				if (c == '=') // >=
				{
					token->token_index = save_token(buffer);
					token->token_type = O_GE;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else if (c == '>') // >>
				{
					token->token_index = save_token(buffer);
					token->token_type = O_RIGHT_ARROW;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else // >
				{
					ungetc(c, fin);
					token->token_index = save_token(buffer);
					token->token_type = O_G;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case O_L:
				if (c == '=') // <=
				{
					token->token_index = save_token(buffer);
					token->token_type = O_LE;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else if (c == '<') // <<
				{
					token->token_index = save_token(buffer);
					token->token_type = O_LEFT_ARROW;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else // <
				{
					ungetc(c, fin);
					token->token_index = save_token(buffer);
					token->token_type = O_L;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case UO_EXCLAMATION:
				if (c == '=') // !=
				{
					token->token_index = save_token(buffer);
					token->token_type = O_NE;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else // ! je zatial errort
				{
					ungetc(c, fin);
					token->token_index = save_token(buffer);
					token->token_type = ERRORT; // token->token_type = UO_EXCLAMATION;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case IDENTIFIER:

				if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58) || c == '_') { // (A-Z, a-z, 0-9, _)
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
				} 
				else if (SEPARATOR)
				{
					token->token_index = save_token(buffer);
					if (!wrong_identifier)
						token->token_type = reservedWord(load_token(buffer, token->token_index));
					else
						token->token_type = ERRORT;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					wrong_identifier = false;
					is_number_after_dot = false;
					is_number_after_sign = false;
					ungetc(c,fin);
					state = START;
					return index;
				}
				else
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					wrong_identifier = true;
				}

				break;

			case L_INT:
				if (c > 47 && c < 58) // 0-9
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
				}
				else if (c == 46) // c == "."
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					state = L_DOUBLE;
				}
				else if (SEPARATOR)
				{
					token->token_index = save_token(buffer);
					token->token_type = L_INT;
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					ungetc(c,fin);
					state = START;
					return index;
				}
				else if (c == 'e' || c == 'E')
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					state = SCIENTIFIC;
				}
				else
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					wrong_identifier = true;
					state = IDENTIFIER;
				}

				break;

			case L_DOUBLE:
				if (c > 47 && c < 58) // 0-9
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					is_number_after_dot = true;
				}
				else if ((SEPARATOR) && is_number_after_dot)
				{
					token->token_type = L_DOUBLE;
					token->token_index = save_token(buffer);
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					ungetc(c,fin);
					state = START;
					return index;
				}
				else if ((SEPARATOR) && !is_number_after_dot)
				{
					token->token_type = ERRORT;
					token->token_index = save_token(buffer);
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					ungetc(c,fin);
					state = START;
					return index;
				}
				else if ((c == 'e' || c == 'E') && is_number_after_dot)
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					state = SCIENTIFIC;
				}
				else
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					wrong_identifier = true;
					state = IDENTIFIER;
				}

				break;

			case SCIENTIFIC:
				if ((c == '+' || c == '-') && !signed_scientific && !is_number_after_sign)
				{
					signed_scientific = true;
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					state = SCIENTIFIC;
				}
				else if (c > 47 && c < 58)
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					is_number_after_sign = true;
				}
				else if ((SEPARATOR) && is_number_after_sign)
				{
					token->token_type = L_DOUBLE;
					token->token_index = save_token(buffer);
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					ungetc(c,fin);
					state = START;
					return index;
				}
				else
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					wrong_identifier = true;
					state = IDENTIFIER;
				}
				break;

			// case O_MINUS:
			// 	if (c > 47 && c < 58) // 0-9
			// 	{
			// 		catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
			// 		state = L_INT;
			// 	}
			// 	else // is '-'
			// 	{
			// 		ungetc(c, fin);
			// 		state = START;
			// 		token->token_index = save_token(buffer);
			// 		token->token_type = O_MINUS;
			// 		// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
			// 		return index;
			// 	}

			// 	break;

			case L_STRING:
				if (c == '"')
				{
					token->token_type = L_STRING;
					token->token_index = save_token(buffer);
					// printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
				}
				else if (c == '\\')
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
					state = L_STRING_BACKSLASH;
				}
				else
				{
					catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
				}
				
				break;

			case L_STRING_BACKSLASH:
				catch_token_internal_error(add_char(buffer, c), INTERNAL_ERROR, token, index);
				state = L_STRING;
				break;

		} // switch(state)
	} // while(1)
	return 0;
}
