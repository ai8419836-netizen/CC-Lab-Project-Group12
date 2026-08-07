#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/ast.h"

/* Walks the (semantically valid) AST and prints Three-Address Code
   (Manual Section 4.6) to stdout. */
void generate_tac(ASTNode *root);

#endif
