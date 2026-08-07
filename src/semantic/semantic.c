#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "semantic.h"
#include "../symbol_table/symbol_table.h"

static int semantic_errors = 0;

/* Print a nicely formatted semantic error and bump the counter */
static void semantic_error(int line, const char *fmt, ...) {
    va_list args;
    printf("SEMANTIC ERROR (line %d): ", line);
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
    semantic_errors++;
}

static int is_numeric(const char *type) {
    return type && (strcmp(type, "int") == 0 || strcmp(type, "float") == 0);
}

/* Forward declaration */
static void check_stmt(ASTNode *stmt);

/* ============================================================
   infer_expr_type
   Walks an expression node, checks it for semantic correctness,
   annotates node->expr_type, and returns the resulting type
   ("int" | "float" | "bool" | "error").
   ============================================================ */
static const char *infer_expr_type(ASTNode *e) {
    if (e == NULL) return "error";

    switch (e->type) {

        case NODE_ID: {
            const char *t = scope_lookup(e->name);
            if (t == NULL) {
                semantic_error(e->line, "undeclared variable '%s' used", e->name);
                e->expr_type = strdup("error");
                return "error";
            }
            e->expr_type = strdup(t);
            return e->expr_type;
        }

        case NODE_INT_LIT:
            e->expr_type = strdup("int");
            return "int";

        case NODE_FLOAT_LIT:
            e->expr_type = strdup("float");
            return "float";

        case NODE_BOOL_LIT:
            e->expr_type = strdup("bool");
            return "bool";

        case NODE_UNOP: {
            const char *operand_type = infer_expr_type(e->left);
            if (strcmp(operand_type, "error") == 0) { e->expr_type = strdup("error"); return "error"; }

            if (strcmp(e->op, "!") == 0) {
                if (strcmp(operand_type, "bool") != 0) {
                    semantic_error(e->line, "logical NOT ('!') requires a boolean operand, got '%s'", operand_type);
                    e->expr_type = strdup("error");
                    return "error";
                }
                e->expr_type = strdup("bool");
                return "bool";
            } else { /* unary minus */
                if (!is_numeric(operand_type)) {
                    semantic_error(e->line, "unary minus requires a numeric operand, got '%s'", operand_type);
                    e->expr_type = strdup("error");
                    return "error";
                }
                e->expr_type = strdup(operand_type);
                return e->expr_type;
            }
        }

        case NODE_BINOP: {
            const char *lt = infer_expr_type(e->left);
            const char *rt = infer_expr_type(e->right);
            if (strcmp(lt, "error") == 0 || strcmp(rt, "error") == 0) {
                e->expr_type = strdup("error");
                return "error";
            }

            const char *op = e->op;

            /* Arithmetic operators: + - * / % */
            if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 ||
                strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
                if (!is_numeric(lt) || !is_numeric(rt)) {
                    semantic_error(e->line, "arithmetic operator '%s' cannot be applied to '%s' and '%s' (boolean operands not allowed)", op, lt, rt);
                    e->expr_type = strdup("error");
                    return "error";
                }
                /* float wins over int (standard numeric promotion) */
                e->expr_type = strdup((strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0) ? "float" : "int");
                return e->expr_type;
            }

            /* Relational operators: < > <= >= */
            if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0) {
                if (!is_numeric(lt) || !is_numeric(rt)) {
                    semantic_error(e->line, "relational operator '%s' requires numeric operands, got '%s' and '%s'", op, lt, rt);
                    e->expr_type = strdup("error");
                    return "error";
                }
                e->expr_type = strdup("bool");
                return "bool";
            }

            /* Equality operators: == != (numeric-numeric or bool-bool only) */
            if (strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
                int l_is_bool = strcmp(lt, "bool") == 0;
                int r_is_bool = strcmp(rt, "bool") == 0;
                if (l_is_bool != r_is_bool) {
                    semantic_error(e->line, "cannot compare '%s' with '%s' using '%s'", lt, rt, op);
                    e->expr_type = strdup("error");
                    return "error";
                }
                e->expr_type = strdup("bool");
                return "bool";
            }

            /* Logical operators: && || */
            if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
                if (strcmp(lt, "bool") != 0 || strcmp(rt, "bool") != 0) {
                    semantic_error(e->line, "logical operator '%s' requires boolean operands, got '%s' and '%s'", op, lt, rt);
                    e->expr_type = strdup("error");
                    return "error";
                }
                e->expr_type = strdup("bool");
                return "bool";
            }

            /* Should not happen */
            e->expr_type = strdup("error");
            return "error";
        }

        default:
            return "error";
    }
}

/* An rhs of type rhs_type can be assigned into a variable of declared_type? */
static int types_assignable(const char *declared_type, const char *rhs_type) {
    if (strcmp(declared_type, rhs_type) == 0) return 1;
    /* allow int -> float widening, e.g. float x; x = 5; */
    if (strcmp(declared_type, "float") == 0 && strcmp(rhs_type, "int") == 0) return 1;
    return 0;
}

/* ============================================================
   check_stmt - validates one statement (Manual Section 4.5)
   ============================================================ */
static void check_stmt(ASTNode *stmt) {
    if (stmt == NULL) return; /* NULL can appear after Bison error-recovery */

    switch (stmt->type) {

        case NODE_BLOCK:
            scope_enter();
            for (int i = 0; i < stmt->stmt_count; i++)
                check_stmt(stmt->stmts[i]);
            scope_exit();
            break;

        case NODE_DECL:
            if (!scope_declare(stmt->name, stmt->data_type, stmt->line)) {
                semantic_error(stmt->line, "redeclaration of variable '%s' in the same scope", stmt->name);
            }
            break;

        case NODE_ASSIGN: {
            const char *rhs_type = infer_expr_type(stmt->rhs);
            const char *declared_type = scope_lookup(stmt->name);
            if (declared_type == NULL) {
                semantic_error(stmt->line, "undeclared variable '%s' used in assignment", stmt->name);
            } else if (strcmp(rhs_type, "error") != 0 && !types_assignable(declared_type, rhs_type)) {
                semantic_error(stmt->line, "type mismatch: cannot assign value of type '%s' to variable '%s' of type '%s'",
                               rhs_type, stmt->name, declared_type);
            }
            break;
        }

        case NODE_IF: {
            const char *cond_type = infer_expr_type(stmt->cond);
            if (strcmp(cond_type, "bool") != 0 && strcmp(cond_type, "error") != 0) {
                semantic_error(stmt->line, "if-condition must be boolean, got '%s'", cond_type);
            }
            check_stmt(stmt->then_branch);
            if (stmt->else_branch) check_stmt(stmt->else_branch);
            break;
        }

        case NODE_WHILE: {
            const char *cond_type = infer_expr_type(stmt->cond);
            if (strcmp(cond_type, "bool") != 0 && strcmp(cond_type, "error") != 0) {
                semantic_error(stmt->line, "while-condition must be boolean, got '%s'", cond_type);
            }
            check_stmt(stmt->body);
            break;
        }

        case NODE_PRINT:
            infer_expr_type(stmt->expr);
            break;

        default:
            /* Not a statement node -- ignore */
            break;
    }
}

int semantic_check(ASTNode *root) {
    semantic_errors = 0;
    if (root == NULL) return 0;
    for (int i = 0; i < root->stmt_count; i++) {
        check_stmt(root->stmts[i]);
    }
    return semantic_errors;
}
