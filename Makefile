CC := gcc
CFLAGS := -Wall -Wpedantic -pedantic -std=c99

all: repl.o utils.o lexer.o main.o
	$(CC) $(CFLAGS) -o main repl.o utils.o lexer.o main.o

main.o: repl.h main.c
repl.o: utils.h repl.h
utils.o: utils.h utils.c
lexer.o: lexer.h lexer.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm *.o

.PHONY: clean
.NOTPARALLEL: clean
.SUFFIXES: .c .o
