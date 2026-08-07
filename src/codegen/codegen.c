#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

static int temp_count = 0;
static int label_count = 0;

static char *new_temp(void) {
    char *buf = (char *)malloc(16);
    sprintf(buf, "t%d", ++temp_count);
    return buf;
}

static char *new_label(void) {
    char *buf = (char *)malloc(16);
    sprintf(buf, "L%d", ++label_count);
    return buf;
}

static void gen_stmt(ASTNode *stmt);

/* Generates code for an expression, returns a string naming the place
   holding its value (a variable name, a literal, or a fresh temp).
   The caller does not need to free this in this student project;
   a small amount of leaked memory here is acceptable trade-off for
   simplicity, exactly as the lab manuals (Lab 4) do. */
static char *gen_expr(ASTNode *e) {
    if (e == NULL) return strdup("?");

    char buf[64];

    switch (e->type) {
        case NODE_ID:
            return strdup(e->name);

        case NODE_INT_LIT:
            sprintf(buf, "%d", e->ival);
            return strdup(buf);

        case NODE_FLOAT_LIT:
            sprintf(buf, "%g", e->fval);
            return strdup(buf);

        case NODE_BOOL_LIT:
            return strdup(e->bval ? "true" : "false");

        case NODE_UNOP: {
            char *operand = gen_expr(e->left);
            char *t = new_temp();
            if (strcmp(e->op, "-") == 0)
                printf("%s = -%s\n", t, operand);
            else /* "!" */
                printf("%s = !%s\n", t, operand);
            return t;
        }

        case NODE_BINOP: {
            char *l = gen_expr(e->left);
            char *r = gen_expr(e->right);
            char *t = new_temp();
            printf("%s = %s %s %s\n", t, l, e->op, r);
            return t;
        }

        default:
            return strdup("?");
    }
}

static void gen_stmt(ASTNode *stmt) {
    if (stmt == NULL) return;

    switch (stmt->type) {

        case NODE_BLOCK:
            for (int i = 0; i < stmt->stmt_count; i++)
                gen_stmt(stmt->stmts[i]);
            break;

        case NODE_DECL:
            /* Declarations produce no TAC instruction -- only a symbol
               table entry, which is already handled by the semantic phase. */
            break;

        case NODE_ASSIGN: {
            char *val = gen_expr(stmt->rhs);
            printf("%s = %s\n", stmt->name, val);
            break;
        }

        case NODE_IF: {
            char *cond = gen_expr(stmt->cond);
            if (stmt->else_branch == NULL) {
                char *L1 = new_label();
                printf("ifFalse %s goto %s\n", cond, L1);
                gen_stmt(stmt->then_branch);
                printf("%s:\n", L1);
            } else {
                char *L1 = new_label();
                char *L2 = new_label();
                printf("ifFalse %s goto %s\n", cond, L1);
                gen_stmt(stmt->then_branch);
                printf("goto %s\n", L2);
                printf("%s:\n", L1);
                gen_stmt(stmt->else_branch);
                printf("%s:\n", L2);
            }
            break;
        }

        case NODE_WHILE: {
            char *L1 = new_label();
            char *L2 = new_label();
            printf("%s:\n", L1);
            char *cond = gen_expr(stmt->cond);
            printf("ifFalse %s goto %s\n", cond, L2);
            gen_stmt(stmt->body);
            printf("goto %s\n", L1);
            printf("%s:\n", L2);
            break;
        }

        case NODE_PRINT: {
            char *val = gen_expr(stmt->expr);
            printf("print %s\n", val);
            break;
        }

        default:
            break;
    }
}

void generate_tac(ASTNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < root->stmt_count; i++) {
        gen_stmt(root->stmts[i]);
    }
}
