# Makefile
# Řešení IJC-DU2, příklad b), 26.4.2015
# Autor: Radovan Sroka, FIT

CFLAGS = -std=c99 -pedantic -Wall -Wextra -g
CC = gcc
CPFLAFS = -std=c++11 -pedantic -Wall -g
CP = g++

default: tail tail2 wordcount wordcount-dynamic

tail: tail.o

tail.o: tail.c

tail2: tail2.o
	$(CP) tail2.o -o tail2

tail2.o: tail2.cc
	$(CP) $(CPFLAFS) -c tail2.cc

wordcount: wordcount.o libhtable.a io.o
	$(CC) -static wordcount.o io.o -L. -lhtable -o wordcount

wordcount-dynamic: wordcount.o libhtable.so io.o
	$(CC) wordcount.o io.o -o wordcount-dynamic -L. -lhtable

wordcount.o: wordcount.c



libhtable.a: hash.o htab_init.o htab_clear.o htab_remove.o htab_foreach.o htab_lookup.o htab_statistics.o 
	ar rcs libhtable.a hash.o htab_init.o htab_clear.o htab_remove.o htab_foreach.o htab_lookup.o htab_statistics.o 

libhtable.so: hashd.o htab_initd.o htab_cleard.o htab_removed.o htab_foreachd.o htab_lookupd.o htab_statisticsd.o 
	gcc -shared -Wl,-soname,libhtable.so.1 -o libhtable.so.1.0.1  hashd.o htab_initd.o htab_cleard.o htab_removed.o htab_foreachd.o htab_lookupd.o htab_statisticsd.o 


#dynamic
hashd.o: hash.c
	$(CC) $(CFLAGS) -c -fPIC hash.c -o hashd.o 

htab_initd.o: htab_init.c
	$(CC) $(CFLAGS) -c -fPIC htab_init.c -o htab_initd.o 

htab_cleard.o: htab_clear.c
	$(CC) $(CFLAGS) -c -fPIC htab_clear.c -o htab_cleard.o 

htab_removed.o: htab_remove.c
	$(CC) $(CFLAGS) -c -fPIC htab_remove.c -o htab_removed.o 

htab_foreachd.o: htab_foreach.c
	$(CC) $(CFLAGS) -c -fPIC htab_foreach.c -o htab_foreachd.o 

htab_lookupd.o: htab_lookup.c
	$(CC) $(CFLAGS) -c -fPIC htab_lookup.c -o htab_lookupd.o 

htab_statisticsd.o: htab_statistics.c
	$(CC) $(CFLAGS) -c -fPIC htab_statistics.c -o htab_statisticsd.o 


#static
io.o: io.c

hash.o: hash.c

htab_init.o: htab_init.c

htab_clear.o: htab_clear.c

htab_remove.o: htab_remove.c

htab_foreach.o: htab_foreach.c

htab_lookup.o: htab_lookup.c

htab_statistics.o: htab_statistics.c



clean:
	rm *.o

zip:
	zip xsroka00.zip *.c *.h *.cc Makefile
