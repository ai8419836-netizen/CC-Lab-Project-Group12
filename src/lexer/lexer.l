%option noyywrap
%option yylineno

%{
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
%}

DIGIT   [0-9]
LETTER  [a-zA-Z_]
ID_PAT  {LETTER}({LETTER}|{DIGIT})*

%%

"int"       { return KW_INT; }
"float"     { return KW_FLOAT; }
"bool"      { return KW_BOOL; }
"if"        { return KW_IF; }
"else"      { return KW_ELSE; }
"while"     { return KW_WHILE; }
"print"     { return KW_PRINT; }
"true"      { return KW_TRUE; }
"false"     { return KW_FALSE; }

{DIGIT}+"."{DIGIT}+    { yylval.fval = atof(yytext); return FLOAT_CONST; }
{DIGIT}+               { yylval.ival = atoi(yytext); return INT_CONST; }
{ID_PAT}               { yylval.sval = strdup(yytext); return ID; }

"<="   { return LE; }
">="   { return GE; }
"=="   { return EQ; }
"!="   { return NE; }
"<"    { return LT; }
">"    { return GT; }
"&&"   { return AND_OP; }
"||"   { return OR_OP; }
"!"    { return NOT_OP; }
"+"    { return PLUS; }
"-"    { return MINUS; }
"*"    { return MUL; }
"/"    { return DIV; }
"%"    { return MOD; }
"="    { return ASSIGN; }
";"    { return SEMI; }
"{"    { return LBRACE; }
"}"    { return RBRACE; }
"("    { return LPAREN; }
")"    { return RPAREN; }

"//".*                          { /* single-line comment: discard */ }
"/*"([^*]|\*+[^*/])*\*+"/"      { /* block comment: discard */ }

[ \t]+   { /* whitespace: discard */ }
\n       { /* newline: discard, yylineno auto-increments */ }

.        { printf("LEXICAL ERROR: invalid character '%s' (line %d)\n", yytext, yylineno); }

%%
