CC=g++
CCFLAGS=-std=c++11 -pedantic -Wall 


PROG=test

$(PROG) : main.o parser.o
	$(CC) $(CCFLAGS) main.o parser.o -o $(PROG)

all : 
	clean
	$(PROG)

parser.o : parser.cpp parser.h
	$(CC) -c $(CCFLAGS) parser.cpp -o parser.o

main.o : main.cpp
	$(CC) -c $(CCFLAGS) main.cpp -o main.o

clean:
	rm -rf *.o
	rm -rf *.*~
	rm -rf $(PROG)
