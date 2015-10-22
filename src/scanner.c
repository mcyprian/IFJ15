/**
 * @file scanner.c
 * @author Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz>, Michal Durista <xduris04@stud.fit.vutbr.cz>
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
				return CIN;
			}
			else if (strcmp(identifier, "cout") == 0)
			{
				return COUT;
			}
				
		case 'd':
			if (strcmp(identifier, "double") == 0)
			{	
				return DOUBLE;
			}

		case 'e':
			if (strcmp(identifier, "else") == 0)
			{	
				return ELSE;
			}

		case 'f':
			if (strcmp(identifier, "for") == 0)
			{	
				return FOR;
			}

		case 'i':
			if (strcmp(identifier, "if") == 0)
			{	
				return IF;
			}
			else if (strcmp(identifier, "int") == 0)
			{	
				return INT;
			}
				
		case 'r':
			if (strcmp(identifier, "return") == 0)
			{	
				return RETURN;
			}

		case 's':
			if (strcmp(identifier, "string") == 0)
			{	
				return STRING;
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


index_t get_token(FILE *fin, TDynamic_buffer *buffer, TDynamic_structure_buffer *struct_buffer) {
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
				printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);

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
					state = INTEGER;
					add_char(buffer, c);
				}
				else  
				{
					switch (c)
					{
						case '+':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPERATOR;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '-':
							add_char(buffer, c);
							state = MINUS;
							break;
						case '/':
							state = COMMENT;
							break;
						case '*':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPERATOR;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ';':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = SEMICOLON;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ':':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = COLON;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ',':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = COMMA;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '(':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPENING_BRACKET;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ')':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_BRACKET;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '[':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPENING_SQUARE_BRACKET;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case ']':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_SQUARE_BRACKET;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '{':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = OPENING_CURLY_BRACKET;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '}':
							add_char(buffer, c);
							token->token_index = save_token(buffer);
							token->token_type = CLOSING_CURLY_BRACKET;
							printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
							return index;
							break;
						case '=':
							add_char(buffer, c);
							state = EQUALS;
							break;
						case '<':
							add_char(buffer, c);
							state = LESS;
							break;
						case '>':
							add_char(buffer, c);
							state = GREATER;
							break;
						case '!':
							add_char(buffer, c);
							state = EXCLAMATION;
							break;
						
						case '"':
							add_char(buffer, c);
							state = STRING_LITERAL;
							break;

						default:
							if (!isspace(c))
							{
								add_char(buffer, c);
								token->token_index = save_token(buffer);
								token->token_type = ERRORT;
								printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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

			case EQUALS:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					return index;
					break;
				}

			case GREATER:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else if (c == '>')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case LESS:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}
				else if (c == '<')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
					token->token_type = LESS;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					state = START;
					return index;
					break;
				}

			case EXCLAMATION:
				if (c == '=')
				{
					add_char(buffer, c);
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
					token->token_type = reservedWord(load_token(buffer, token->token_index));
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					// previous = c;
					// read = false;
					ungetc(c,fin);
					state = START;
					return index;
				}
				break;

			case INTEGER:
				if (c > 47 && c < 58)
				{
					add_char(buffer, c);
				}
				else if (c == 46) // c == "."
				{
					add_char(buffer, c);
					state = DOUBLE;
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
					token->token_type = INTEGER;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					ungetc(c,fin);
					state = START;
					return index;
				}
				break;

			case DOUBLE:
				if (c > 47 && c < 58)
				{
					add_char(buffer, c);
				}
				else
				{
					token->token_type = DOUBLE;
					token->token_index = save_token(buffer);
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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

			case MINUS:
				if (c > 47 && c < 58) // is negative integer or double
				{
					add_char(buffer, c);
					state = INTEGER;
				}
				else // is '-'
				{
					// previous = c;
					// read = false;
					ungetc(c, fin);
					state = START;
					token->token_index = save_token(buffer);
					token->token_type = OPERATOR;
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
					return  index;
				}
				break;

			case STRING_LITERAL:
				if (c == '"')
				{
					add_char(buffer, c);
					token->token_type = STRING_LITERAL;
					token->token_index = save_token(buffer);
					printf("%s    %d\n", load_token(buffer, token->token_index), token->token_type);
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
