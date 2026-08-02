#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

/* ---- small helper: allocate and zero a node, set type/line ---- */
static ASTNode *ast_alloc(NodeType type, int line) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    n->type = type;
    n->line = line;
    return n;
}

static char *str_dup(const char *s) {
    if (!s) return NULL;
    char *d = (char *)malloc(strlen(s) + 1);
    strcpy(d, s);
    return d;
}

/* ---- PROGRAM / BLOCK (both are just a growable list of statements) ---- */
ASTNode *ast_new_program(void) {
    ASTNode *n = ast_alloc(NODE_PROGRAM, 0);
    n->stmt_capacity = 8;
    n->stmts = (ASTNode **)malloc(sizeof(ASTNode *) * n->stmt_capacity);
    n->stmt_count = 0;
    return n;
}

ASTNode *ast_new_block(void) {
    ASTNode *n = ast_alloc(NODE_BLOCK, 0);
    n->stmt_capacity = 8;
    n->stmts = (ASTNode **)malloc(sizeof(ASTNode *) * n->stmt_capacity);
    n->stmt_count = 0;
    return n;
}

void ast_add_stmt(ASTNode *listnode, ASTNode *stmt) {
    if (listnode->stmt_count >= listnode->stmt_capacity) {
        listnode->stmt_capacity *= 2;
        listnode->stmts = (ASTNode **)realloc(listnode->stmts,
                              sizeof(ASTNode *) * listnode->stmt_capacity);
    }
    listnode->stmts[listnode->stmt_count++] = stmt;
}

/* ---- DECL ---- */
ASTNode *ast_new_decl(char *data_type, char *name, int line) {
    ASTNode *n = ast_alloc(NODE_DECL, line);
    n->data_type = str_dup(data_type);
    n->name = str_dup(name);
    return n;
}

/* ---- ASSIGN ---- */
ASTNode *ast_new_assign(char *name, ASTNode *rhs, int line) {
    ASTNode *n = ast_alloc(NODE_ASSIGN, line);
    n->name = str_dup(name);
    n->rhs = rhs;
    return n;
}

/* ---- IF / WHILE ---- */
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch, int line) {
    ASTNode *n = ast_alloc(NODE_IF, line);
    n->cond = cond;
    n->then_branch = then_branch;
    n->else_branch = else_branch;
    return n;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = ast_alloc(NODE_WHILE, line);
    n->cond = cond;
    n->body = body;
    return n;
}

/* ---- PRINT ---- */
ASTNode *ast_new_print(ASTNode *expr, int line) {
    ASTNode *n = ast_alloc(NODE_PRINT, line);
    n->expr = expr;
    return n;
}

/* ---- BINOP / UNOP ---- */
ASTNode *ast_new_binop(const char *op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *n = ast_alloc(NODE_BINOP, line);
    n->op = str_dup(op);
    n->left = left;
    n->right = right;
    return n;
}

ASTNode *ast_new_unop(const char *op, ASTNode *operand, int line) {
    ASTNode *n = ast_alloc(NODE_UNOP, line);
    n->op = str_dup(op);
    n->left = operand;
    n->right = NULL;
    return n;
}

/* ---- Leaves ---- */
ASTNode *ast_new_id(char *name, int line) {
    ASTNode *n = ast_alloc(NODE_ID, line);
    n->name = str_dup(name);
    return n;
}

ASTNode *ast_new_int_lit(int value, int line) {
    ASTNode *n = ast_alloc(NODE_INT_LIT, line);
    n->ival = value;
    return n;
}

ASTNode *ast_new_float_lit(double value, int line) {
    ASTNode *n = ast_alloc(NODE_FLOAT_LIT, line);
    n->fval = value;
    return n;
}

ASTNode *ast_new_bool_lit(int value, int line) {
    ASTNode *n = ast_alloc(NODE_BOOL_LIT, line);
    n->bval = value;
    return n;
}

/* ============================================================
   PRINTING - text-indented tree (Manual Section 4.3 requirement)
   ============================================================ */
static void indent(int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            indent(depth); printf("Program\n");
            for (int i = 0; i < node->stmt_count; i++)
                ast_print(node->stmts[i], depth + 1);
            break;

        case NODE_BLOCK:
            indent(depth); printf("Block\n");
            for (int i = 0; i < node->stmt_count; i++)
                ast_print(node->stmts[i], depth + 1);
            break;

        case NODE_DECL:
            indent(depth); printf("Decl (%s %s) [line %d]\n", node->data_type, node->name, node->line);
            break;

        case NODE_ASSIGN:
            indent(depth); printf("Assign (%s) [line %d]\n", node->name, node->line);
            ast_print(node->rhs, depth + 1);
            break;

        case NODE_IF:
            indent(depth); printf("If [line %d]\n", node->line);
            indent(depth + 1); printf("Cond:\n");
            ast_print(node->cond, depth + 2);
            indent(depth + 1); printf("Then:\n");
            ast_print(node->then_branch, depth + 2);
            if (node->else_branch) {
                indent(depth + 1); printf("Else:\n");
                ast_print(node->else_branch, depth + 2);
            }
            break;

        case NODE_WHILE:
            indent(depth); printf("While [line %d]\n", node->line);
            indent(depth + 1); printf("Cond:\n");
            ast_print(node->cond, depth + 2);
            indent(depth + 1); printf("Body:\n");
            ast_print(node->body, depth + 2);
            break;

        case NODE_PRINT:
            indent(depth); printf("Print [line %d]\n", node->line);
            ast_print(node->expr, depth + 1);
            break;

        case NODE_BINOP:
            indent(depth); printf("BinOp (%s) [line %d]\n", node->op, node->line);
            ast_print(node->left, depth + 1);
            ast_print(node->right, depth + 1);
            break;

        case NODE_UNOP:
            indent(depth); printf("UnOp (%s) [line %d]\n", node->op, node->line);
            ast_print(node->left, depth + 1);
            break;

        case NODE_ID:
            indent(depth); printf("Id (%s) [line %d]\n", node->name, node->line);
            break;

        case NODE_INT_LIT:
            indent(depth); printf("IntLit (%d) [line %d]\n", node->ival, node->line);
            break;

        case NODE_FLOAT_LIT:
            indent(depth); printf("FloatLit (%f) [line %d]\n", node->fval, node->line);
            break;

        case NODE_BOOL_LIT:
            indent(depth); printf("BoolLit (%s) [line %d]\n", node->bval ? "true" : "false", node->line);
            break;
    }
}

/* ============================================================
   FREE - recursively release every node
   ============================================================ */
void ast_free(ASTNode *node) {
    if (!node) return;

    for (int i = 0; i < node->stmt_count; i++)
        ast_free(node->stmts[i]);
    if (node->stmts) free(node->stmts);

    ast_free(node->rhs);
    ast_free(node->cond);
    ast_free(node->then_branch);
    ast_free(node->else_branch);
    ast_free(node->body);
    ast_free(node->expr);
    ast_free(node->left);
    ast_free(node->right);

    if (node->data_type) free(node->data_type);
    if (node->name) free(node->name);
    if (node->op) free(node->op);
    if (node->expr_type) free(node->expr_type);

    free(node);
}
