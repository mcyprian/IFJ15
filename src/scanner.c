/**
* autor Tomas Scavnicky, Michal Durista
*
* zatial rozozna:	operatory + - * / > >= < <= == << >> ; , () {} []
* 					indentifikatory (v ramci ident. nerozoznava klucove slova)
					integer (iba nezaporne)
					double (iba nezaporne)

* vstup: prvy argument
*/

#include <scanner.h>

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

int tokenize(FILE *fin, TDynamic_buffer *buffer) 
{
	int c;
	int previous = 0;
	int state = START;
	bool read = true;

	/* !!!!!!!!!!!! TOTO JE LEN DOCASNE RIESENIE !!!!!!!!!!!!!!!! */
	Token *token = (Token *) malloc(sizeof(Token));
	if (token == NULL)
    {
        printf("Je to v pici, nenaalokovali sme\n");
        fclose(fin);
        return -1;
    }

	token->type = IDENTIFIER;	// aby tam nieco bolo
	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

	while (1) {
		if (read)
		{
			if ((c = fgetc(fin)) == EOF)
				return 0;
		}
		else
		{
			c = previous;
			read = true;
		}

		switch (state) 
		{
			case START:

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
							token->value = save_token(buffer);
							token->type = OPERATOR;
							printf("%s    %d\n", token->value, token->type);	// bude return token; velmi pravdepodobne :D // docasne vypisuje uz len cislo z enumeracie "typu" - aj tak to vypisovat uz nebude, len vvraciat token s typom
							break;
						case '-':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = OPERATOR;
							printf("%s    %d\n", token->value, token->type);
							
							break;
						case '/':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = OPERATOR;
							printf("%s    %d\n", token->value, token->type);
							
							break;
						case '*':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = OPERATOR;
							printf("%s    %d\n", token->value, token->type);
							
							break;
						case ';':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = SEMICOLON;
							printf("%s    %d\n", token->value, token->type);
							break;
						case ':':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = COLON;
							printf("%s    %d\n", token->value, token->type);
							break;
						case ',':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = COMMA;
							printf("%s    %d\n", token->value, token->type);
							break;
						case '(':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = OPENING_BRACKET;
							printf("%s    %d\n", token->value, token->type);
							break;
						case ')':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = CLOSING_BRACKET;
							printf("%s    %d\n", token->value, token->type);
							break;
						case '[':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = OPENING_SQUARE_BRACKET;
							printf("%s    %d\n", token->value, token->type);
							break;
						case ']':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = CLOSING_SQUARE_BRACKET;
							printf("%s    %d\n", token->value, token->type);
							break;
						case '{':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = OPENING_CURLY_BRACKET;
							printf("%s    %d\n", token->value, token->type);
							break;
						case '}':
							add_char(buffer, c);
							token->value = save_token(buffer);
							token->type = CLOSING_CURLY_BRACKET;
							printf("%s    %d\n", token->value, token->type);
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
						}
				}
				break;

			case EQUALS:
				if (c == '=')
				{
					add_char(buffer, c);
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					break;
				}

			case GREATER:
				if (c == '=')
				{
					add_char(buffer, c);
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					state = START;
					break;
				}
				else if (c == '>')
				{
					add_char(buffer, c);
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					break;
				}

			case LESS:
				if (c == '=')
				{
					add_char(buffer, c);
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					state = START;
					break;
				}
				else if (c == '<')
				{
					add_char(buffer, c);
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					break;
				}

			case EXCLAMATION:
				if (c == '=')
				{
					add_char(buffer, c);
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					token->value = save_token(buffer);
					token->type = OPERATOR;
					printf("%s    %d\n", token->value, token->type);
					break;
				}

			case IDENTIFIER:

				if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58) || c == '_') { // (A-Z, a-z, 0-9, _)
					add_char(buffer, c);
				} 
				else 
				{
					token->value = save_token(buffer);
					token->type = reservedWord(token->value);
					printf("%s    %d\n", token->value, token->type);
					previous = c;
					read = false;
					state = START;
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
				else
				{
					token->value = save_token(buffer);
					token->type = INTEGER;
					printf("%s    %d\n", token->value, token->type);
					previous = c;
					read = false;
					state = START;
				}
				break;

			case DOUBLE:
				if (c > 47 && c < 58)
				{
					add_char(buffer, c);
				}
				else
				{
					token->type = DOUBLE;
					token->value = save_token(buffer);
					printf("%s    %d\n", token->value, token->type);
					previous = c;
					read = false;
					state = START;
				}
				break;


		} // switch(state)
	} // while(1)

	free(token);
	return 0;
}
