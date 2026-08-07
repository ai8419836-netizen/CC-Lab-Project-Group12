# Mini Language Compiler — Compiler Construction Lab Project

A compiler front-end (Lexer → Parser → AST → Symbol Table → Semantic Analyzer → TAC
Generator) for the mini language defined in the Project Manual, Section 5.

## Team Members
- Ariful (Team Leader) — Lexer, integration, Makefile, testing
- Mazharul Islam Tahmid — Parser (Bison), AST
- Sharmin Akter Sipu — Symbol Table, Semantic Analyzer, TAC Generator

## Build Instructions

Requires: `gcc`, `flex`, `bison`, `make`
```bash
sudo apt install gcc flex bison make -y
```

Build:
```bash
make clean
make
```

## Run Instructions

```bash
./compiler path/to/source_file.txt
```

Example:
```bash
./compiler examples/sample1.txt
```

## Project Structure
project-root/
├── src/
│ ├── lexer/ lexer.l
│ ├── parser/ parser.y
│ ├── ast/ ast.h / ast.c
│ ├── symbol_table/ symbol_table.h / symbol_table.c
│ ├── semantic/ semantic.h / semantic.c
│ ├── codegen/ codegen.h / codegen.c
│ └── main.c
├── tests/ 9 required test cases + expected_outputs.txt
├── examples/ sample1.txt
└── Makefile


## Language Supported

Types: `int`, `float`, `bool`
Statements: declaration, assignment, if/if-else, while, print, nested blocks
Operators: `+ - * / %`  `< > <= >= == !=`  `&& || !`
Comments: `//` and `/* */`

## Test Cases

| File | Demonstrates |
|---|---|
| test1_valid_program.txt | Full valid program compiling to TAC |
| test2_lexical_error.txt | Invalid character detection |
| test3_syntax_error.txt | Grammar violation |
| test4_semantic_redeclaration.txt | Redeclaration in same scope |
| test5_semantic_undeclared.txt | Undeclared variable use |
| test6_semantic_scope_violation.txt | Variable used outside declared block |
| test7_semantic_type_mismatch.txt | Type mismatch in expression |
| test8_semantic_invalid_assignment.txt | Invalid type assignment |
| test9_semantic_invalid_expression.txt | Invalid operator on operand types |
