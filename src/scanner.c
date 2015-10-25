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


// int save_token_structure(TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer, int operator, index_t index, index_t *items)
// {
	
	
// 	token->token_type = operator;
// 	token->token_index = save_token(buffer);
	
// 	return 0;
// }


index_t get_token_(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer) {
	args_assert(buffer != NULL && struct_buffer != NULL, INTERNAL_ERROR);
	debug_print("%s\n", "GET_TOKEN");


	int c;
	int previous = 0;
	int state = START;
	bool read = true;
	index_t index = 0;
	index_t items[5];
	TToken *token;

	new_item(struct_buffer, index, token);

	while (1) {
		if (read)
		{	
			c = fgetc(fin);

			if (feof(fin))
			{
				new_item(struct_buffer, index, items);
				add_char(buffer, 'E');
				add_char(buffer, 'O');
				add_char(buffer, 'F');
				token->token_index = save_token(buffer);
				token->token_type = EOFT;
				printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);

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
				new_item(struct_buffer, index, items); // pridat overenie, ci to vyslo

				if ((c > 64 && c < 91) || (c > 96 && c < 123)) //A-Z || a-z
				{
					state = IDENTIFIER;
					add_char(buffer, c);
					
				}
				else if (c > 47 && c < 58)
				{
					state = L_INT;
					add_char(buffer, c);
				}
				else  
				{
					switch (c)
					{
						case '+':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = O_PLUS;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '-':
							add_char(buffer, c);
							state = O_MINUS;
							break;
						case '/':
							state = COMMENT;
							break;
						case '*':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = O_MUL;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ';':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = SEMICOLON;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						// case ':':
						// 	add_char(buffer, c);
						// 	token->token_index = save_token(buffer);
						// 	token->token_type = COLON;
						// 	printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
						// 	return index;
						// 	break;
						case ',':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = COMMA;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '(':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPENING_BRACKET;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ')':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_BRACKET;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						// case '[':
						// 	add_char(buffer, c);
						// 	token->token_index = save_token(buffer);
						// 	token->token_type = OPENING_SQUARE_BRACKET;
						// 	printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
						// 	return index;
						// 	break;
						// case ']':
						// 	add_char(buffer, c);
						// 	token->token_index = save_token(buffer);
						// 	token->token_type = CLOSING_SQUARE_BRACKET;
						// 	printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
						// 	return index;
						// 	break;
						case '{':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPENING_CURLY_BRACKET;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '}':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_CURLY_BRACKET;
							printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '=':
							add_char(buffer, c);
							state = O_EQUALS;
							break;
						case '<':
							add_char(buffer, c);
							state = O_L;
							break;
						case '>':
							add_char(buffer, c);
							state = O_G;
							break;
						case '!':
							add_char(buffer, c);
							state = UO_EXCLAMATION;
							break;
						
						case '"':
							add_char(buffer, c);
							state = L_STRING;
							break;

						default:
							if (!isspace(c))
							{
								add_char(buffer, c);
								token->token_index = save_token(buffer);
								token->token_type = ERRORT;
								printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
								return index;
							}
							break;
					} // switch(c)
				} // else
				break;

			case COMMENT:
				if (c == '/')
				{
					state = LINE_COMMENT;
				}
				else if (c == '*')
				{
					state = BLOCK_COMMENT;
				}
				else // '/'
				{
					add_char(buffer, '/');
					previous = c;
					read = false;
					state = START;
					token->token_index = save_token(buffer);
					token->token_type = O_DIV;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
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
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = O_ASSIGN;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else
				{
					// previous = c;
					// read = false;
					ungetc(c, fin);
					state = START;
					token->token_index = save_token(buffer);
					token->token_type = O_ASSIGN;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					return index;
					break;
				}

			case O_G:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = O_GE;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else if (c == '>')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = O_RIGHT_ARROW;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else
				{
					// previous = c;
					// read = false;
					ungetc(c, fin);
					token->token_index = save_token(buffer);
					token->token_type = O_G;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case O_L:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = O_LE;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else if (c == '<')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = O_LEFT_ARROW;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else
				{
					// previous = c;
					// read = false;
					ungetc(c, fin);
					token->token_index = save_token(buffer);
					token->token_type = O_L;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case UO_EXCLAMATION:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = O_NE;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else
				{
					// previous = c;
					// read = false;
					ungetc(c, fin);
					token->token_index = save_token(buffer);
					token->token_type = UO_EXCLAMATION;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					state = START;
					//return index;
					break;
				}

			case IDENTIFIER:

				if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58) || c == '_') { // (A-Z, a-z, 0-9, _)
					add_char(buffer, c);
				} 
				else 
				{
					token->token_index = save_token(buffer);
					token->token_type = reservedWord(get_token(buffer, token->token_index));
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					// previous = c;
					// read = false;
					ungetc(c,fin);
					state = START;
					return index;
				}
				break;

			case L_INT:
				if (c > 47 && c < 58)
				{
					add_char(buffer, c);
				}
				else if (c == 46) // c == "."
				{
					add_char(buffer, c);
					state = L_DOUBLE;
				}
				// else if (c == '+' || c == '-')
				// {
				// 	signed_scientific = true;
				// 	add_char(buffer, c);
				// 	state = SCIENTIFIC;
				// }
				// else if (c == 'e' || c == 'E')
				// {
				// 	add_char(buffer, c);
				// 	state = SCIENTIFIC;
				// }
				else
				{
					token->token_index = save_token(buffer);
					token->token_type = L_INT;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					ungetc(c,fin);
					state = START;
					return index;
				}
				break;

			case L_DOUBLE:
				if (c > 47 && c < 58)
				{
					add_char(buffer, c);
				}
				else
				{
					token->token_type = L_DOUBLE;
					token->token_index = save_token(buffer);
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					// previous = c;
					// read = false;
					ungetc(c,fin);
					state = START;
					return index;
				}
				break;

			// case SCIENTIFIC:
			// 	if ()
			// 	break;

			case O_MINUS:
				if (c > 47 && c < 58) // is negative integer or double
				{
					add_char(buffer, c);
					state = L_INT;
				}
				else // is '-'
				{
					// previous = c;
					// read = false;
					ungetc(c, fin);
					state = START;
					token->token_index = save_token(buffer);
					token->token_type = O_MINUS;
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					return  index;
				}
				break;

			case L_STRING:
				if (c == '"')
				{
					add_char(buffer, c);
					token->token_type = L_STRING;
					token->token_index = save_token(buffer);
					printf("%s    %d\n", get_token(buffer, token->token_index), token->token_type);
					//ungetc(c,fin);
					state = START;
					return index;
				}
				else
				{
					add_char(buffer, c);
				}
				break;

		} // switch(state)
	} // while(1)
	return 0;
}


// int tokenize(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buff) 
// {
// 	int c;
// 	int previous = 0;
// 	int state = START;
// 	bool read = true;
// 	TToken *token;

// 	// /* !!!!!!!!!!!! TOTO JE LEN DOCASNE RIESENIE !!!!!!!!!!!!!!!! */
// 	// Token *token = (Token *) malloc(sizeof(Token));
// 	// if (token == NULL)
//  //    {
//  //        printf("Je to v pici, nenaalokovali sme\n");
//  //        fclose(fin);
//  //        return -1;
//  //    }

// 	// token->type = IDENTIFIER;	// aby tam nieco bolo
// 	// /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

// 	while (1) {
// 		if (read)
// 		{
// 			if ((c = fgetc(fin)) == EOF)
// 				return 0; // toto treba prerobit, nefunguje pripad ked je v test.txt abcEOF
// 		}
// 		else
// 		{
// 			c = previous;
// 			read = true;
// 		}

// 		switch (state) 
// 		{
// 			case START:

// 				if (new_item(&struct_buff))

// 				if ((c > 64 && c < 91) || (c > 96 && c < 123)) //A-Z || a-z
// 				{
// 					state = IDENTIFIER;
// 					add_char(buffer, c);
					
// 				}
// 				else if (c > 47 && c < 58)
// 				{
// 					state = INTEGER;
// 					add_char(buffer, c);
// 				}
// 				else  
// 				{
// 					switch (c)
// 					{
// 						case '+':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPERATOR;
// 							printf("%s    %d\n", token->value, token->type);	// bude return token; velmi pravdepodobne :D // docasne vypisuje uz len cislo z enumeracie "typu" - aj tak to vypisovat uz nebude, len vvraciat token s typom
// 							break;
// 						case '-':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPERATOR;
// 							printf("%s    %d\n", token->value, token->type);
							
// 							break;
// 						case '/':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPERATOR;
// 							printf("%s    %d\n", token->value, token->type);
							
// 							break;
// 						case '*':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPERATOR;
// 							printf("%s    %d\n", token->value, token->type);
							
// 							break;
// 						case ';':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = SEMICOLON;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case ':':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = COLON;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case ',':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = COMMA;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case '(':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPENING_BRACKET;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case ')':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = CLOSING_BRACKET;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case '[':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPENING_SQUARE_BRACKET;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case ']':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = CLOSING_SQUARE_BRACKET;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case '{':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = OPENING_CURLY_BRACKET;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case '}':
// 							add_char(buffer, c);
// 							token->value = save_token(buffer);
// 							token->type = CLOSING_CURLY_BRACKET;
// 							printf("%s    %d\n", token->value, token->type);
// 							break;
// 						case '=':
// 							add_char(buffer, c);
// 							state = EQUALS;
// 							break;
// 						case '<':
// 							add_char(buffer, c);
// 							state = LESS;
// 							break;
// 						case '>':
// 							add_char(buffer, c);
// 							state = GREATER;
// 							break;
// 						case '!':
// 							add_char(buffer, c);
// 							state = EXCLAMATION;
// 							break;
// 						}
// 				}
// 				break;

// 			case EQUALS:
// 				if (c == '=')
// 				{
// 					add_char(buffer, c);
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					state = START;
// 					break;
// 				}
// 				else
// 				{
// 					previous = c;
// 					read = false;
// 					state = START;
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					break;
// 				}

// 			case GREATER:
// 				if (c == '=')
// 				{
// 					add_char(buffer, c);
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					state = START;
// 					break;
// 				}
// 				else if (c == '>')
// 				{
// 					add_char(buffer, c);
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					state = START;
// 					break;
// 				}
// 				else
// 				{
// 					previous = c;
// 					read = false;
// 					state = START;
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					break;
// 				}

// 			case LESS:
// 				if (c == '=')
// 				{
// 					add_char(buffer, c);
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					state = START;
// 					break;
// 				}
// 				else if (c == '<')
// 				{
// 					add_char(buffer, c);
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					state = START;
// 					break;
// 				}
// 				else
// 				{
// 					previous = c;
// 					read = false;
// 					state = START;
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					break;
// 				}

// 			case EXCLAMATION:
// 				if (c == '=')
// 				{
// 					add_char(buffer, c);
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					state = START;
// 					break;
// 				}
// 				else
// 				{
// 					previous = c;
// 					read = false;
// 					state = START;
// 					token->value = save_token(buffer);
// 					token->type = OPERATOR;
// 					printf("%s    %d\n", token->value, token->type);
// 					break;
// 				}

// 			case IDENTIFIER:

// 				if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58) || c == '_') { // (A-Z, a-z, 0-9, _)
// 					add_char(buffer, c);
// 				} 
// 				else 
// 				{
// 					token->value = save_token(buffer);
// 					token->type = reservedWord(token->value);
// 					printf("%s    %d\n", token->value, token->type);
// 					previous = c;
// 					read = false;
// 					state = START;
// 				}
// 				break;

// 			case INTEGER:
// 				if (c > 47 && c < 58)
// 				{
// 					add_char(buffer, c);
// 				}
// 				else if (c == 46) // c == "."
// 				{
// 					add_char(buffer, c);
// 					state = DOUBLE;
// 				}
// 				else
// 				{
// 					token->value = save_token(buffer);
// 					token->type = INTEGER;
// 					printf("%s    %d\n", token->value, token->type);
// 					previous = c;
// 					read = false;
// 					state = START;
// 				}
// 				break;

// 			case DOUBLE:
// 				if (c > 47 && c < 58)
// 				{
// 					add_char(buffer, c);
// 				}
// 				else
// 				{
// 					token->type = DOUBLE;
// 					token->value = save_token(buffer);
// 					printf("%s    %d\n", token->value, token->type);
// 					previous = c;
// 					read = false;
// 					state = START;
// 				}
// 				break;


// 		} // switch(state)
// 	} // while(1)
// 	return 0;
// }
