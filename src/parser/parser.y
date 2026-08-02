/* ============================================================
   parser.y - Bison grammar for the Mini Language
   (Project Manual Section 5: types int/float/bool, if/if-else,
   while, print, nested blocks, arithmetic/relational/logical ops)
   ============================================================ */

%code requires {
#include "../ast/ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ast/ast.h"

int yylex(void);
extern int yylineno;
void yyerror(const char *s);

ASTNode *root = NULL;          /* final parsed program, read by main.c */
int syntax_error_count = 0;    /* read by main.c to decide whether to continue */
%}

%union {
    int ival;
    double fval;
    char *sval;
    ASTNode *node;
}

/* ---- Tokens with values ---- */
%token <sval> ID
%token <ival> INT_CONST
%token <fval> FLOAT_CONST

/* ---- Keywords ---- */
%token KW_INT KW_FLOAT KW_BOOL KW_IF KW_ELSE KW_WHILE KW_PRINT KW_TRUE KW_FALSE

/* ---- Punctuation ---- */
%token ASSIGN SEMI LBRACE RBRACE LPAREN RPAREN

/* ---- Operators ---- */
%token PLUS MINUS MUL DIV MOD
%token LT GT LE GE EQ NE
%token AND_OP OR_OP NOT_OP

/* ---- Precedence: lowest to highest (Manual 5.3) ---- */
%left OR_OP
%left AND_OP
%right NOT_OP
%nonassoc LT GT LE GE EQ NE
%left PLUS MINUS
%left MUL DIV MOD
%right UMINUS

/* ---- Non-terminal types ---- */
%type <sval> type_kw
%type <node> stmt_list stmt decl_stmt assign_stmt if_stmt while_stmt print_stmt block expr

%%

/* ============================================================
   Top-level program = a list of statements
   ============================================================ */
program:
    stmt_list { root = $1; root->type = NODE_PROGRAM; }
    ;

stmt_list:
    /* empty */              { $$ = ast_new_block(); }
    | stmt_list stmt          { ast_add_stmt($1, $2); $$ = $1; }
    ;

block:
    LBRACE stmt_list RBRACE   { $$ = $2; /* already NODE_BLOCK */ }
    ;

stmt:
      decl_stmt
    | assign_stmt
    | if_stmt
    | while_stmt
    | print_stmt
    | block
    | error SEMI              { yyerrok; $$ = NULL; /* skip malformed statement, keep going */ }
    ;

decl_stmt:
    type_kw ID SEMI           { $$ = ast_new_decl($1, $2, yylineno); }
    ;

type_kw:
      KW_INT   { $$ = "int"; }
    | KW_FLOAT { $$ = "float"; }
    | KW_BOOL  { $$ = "bool"; }
    ;

assign_stmt:
    ID ASSIGN expr SEMI       { $$ = ast_new_assign($1, $3, yylineno); }
    ;

if_stmt:
      KW_IF LPAREN expr RPAREN stmt                 { $$ = ast_new_if($3, $5, NULL, yylineno); }
    | KW_IF LPAREN expr RPAREN stmt KW_ELSE stmt    { $$ = ast_new_if($3, $5, $7, yylineno); }
    ;

while_stmt:
    KW_WHILE LPAREN expr RPAREN stmt   { $$ = ast_new_while($3, $5, yylineno); }
    ;

print_stmt:
    KW_PRINT expr SEMI        { $$ = ast_new_print($2, yylineno); }
    ;

/* ============================================================
   Expressions (Manual Section 5.3 operators, precedence above)
   ============================================================ */
expr:
      expr OR_OP expr         { $$ = ast_new_binop("||", $1, $3, yylineno); }
    | expr AND_OP expr        { $$ = ast_new_binop("&&", $1, $3, yylineno); }
    | NOT_OP expr             { $$ = ast_new_unop("!", $2, yylineno); }
    | expr LT expr            { $$ = ast_new_binop("<",  $1, $3, yylineno); }
    | expr GT expr            { $$ = ast_new_binop(">",  $1, $3, yylineno); }
    | expr LE expr            { $$ = ast_new_binop("<=", $1, $3, yylineno); }
    | expr GE expr            { $$ = ast_new_binop(">=", $1, $3, yylineno); }
    | expr EQ expr            { $$ = ast_new_binop("==", $1, $3, yylineno); }
    | expr NE expr            { $$ = ast_new_binop("!=", $1, $3, yylineno); }
    | expr PLUS expr          { $$ = ast_new_binop("+",  $1, $3, yylineno); }
    | expr MINUS expr         { $$ = ast_new_binop("-",  $1, $3, yylineno); }
    | expr MUL expr           { $$ = ast_new_binop("*",  $1, $3, yylineno); }
    | expr DIV expr           { $$ = ast_new_binop("/",  $1, $3, yylineno); }
    | expr MOD expr           { $$ = ast_new_binop("%",  $1, $3, yylineno); }
    | MINUS expr %prec UMINUS { $$ = ast_new_unop("-", $2, yylineno); }
    | LPAREN expr RPAREN      { $$ = $2; }
    | ID                      { $$ = ast_new_id($1, yylineno); }
    | INT_CONST               { $$ = ast_new_int_lit($1, yylineno); }
    | FLOAT_CONST             { $$ = ast_new_float_lit($1, yylineno); }
    | KW_TRUE                 { $$ = ast_new_bool_lit(1, yylineno); }
    | KW_FALSE                { $$ = ast_new_bool_lit(0, yylineno); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "SYNTAX ERROR (line %d): %s\n", yylineno, s);
    syntax_error_count++;
}
