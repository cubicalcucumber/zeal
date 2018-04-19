CC := gcc
CFLAGS := -Wall -Wpedantic -pedantic -std=c99

all: repl.o utils.o \
		 compiler/lexer.o compiler/parser.o compiler/compiler.o vm/value.o vm/vm.o main.o
	$(CC) $(CFLAGS) -o main \
		repl.o utils.o compiler/lexer.o compiler/parser.o compiler/compiler.o \
		vm/value.o vm/vm.o main.o

main.o: compiler/compiler.h repl.h vm/fragment.h vm/vm.h main.c
utils.o: utils.h utils.c
repl.o: utils.h repl.h
compiler/lexer.o: compiler/lexer.h compiler/lexer.c
compiler/parser.o: \
	compiler/lexer.h compiler/parser.h compiler/token.h compiler/parser.c
compiler/compiler.o: \
	compiler/compiler.h compiler/compiler.c compiler/parser.h utils.h \
	vm/fragment.h vm/value.h
vm/value.o: utils.h vm/value.h vm/value.c
vm/vm.o: utils.h vm/fragment.h vm/value.h vm/vm.h vm/vm.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm compiler/*.o vm/*.o *.o

.PHONY: clean
.NOTPARALLEL: clean
.SUFFIXES: .c .o
