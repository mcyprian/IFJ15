/**
* autor Tomas Scavnicky, Michal Durista
*
* zatial rozozna len operatory + - * / , > >= < <= == a << (pridat !=)
* vstup: prvy argument
*/

#include scanner.h

int tokenize(FILE *fin) {
	int c;
	int state = START;
	char opbuffer[2];	// nahradi dynamicky buffer
	bool double_op = false;

	while ((c = fgetc(fin)) != EOF) {
		switch (state) {

			case IDENTIFIER:
				state = START;
				break;

			case OPERATOR:
				if ((double_op && c == '=') || (opbuffer[0] == '<' && c == '<')) { // dvojity operator >= <= == !=
					opbuffer[1] = c;

					printf("%s OPERATOR\n", opbuffer);
					memset(opbuffer, 0, sizeof(opbuffer));
					
					state = START;
					double_op = false;
					break;
					
				} else {
					printf("%s OPERATOR\n", opbuffer);
					memset(opbuffer, 0, sizeof(opbuffer));
					state = START;
				}

			case START:
				if ((c > 41 && c < 46) || c == '/' || c ==';' || (c > 59 && c < 63)) { 	// vysporiadat sa s ciarkou
					double_op = (c > 59 && c < 63);
					opbuffer[0] = c;
					state = OPERATOR;
				} else if ((c > 64 && c < 91) || (c > 96 && c < 123)) {
					// tu uz potrebujem dynamicky buffer
					state = IDENTIFIER;
					break;
				}
				break;
		}	
	
	}
	return 0;
}