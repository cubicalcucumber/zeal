CC := gcc
CFLAGS := -Wall -Wpedantic -pedantic -std=c99

ZEAL_OBJS := \
	compiler/compiler.o \
	compiler/lexer.o \
	compiler/parser.o \
	repl.o \
	utils.o \
	vm/fragment.o \
	vm/value.o \
	vm/vm.o \
	zeal.o

all: test.o
	$(CC) $(CFLAGS) zeal-tests.c -o zeal-tests test.o \
	$(foreach obj, $(ZEAL_OBJS), ../$(obj))

test.o: test.h test.c
	$(CC) $(CFLAGS) -c test.c

clean:
	@rm *.o

.PHONY: clean
.NOTPARALLEL: clean
.SUFFIXES: .c .o