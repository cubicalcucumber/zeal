CC := C:\Coding\MinGW\bin\gcc
CFLAGS := -Wall -Wpedantic -pedantic -std=c99

all: repl.o utils.o lexer.o parser.o compiler.o value.o vm.o main.o
	$(CC) $(CFLAGS) -o main \
		repl.o utils.o lexer.o parser.o compiler.o value.o vm.o main.o

main.o: compiler.h repl.h vm.h main.c
utils.o: utils.h utils.c
repl.o: utils.h repl.h
lexer.o: lexer.h lexer.c
parser.o: lexer.h parser.h token.h parser.c
value.o: utils.h value.h value.c
compiler.o: compiler.h fragment.h parser.h utils.h value.h compiler.c
vm.o: fragment.h utils.h value.h vm.h vm.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm *.o

.PHONY: clean
.NOTPARALLEL: clean
.SUFFIXES: .c .o
