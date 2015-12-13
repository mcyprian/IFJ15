
CC=gcc
CFLAGS=-std=c99 -g -Wall -Wextra -pedantic -O2

all: interpreter

interpreter: main.o scanner.o dynamic_buffer.o dynamic_structure_buffer.o syntax_analysis.c precedence_analysis.o stack.o built_functions.o ial.o semantics.o interpreter.o instructions.o instruction_generator.o
	$(CC) main.o scanner.o dynamic_buffer.o dynamic_structure_buffer.o syntax_analysis.c precedence_analysis.o stack.o built_functions.o ial.o semantics.o interpreter.o instructions.o instruction_generator.o -o interpreter
	
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

interpreter.o: interpreter.c
	$(CC) $(CFLAGS) -c interpreter.c

scanner.o: scanner.c
	$(CC) $(CFLAGS) -c scanner.c

dynamic_buffer.o: dynamic_buffer.c
	$(CC) $(CFLAGS) -c dynamic_buffer.c

dynamic_structure_buffer.o: dynamic_structure_buffer.c
	$(CC) $(CFLAGS) -c dynamic_structure_buffer.c

syntax_analysis.o: syntax_analysis.c
	$(CC) $(CFLAGS) -c syntax_analysis.c

precedence_analysis.o: precedence_analysis.c
	$(CC) $(CFLAGS) -c precedence_analysis.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

built_functions.o: built_functions.c
	$(CC) $(CFLAGS) -c built_functions.c

sematics.o: sematics.c
	$(CC) $(CFLAGS) -c sematics.c

instructions.o: instructions.c
	$(CC) $(CFLAGS) -c instructions.c

instruction_generator.o: instruction_generator.c
	$(CC) $(CFLAGS) -c instruction_generator.c

ial.o: ial.c
	$(CC) $(CFLAGS) -c ial.c


clean: 
	rm -rf *.o

clean_all:
	rm -rf *.o interpreter

pack:
	tar -zcvf xscavn00.tgz *.h *.c Makefile rozsireni dokumentace.pdf
