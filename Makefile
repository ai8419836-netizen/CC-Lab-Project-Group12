CC = gcc
CFLAGS = -Wall -Isrc -Isrc/parser

all: compiler

src/parser/parser.tab.c src/parser/parser.tab.h: src/parser/parser.y
	bison -d -o src/parser/parser.tab.c src/parser/parser.y

src/lexer/lex.yy.c: src/lexer/lexer.l src/parser/parser.tab.h
	flex -o src/lexer/lex.yy.c src/lexer/lexer.l

compiler: src/parser/parser.tab.c src/lexer/lex.yy.c \
          src/ast/ast.c src/symbol_table/symbol_table.c \
          src/semantic/semantic.c src/codegen/codegen.c src/main.c
	$(CC) $(CFLAGS) -o compiler \
		src/parser/parser.tab.c src/lexer/lex.yy.c \
		src/ast/ast.c src/symbol_table/symbol_table.c \
		src/semantic/semantic.c src/codegen/codegen.c src/main.c -lfl

test: compiler
	./compiler examples/sample1.txt

clean:
	rm -f compiler src/parser/parser.tab.c src/parser/parser.tab.h src/lexer/lex.yy.c

.PHONY: all test clean
