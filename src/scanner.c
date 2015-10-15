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


int tokenize(FILE *fin, TDynamic_buffer *b) 
{
	int c;
	int previous = 0;
	int state = START;
	bool read = true;

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
					add_char(b, c);
					
				}
				else if (c > 47 && c < 58)
				{
					state = INTEGER;
					add_char(b, c);
				}
				else  
				{
					switch (c)
					{
						case '+':
							add_char(b, c);
							printf("%s OPERATOR\n", save_token(b));
							break;
						case '-':
							add_char(b, c);
							printf("%s OPERATOR\n", save_token(b));
							
							break;
						case '/':
							add_char(b, c);
							printf("%s OPERATOR\n", save_token(b));
							
							break;
						case '*':
							add_char(b, c);
							printf("%s OPERATOR\n", save_token(b));
							
							break;
						case ';':
							add_char(b, c);
							printf("%s SEMICOLON\n", save_token(b));
							break;
						case ':':
							add_char(b, c);
							printf("%s COLON\n", save_token(b));
							break;
						case ',':
							add_char(b, c);
							printf("%s COMMA\n", save_token(b));
							break;
						case '(':
							add_char(b, c);
							printf("%s OPENING BRACKET\n", save_token(b));
							break;
						case ')':
							add_char(b, c);
							printf("%s CLOSING BRACKET\n", save_token(b));
							break;
						case '[':
							add_char(b, c);
							printf("%s OPENING SQUARE BRACKET\n", save_token(b));
							break;
						case ']':
							add_char(b, c);
							printf("%s CLOSING SQUARE BRACKET\n", save_token(b));
							break;
						case '{':
							add_char(b, c);
							printf("%s OPENING CURLY BRACKET\n", save_token(b));
							break;
						case '}':
							add_char(b, c);
							printf("%s CLOSING CURLY BRACKET\n", save_token(b));
							break;
						case '=':
							add_char(b, c);
							state = EQUALS;
							break;
						case '<':
							add_char(b, c);
							state = LESS;
							break;
						case '>':
							add_char(b, c);
							state = GREATER;
							break;
						case '!':
							add_char(b, c);
							state = EXCLAMATION;
							break;
						}
				}
				break;

			case EQUALS:
				if (c == '=')
				{
					add_char(b, c);
					printf("%s OPERATOR\n", save_token(b));
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					printf("%s OPERATOR\n", save_token(b));
					break;
				}

			case GREATER:
				if (c == '=')
				{
					add_char(b, c);
					printf("%s OPERATOR\n", save_token(b));
					state = START;
					break;
				}
				else if (c == '>')
				{
					add_char(b, c);
					printf("%s OPERATOR\n", save_token(b));
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					printf("%s OPERATOR\n", save_token(b));
					break;
				}

			case LESS:
				if (c == '=')
				{
					add_char(b, c);
					printf("%s OPERATOR\n", save_token(b));
					state = START;
					break;
				}
				else if (c == '<')
				{
					add_char(b, c);
					printf("%s OPERATOR\n", save_token(b));
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					printf("%s OPERATOR\n", save_token(b));
					break;
				}

			case EXCLAMATION:
				if (c == '=')
				{
					add_char(b, c);
					printf("%s OPERATOR\n", save_token(b));
					state = START;
					break;
				}
				else
				{
					previous = c;
					read = false;
					state = START;
					printf("%s OPERATOR\n", save_token(b));
					break;
				}

			case IDENTIFIER:

				if ((c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58) || c == '_') { // (A-Z, a-z, 0-9, _)
					add_char(b, c);
				} else {
					printf("%s IDENTIFIER\n", save_token(b));
					previous = c;
					read = false;
					state = START;
				}
				break;

			case INTEGER:
				if (c > 47 && c < 58)
				{
					add_char(b, c);
				}
				else if (c == 46) // c == "."
				{
					add_char(b, c);
					state = DOUBLE;
				}
				else
				{
					printf("%s INTEGER\n", save_token(b));
					previous = c;
					read = false;
					state = START;
				}
				break;

			case DOUBLE:
				if (c > 47 && c < 58)
				{
					add_char(b, c);
				}
				else
				{
					printf("%s DOUBLE\n", save_token(b));
					previous = c;
					read = false;
					state = START;
				}
				break;


		} // switch(state)
	} // while(1)
	return 0;
}
