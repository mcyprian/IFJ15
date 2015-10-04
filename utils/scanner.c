/**
*autor Tomas Scavnicky
*
* zatial rozozna len operatory + - * / , > >= < <= a ==
* vstup: textovy subort, treba ho vytvorit v danom adresari a zadat do funkcie fopen() v main()
*
*
*/


#include <stdio.h>
#include <stdlib.h>

#define PLUS 42
#define MUL 43
#define MINUS 45
#define DIV	47
#define SEMICOLON 59
#define LESS 60
#define GREATER 62
#define EQUALS 61

enum {START, LETTER, POTENTIAL_DOUBLE_OPERATOR, DOUBLE_OPERATOR, WHITE_SYMBOL};


int scan(FILE *fin) {
	int c;
	int state = START;
	int previous;
	char buffer[3];

	while ((c = fgetc(fin)) != EOF) {
		switch (state) {


			case START:
				if ((c > 41 && c < 46) || c == '/' || c ==';') { 	// vysporiadat sa s ciarkou
					printf("%c OPERATOR\n", c);
				
				} else if (c > 59 && c < 63) { // > = <
					state = POTENTIAL_DOUBLE_OPERATOR;
					buffer[0] = c;
				}
				break;

			case POTENTIAL_DOUBLE_OPERATOR:
				if (c == '=') {
					buffer[1] = c;
					printf("%s OPERATOR\n", buffer);

				} 
				else {
					printf("%c OPERATOR\n", buffer[0]);
					if ((c > 41 && c < 46) || c == '/' || c ==';') { 	// vysporiadat sa s ciarkou
						printf("%c OPERATOR\n", c);
					}
				}
				state = START;
				break;

			

		}
	
	}
	return 0;
}



int main() {
	
	FILE *f = fopen("xyz.c", "r");	//zadat nazov suboru na scanovanie
	
	scan(f);

	fclose(f);

	return 0;
}




