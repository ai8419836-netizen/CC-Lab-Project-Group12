#include <stdio.h>
#include "ast/ast.h"
#include "symbol_table/symbol_table.h"
#include "semantic/semantic.h"
#include "codegen/codegen.h"

extern int yyparse(void);
extern FILE *yyin;
extern ASTNode *root;
extern int syntax_error_count;

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            printf("Error: cannot open file %s\n", argv[1]);
            return 1;
        }
        yyin = f;
    }

    printf("==================================================\n");
    printf(" Mini Language Compiler - Front End\n");
    printf("==================================================\n\n");

    printf("---- Parsing ----\n");
    yyparse();

    if (syntax_error_count > 0) {
        printf("\nCompilation halted: %d syntax error(s) found.\n", syntax_error_count);
        return 1;
    }
    printf("Parsing completed successfully.\n");

    printf("\n---- Abstract Syntax Tree ----\n");
    ast_print(root, 0);

    printf("\n---- Semantic Analysis ----\n");
    scope_init();
    int sem_errors = semantic_check(root);
    if (sem_errors > 0) {
        printf("\nCompilation halted: %d semantic error(s) found.\n", sem_errors);
        return 1;
    }
    printf("Semantic analysis completed successfully. No errors found.\n");

    printf("\n---- Three Address Code (TAC) ----\n");
    generate_tac(root);

    ast_free(root);
    return 0;
}
