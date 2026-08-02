#ifndef AST_H
#define AST_H

/* ============================================================
   AST NODE TYPES
   Every construct in the language becomes one of these node types.
   ============================================================ */
typedef enum {
    NODE_PROGRAM,      /* list of statements (top-level) */
    NODE_BLOCK,        /* { ... } - list of statements, new scope */
    NODE_DECL,         /* int x;  / float x; / bool x; */
    NODE_ASSIGN,       /* x = expr; */
    NODE_IF,           /* if (cond) then_branch [else else_branch] */
    NODE_WHILE,        /* while (cond) body */
    NODE_PRINT,        /* print expr; */
    NODE_BINOP,        /* left OP right   (+ - * / % < > <= >= == != && ||) */
    NODE_UNOP,         /* OP operand      (! logical not, - unary minus) */
    NODE_ID,           /* identifier reference inside an expression */
    NODE_INT_LIT,      /* integer literal */
    NODE_FLOAT_LIT,    /* float literal */
    NODE_BOOL_LIT      /* true / false literal */
} NodeType;

typedef struct ASTNode {
    NodeType type;
    int line;                  /* source line number, for error messages */

    /* --- PROGRAM / BLOCK: list of statements --- */
    struct ASTNode **stmts;
    int stmt_count;
    int stmt_capacity;

    /* --- DECL --- */
    char *data_type;           /* "int" | "float" | "bool" */
    char *name;                /* used by DECL, ASSIGN (lhs name), ID */

    /* --- ASSIGN --- */
    struct ASTNode *rhs;

    /* --- IF / WHILE --- */
    struct ASTNode *cond;
    struct ASTNode *then_branch;
    struct ASTNode *else_branch;  /* NULL if no else */
    struct ASTNode *body;         /* while body */

    /* --- PRINT --- */
    struct ASTNode *expr;

    /* --- BINOP / UNOP --- */
    char *op;                  /* "+", "-", "==", "&&", "!" etc. */
    struct ASTNode *left;
    struct ASTNode *right;     /* NULL for UNOP */

    /* --- Literals --- */
    int ival;
    double fval;
    int bval;                  /* 1 = true, 0 = false */

    /* --- Semantic analysis fills this in (expression type) --- */
    char *expr_type;           /* "int" | "float" | "bool" | "error" */
} ASTNode;

/* Constructors */
ASTNode *ast_new_program(void);
ASTNode *ast_new_block(void);
void     ast_add_stmt(ASTNode *listnode, ASTNode *stmt);

ASTNode *ast_new_decl(char *data_type, char *name, int line);
ASTNode *ast_new_assign(char *name, ASTNode *rhs, int line);
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_branch, ASTNode *else_branch, int line);
ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line);
ASTNode *ast_new_print(ASTNode *expr, int line);

ASTNode *ast_new_binop(const char *op, ASTNode *left, ASTNode *right, int line);
ASTNode *ast_new_unop(const char *op, ASTNode *operand, int line);
ASTNode *ast_new_id(char *name, int line);
ASTNode *ast_new_int_lit(int value, int line);
ASTNode *ast_new_float_lit(double value, int line);
ASTNode *ast_new_bool_lit(int value, int line);

/* Printing (text-indented tree, Section 4.3 requirement) */
void ast_print(ASTNode *node, int depth);

/* Cleanup */
void ast_free(ASTNode *node);

#endif
