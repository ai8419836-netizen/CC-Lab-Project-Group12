#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "../ast/ast.h"

/* Runs full semantic analysis on the AST (Manual Section 4.5).
   Assumes scope_init() has already been called (global scope active).
   Returns the number of semantic errors found (0 = clean). */
int semantic_check(ASTNode *root);

#endif
